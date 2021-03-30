/*
RailControl - Model Railway Control Software

Copyright (c) 2017-2021 Dominik (Teddy) Mahrer - www.railcontrol.org

RailControl is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3, or (at your option) any
later version.

RailControl is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with RailControl; see the file LICENCE. If not see
<http://www.gnu.org/licenses/>.
*/

#include <future>
#include <iostream>
#include <sstream>
#include <unistd.h>

#include "DataModel/LayoutItem.h"
#include "Languages.h"
#include "Hardware/HardwareHandler.h"
#include "Hardware/HardwareParams.h"
#include "Manager.h"
#include "RailControl.h"
#include "Storage/TransactionGuard.h"
#include "Utils/Utils.h"
#include "WebServer/WebServer.h"

using namespace DataModel;
using LayoutPosition = DataModel::LayoutItem::LayoutPosition;
using LayoutItemSize = DataModel::LayoutItem::LayoutItemSize;
using LayoutRotation = DataModel::LayoutItem::LayoutRotation;
using Visible = DataModel::LayoutItem::Visible;
using Hardware::HardwareHandler;
using Hardware::HardwareParams;
using std::map;
using std::string;
using std::stringstream;
using std::vector;
using Storage::StorageHandler;
using Storage::StorageParams;

Manager::Manager(Config& config)
:	logger(Logger::Logger::GetLogger(Languages::GetText(Languages::TextManager))),
 	boosterState(BoosterStateStop),
	storage(nullptr),
	defaultAccessoryDuration(DataModel::DefaultAccessoryPulseDuration),
	autoAddFeedback(false),
	stopOnFeedbackInFreeTrack(true),
	selectRouteApproach(DataModel::SelectRouteRandom),
	nrOfTracksToReserve(DataModel::Loco::ReserveOne),
	run(false),
	debounceRun(false),
	initLocosDone(false),
	unknownControl(Languages::GetText(Languages::TextControlDoesNotExist)),
	unknownLoco(Languages::GetText(Languages::TextLocoDoesNotExist)),
	unknownAccessory(Languages::GetText(Languages::TextAccessoryDoesNotExist)),
	unknownFeedback(Languages::GetText(Languages::TextFeedbackDoesNotExist)),
	unknownTrack(Languages::GetText(Languages::TextTrackDoesNotExist)),
	unknownSwitch(Languages::GetText(Languages::TextSwitchDoesNotExist)),
	unknownRoute(Languages::GetText(Languages::TextRouteDoesNotExist)),
	unknownSignal(Languages::GetText(Languages::TextSignalDoesNotExist))
{
	StorageParams storageParams;
	storageParams.module = "Sqlite";
	storageParams.filename = config.getValue("dbfilename", "railcontrol.sqlite");
	storageParams.keepBackups = config.getValue("dbkeepbackups", 10);
	storage = new StorageHandler(this, &storageParams);
	if (storage == nullptr)
	{
		logger->Info(Languages::TextUnableToCreateStorageHandler);
		return;
	}

	Logger::Logger::SetLogLevel(static_cast<Logger::Logger::Level>(Utils::Utils::StringToInteger(storage->GetSetting("LogLevel"), Logger::Logger::LevelInfo)));
	Languages::SetDefaultLanguage(static_cast<Languages::Language>(Utils::Utils::StringToInteger(storage->GetSetting("Language"), Languages::EN)));
	defaultAccessoryDuration = Utils::Utils::StringToInteger(storage->GetSetting("DefaultAccessoryDuration"), 250);
	autoAddFeedback = Utils::Utils::StringToBool(storage->GetSetting("AutoAddFeedback"));
	stopOnFeedbackInFreeTrack = Utils::Utils::StringToBool(storage->GetSetting("StopOnFeedbackInFreeTrack"), true);
	selectRouteApproach = static_cast<DataModel::SelectRouteApproach>(Utils::Utils::StringToInteger(storage->GetSetting("SelectRouteApproach")));
	nrOfTracksToReserve = static_cast<DataModel::Loco::NrOfTracksToReserve>(Utils::Utils::StringToInteger(storage->GetSetting("NrOfTracksToReserve"), 2));

	controls[ControlIdWebserver] = new WebServer::WebServer(*this, config.getValue("webserverport", 8082));

	storage->AllHardwareParams(hardwareParams);
	for (auto hardwareParam : hardwareParams)
	{
		hardwareParam.second->SetManager(this);
		controls[hardwareParam.second->GetControlID()] = new HardwareHandler(hardwareParam.second);
		logger->Info(Languages::TextLoadedControl, hardwareParam.first, hardwareParam.second->GetName());
	}

	storage->AllLayers(layers);
	for (auto layer : layers)
	{
		logger->Info(Languages::TextLoadedLayer, layer.second->GetID(), layer.second->GetName());
	}
	if (layers.count(LayerUndeletable) != 1)
	{
		string result;
		bool initLayer1 = LayerSave(0, Languages::GetText(Languages::TextLayer1), result);
		if (initLayer1 == false)
		{
			logger->Error(Languages::TextUnableToAddLayer1);
		}
	}

	storage->AllAccessories(accessories);
	for (auto accessory : accessories)
	{
		// We set the protocol MM2 to MM when control is a CS2 or CC-Schnitte
		// FIXME: remove again later 2020-10-27
		if (accessory.second->GetProtocol() == ProtocolMM2
			&& (ControlIsOfHardwareType(accessory.second->GetControlID(), HardwareTypeCS2Udp)
				|| ControlIsOfHardwareType(accessory.second->GetControlID(), HardwareTypeCcSchnitte)))
		{
			accessory.second->SetProtocol(ProtocolMM);
		}
		logger->Info(Languages::TextLoadedAccessory, accessory.second->GetID(), accessory.second->GetName());
	}

	storage->AllFeedbacks(feedbacks);
	for (auto feedback : feedbacks)
	{
		logger->Info(Languages::TextLoadedFeedback, feedback.second->GetID(), feedback.second->GetName());
	}

	storage->AllSignals(signals);
	for (auto signal : signals)
	{
		// We set the protocol MM2 to MM when control is a CS2 or CC-Schnitte
		// FIXME: remove again later 2020-10-27
		if (signal.second->GetProtocol() == ProtocolMM2
			&& (ControlIsOfHardwareType(signal.second->GetControlID(), HardwareTypeCS2Udp)
				|| ControlIsOfHardwareType(signal.second->GetControlID(), HardwareTypeCcSchnitte)))
		{
			signal.second->SetProtocol(ProtocolMM);
		}
		logger->Info(Languages::TextLoadedSignal, signal.second->GetID(), signal.second->GetName());
	}

	storage->AllTracks(tracks);
	for (auto track : tracks)
	{
		logger->Info(Languages::TextLoadedTrack, track.second->GetID(), track.second->GetName());
	}

	storage->AllSwitches(switches);
	for (auto mySwitch : switches)
	{
		// We set the protocol MM2 to MM when control is a CS2 or CC-Schnitte
		// FIXME: remove again later 2020-10-27
		if (mySwitch.second->GetProtocol() == ProtocolMM2
			&& (ControlIsOfHardwareType(mySwitch.second->GetControlID(), HardwareTypeCS2Udp)
				|| ControlIsOfHardwareType(mySwitch.second->GetControlID(), HardwareTypeCcSchnitte)))
		{
			mySwitch.second->SetProtocol(ProtocolMM);
		}
		logger->Info(Languages::TextLoadedSwitch, mySwitch.second->GetID(), mySwitch.second->GetName());
	}

	storage->AllClusters(clusters);
	for (auto cluster : clusters)
	{
		logger->Info(Languages::TextLoadedCluster, cluster.second->GetID(), cluster.second->GetName());
	}

	storage->AllRoutes(routes);
	for (auto route : routes)
	{
		logger->Info(Languages::TextLoadedRoute, route.second->GetID(), route.second->GetName());
	}

	storage->AllLocos(locos);
	for (auto loco : locos)
	{
		// We set the protocol MM2 to MM when control is a CS2 or CC-Schnitte
		// FIXME: remove again later 2020-10-27
		if (loco.second->GetProtocol() == ProtocolMM2
			&& (ControlIsOfHardwareType(loco.second->GetControlID(), HardwareTypeCS2Udp)
				|| ControlIsOfHardwareType(loco.second->GetControlID(), HardwareTypeCcSchnitte)))
		{
			loco.second->SetProtocol(ProtocolMM);
		}
		logger->Info(Languages::TextLoadedLoco, loco.second->GetID(), loco.second->GetName());
	}

	run = true;
	debounceRun = true;
	debounceThread = std::thread(&Manager::DebounceWorker, this);
	InitLocos();
}

Manager::~Manager()
{
	while (!LocoStopAll())
	{
		Utils::Utils::SleepForSeconds(1);
	}

	debounceRun = false;
	debounceThread.join();

	Booster(ControlTypeInternal, BoosterStateStop);

	run = false;
	{
		std::lock_guard<std::mutex> guard(controlMutex);
		for (auto control : controls)
		{
			ControlID controlID = control.first;
			if (controlID < ControlIdFirstHardware)
			{
				delete control.second;
				continue;
			}
			if (hardwareParams.count(controlID) != 1)
			{
				continue;
			}
			HardwareParams* params = hardwareParams.at(controlID);
			if (params == nullptr)
			{
				continue;
			}
			logger->Info(Languages::TextUnloadingControl, controlID, params->GetName());
			delete control.second;
			hardwareParams.erase(controlID);
			if (storage != nullptr)
			{
				logger->Info(Languages::TextSaving, params->GetName());
				storage->Save(*params);
			}
			delete params;
		}
	}

	{
		Storage::TransactionGuard guard(storage);
		DeleteAllMapEntries(locos, locoMutex);
		DeleteAllMapEntries(routes, routeMutex);
		DeleteAllMapEntries(clusters, clusterMutex);
		DeleteAllMapEntries(switches, switchMutex);
		DeleteAllMapEntries(tracks, trackMutex);
		DeleteAllMapEntries(signals, signalMutex);
		DeleteAllMapEntries(feedbacks, feedbackMutex);
		DeleteAllMapEntries(accessories, accessoryMutex);
		DeleteAllMapEntries(layers, layerMutex);
	}

	if (storage == nullptr)
	{
		return;
	}

	delete storage;
	storage = nullptr;
}

/***************************
* Booster                  *
***************************/

void Manager::Booster(const ControlType controlType, const BoosterState state)
{
	if (!run)
	{
		return;
	}
	boosterState = state;
	{
		std::lock_guard<std::mutex> guard(controlMutex);
		for (auto control : controls)
		{
			control.second->Booster(controlType, state);
		}
	}

	if (boosterState != BoosterStateGo || initLocosDone == true)
	{
		return;
	}

	std::async(std::launch::async, InitLocosStatic, this);
	initLocosDone = true;
}

void Manager::InitLocos()
{
	Utils::Utils::SleepForSeconds(1);
	std::lock_guard<std::mutex> guard(locoMutex);
	for (auto loco : locos)
	{
		std::lock_guard<std::mutex> guard(controlMutex);
		for (auto control : controls)
		{
			std::vector<DataModel::LocoFunctionEntry> functions = loco.second->GetFunctionStates();
			control.second->LocoSpeedOrientationFunctions(loco.second, loco.second->GetSpeed(), loco.second->GetOrientation(), functions);
		}
	}
}

/***************************
* Control                  *
***************************/

bool Manager::ControlSave(ControlID controlID,
	const HardwareType& hardwareType,
	const std::string& name,
	const std::string& arg1,
	const std::string& arg2,
	const std::string& arg3,
	const std::string& arg4,
	const std::string& arg5,
	string& result)
{
	if (controlID != ControlIdNone && controlID < ControlIdFirstHardware)
	{
		result = Languages::GetText(Languages::TextInvalidControlID);
		return false;
	}

	HardwareParams* params = GetHardware(controlID);
	bool newControl = false;
	if (params == nullptr)
	{
		params = CreateAndAddControl();
		newControl = true;
	}

	if (params == nullptr)
	{
		result = Languages::GetText(Languages::TextUnableToAddControl);
		return false;
	}

	// if we have a new object we have to update controlID
	controlID = params->GetControlID();

	params->SetName(CheckObjectName(hardwareParams, hardwareMutex, controlID, name.size() == 0 ? "C" : name));
	params->SetHardwareType(hardwareType);
	params->SetArg1(arg1);
	params->SetArg2(arg2);
	params->SetArg3(arg3);
	params->SetArg4(arg4);
	params->SetArg5(arg5);

	if (storage != nullptr)
	{
		storage->Save(*params);
	}

	if (newControl == true)
	{
		std::lock_guard<std::mutex> Guard(controlMutex);
		ControlInterface* control = new HardwareHandler(params);
		if (control == nullptr)
		{
			return false;
		}
		controls[controlID] = control;
		return true;
	}

	ControlInterface* control = GetControl(controlID);
	if (control == nullptr)
	{
		return false;
	}

	control->ReInit(params);
	return true;
}

bool Manager::ControlDelete(ControlID controlID)
{
	{
		std::lock_guard<std::mutex> guard(hardwareMutex);
		if (controlID < ControlIdFirstHardware || hardwareParams.count(controlID) != 1)
		{
			return false;
		}

		HardwareParams* params = hardwareParams.at(controlID);
		if (params == nullptr)
		{
			return false;
		}
		hardwareParams.erase(controlID);
		delete params;
	}
	{
		std::lock_guard<std::mutex> guard(controlMutex);
		if (controls.count(controlID) != 1)
		{
			return false;
		}
		ControlInterface* control = controls.at(controlID);
		if (control == nullptr)
		{
			return false;
		}
		controls.erase(controlID);
		delete control;
	}

	if (storage)
	{
		storage->DeleteHardwareParams(controlID);
	}
	return true;
}

HardwareParams* Manager::GetHardware(const ControlID controlID)
{
	std::lock_guard<std::mutex> guard(hardwareMutex);
	if (hardwareParams.count(controlID) != 1)
	{
		return nullptr;
	}
	return hardwareParams.at(controlID);
}

unsigned int Manager::ControlsOfHardwareType(const HardwareType hardwareType)
{
	std::lock_guard<std::mutex> guard(hardwareMutex);
	unsigned int counter = 0;
	for (auto hardwareParam : hardwareParams)
	{
		if (hardwareParam.second->GetHardwareType() == hardwareType)
		{
			++counter;
		}
	}
	return counter;
}

bool Manager::ControlIsOfHardwareType(const ControlID controlID, const HardwareType hardwareType)
{
	std::lock_guard<std::mutex> guard(hardwareMutex);
	for (auto hardwareParam : hardwareParams)
	{
		if (hardwareParam.second->GetControlID() != controlID)
		{
			continue;
		}
		return hardwareParam.second->GetHardwareType() == hardwareType;
	}
	return false;
}

ControlInterface* Manager::GetControl(const ControlID controlID) const
{
	std::lock_guard<std::mutex> guard(controlMutex);
	if (controls.count(controlID) != 1)
	{
		return nullptr;
	}
	return controls.at(controlID);
}

const std::string Manager::GetControlName(const ControlID controlID)
{
	std::lock_guard<std::mutex> guard(controlMutex);
	if (controls.count(controlID) != 1)
	{
		return unknownControl;
	}
	ControlInterface* control = controls.at(controlID);
	return control->GetName();
}

const std::map<ControlID,std::string> Manager::LocoControlListNames() const
{
	std::map<ControlID,std::string> ret;
	std::lock_guard<std::mutex> guard(hardwareMutex);
	for (auto hardware : hardwareParams)
	{
		std::lock_guard<std::mutex> guard2(controlMutex);
		if (controls.count(hardware.second->GetControlID()) != 1)
		{
			continue;
		}
		ControlInterface* c = controls.at(hardware.second->GetControlID());
		if (c->CanHandle(Hardware::CapabilityLoco) == false)
		{
			continue;
		}
		ret[hardware.first] = hardware.second->GetName();
	}
	return ret;
}

const std::map<ControlID,std::string> Manager::AccessoryControlListNames() const
{
	std::map<ControlID,std::string> ret;
	std::lock_guard<std::mutex> guard(hardwareMutex);
	for (auto hardware : hardwareParams)
	{
		std::lock_guard<std::mutex> guard2(controlMutex);
		if (controls.count(hardware.second->GetControlID()) != 1)
		{
			continue;
		}
		ControlInterface* c = controls.at(hardware.second->GetControlID());
		if (c->CanHandle(Hardware::CapabilityAccessory) == false)
		{
			continue;
		}
		ret[hardware.first] = hardware.second->GetName();
	}
	return ret;
}

const std::map<ControlID,std::string> Manager::FeedbackControlListNames() const
{
	std::map<ControlID,std::string> ret;
	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		if (control.second->GetControlType() != ControlTypeHardware || control.second->CanHandle(Hardware::CapabilityFeedback) == false)
		{
			continue;
		}
		ret[control.first] = control.second->GetShortName();
	}
	return ret;
}

const std::map<ControlID,std::string> Manager::ProgramControlListNames() const
{
	std::map<ControlID,std::string> ret;
	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		if (control.second->GetControlType() != ControlTypeHardware)
		{
			continue;
		}
		if (control.second->CanHandle(Hardware::CapabilityProgram) == false)
		{
			continue;
		}
		ret[control.first] = control.second->GetShortName();
	}
	return ret;
}

const map<string,Hardware::HardwareParams*> Manager::ControlListByName() const
{
	map<string,Hardware::HardwareParams*> out;
	std::lock_guard<std::mutex> guard(hardwareMutex);
	for(auto hardware : hardwareParams)
	{
		out[hardware.second->GetName()] = hardware.second;
	}
	return out;
}

const std::map<std::string, Protocol> Manager::ProtocolsOfControl(const AddressType type, const ControlID controlID) const
{
	std::map<std::string,Protocol> ret;
	{
		const ControlInterface* control = GetControl(controlID);
		if (control == nullptr || control->GetControlType() != ControlTypeHardware)
		{
			ret[ProtocolSymbols[ProtocolNone]] = ProtocolNone;
			return ret;
		}

		const HardwareHandler* hardware = static_cast<const HardwareHandler*>(control);
		if (hardware->GetControlID() != controlID)
		{
			ret[ProtocolSymbols[ProtocolNone]] = ProtocolNone;
			return ret;
		}

		std::vector<Protocol> protocols;
		if (type == AddressTypeLoco)
		{
			hardware->LocoProtocols(protocols);
		}
		else
		{
			hardware->AccessoryProtocols(protocols);
		}
		for (auto protocol : protocols)
		{
			ret[ProtocolSymbols[protocol]] = protocol;
		}
	}
	return ret;
}

/***************************
* Loco                     *
***************************/

Loco* Manager::GetLoco(const LocoID locoID) const
{
	std::lock_guard<std::mutex> guard(locoMutex);
	if (locos.count(locoID) != 1)
	{
		return nullptr;
	}
	return locos.at(locoID);
}

LocoConfig Manager::GetLocoOfConfigByMatchKey(const ControlID controlId, const string& matchKey) const
{
	ControlInterface* control = GetControl(controlId);
	if (control == nullptr)
	{
		return LocoConfig();
	}
	return control->GetLocoByMatch(matchKey);
}

Loco* Manager::GetLocoByMatchKey(const ControlID controlId, const string& matchKey) const
{
	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto loco : locos)
	{
		Loco* locoConfig = loco.second;
		if (locoConfig->GetControlID() == controlId && locoConfig->GetMatchKey().compare(matchKey) == 0)
		{
			return loco.second;
		}
	}
	return nullptr;
}

void Manager::LocoRemoveMatchKey(const LocoID locoId)
{
	Loco* loco = GetLoco(locoId);
	if (loco == nullptr)
	{
		return;
	}
	loco->ClearMatchKey();
}

void Manager::LocoReplaceMatchKey(const LocoID locoId, const std::string& newMatchKey)
{
	Loco* loco = GetLoco(locoId);
	if (loco == nullptr)
	{
		return;
	}
	loco->SetMatchKey(newMatchKey);
}

const map<string,LocoConfig> Manager::GetUnmatchedLocosOfControl(const ControlID controlId) const
{
	ControlInterface* control = GetControl(controlId);
	map<string,LocoConfig> out;
	if (control == nullptr || !control->CanHandle(Hardware::CapabilityLocoDatabase))
	{
		return out;
	}
	out = control->GetUnmatchedLocos();
	out[""].SetName("");
	return out;
}

Loco* Manager::GetLoco(const ControlID controlID, const Protocol protocol, const Address address) const
{
	std::lock_guard<std::mutex> guard(locoMutex);
	for (auto loco : locos)
	{
		if (loco.second->GetControlID() == controlID
			&& loco.second->GetProtocol() == protocol
			&& loco.second->GetAddress() == address)
		{
			return loco.second;
		}
	}
	return nullptr;
}

const std::string& Manager::GetLocoName(const LocoID locoID) const
{
	std::lock_guard<std::mutex> guard(locoMutex);
	if (locos.count(locoID) != 1)
	{
		return unknownLoco;
	}
	return locos.at(locoID)->GetName();
}

const map<string,LocoID> Manager::LocoListFree() const
{
	map<string,LocoID> out;
	std::lock_guard<std::mutex> guard(locoMutex);
	for(auto loco : locos)
	{
		if (loco.second->IsInUse() == false)
		{
			out[loco.second->GetName()] = loco.second->GetID();
		}
	}
	return out;
}

const map<string,DataModel::LocoConfig> Manager::LocoListByName() const
{
	map<string,DataModel::LocoConfig> out;
	{
		std::lock_guard<std::mutex> guard(locoMutex);
		for (auto loco : locos)
		{
			out[loco.second->GetName()] = *(loco.second);
		}
	}
	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		control.second->AddUnmatchedLocos(out);
	}

	return out;
}

const map<string,LocoID> Manager::LocoIdsByName() const
{
	map<string,LocoID> out;
	{
		std::lock_guard<std::mutex> guard(locoMutex);
		for (auto loco : locos)
		{
			Loco* locoEntry = loco.second;
			out[locoEntry->GetName()] = locoEntry->GetID();
		}
	}
	return out;
}

bool Manager::LocoSave(LocoID locoID,
	const string& name,
	const ControlID controlID,
	const std::string& matchKey,
	const Protocol protocol,
	const Address address,
	const Length length,
	const bool pushpull,
	const Speed maxSpeed,
	const Speed travelSpeed,
	const Speed reducedSpeed,
	const Speed creepingSpeed,
	const std::vector<DataModel::LocoFunctionEntry>& locoFunctions,
	const std::vector<DataModel::Relation*>& slaves,
	string& result)
{
	if (!CheckControlLocoProtocolAddress(controlID, protocol, address, result))
	{
		return false;
	}

	Loco* loco = GetLoco(locoID);
	if (loco == nullptr)
	{
		loco = CreateAndAddObject(locos, locoMutex);
	}

	if (loco == nullptr)
	{
		result = Languages::GetText(Languages::TextUnableToAddLoco);
		return false;
	}

	// if we have a new object we have to update locoID
	locoID = loco->GetID();

	loco->SetName(CheckObjectName(locos, locoMutex, locoID, name.size() == 0 ? "L" : name));
	loco->SetControlID(controlID);
	loco->SetMatchKey(matchKey);
	loco->SetProtocol(protocol);
	loco->SetAddress(address);
	loco->SetLength(length);
	loco->SetPushpull(pushpull);
	loco->SetMaxSpeed(maxSpeed);
	loco->SetTravelSpeed(travelSpeed);
	loco->SetReducedSpeed(reducedSpeed);
	loco->SetCreepingSpeed(creepingSpeed);
	loco->ConfigureFunctions(locoFunctions);
	loco->AssignSlaves(slaves);

	// save in db
	if (storage)
	{
		storage->Save(*loco);
	}
	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		control.second->LocoSettings(locoID, name, matchKey);
	}
	return true;
}

bool Manager::LocoDelete(const LocoID locoID, string& result)
{
	Loco* loco = nullptr;
	{
		std::lock_guard<std::mutex> guard(locoMutex);
		if (locoID == LocoNone || locos.count(locoID) != 1)
		{
			result = Languages::GetText(Languages::TextLocoDoesNotExist);
			return false;
		}

		loco = locos.at(locoID);
		if (loco == nullptr)
		{
			result = Languages::GetText(Languages::TextLocoDoesNotExist);
			return false;
		}

		if (loco->IsInUse())
		{
			result = Logger::Logger::Format(Languages::GetText(Languages::TextLocoIsInUse), loco->GetName());
			return false;
		}

		locos.erase(locoID);
	}

	if (storage)
	{
		storage->DeleteLoco(locoID);
	}
	const string& name = loco->GetName();
	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		control.second->LocoDelete(locoID, name);
	}
	delete loco;
	return true;
}

bool Manager::LocoProtocolAddress(const LocoID locoID, ControlID& controlID, Protocol& protocol, Address& address) const
{
	std::lock_guard<std::mutex> guard(locoMutex);
	if (locos.count(locoID) != 1)
	{
		controlID = 0;
		protocol = ProtocolNone;
		address = 0;
		return false;
	}
	Loco* loco = locos.at(locoID);
	if (loco == nullptr)
	{
		return false;
	}
	controlID = loco->GetControlID();
	protocol = loco->GetProtocol();
	address = loco->GetAddress();
	return true;
}

void Manager::LocoSpeed(const ControlType controlType, const ControlID controlID, const Protocol protocol, const Address address, const Speed speed)
{
	Loco* loco = GetLoco(controlID, protocol, address);
	if (loco == nullptr)
	{
		return;
	}
	LocoSpeed(controlType, loco, speed);
}

bool Manager::LocoSpeed(const ControlType controlType, const LocoID locoID, const Speed speed, const bool withSlaves)
{
	Loco* loco = GetLoco(locoID);
	return LocoSpeed(controlType, loco, speed, withSlaves);
}

bool Manager::LocoSpeed(const ControlType controlType, Loco* loco, const Speed speed, const bool withSlaves)
{
	if (loco == nullptr)
	{
		return false;
	}
	Speed s = speed;
	if (speed > MaxSpeed)
	{
		s = MaxSpeed;
	}
	const string& locoName = loco->GetName();
	logger->Info(Languages::TextLocoSpeedIs, locoName, s);
	loco->SetSpeed(s, withSlaves);
	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		control.second->LocoSpeed(controlType, loco, s);
	}
	return true;
}

Speed Manager::LocoSpeed(const LocoID locoID) const
{
	Loco* loco = GetLoco(locoID);
	if (loco == nullptr)
	{
		return MinSpeed;
	}
	return loco->GetSpeed();
}

void Manager::LocoOrientation(const ControlType controlType, const ControlID controlID, const Protocol protocol, const Address address, const Orientation orientation)
{
	Loco* loco = GetLoco(controlID, protocol, address);
	if (loco == nullptr)
	{
		return;
	}
	LocoOrientation(controlType, loco, orientation);
}

void Manager::LocoOrientation(const ControlType controlType, const LocoID locoID, const Orientation orientation)
{
	Loco* loco = GetLoco(locoID);
	LocoOrientation(controlType, loco, orientation);
}

void Manager::LocoOrientation(const ControlType controlType, Loco* loco, const Orientation orientation)
{
	if (loco == nullptr)
	{
		return;
	}
	loco->SetOrientation(orientation);
	logger->Info(orientation ? Languages::TextLocoDirectionOfTravelIsRight : Languages::TextLocoDirectionOfTravelIsLeft, loco->GetName());
	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		control.second->LocoOrientation(controlType, loco, orientation);
	}
}

void Manager::LocoFunctionState(const ControlType controlType,
	const ControlID controlID,
	const Protocol protocol,
	const Address address,
	const DataModel::LocoFunctionNr function,
	const DataModel::LocoFunctionState on)
{
	Loco* loco = GetLoco(controlID, protocol, address);
	if (loco == nullptr)
	{
		return;
	}
	LocoFunctionState(controlType, loco, function, on);
}

void Manager::LocoFunctionState(const ControlType controlType,
	const LocoID locoID,
	const DataModel::LocoFunctionNr function,
	const DataModel::LocoFunctionState on)
{
	Loco* loco = GetLoco(locoID);
	LocoFunctionState(controlType, loco, function, on);
}

void Manager::LocoFunctionState(const ControlType controlType,
	Loco* loco,
	const DataModel::LocoFunctionNr function,
	const DataModel::LocoFunctionState on)
{
	if (loco == nullptr)
	{
		return;
	}

	loco->SetFunctionState(function, on);
	logger->Info(on ? Languages::TextLocoFunctionIsOn : Languages::TextLocoFunctionIsOff, loco->GetName(), function);
	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		control.second->LocoFunction(controlType, loco, function, on);
	}
}

/***************************
* Accessory                *
***************************/

void Manager::AccessoryState(const ControlType controlType, const ControlID controlID, const Protocol protocol, const Address address, const DataModel::AccessoryState state)
{
	Accessory* accessory = GetAccessory(controlID, protocol, address);
	if (accessory != nullptr)
	{
		AccessoryState(controlType, accessory, accessory->CalculateInvertedAccessoryState(state), true);
		return;
	}

	Switch* mySwitch = GetSwitch(controlID, protocol, address);
	if (mySwitch != nullptr)
	{
		SwitchState(controlType, mySwitch, mySwitch->CalculateInvertedAccessoryState(state), true);
		return;
	}

	Signal* signal = GetSignal(controlID, protocol, address);
	if (signal != nullptr)
	{
		SignalState(controlType, signal, signal->CalculateInvertedAccessoryState(state), true);
		return;
	}
}

bool Manager::AccessoryState(const ControlType controlType, const AccessoryID accessoryID, const DataModel::AccessoryState state, const bool force)
{
	if (boosterState == BoosterStateStop)
	{
		return false;
	}
	Accessory* accessory = GetAccessory(accessoryID);
	AccessoryState(controlType, accessory, state, force);
	return true;
}

void Manager::AccessoryState(const ControlType controlType, Accessory* accessory, const DataModel::AccessoryState state, const bool force)
{
	if (accessory == nullptr)
	{
		return;
	}

	if (force == false && accessory->IsInUse())
	{
		logger->Warning(Languages::TextAccessoryIsLocked, accessory->GetName());
		return;
	}

	accessory->SetAccessoryState(state);

	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		control.second->AccessoryState(controlType, accessory);
	}
}

Accessory* Manager::GetAccessory(const AccessoryID accessoryID) const
{
	std::lock_guard<std::mutex> guard(accessoryMutex);
	if (accessories.count(accessoryID) != 1)
	{
		return nullptr;
	}
	return accessories.at(accessoryID);
}

Accessory* Manager::GetAccessory(const ControlID controlID, const Protocol protocol, const Address address) const
{
	std::lock_guard<std::mutex> guard(accessoryMutex);
	for (auto accessory : accessories)
	{
		if (accessory.second->GetControlID() == controlID
			&& accessory.second->GetProtocol() == protocol
			&& accessory.second->GetAddress() == address)
		{
			return accessory.second;
		}
	}
	return nullptr;
}

const std::string& Manager::GetAccessoryName(const AccessoryID accessoryID) const
{
	std::lock_guard<std::mutex> guard(accessoryMutex);
	if (accessories.count(accessoryID) != 1)
	{
		return unknownAccessory;
	}
	return accessories.at(accessoryID)->GetName();
}

bool Manager::CheckAccessoryPosition(const Accessory* accessory, const LayoutPosition posX, const LayoutPosition posY, const LayoutPosition posZ, string& result) const
{
	if (accessory == nullptr)
	{
		return CheckPositionFree(posX, posY, posZ, DataModel::LayoutItem::Width1, DataModel::LayoutItem::Height1, DataModel::LayoutItem::Rotation0, result);
	}

	if (accessory->HasPosition(posX, posY, posZ))
	{
		return true;
	}
	return CheckPositionFree(posX, posY, posZ, DataModel::LayoutItem::Width1, DataModel::LayoutItem::Height1, DataModel::LayoutItem::Rotation0, result);
}

bool Manager::AccessorySave(AccessoryID accessoryID, const string& name, const LayoutPosition posX, const LayoutPosition posY, const LayoutPosition posZ, const ControlID controlID, const Protocol protocol, const Address address, const DataModel::AccessoryType type, const DataModel::AccessoryPulseDuration duration, const bool inverted, string& result)
{
	if (!CheckControlAccessoryProtocolAddress(controlID, protocol, address, result))
	{
		return false;
	}

	Accessory* accessory = GetAccessory(accessoryID);
	if (!CheckAccessoryPosition(accessory, posX, posY, posZ, result))
	{
		return false;
	}

	if (accessory == nullptr)
	{
		accessory = CreateAndAddObject(accessories, accessoryMutex);
	}

	if (accessory == nullptr)
	{
		result = Languages::GetText(Languages::TextUnableToAddAccessory);
		return false;
	}

	// if we have a new object we have to update accessoryID
	accessoryID = accessory->GetID();

	// update existing accessory
	accessory->SetName(CheckObjectName(accessories, accessoryMutex, accessoryID, name.size() == 0 ? "A" : name));
	accessory->SetPosX(posX);
	accessory->SetPosY(posY);
	accessory->SetPosZ(posZ);
	accessory->SetControlID(controlID);
	accessory->SetProtocol(protocol);
	accessory->SetAddress(address);
	accessory->SetType(type);
	accessory->SetAccessoryPulseDuration(duration);
	accessory->SetInverted(inverted);

	// save in db
	if (storage)
	{
		storage->Save(*accessory);
	}

	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		control.second->AccessorySettings(accessoryID, name);
	}
	return true;
}

const map<string,DataModel::Accessory*> Manager::AccessoryListByName() const
{
	map<string,DataModel::Accessory*> out;
	std::lock_guard<std::mutex> guard(accessoryMutex);
	for(auto accessory : accessories)
	{
		out[accessory.second->GetName()] = accessory.second;
	}
	return out;
}

bool Manager::AccessoryDelete(const AccessoryID accessoryID,
	string& result)
{
	Accessory* accessory = nullptr;
	{
		std::lock_guard<std::mutex> guard(accessoryMutex);
		if (accessoryID == AccessoryNone || accessories.count(accessoryID) != 1)
		{
			result = Languages::GetText(Languages::TextAccessoryDoesNotExist);
			return false;
		}

		accessory = accessories.at(accessoryID);
		if (accessory == nullptr)
		{
			result = Languages::GetText(Languages::TextAccessoryDoesNotExist);
			return false;
		}

		ObjectIdentifier accessoryIdentifier(ObjectTypeAccessory, accessoryID);
		if (ObjectIsPartOfRoute(accessoryIdentifier, accessory, result))
		{
			return false;
		}

		accessories.erase(accessoryID);
	}

	if (storage)
	{
		storage->DeleteAccessory(accessoryID);
	}
	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		control.second->AccessoryDelete(accessoryID, accessory->GetName());
	}
	delete accessory;
	return true;
}

bool Manager::AccessoryRelease(const AccessoryID accessoryID)
{
	Accessory* accessory = GetAccessory(accessoryID);
	if (accessory == nullptr)
	{
		return false;
	}
	LocoID locoID = accessory->GetLoco();
	return accessory->Release(logger, locoID);
}

/***************************
* Feedback                 *
***************************/

void Manager::FeedbackState(const ControlID controlID, const FeedbackPin pin, const DataModel::Feedback::FeedbackState state)
{
	Feedback* feedback = GetFeedback(controlID, pin);
	if (feedback != nullptr)
	{
		FeedbackState(feedback, state);
		return;
	}

	if (GetAutoAddFeedback() == false)
	{
		return;
	}

	string name = "Feedback auto added " + std::to_string(controlID) + "/" + std::to_string(pin);
	logger->Info(Languages::TextAddingFeedback, name);
	string result;

	FeedbackSave(FeedbackNone, name, DataModel::LayoutItem::VisibleNo, 0, 0, 0, controlID, pin, false, result);
}

void Manager::FeedbackState(const FeedbackID feedbackID, const DataModel::Feedback::FeedbackState state)
{
	Feedback* feedback = GetFeedback(feedbackID);
	if (feedback == nullptr)
	{
		return;
	}
	FeedbackState(feedback, state);
}

void Manager::FeedbackPublishState(const Feedback* feedback)
{
	if (feedback == nullptr)
	{
		return;
	}
	DataModel::Feedback::FeedbackState state = feedback->GetState();
	const string& feedbackName = feedback->GetName();
	logger->Info(state ? Languages::TextFeedbackStateIsOn : Languages::TextFeedbackStateIsOff, feedbackName);
	const FeedbackID feedbackID = feedback->GetID();
	{
		std::lock_guard<std::mutex> guard(controlMutex);
		for (auto control : controls)
		{
			control.second->FeedbackState(feedbackName, feedbackID, state);
		}
	}
}

Feedback* Manager::GetFeedback(const FeedbackID feedbackID) const
{
	std::lock_guard<std::mutex> guard(feedbackMutex);
	return GetFeedbackUnlocked(feedbackID);
}

Feedback* Manager::GetFeedbackUnlocked(const FeedbackID feedbackID) const
{
	if (feedbacks.count(feedbackID) != 1)
	{
		return nullptr;
	}
	return feedbacks.at(feedbackID);
}

Feedback* Manager::GetFeedback(const ControlID controlID, const FeedbackPin pin) const
{
	std::lock_guard<std::mutex> guard(feedbackMutex);
	for (auto feedback : feedbacks)
	{
		if (feedback.second->GetControlID() == controlID
			&& feedback.second->GetPin() == pin)
		{
			return feedback.second;
		}
	}
	return nullptr;
}

const std::string& Manager::GetFeedbackName(const FeedbackID feedbackID) const
{
	std::lock_guard<std::mutex> guard(feedbackMutex);
	if (feedbacks.count(feedbackID) != 1)
	{
		return unknownFeedback;
	}
	return feedbacks.at(feedbackID)->GetName();
}

bool Manager::CheckFeedbackPosition(const Feedback* feedback, const LayoutPosition posX, const LayoutPosition posY, const LayoutPosition posZ, string& result) const
{
	if (feedback == nullptr)
	{
		return CheckPositionFree(posX, posY, posZ, DataModel::LayoutItem::Width1, DataModel::LayoutItem::Height1, DataModel::LayoutItem::Rotation0, result);
	}

	if (feedback->GetVisible() == DataModel::LayoutItem::VisibleNo)
	{
		return true;
	}

	if (feedback->HasPosition(posX, posY, posZ))
	{
		return true;
	}
	return CheckPositionFree(posX, posY, posZ, DataModel::LayoutItem::Width1, DataModel::LayoutItem::Height1, DataModel::LayoutItem::Rotation0, result);
}

bool Manager::FeedbackSave(FeedbackID feedbackID, const std::string& name, const Visible visible, const LayoutPosition posX, const LayoutPosition posY, const LayoutPosition posZ, const ControlID controlID, const FeedbackPin pin, const bool inverted, string& result)
{
	Feedback* feedback = GetFeedback(feedbackID);
	if (visible && !CheckFeedbackPosition(feedback, posX, posY, posZ, result))
	{
		return false;
	}

	if (feedback == nullptr)
	{
		feedback = CreateAndAddObject(feedbacks, feedbackMutex);
	}

	if (feedback == nullptr)
	{
		result = Languages::GetText(Languages::TextUnableToAddFeedback);
		return false;
	}

	// if we have a new object we have to update feedbackID
	feedbackID = feedback->GetID();

	feedback->SetName(CheckObjectName(feedbacks, feedbackMutex, feedbackID, name.size() == 0 ? "F" : name));
	feedback->SetVisible(visible);
	feedback->SetPosX(posX);
	feedback->SetPosY(posY);
	feedback->SetPosZ(posZ);
	feedback->SetControlID(controlID);
	feedback->SetPin(pin);
	feedback->SetInverted(inverted);

	// save in db
	if (storage)
	{
		storage->Save(*feedback);
	}
	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		control.second->FeedbackSettings(feedbackID, name);
	}
	return true;
}

const map<string,DataModel::Feedback*> Manager::FeedbackListByName() const
{
	map<string,DataModel::Feedback*> out;
	std::lock_guard<std::mutex> guard(feedbackMutex);
	for(auto feedback : feedbacks)
	{
		out[feedback.second->GetName()] = feedback.second;
	}
	return out;
}

const map<string,FeedbackID> Manager::FeedbacksOfTrack(const ObjectIdentifier& identifier) const
{
	map<string,FeedbackID> out;
	TrackBase* track = GetTrackBase(identifier);
	if (track == nullptr)
	{
		return out;
	}
	vector<FeedbackID> feedbacksOfTrack = track->GetFeedbacks();
	for (auto feedbackID : feedbacksOfTrack)
	{
		Feedback* feedback = GetFeedback(feedbackID);
		if (feedback == nullptr)
		{
			continue;
		}
		out[feedback->GetName()] = feedbackID;
	}
	return out;
}

bool Manager::FeedbackDelete(const FeedbackID feedbackID,
	string& result)
{
	Feedback* feedback = nullptr;
	{
		std::lock_guard<std::mutex> guard(feedbackMutex);
		if (feedbackID == FeedbackNone || feedbacks.count(feedbackID) != 1)
		{
			result = Languages::GetText(Languages::TextFeedbackDoesNotExist);
			return false;
		}

		feedback = feedbacks.at(feedbackID);
		TrackBase* trackBase = feedback->GetTrack();
		if (trackBase != nullptr)
		{
			result = Logger::Logger::Format(Languages::GetText(Languages::TextFeedbackIsUsedByTrack), feedback->GetName(), trackBase->GetMyName());
			return false;
		}

		feedbacks.erase(feedbackID);
	}

	if (storage)
	{
		storage->DeleteFeedback(feedbackID);
	}
	const string& name = feedback->GetName();
	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		control.second->FeedbackDelete(feedbackID, name);
	}
	delete feedback;
	return true;
}

/***************************
* Track                    *
***************************/

TrackBase* Manager::GetTrackBase(const ObjectIdentifier& identifier) const
{
	switch (identifier.GetObjectType())
	{
		case ObjectTypeTrack:
			return GetTrack(identifier.GetObjectID());

		case ObjectTypeSignal:
			return GetSignal(identifier.GetObjectID());

		default:
			return nullptr;
	}
}

Track* Manager::GetTrack(const TrackID trackID) const
{
	std::lock_guard<std::mutex> guard(trackMutex);
	if (tracks.count(trackID) != 1)
	{
		return nullptr;
	}
	return tracks.at(trackID);
}

const std::string& Manager::GetTrackName(const TrackID trackID) const
{
	if (tracks.count(trackID) != 1)
	{
		return unknownTrack;
	}
	return tracks.at(trackID)->GetName();
}

const map<string,DataModel::Track*> Manager::TrackListByName() const
{
	map<string,DataModel::Track*> out;
	std::lock_guard<std::mutex> guard(trackMutex);
	for(auto track : tracks)
	{
		out[track.second->GetName()] = track.second;
	}
	return out;
}

const map<string,TrackID> Manager::TrackListIdByName() const
{
	map<string,TrackID> out;
	std::lock_guard<std::mutex> guard(trackMutex);
	for(auto track : tracks)
	{
		out[track.second->GetName()] = track.second->GetID();
	}
	return out;
}

// FIXME: replace with TrackListByName later. 2021-02-10
const map<string,ObjectIdentifier> Manager::TrackBaseListIdentifierByName() const
{
	map<string,ObjectIdentifier> out;
	{
		std::lock_guard<std::mutex> guard(trackMutex);
		for (auto track : tracks)
		{
			out[track.second->GetName()] = ObjectIdentifier(ObjectTypeTrack, track.second->GetID());
		}
	}
	return out;
}

bool Manager::CheckTrackPosition(const Track* track,
	const LayoutPosition posX,
	const LayoutPosition posY,
	const LayoutPosition posZ,
	const LayoutItemSize height,
	const LayoutRotation rotation,
	string& result) const
{
	LayoutPosition x1;
	LayoutPosition y1;
	LayoutPosition z1 = posZ;
	LayoutItemSize w1;
	LayoutItemSize h1;
	bool ret = DataModel::LayoutItem::MapPosition(posX, posY, DataModel::LayoutItem::Width1, height, rotation, x1, y1, w1, h1);
	if (ret == false)
	{
		result = Languages::GetText(Languages::TextUnableToCalculatePosition);
		return false;
	}

	LayoutPosition x2 = 0;
	LayoutPosition y2 = 0;
	LayoutPosition z2 = 0;
	LayoutItemSize w2 = 0;
	LayoutItemSize h2 = 0;

	if (track != nullptr)
	{
		z2 = track->GetPosZ();
		ret = DataModel::LayoutItem::MapPosition(track->GetPosX(), track->GetPosY(), DataModel::LayoutItem::Width1, track->GetHeight(), track->GetRotation(), x2, y2, w2, h2);
		if (ret == false)
		{
			result = Languages::GetText(Languages::TextUnableToCalculatePosition);
			return false;
		}
	}

	for(LayoutPosition ix = x1; ix < x1 + w1; ++ix)
	{
		for(LayoutPosition iy = y1; iy < y1 + h1; ++iy)
		{
			ret = (ix >= x2 && ix < x2 + w2 && iy >= y2 && iy < y2 + h2 && z1 == z2);
			if (ret == true)
			{
				continue;
			}

			ret = CheckPositionFree(ix, iy, z1, result);
			if (ret == false)
			{
				return false;
			}
		}
	}
	return true;
}

const std::vector<FeedbackID> Manager::CleanupAndCheckFeedbacksForTrack(const ObjectIdentifier& identifier, const std::vector<FeedbackID>& newFeedbacks)
{
	Storage::TransactionGuard guard(storage);
	{
		std::lock_guard<std::mutex> feedbackguard(feedbackMutex);
		for (auto feedback : feedbacks)
		{
			if (feedback.second->CompareRelatedObject(identifier) == false)
			{
				continue;
			}

			feedback.second->ClearRelatedObject();
			if (storage != nullptr)
			{
				storage->Save(*feedback.second);
			}
		}
	}

	std::vector<FeedbackID> checkedFeedbacks;
	for (auto feedbackID : newFeedbacks)
	{
		Feedback* feedback = GetFeedback(feedbackID);
		if (feedback == nullptr)
		{
			continue;
		}
		if (feedback->IsRelatedObjectSet())
		{
			continue;
		}
		checkedFeedbacks.push_back(feedbackID);
		feedback->SetRelatedObject(identifier);
		if (storage != nullptr)
		{
			storage->Save(*feedback);
		}
	}
	return checkedFeedbacks;
}

bool Manager::TrackSave(TrackID trackID,
	const std::string& name,
	const bool showName,
	const LayoutPosition posX,
	const LayoutPosition posY,
	const LayoutPosition posZ,
	const LayoutItemSize height,
	const LayoutRotation rotation,
	const DataModel::TrackType trackType,
	const vector<FeedbackID>& newFeedbacks,
	const vector<Relation*>& newSignals,
	const DataModel::SelectRouteApproach selectRouteApproach,
	const bool allowLocoTurn,
	const bool releaseWhenFree,
	string& result)
{
	Track* track = GetTrack(trackID);
	if (!CheckTrackPosition(track, posX, posY, posZ, height, rotation, result))
	{
		return false;
	}

	if (track == nullptr)
	{
		track = CreateAndAddObject(tracks, trackMutex);
	}

	if (track == nullptr)
	{
		result = Languages::GetText(Languages::TextUnableToAddTrack);
		return false;
	}

	// if we have a new object we have to update trackID
	trackID = track->GetID();

	// update existing track
	track->SetName(CheckObjectName(tracks, trackMutex, trackID, name.size() == 0 ? "T" : name));
	track->SetShowName(showName);
	track->SetHeight(height);
	track->SetRotation(rotation);
	track->SetPosX(posX);
	track->SetPosY(posY);
	track->SetPosZ(posZ);
	track->SetTrackType(trackType);
	track->Feedbacks(CleanupAndCheckFeedbacksForTrack(ObjectIdentifier(ObjectTypeTrack, trackID), newFeedbacks));
	track->AssignSignals(newSignals);
	track->SetSelectRouteApproach(selectRouteApproach);
	track->SetAllowLocoTurn(allowLocoTurn);
	track->SetReleaseWhenFree(releaseWhenFree);

	// save in db
	if (storage)
	{
		storage->Save(*track);
	}

	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		control.second->TrackSettings(trackID, name);
	}
	return true;
}

bool Manager::TrackDelete(const TrackID trackID,
	string& result)
{
	Track* track = nullptr;
	{
		std::lock_guard<std::mutex> guard(trackMutex);
		if (trackID == TrackNone || tracks.count(trackID) != 1)
		{
			result = Languages::GetText(Languages::TextTrackDoesNotExist);
			return false;
		}

		track = tracks.at(trackID);
		if (track == nullptr)
		{
			result = Languages::GetText(Languages::TextTrackDoesNotExist);
			return false;
		}

		if (track->IsInUse())
		{
			result = Logger::Logger::Format(Languages::GetText(Languages::TextTrackIsUsedByLoco), track->GetName(), GetLocoName(track->GetLoco()));
			return false;
		}

		ObjectIdentifier trackIdentifier(ObjectTypeTrack, trackID);
		Route* route = GetFirstRouteFromOrToTrackBase(trackIdentifier);
		if (route != nullptr)
		{
			result = Logger::Logger::Format(Languages::GetText(Languages::TextTrackIsUsedByRoute), track->GetName(), route->GetName());
			return false;
		}

		if (ObjectIsPartOfRoute(trackIdentifier, track, result))
		{
			return false;
		}

		FeedbackID feedbackId = track->GetFirstFeedbackId();
		if (feedbackId != FeedbackNone)
		{
			result = Logger::Logger::Format(Languages::GetText(Languages::TextTrackHasAssociatedFeedback), track->GetName(), GetFeedbackName(feedbackId));
			return false;
		}

		tracks.erase(trackID);
	}

	if (storage)
	{
		storage->DeleteTrack(trackID);
	}
	const string& name = track->GetName();
	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		control.second->TrackDelete(trackID, name);
	}

	Cluster* cluster = track->GetCluster();
	if (cluster != nullptr)
	{
		cluster->DeleteTrack(track);
	}

	delete track;
	return true;
}

/***************************
* Switch                   *
***************************/

bool Manager::SwitchState(const ControlType controlType, const SwitchID switchID, const DataModel::AccessoryState state, const bool force)
{
	if (boosterState == BoosterStateStop)
	{
		return false;
	}

	Switch* mySwitch = GetSwitch(switchID);
	SwitchState(controlType, mySwitch, state, force);
	return true;
}

void Manager::SwitchState(const ControlType controlType, Switch* mySwitch, const DataModel::AccessoryState state, const bool force)
{
	if (mySwitch == nullptr)
	{
		return;
	}

	if (force == false && mySwitch->IsInUse())
	{
		logger->Warning(Languages::TextSwitchIsLocked, mySwitch->GetName());
		return;
	}

	mySwitch->SetAccessoryState(state);

	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		control.second->SwitchState(controlType, mySwitch);
	}
}

Switch* Manager::GetSwitch(const SwitchID switchID) const
{
	std::lock_guard<std::mutex> guard(switchMutex);
	if (switches.count(switchID) != 1)
	{
		return nullptr;
	}
	return switches.at(switchID);
}

Switch* Manager::GetSwitch(const ControlID controlID, const Protocol protocol, const Address address) const
{
	std::lock_guard<std::mutex> guard(switchMutex);
	for (auto mySwitch : switches)
	{
		if (mySwitch.second->GetControlID() == controlID
			&& mySwitch.second->GetProtocol() == protocol
			&& mySwitch.second->GetAddress() == address)
		{
			return mySwitch.second;
		}
	}
	return nullptr;
}

const std::string& Manager::GetSwitchName(const SwitchID switchID) const
{
	if (switches.count(switchID) != 1)
	{
		return unknownSwitch;
	}
	return switches.at(switchID)->GetName();
}

bool Manager::CheckSwitchPosition(const Switch* mySwitch, const LayoutPosition posX, const LayoutPosition posY, const LayoutPosition posZ, string& result) const
{
	if (mySwitch == nullptr)
	{
		return CheckPositionFree(posX, posY, posZ, DataModel::LayoutItem::Width1, DataModel::LayoutItem::Height1, DataModel::LayoutItem::Rotation0, result);
	}

	if (mySwitch->HasPosition(posX, posY, posZ))
	{
		return true;
	}
	return CheckPositionFree(posX, posY, posZ, DataModel::LayoutItem::Width1, DataModel::LayoutItem::Height1, DataModel::LayoutItem::Rotation0, result);
}

bool Manager::SwitchSave(SwitchID switchID,
	const string& name,
	const LayoutPosition posX,
	const LayoutPosition posY,
	const LayoutPosition posZ,
	const LayoutRotation rotation,
	const ControlID controlID,
	const Protocol protocol,
	const Address address,
	const DataModel::AccessoryType type,
	const DataModel::AccessoryPulseDuration duration,
	const bool inverted, string& result)
{
	if (!CheckControlAccessoryProtocolAddress(controlID, protocol, address, result))
	{
		return false;
	}

	Switch* mySwitch = GetSwitch(switchID);
	if (!CheckSwitchPosition(mySwitch, posX, posY, posZ, result))
	{
		return false;
	}

	if (mySwitch == nullptr)
	{
		mySwitch = CreateAndAddObject(switches, switchMutex);
	}

	if (mySwitch == nullptr)
	{
		result = Languages::GetText(Languages::TextUnableToAddSwitch);
		return false;
	}

	// if we have a new object we have to update switchID
	switchID = mySwitch->GetID();

	// update existing switch
	mySwitch->SetName(CheckObjectName(switches, switchMutex, switchID, name.size() == 0 ? "S" : name));
	mySwitch->SetPosX(posX);
	mySwitch->SetPosY(posY);
	mySwitch->SetPosZ(posZ);
	mySwitch->SetRotation(rotation);
	mySwitch->SetControlID(controlID);
	mySwitch->SetProtocol(protocol);
	mySwitch->SetAddress(address);
	mySwitch->SetType(type);
	mySwitch->SetAccessoryPulseDuration(duration);
	mySwitch->SetInverted(inverted);

	// save in db
	if (storage)
	{
		storage->Save(*mySwitch);
	}

	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		control.second->SwitchSettings(switchID, name);
	}
	return true;
}

bool Manager::SwitchDelete(const SwitchID switchID,
	string& result)
{
	Switch* mySwitch = nullptr;
	{
		std::lock_guard<std::mutex> guard(switchMutex);
		if (switchID == SwitchNone || switches.count(switchID) != 1)
		{
			result = Languages::GetText(Languages::TextSwitchDoesNotExist);
			return false;
		}

		mySwitch = switches.at(switchID);
		if (mySwitch == nullptr)
		{
			result = Languages::GetText(Languages::TextSwitchDoesNotExist);
			return false;
		}

		ObjectIdentifier switchIdentifier(ObjectTypeSwitch, switchID);
		if (ObjectIsPartOfRoute(switchIdentifier, mySwitch, result))
		{
			return false;
		}
		switches.erase(switchID);
	}

	if (storage)
	{
		storage->DeleteSwitch(switchID);
	}

	const string& switchName = mySwitch->GetName();
	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		control.second->SwitchDelete(switchID, switchName);
	}
	delete mySwitch;
	return true;
}

const map<string,DataModel::Switch*> Manager::SwitchListByName() const
{
	map<string,DataModel::Switch*> out;
	std::lock_guard<std::mutex> guard(switchMutex);
	for(auto mySwitch : switches)
	{
		out[mySwitch.second->GetName()] = mySwitch.second;
	}
	return out;
}

bool Manager::SwitchRelease(const RouteID switchID)
{
	Switch* mySwitch = GetSwitch(switchID);
	if (mySwitch == nullptr)
	{
		return false;
	}
	LocoID locoID = mySwitch->GetLoco();
	return mySwitch->Release(logger, locoID);
}

/***************************
* Route                   *
***************************/

bool Manager::RouteExecute(Logger::Logger* logger, const LocoID locoID, const RouteID routeID)
{
	Route* route = GetRoute(routeID);
	if (route == nullptr)
	{
		return false;
	}
	return route->Execute(logger, locoID);
}

void Manager::RouteExecuteAsync(Logger::Logger* logger, const RouteID routeID)
{
	Route* route = GetRoute(routeID);
	if (route == nullptr)
	{
		return;
	}
	std::async(std::launch::async, Route::ExecuteStatic, logger, route);
}

Route* Manager::GetRoute(const RouteID routeID) const
{
	std::lock_guard<std::mutex> guard(routeMutex);
	if (routes.count(routeID) != 1)
	{
		return nullptr;
	}
	return routes.at(routeID);
}

const string& Manager::GetRouteName(const RouteID routeID) const
{
	std::lock_guard<std::mutex> guard(routeMutex);
	if (routes.count(routeID) != 1)
	{
		return unknownRoute;
	}
	return routes.at(routeID)->GetName();
}

bool Manager::CheckRoutePosition(const Route* route, const LayoutPosition posX, const LayoutPosition posY, const LayoutPosition posZ, string& result) const
{
	if (route == nullptr)
	{
		return CheckPositionFree(posX, posY, posZ, DataModel::LayoutItem::Width1, DataModel::LayoutItem::Height1, DataModel::LayoutItem::Rotation0, result);
	}

	if (route->GetVisible() == DataModel::LayoutItem::VisibleNo)
	{
		return true;
	}

	if (route->HasPosition(posX, posY, posZ))
	{
		return true;
	}
	return CheckPositionFree(posX, posY, posZ, DataModel::LayoutItem::Width1, DataModel::LayoutItem::Height1, DataModel::LayoutItem::Rotation0, result);
}

bool Manager::RouteSave(RouteID routeID,
	const std::string& name,
	const Delay delay,
	const Route::PushpullType pushpull,
	const Length minTrainLength,
	const Length maxTrainLength,
	const std::vector<DataModel::Relation*>& relationsAtLock,
	const std::vector<DataModel::Relation*>& relationsAtUnlock,
	const Visible visible,
	const LayoutPosition posX,
	const LayoutPosition posY,
	const LayoutPosition posZ,
	const Automode automode,
	const ObjectIdentifier& fromTrack,
	const Orientation fromOrientation,
	const ObjectIdentifier& toTrack,
	const Orientation toOrientation,
	const Route::Speed speed,
	const FeedbackID feedbackIdReduced,
	const FeedbackID feedbackIdCreep,
	const FeedbackID feedbackIdStop,
	const FeedbackID feedbackIdOver,
	const Pause waitAfterRelease,
	string& result)
{

	Route* route = GetRoute(routeID);
	if (visible && !CheckRoutePosition(route, posX, posY, posZ, result))
	{
		return false;
	}

	if (route == nullptr)
	{
		route = CreateAndAddObject(routes, routeMutex);
	}

	if (route == nullptr)
	{
		result = Languages::GetText(Languages::TextUnableToAddRoute);
		return false;
	}

	// remove route from old track
	ObjectIdentifier oldFromTrack = route->GetFromTrack();
	TrackBase* oldTrack = GetTrackBase(oldFromTrack);
	if (oldTrack != nullptr)
	{
		oldTrack->RemoveRoute(route);
		if (storage == nullptr)
		{
			switch (oldFromTrack.GetObjectType())
			{
				case ObjectTypeTrack:
					storage->Save(*dynamic_cast<Track*>(oldTrack));
					break;

				case ObjectTypeSignal:
					storage->Save(*dynamic_cast<Signal*>(oldTrack));
					break;

				default:
					break;
			}
		}
	}

	// if we have a new object we have to update routeID
	routeID = route->GetID();

	// update existing route
	route->SetName(CheckObjectName(routes, routeMutex, routeID, name.size() == 0 ? "S" : name));
	route->SetDelay(delay);
	route->AssignRelationsAtLock(relationsAtLock);
	route->AssignRelationsAtUnlock(relationsAtUnlock);
	route->SetVisible(visible);
	route->SetPosX(posX);
	route->SetPosY(posY);
	route->SetPosZ(posZ);
	route->SetAutomode(automode);
	if (automode == AutomodeYes)
	{
		route->SetFromTrack(fromTrack);
		route->SetFromOrientation(fromOrientation);
		route->SetToTrack(toTrack);
		route->SetToOrientation(toOrientation);
		route->SetSpeed(speed);
		route->SetFeedbackIdReduced(feedbackIdReduced);
		route->SetFeedbackIdCreep(feedbackIdCreep);
		route->SetFeedbackIdStop(feedbackIdStop);
		route->SetFeedbackIdOver(feedbackIdOver);
		route->SetPushpull(pushpull);
		route->SetMinTrainLength(minTrainLength);
		route->SetMaxTrainLength(maxTrainLength);
		route->SetWaitAfterRelease(waitAfterRelease);
	}
	else
	{
		route->SetFromTrack(ObjectIdentifier());
		route->SetFromOrientation(OrientationRight);
		route->SetToTrack(ObjectIdentifier());
		route->SetToOrientation(OrientationLeft);
		route->SetSpeed(Route::SpeedTravel);
		route->SetFeedbackIdReduced(FeedbackNone);
		route->SetFeedbackIdCreep(FeedbackNone);
		route->SetFeedbackIdStop(FeedbackNone);
		route->SetFeedbackIdOver(FeedbackNone);
		route->SetPushpull(Route::PushpullTypeBoth);
		route->SetMinTrainLength(0);
		route->SetMaxTrainLength(0);
		route->SetWaitAfterRelease(0);
	}

	//Add new route
	ObjectIdentifier newFromTrack = route->GetFromTrack();
	TrackBase* newTrack = GetTrackBase(newFromTrack);
	if (newTrack != nullptr)
	{
		newTrack->AddRoute(route);
		if (storage != nullptr)
		{
			switch (newFromTrack.GetObjectType())
			{
				case ObjectTypeTrack:
					storage->Save(*dynamic_cast<Track*>(newTrack));
					break;

				case ObjectTypeSignal:
					storage->Save(*dynamic_cast<Signal*>(newTrack));
					break;

				default:
					break;
			}
		}
	}

	// save in db
	if (storage)
	{
		storage->Save(*route);
	}
	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		control.second->RouteSettings(routeID, name);
	}
	return true;
}

const map<string,DataModel::Route*> Manager::RouteListByName() const
{
	map<string,DataModel::Route*> out;
	std::lock_guard<std::mutex> guard(routeMutex);
	for(auto route : routes)
	{
		out[route.second->GetName()] = route.second;
	}
	return out;
}

bool Manager::RouteDelete(const RouteID routeID,
	string& result)
{
	Route* route = nullptr;
	{
		{
			std::lock_guard<std::mutex> guard(routeMutex);
			if (routeID == RouteNone || routes.count(routeID) != 1)
			{
				result = Languages::GetText(Languages::TextRouteDoesNotExist);
				return false;
			}

			route = routes.at(routeID);
			if (route == nullptr)
			{
				result = Languages::GetText(Languages::TextRouteDoesNotExist);
				return false;
			}

			if (route->IsInUse())
			{
				result = Logger::Logger::Format(Languages::GetText(Languages::TextRouteIsInUse), route->GetName());
				return false;
			}
		}

		// this must not be in the lock_guard, it would be a deadlock.
		ObjectIdentifier routeIdentifier(ObjectTypeRoute, routeID);
		if (ObjectIsPartOfRoute(routeIdentifier, route, result))
		{
			return false;
		}

		{
			std::lock_guard<std::mutex> guard(routeMutex);
			routes.erase(routeID);
		}
	}

	if (storage)
	{
		storage->DeleteRoute(routeID);
	}

	const string& routeName = route->GetName();
	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		control.second->RouteDelete(routeID, routeName);
	}
	delete route;
	return true;
}

Route* Manager::GetFirstRouteFromOrToTrackBase(const ObjectIdentifier& identifier) const
{
	std::lock_guard<std::mutex> guard(routeMutex);
	for (auto route : routes)
	{
		if (route.second->GetToTrack() == identifier || route.second->GetFromTrack() == identifier)
		{
			return route.second;
		}
	}
	return nullptr;
}

Layer* Manager::GetLayer(const LayerID layerID) const
{
	std::lock_guard<std::mutex> guard(layerMutex);
	if (layers.count(layerID) != 1)
	{
		return nullptr;
	}
	return layers.at(layerID);
}

const map<string,LayerID> Manager::LayerListByName() const
{
	map<string,LayerID> list;
	std::lock_guard<std::mutex> guard(layerMutex);
	for (auto layer : layers)
	{
		list[layer.second->GetName()] = layer.first;
	}
	return list;
}

const map<string,LayerID> Manager::LayerListByNameWithFeedback() const
{
	map<string,LayerID> list = LayerListByName();
	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		if (!control.second->CanHandle(Hardware::CapabilityFeedback))
		{
			continue;
		}
		list["| Feedbacks of " + control.second->GetShortName()] = -control.first;
	}
	return list;
}

bool Manager::LayerSave(LayerID layerID, const std::string&name, std::string& result)
{
	Layer* layer = GetLayer(layerID);
	if (layer == nullptr)
	{
		layer = CreateAndAddObject(layers, layerMutex);
	}

	if (layer == nullptr)
	{
		result = Languages::GetText(Languages::TextUnableToAddLayer);
		return false;
	}

	// if we have a new object we have to update layerID
	layerID = layer->GetID();

	// update existing layer
	layer->SetName(CheckObjectName(layers, layerMutex, layerID, name.size() == 0 ? "L" : name));

	// save in db
	if (storage)
	{
		storage->Save(*layer);
	}
	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		control.second->LayerSettings(layerID, name);
	}
	return true;
}

bool Manager::LayerHasElements(const Layer* layer,
	string& result)
{
	LayerID layerId = layer->GetID();
	for (auto track : tracks)
	{
		if (track.second->IsVisibleOnLayer(layerId))
		{
			result = Logger::Logger::Format(Languages::GetText(Languages::TextLayerIsUsedByTrack), layer->GetName(), track.second->GetName());
			return true;
		}
	}
	for (auto mySwitch : switches)
	{
		if (mySwitch.second->IsVisibleOnLayer(layerId))
		{
			result = Logger::Logger::Format(Languages::GetText(Languages::TextLayerIsUsedBySwitch), layer->GetName(), mySwitch.second->GetName());
			return true;
		}
	}
	for (auto signal : signals)
	{
		if (signal.second->IsVisibleOnLayer(layerId))
		{
			result = Logger::Logger::Format(Languages::GetText(Languages::TextLayerIsUsedBySignal), layer->GetName(), signal.second->GetName());
			return true;
		}
	}
	for (auto accessory : accessories)
	{
		if (accessory.second->IsVisibleOnLayer(layerId))
		{
			result = Logger::Logger::Format(Languages::GetText(Languages::TextLayerIsUsedByAccessory), layer->GetName(), accessory.second->GetName());
			return true;
		}
	}
	for (auto route : routes)
	{
		if (route.second->IsVisibleOnLayer(layerId))
		{
			result = Logger::Logger::Format(Languages::GetText(Languages::TextLayerIsUsedByRoute), layer->GetName(), route.second->GetName());
			return true;
		}
	}
	for (auto feedback : feedbacks)
	{
		if (feedback.second->IsVisibleOnLayer(layerId))
		{
			result = Logger::Logger::Format(Languages::GetText(Languages::TextLayerIsUsedByFeedback), layer->GetName(), feedback.second->GetName());
			return true;
		}
	}
	return false;
}

bool Manager::LayerDelete(const LayerID layerID,
	string& result)
{
	if (layerID == LayerUndeletable)
	{
		result = Languages::GetText(Languages::TextLayer1IsUndeletable);
		return false;
	}

	if (layerID == LayerNone)
	{
		result = Languages::GetText(Languages::TextLayerDoesNotExist);
		return false;
	}

	Layer* layer = nullptr;
	{
		std::lock_guard<std::mutex> guard(layerMutex);
		if (layers.count(layerID) != 1)
		{
			result = Languages::GetText(Languages::TextLayerDoesNotExist);
			return false;
		}

		layer = layers.at(layerID);
		if (layer == nullptr)
		{
			result = Languages::GetText(Languages::TextLayerDoesNotExist);
			return false;
		}

		if (LayerHasElements(layer, result))
		{
			return false;
		}

		layers.erase(layerID);
	}

	if (storage)
	{
		storage->DeleteLayer(layerID);
	}

	const string& layerName = layer->GetName();
	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		control.second->LayerDelete(layerID, layerName);
	}
	delete layer;
	return true;
}

/***************************
* Signal                   *
***************************/

bool Manager::SignalState(const ControlType controlType, const SignalID signalID, const DataModel::AccessoryState state, const bool force)
{
	if (boosterState == BoosterStateStop)
	{
		return false;
	}
	Signal* signal = GetSignal(signalID);
	if (signal == nullptr)
	{
		return false;
	}
	return SignalState(controlType, signal, state, force);
}

bool Manager::SignalState(const ControlType controlType, Signal* signal, const DataModel::AccessoryState state, const bool force)
{
	if (signal == nullptr)
	{
		return false;
	}

	if (force == false && signal->IsInUse())
	{
		logger->Warning(Languages::TextSignalIsLocked, signal->GetName());
		return false;
	}

	signal->SetAccessoryState(state);

	SignalPublishState(controlType, signal);
	return true;
}

void Manager::SignalPublishState(const ControlType controlType, const DataModel::Signal* signal)
{
	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		control.second->SignalState(controlType, signal);
	}
}

Signal* Manager::GetSignal(const SignalID signalID) const
{
	std::lock_guard<std::mutex> guard(signalMutex);
	if (signals.count(signalID) != 1)
	{
		return nullptr;
	}
	return signals.at(signalID);
}

Signal* Manager::GetSignal(const ControlID controlID, const Protocol protocol, const Address address) const
{
	std::lock_guard<std::mutex> guard(signalMutex);
	for (auto signal : signals)
	{
		if (signal.second->GetControlID() == controlID
			&& signal.second->GetProtocol() == protocol
			&& signal.second->GetAddress() == address)
		{
			return signal.second;
		}
	}
	return nullptr;
}

const std::string& Manager::GetSignalName(const SignalID signalID) const
{
	if (signals.count(signalID) != 1)
	{
		return unknownSignal;
	}
	return signals.at(signalID)->GetName();
}

bool Manager::CheckSignalPosition(const Signal* signal, const LayoutPosition posX, const LayoutPosition posY, const LayoutPosition posZ, string& result) const
{
	if (signal == nullptr)
	{
		return CheckPositionFree(posX, posY, posZ, DataModel::LayoutItem::Width1, DataModel::LayoutItem::Height1, DataModel::LayoutItem::Rotation0, result);
	}

	if (signal->HasPosition(posX, posY, posZ))
	{
		return true;
	}
	return CheckPositionFree(posX, posY, posZ, DataModel::LayoutItem::Width1, DataModel::LayoutItem::Height1, DataModel::LayoutItem::Rotation0, result);

}

bool Manager::SignalSave(SignalID signalID,
	const string& name,
	const Orientation signalOrientation,
	const LayoutPosition posX,
	const LayoutPosition posY,
	const LayoutPosition posZ,
	const LayoutItemSize height,
	const LayoutRotation rotation,
	const std::vector<FeedbackID>& newFeedbacks,
	const DataModel::SelectRouteApproach selectRouteApproach,
	const bool allowLocoTurn,
	const bool releaseWhenFree,
	const ControlID controlID,
	const Protocol protocol,
	const Address address,
	const DataModel::AccessoryType type,
	const DataModel::AccessoryPulseDuration duration,
	const bool inverted,
	string& result)
{
	if (!CheckControlAccessoryProtocolAddress(controlID, protocol, address, result))
	{
		return false;
	}

	Signal* signal = GetSignal(signalID);
	if (!CheckSignalPosition(signal, posX, posY, posZ, result))
	{
		return false;
	}

	if (signal == nullptr)
	{
		signal = CreateAndAddObject(signals, signalMutex);
	}

	if (signal == nullptr)
	{
		result = Languages::GetText(Languages::TextUnableToAddSignal);
		return false;
	}

	// if we have a new object we have to update locoID
	signalID = signal->GetID();

	signal->SetName(CheckObjectName(signals, signalMutex, signalID, name.size() == 0 ? "S" : name));
	signal->SetSignalOrientation(signalOrientation);
	signal->SetPosX(posX);
	signal->SetPosY(posY);
	signal->SetPosZ(posZ);
	signal->SetHeight(height);
	signal->SetRotation(rotation);
	signal->Feedbacks(CleanupAndCheckFeedbacksForTrack(ObjectIdentifier(ObjectTypeSignal, signalID), newFeedbacks));
	signal->SetSelectRouteApproach(selectRouteApproach);
	signal->SetAllowLocoTurn(allowLocoTurn);
	signal->SetReleaseWhenFree(releaseWhenFree);
	signal->SetControlID(controlID);
	signal->SetProtocol(protocol);
	signal->SetAddress(address);
	signal->SetType(type);
	signal->SetAccessoryPulseDuration(duration);
	signal->SetInverted(inverted);

	// save in db
	if (storage)
	{
		storage->Save(*signal);
	}

	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		control.second->SignalSettings(signalID, name);
	}
	return true;
}

bool Manager::SignalDelete(const SignalID signalID,
	string& result)
{
	Signal* signal = nullptr;
	{
		std::lock_guard<std::mutex> guard(signalMutex);
		if (signalID == SignalNone || signals.count(signalID) != 1)
		{
			result = Languages::GetText(Languages::TextSignalDoesNotExist);
			return false;
		}

		signal = signals.at(signalID);
		if (signal == nullptr)
		{
			result = Languages::GetText(Languages::TextSignalDoesNotExist);
			return false;
		}

		if (signal->IsInUse())
		{
			result = Logger::Logger::Format(Languages::GetText(Languages::TextSignalIsUsedByLoco), signal->GetName(), GetLocoName(signal->GetLoco()));
			return false;
		}

		Track* track = signal->GetTrack();
		if (track)
		{
			result = Logger::Logger::Format(Languages::GetText(Languages::TextSignalIsUsedByTrack), signal->GetName(), track->GetName());
			return false;
		}

		ObjectIdentifier signalIdentifier(ObjectTypeSignal, signalID);
		Route* route = GetFirstRouteFromOrToTrackBase(signalIdentifier);
		if (route != nullptr)
		{
			result = Logger::Logger::Format(Languages::GetText(Languages::TextSignalIsUsedByRoute), signal->GetName(), route->GetName());
			return false;
		}

		if (ObjectIsPartOfRoute(signalIdentifier, signal, result))
		{
			return false;
		}

		FeedbackID feedbackId = signal->GetFirstFeedbackId();
		if (feedbackId != FeedbackNone)
		{
			result = Logger::Logger::Format(Languages::GetText(Languages::TextSignalIsUsedByRoute), signal->GetName(), GetFeedbackName(feedbackId));
			return false;
		}

		signals.erase(signalID);
	}

	if (storage)
	{
		storage->DeleteSignal(signalID);
	}

	const string& signalName = signal->GetName();
	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		control.second->SignalDelete(signalID, signalName);
	}

	Cluster* cluster = signal->GetCluster();
	if (cluster != nullptr)
	{
		cluster->DeleteSignal(signal);
	}

	delete signal;
	return true;
}

const map<string,DataModel::Signal*> Manager::SignalListByName() const
{
	map<string,DataModel::Signal*> out;
	std::lock_guard<std::mutex> guard(signalMutex);
	for(auto signal : signals)
	{
		out[signal.second->GetName()] = signal.second;
	}
	return out;
}

/***************************
* Cluster                  *
***************************/

Cluster* Manager::GetCluster(const ClusterID clusterID) const
{
	std::lock_guard<std::mutex> guard(clusterMutex);
	if (clusters.count(clusterID) != 1)
	{
		return nullptr;
	}
	return clusters.at(clusterID);
}

const map<string,DataModel::Cluster*> Manager::ClusterListByName() const
{
	map<string,DataModel::Cluster*> out;
	std::lock_guard<std::mutex> guard(clusterMutex);
	for(auto cluster : clusters)
	{
		out[cluster.second->GetName()] = cluster.second;
	}
	return out;
}

bool Manager::ClusterSave(ClusterID clusterID,
	const string& name,
	const vector<Relation*>& newTracks,
	const vector<Relation*>& newSignals,
	string& result)
{
	Cluster* cluster = GetCluster(clusterID);
	if (cluster == nullptr)
	{
		cluster = CreateAndAddObject(clusters, clusterMutex);
	}

	if (cluster == nullptr)
	{
		result = Languages::GetText(Languages::TextUnableToAddCluster);
		return false;
	}

	// if we have a new object we have to update clusterID
	clusterID = cluster->GetID();

	// update existing cluster
	cluster->SetName(CheckObjectName(clusters, clusterMutex, clusterID, name.size() == 0 ? "C" : name));
	cluster->AssignTracks(newTracks);
	cluster->AssignSignals(newSignals);

	// save in db
	if (storage)
	{
		storage->Save(*cluster);
	}
	return true;
}

bool Manager::ClusterDelete(const ClusterID clusterID)
{
	if (clusterID == ClusterNone)
	{
		return false;
	}
	Cluster* cluster = nullptr;
	{
		std::lock_guard<std::mutex> guard(clusterMutex);
		if (clusters.count(clusterID) != 1)
		{
			return false;
		}

		cluster = clusters.at(clusterID);
		clusters.erase(clusterID);
	}

	cluster->DeleteTracks();
	cluster->DeleteSignals();

	if (storage)
	{
		storage->DeleteCluster(clusterID);
	}

	const string& clusterName = cluster->GetName();
	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		control.second->ClusterDelete(clusterID, clusterName);
	}
	delete cluster;
	return true;
}

/***************************
* Automode                 *
***************************/

bool Manager::LocoIntoTrackBase(Logger::Logger* logger, const LocoID locoID, const ObjectIdentifier& trackIdentifier)
{
	TrackBase* track = GetTrackBase(trackIdentifier);
	if (track == nullptr)
	{
		return false;
	}

	Loco* loco = GetLoco(locoID);
	if (loco == nullptr)
	{
		return false;
	}

	bool reserved = track->BaseReserveForce(logger, locoID);
	if (reserved == false)
	{
		return false;
	}

	reserved = loco->SetTrack(trackIdentifier);
	if (reserved == false)
	{
		track->BaseRelease(logger, locoID);
		return false;
	}

	reserved = track->BaseLock(logger, locoID);
	if (reserved == false)
	{
		loco->Release();
		track->BaseRelease(logger, locoID);
		return false;
	}

	const string& locoName = loco->GetName();
	const string& trackName = track->GetMyName();
	logger->Info(Languages::TextLocoIsOnTrack, locoName, trackName);

	TrackBasePublishState(track);
	return true;
}

bool Manager::LocoRelease(const LocoID locoID)
{
	Loco* loco = GetLoco(locoID);
	if (loco == nullptr)
	{
		return false;
	}
	return LocoReleaseInternal(loco);
}

bool Manager::LocoReleaseInternal(Loco* loco)
{
	LocoSpeed(ControlTypeInternal, loco, MinSpeed);

	bool ret = loco->Release();
	if (ret == false)
	{
		return false;
	}
	LocoID locoID = loco->GetID();
	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		control.second->LocoRelease(locoID);
	}
	return true;
}

bool Manager::TrackBaseRelease(const ObjectIdentifier& identifier)
{
	TrackBase* track = GetTrackBase(identifier);
	if (track == nullptr)
	{
		return false;
	}
	return track->BaseReleaseForce(logger, LocoNone);
}

bool Manager::LocoReleaseOnTrackBase(const ObjectIdentifier& identifier)
{
	TrackBase* track = GetTrackBase(identifier);
	if (track == nullptr)
	{
		return false;
	}
	LocoID locoID = track->GetMyLoco();
	track->BaseReleaseForce(logger, locoID);
	Loco* loco = GetLoco(locoID);
	if (loco == nullptr)
	{
		return false;
	}
	return LocoReleaseInternal(loco);
}

bool Manager::TrackBaseStartLoco(const ObjectIdentifier& identifier)
{
	TrackBase* track = GetTrackBase(identifier);
	if (track == nullptr)
	{
		return false;
	}
	return LocoStart(track->GetMyLoco());
}

bool Manager::TrackBaseStopLoco(const ObjectIdentifier& identifier)
{
	TrackBase* track = GetTrackBase(identifier);
	if (track == nullptr)
	{
		return false;
	}
	return LocoStop(track->GetMyLoco());
}

void Manager::TrackBaseBlock(const ObjectIdentifier& identifier, const bool blocked)
{
	TrackBase* track = GetTrackBase(identifier);
	if (track == nullptr)
	{
		return;
	}
	track->SetBlocked(blocked);
	TrackBasePublishState(track);
}

void Manager::TrackBaseSetLocoOrientation(const ObjectIdentifier& identifier, const Orientation orientation)
{
	TrackBase* track = GetTrackBase(identifier);
	if (track == nullptr)
	{
		return;
	}
	track->SetLocoOrientation(orientation);
	TrackBasePublishState(track);
}

void Manager::TrackBasePublishState(const DataModel::TrackBase* trackBase)
{
	const Track* track = dynamic_cast<const Track*>(trackBase);
	if (track != nullptr)
	{
		TrackPublishState(track);
		return;
	}

	const Signal* signal = dynamic_cast<const Signal*>(trackBase);
	if (signal != nullptr)
	{
		SignalPublishState(ControlTypeInternal, signal);
		return;
	}
}

void Manager::TrackPublishState(const DataModel::Track* track)
{
	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		control.second->TrackState(track);
	}
}

bool Manager::RouteRelease(const RouteID routeID)
{
	Route* route = GetRoute(routeID);
	if (route == nullptr)
	{
		return false;
	}
	LocoID locoID = route->GetLoco();
	return route->Release(logger, locoID);
}

bool Manager::LocoDestinationReached(const Loco* loco, const Route* route, const TrackBase* track)
{
	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		control.second->LocoDestinationReached(loco, route, track);
	}
	return true;
}

bool Manager::LocoStart(const LocoID locoID)
{
	Loco* loco = GetLoco(locoID);
	if (loco == nullptr)
	{
		return false;
	}
	bool ret = loco->GoToAutoMode();
	if (ret == false)
	{
		return false;
	}
	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		control.second->LocoStart(locoID, loco->GetName());
	}
	return true;
}

bool Manager::LocoStartAll()
{
	std::lock_guard<std::mutex> guard(locoMutex);
	for (auto loco : locos)
	{
		loco.second->GoToAutoMode();
		Utils::Utils::SleepForMilliseconds(50);
		/*
		bool ret = loco.second->GoToAutoMode();
		if (ret == false)
		{
			continue;
		}
		{
			std::lock_guard<std::mutex> guard(controlMutex);
			for (auto control : controls)
			{
				control.second->LocoStart(loco.first, loco.second->GetName());
			}
		}
		*/
	}
	return true;
}

bool Manager::LocoStop(const LocoID locoID)
{
	Loco* loco = GetLoco(locoID);
	if (loco == nullptr)
	{
		return false;
	}
	if (loco->IsInManualMode())
	{
		return true;
	}
	loco->RequestManualMode();
	while (loco->GoToManualMode() == false)
	{
		Utils::Utils::SleepForSeconds(1);
	}
	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		control.second->LocoStop(locoID, loco->GetName());
	}
	return true;
}

bool Manager::LocoStopAll()
{
	{
		std::lock_guard<std::mutex> guard(locoMutex);
		for (auto loco : locos)
		{
			if (loco.second->IsInAutoMode() == false)
			{
				continue;
			}
			loco.second->RequestManualMode();
		}
	}
	bool anyLocosInAutoMode = true;
	while (anyLocosInAutoMode)
	{
		Utils::Utils::SleepForSeconds(1);
		anyLocosInAutoMode = false;
		std::lock_guard<std::mutex> guard(locoMutex);
		for (auto loco : locos)
		{
			if (loco.second->IsInManualMode())
			{
				continue;
			}
			bool locoInManualMode = loco.second->GoToManualMode();
			anyLocosInAutoMode |= !locoInManualMode;
			/*
			if (locoInManualMode)
			{
				const string& locoName = loco.second->GetName();
				std::lock_guard<std::mutex> guard(controlMutex);
				for (auto control : controls)
				{
					control.second->LocoStop(loco.first, locoName);
				}
			}
			*/
		}
	}
	return true;
}

void Manager::StopAllLocosImmediately(const ControlType controlType)
{
	std::lock_guard<std::mutex> guard(locoMutex);
	for (auto loco : locos)
	{
		LocoSpeed(controlType, loco.second, MinSpeed, false);
	}
}

/***************************
* Layout                   *
***************************/

bool Manager::CheckPositionFree(const LayoutPosition posX, const LayoutPosition posY, const LayoutPosition posZ, string& result) const
{
	return CheckLayoutPositionFree(posX, posY, posZ, result, accessories, accessoryMutex)
		&& CheckLayoutPositionFree(posX, posY, posZ, result, tracks, trackMutex)
		&& CheckLayoutPositionFree(posX, posY, posZ, result, feedbacks, feedbackMutex)
		&& CheckLayoutPositionFree(posX, posY, posZ, result, switches, switchMutex)
		&& CheckLayoutPositionFree(posX, posY, posZ, result, routes, routeMutex)
		&& CheckLayoutPositionFree(posX, posY, posZ, result, signals, signalMutex);
}

bool Manager::CheckPositionFree(const LayoutPosition posX,
	const LayoutPosition posY,
	const LayoutPosition posZ,
	const LayoutItemSize width,
	const LayoutItemSize height,
	const LayoutRotation rotation,
	string& result) const
{
	if (width == 0)
	{
		result.assign(Languages::GetText(Languages::TextWidthIs0));
		return false;
	}
	if (height == 0)
	{
		result.assign(Languages::GetText(Languages::TextHeightIs0));
		return false;
	}
	LayoutPosition x;
	LayoutPosition y;
	LayoutPosition z = posZ;
	LayoutItemSize w;
	LayoutItemSize h;
	bool ret = DataModel::LayoutItem::MapPosition(posX, posY, width, height, rotation, x, y, w, h);
	if (ret == false)
	{
		return false;
	}
	for(LayoutPosition ix = x; ix < x + w; ix++)
	{
		for(LayoutPosition iy = y; iy < y + h; iy++)
		{
			bool ret = CheckPositionFree(ix, iy, z, result);
			if (ret == false)
			{
				return false;
			}
		}
	}
	return true;
}

template<class Type>
bool Manager::CheckLayoutPositionFree(const LayoutPosition posX, const LayoutPosition posY, const LayoutPosition posZ, string& result, const map<ObjectID, Type*>& layoutVector, std::mutex& mutex) const
{
	std::lock_guard<std::mutex> guard(mutex);
	for (auto layout : layoutVector)
	{
		if (layout.second->CheckPositionFree(posX, posY, posZ))
		{
			continue;
		}
		result.assign(Logger::Logger::Format(Languages::GetText(Languages::TextPositionAlreadyInUse), static_cast<int>(posX), static_cast<int>(posY), static_cast<int>(posZ), layout.second->GetLayoutType(), layout.second->GetName()));
		return false;
	}
	return true;
}

bool Manager::CheckAddressLoco(const Protocol protocol, const Address address, string& result)
{
	switch (protocol)
	{
		case ProtocolDCC:
			if (address > 10239)
			{
				result.assign(Languages::GetText(Languages::TextLocoAddressDccTooHigh));
				return false;
			}
			return true;

		case ProtocolMM1:
		case ProtocolMM2:
			if (address > 80)
			{
				result.assign(Languages::GetText(Languages::TextLocoAddressMmTooHigh));
				return false;
			}
			return true;

		default:
			return true;
	}
}

bool Manager::CheckAddressAccessory(const Protocol protocol, const Address address, string& result)
{
	switch (protocol)
	{
		case ProtocolDCC:
			if (address > 2044)
			{
				result.assign(Languages::GetText(Languages::TextAccessoryAddressDccTooHigh));
				return false;
			}
			return true;

		case ProtocolMM1:
		case ProtocolMM2:
			if (address > 320) {
				result.assign(Languages::GetText(Languages::TextAccessoryAddressMmTooHigh));
				return false;
			}
			return true;

		default:
			return true;
	}
}

bool Manager::CheckControlProtocolAddress(const AddressType type, const ControlID controlID, const Protocol protocol, const Address address, string& result)
{
	{
		std::lock_guard<std::mutex> guard(controlMutex);
		if (controlID < ControlIdFirstHardware || controls.count(controlID) != 1)
		{
			result.assign(Languages::GetText(Languages::TextControlDoesNotExist));
			return false;
		}
		ControlInterface* control = controls.at(controlID);
		bool ret;
		if (type == AddressTypeLoco)
		{
			ret = control->LocoProtocolSupported(protocol);
		}
		else
		{
			ret = control->AccessoryProtocolSupported(protocol);
		}
		if (!ret)
		{
			string protocolText;
			if (protocol <= ProtocolEnd)
			{
				protocolText = ProtocolSymbols[protocol] + " ";
			}
			std::vector<Protocol> protocols;
			string protocolsText;
			if (type == AddressTypeLoco)
			{
				control->LocoProtocols(protocols);
			}
			else
			{
				control->AccessoryProtocols(protocols);
			}
			for (auto p : protocols)
			{
				if (protocolsText.size() > 0)
				{
					protocolsText.append(", ");
				}
				protocolsText.append(ProtocolSymbols[p]);
			}
			result.assign(Logger::Logger::Format(Languages::GetText(Languages::TextProtocolNotSupported), protocolText, protocolsText));
			return false;
		}
	}
	if (address == 0)
	{
		result.assign(Logger::Logger::Format(Languages::GetText(Languages::TextAddressMustBeHigherThen0)));
		return false;
	}
	switch (type)
	{
		case AddressTypeLoco:
			return CheckAddressLoco(protocol, address, result);

		case AddressTypeAccessory:
			return CheckAddressAccessory(protocol, address, result);

		default:
			return false;
	}
}

bool Manager::SaveSettings(const Languages::Language language,
	const DataModel::AccessoryPulseDuration duration,
	const bool autoAddFeedback,
	const bool stopOnFeedbackInFreeTrack,
	const DataModel::SelectRouteApproach selectRouteApproach,
	const DataModel::Loco::NrOfTracksToReserve nrOfTracksToReserve,
	const Logger::Logger::Level logLevel
	)
{
	Languages::SetDefaultLanguage(language);
	this->defaultAccessoryDuration = duration;
	this->autoAddFeedback = autoAddFeedback;
	this->stopOnFeedbackInFreeTrack = stopOnFeedbackInFreeTrack;
	this->selectRouteApproach = selectRouteApproach;
	this->nrOfTracksToReserve = nrOfTracksToReserve;
	Logger::Logger::SetLogLevel(logLevel);

	if (storage == nullptr)
	{
		return false;
	}
	Storage::TransactionGuard guard(storage);
	storage->SaveSetting("Language", std::to_string(static_cast<int>(language)));
	storage->SaveSetting("DefaultAccessoryDuration", std::to_string(duration));
	storage->SaveSetting("AutoAddFeedback", std::to_string(autoAddFeedback));
	storage->SaveSetting("StopOnFeedbackInFreeTrack", std::to_string(stopOnFeedbackInFreeTrack));
	storage->SaveSetting("SelectRouteApproach", std::to_string(static_cast<int>(selectRouteApproach)));
	storage->SaveSetting("NrOfTracksToReserve", std::to_string(static_cast<int>(nrOfTracksToReserve)));
	storage->SaveSetting("LogLevel", std::to_string(static_cast<int>(logLevel)));
	return true;
}

void Manager::DebounceWorker()
{
	Utils::Utils::SetThreadName(Languages::GetText(Languages::TextDebouncer));
	logger->Info(Languages::TextDebounceThreadStarted);
	while (debounceRun)
	{
		{
			std::lock_guard<std::mutex> guard(feedbackMutex);
			for (auto feedback : feedbacks)
			{
				feedback.second->Debounce();
			}
		}
		Utils::Utils::SleepForMilliseconds(250);
	}
	logger->Info(Languages::TextDebounceThreadTerminated);
}

template<class ID, class T>
T* Manager::CreateAndAddObject(std::map<ID,T*>& objects, std::mutex& mutex)
{
	std::lock_guard<std::mutex> Guard(mutex);
	ID newObjectID = 0;
	for (auto object : objects)
	{
		if (object.first > newObjectID)
		{
			newObjectID = object.first;
		}
	}
	++newObjectID;
	T* newObject = new T(this, newObjectID);
	if (newObject == nullptr)
	{
		return nullptr;
	}
	objects[newObjectID] = newObject;
	return newObject;
}

ControlID Manager::GetPossibleControlForLoco() const
{
	for (auto control : controls)
	{
		if (control.second->CanHandle(Hardware::CapabilityLoco))
		{
			return control.first;
		}
	}
	return ControlIdNone;
}

ControlID Manager::GetPossibleControlForAccessory() const
{
	for (auto control : controls)
	{
		if (control.second->CanHandle(Hardware::CapabilityAccessory))
		{
			return control.first;
		}
	}
	return ControlIdNone;
}

ControlID Manager::GetPossibleControlForFeedback() const
{
	for (auto control : controls)
	{
		if (control.second->CanHandle(Hardware::CapabilityFeedback))
		{
			return control.first;
		}
	}
	return ControlIdNone;
}

void Manager::ProgramCheckBooster(const ProgramMode mode)
{
	switch (mode)
	{
		case ProgramModeDccPomLoco:
		case ProgramModeDccPomAccessory:
			if (boosterState == BoosterStateGo)
			{
				return;
			}
			Booster(ControlTypeInternal, BoosterStateGo);
			Utils::Utils::SleepForMilliseconds(100);
			return;

		default:
			return;
	}
}

void Manager::ProgramRead(const ControlID controlID, const ProgramMode mode, const Address address, const CvNumber cv)
{
	ControlInterface* control = GetControl(controlID);
	if (control == nullptr)
	{
		return;
	}
	ProgramCheckBooster(mode);
	control->ProgramRead(mode, address, cv);
}

void Manager::ProgramWrite(const ControlID controlID, const ProgramMode mode, const Address address, const CvNumber cv, const CvValue value)
{
	ControlInterface* control = GetControl(controlID);
	if (control == nullptr)
	{
		return;
	}
	ProgramCheckBooster(mode);
	control->ProgramWrite(mode, address, cv, value);
}

void Manager::ProgramValue(const CvNumber cv, const CvValue value)
{
	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		control.second->ProgramValue(cv, value);;
	}
}

bool Manager::CanHandle(const Hardware::Capabilities capability) const
{
	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		bool ret = control.second->CanHandle(capability);
		if (ret == true)
		{
			return true;
		}
	}
	return false;
}

bool Manager::CanHandle(const ControlID controlId, const Hardware::Capabilities capability) const
{
	ControlInterface* control = GetControl(controlId);
	if (control == nullptr)
	{
		return false;
	}
	return control->CanHandle(capability);
}

Hardware::Capabilities Manager::GetCapabilities(const ControlID controlID) const
{
	ControlInterface* control = GetControl(controlID);
	if (control == nullptr)
	{
		return Hardware::CapabilityNone;
	}
	return control->GetCapabilities();
}


Hardware::HardwareParams* Manager::CreateAndAddControl()
{
	std::lock_guard<std::mutex> Guard(hardwareMutex);
	ControlID newObjectID = ControlIdFirstHardware - 1;
	for (auto hardwareParam : hardwareParams)
	{
		if (hardwareParam.first > newObjectID)
		{
			newObjectID = hardwareParam.first;
		}
	}
	++newObjectID;
	Hardware::HardwareParams* newParams = new Hardware::HardwareParams(this, newObjectID);
	if (newParams == nullptr)
	{
		return nullptr;
	}
	hardwareParams[newObjectID] = newParams;
	return newParams;
}

bool Manager::ObjectIsPartOfRoute(const ObjectIdentifier& identifier,
	const Object* object,
	string& result)
{
	std::lock_guard<std::mutex> Guard(routeMutex);
	for (auto route : routes)
	{
		if (!route.second->ObjectIsPartOfRoute(identifier))
		{
			continue;
		}

		Languages::TextSelector selector;
		switch (identifier.GetObjectType())
		{
			case ObjectTypeTrack:
				selector = Languages::TextTrackIsUsedByRoute;
				break;

			case ObjectTypeAccessory:
				selector = Languages::TextAccessoryIsUsedByRoute;
				break;

			case ObjectTypeSwitch:
				selector = Languages::TextSwitchIsUsedByRoute;
				break;

			case ObjectTypeRoute:
				selector = Languages::TextRouteIsUsedByRoute;
				break;

			case ObjectTypeSignal:
				selector = Languages::TextSignalIsUsedByRoute;
				break;

			default:
				selector = Languages::TextObjectIsUsedByRoute;
				break;
		}
		result = Logger::Logger::Format(Languages::GetText(selector), object->GetName(), route.second->GetName());
		return true;
	}
	return false;
}
