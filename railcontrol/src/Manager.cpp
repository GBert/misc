/*
RailControl - Model Railway Control Software

Copyright (c) 2017-2020 Dominik (Teddy) Mahrer - www.railcontrol.org

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
#include "Utils/Utils.h"
#include "WebServer/WebServer.h"

using DataModel::Accessory;
using DataModel::Track;
using DataModel::Feedback;
using DataModel::Layer;
using DataModel::LayoutItem;
using DataModel::Loco;
using DataModel::Signal;
using DataModel::Street;
using DataModel::Switch;
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
 	boosterState(BoosterStop),
	storage(nullptr),
	defaultAccessoryDuration(DefaultAccessoryDuration),
	autoAddFeedback(false),
	selectStreetApproach(DataModel::Track::SelectStreetRandom),
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
	unknownStreet(Languages::GetText(Languages::TextStreetDoesNotExist)),
	unknownSignal(Languages::GetText(Languages::TextSignalDoesNotExist))
{
	StorageParams storageParams;
	storageParams.module = "Sqlite";
	storageParams.filename = config.getValue("dbfilename", "railcontrol.sqlite");
	storage = new StorageHandler(this, storageParams);
	if (storage == nullptr)
	{
		logger->Info(Languages::TextUnableToCreateStorageHandler);
		return;
	}

	Logger::Logger::SetLogLevel(static_cast<Logger::Logger::logLevel_t>(Utils::Utils::StringToInteger(storage->GetSetting("LogLevel"), Logger::Logger::LevelInfo)));
	Languages::SetDefaultLanguage(static_cast<Languages::language_t>(Utils::Utils::StringToInteger(storage->GetSetting("Language"), Languages::EN)));
	defaultAccessoryDuration = Utils::Utils::StringToInteger(storage->GetSetting("DefaultAccessoryDuration"), 250);
	autoAddFeedback = Utils::Utils::StringToBool(storage->GetSetting("AutoAddFeedback"));
	selectStreetApproach = static_cast<DataModel::Track::selectStreetApproach_t>(Utils::Utils::StringToInteger(storage->GetSetting("SelectStreetApproach")));
	nrOfTracksToReserve = static_cast<DataModel::Loco::nrOfTracksToReserve_t>(Utils::Utils::StringToInteger(storage->GetSetting("NrOfTracksToReserve"), 2));


	controls[ControlIdWebserver] = new WebServer::WebServer(*this, config.getValue("webserverport", 80));

	storage->AllHardwareParams(hardwareParams);
	for (auto hardwareParam : hardwareParams)
	{
		hardwareParam.second->SetManager(this);
		controls[hardwareParam.second->GetControlID()] = new HardwareHandler(*this, hardwareParam.second);
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
		logger->Info(Languages::TextLoadedAccessory, accessory.second->GetID(), accessory.second->GetName());
	}

	storage->AllFeedbacks(feedbacks);
	for (auto feedback : feedbacks)
	{
		logger->Info(Languages::TextLoadedFeedback, feedback.second->GetID(), feedback.second->GetName());
	}

	storage->AllTracks(tracks);
	for (auto track : tracks)
	{
		logger->Info(Languages::TextLoadedTrack, track.second->GetID(), track.second->GetName());
	}

	storage->AllSwitches(switches);
	for (auto mySwitch : switches)
	{
		logger->Info(Languages::TextLoadedSwitch, mySwitch.second->GetID(), mySwitch.second->GetName());
	}

	storage->AllSignals(signals);
	for (auto signal : signals)
	{
		logger->Info(Languages::TextLoadedSignal, signal.second->GetID(), signal.second->GetName());
	}

	storage->AllStreets(streets);
	for (auto street : streets)
	{
		logger->Info(Languages::TextLoadedStreet, street.second->GetID(), street.second->GetName());
	}

	storage->AllLocos(locos);
	for (auto loco : locos)
	{
		logger->Info(Languages::TextLoadedLoco, loco.second->GetID(), loco.second->GetName());
	}

	run = true;
	debounceRun = true;
	debounceThread = std::thread(&Manager::DebounceWorker, this);
}

Manager::~Manager()
{
	while (!LocoStopAll())
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	debounceRun = false;
	debounceThread.join();

	Booster(ControlTypeInternal, BoosterStop);

	run = false;
	{
		std::lock_guard<std::mutex> guard(controlMutex);
		for (auto control : controls)
		{
			controlID_t controlID = control.first;
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
			delete params;
		}
	}

	if (storage != nullptr)
	{
		storage->StartTransaction();
	}

	DeleteAllMapEntries(locos, locoMutex);
	DeleteAllMapEntries(streets, streetMutex);
	DeleteAllMapEntries(signals, signalMutex);
	DeleteAllMapEntries(switches, switchMutex);
	DeleteAllMapEntries(accessories, accessoryMutex);
	DeleteAllMapEntries(feedbacks, feedbackMutex);
	DeleteAllMapEntries(tracks, trackMutex);
	DeleteAllMapEntries(layers, layerMutex);

	if (storage == nullptr)
	{
		return;
	}

	storage->CommitTransaction();
	delete storage;
	storage = nullptr;
}

/***************************
* Booster                  *
***************************/

void Manager::Booster(const controlType_t controlType, const boosterState_t state)
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
	if (boosterState == BoosterStop)
	{
		return;
	}
	if (!initLocosDone)
	{
		InitLocos();
	}
}

void Manager::InitLocos()
{
	std::lock_guard<std::mutex> guard(locoMutex);
	for (auto loco : locos)
	{
		if (boosterState == BoosterStop)
		{
			return;
		}
		std::lock_guard<std::mutex> guard(controlMutex);
		for (auto control : controls)
		{
			std::vector<bool> functions = loco.second->GetFunctions();
			control.second->LocoSpeedDirectionFunctions(loco.second, loco.second->Speed(), loco.second->GetDirection(), functions);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
	}
	initLocosDone = true;
}

/***************************
* Control                  *
***************************/

const std::map<hardwareType_t,string> Manager::HardwareListNames()
{
	std::map<hardwareType_t,string> hardwareList;
	hardwareList[HardwareTypeCcSchnitte] = "CC-Schnitte";
	hardwareList[HardwareTypeEcos] = "ESU Ecos / Märklin Central Station 1 (CS1)";
	hardwareList[HardwareTypeM6051] = "Märklin Interface 6050/6051";
	hardwareList[HardwareTypeCS2] = "Märklin Central Station 2 (CS2)";
	hardwareList[HardwareTypeOpenDcc] = "OpenDCC Z1";
	hardwareList[HardwareTypeRM485] = "RM485";
	hardwareList[HardwareTypeHsi88] = "HSI-88";
	hardwareList[HardwareTypeVirtual] = "Virtual Command Station (no Hardware)";
	hardwareList[HardwareTypeZ21] = "Z21 (Power On/Off only, untested)";
	return hardwareList;
}

bool Manager::ControlSave(const controlID_t& controlID,
	const hardwareType_t& hardwareType,
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
		ControlInterface* control = new HardwareHandler(*this, params);
		if (control == nullptr)
		{
			return false;
		}
		controls[params->GetControlID()] = control;
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

bool Manager::ControlDelete(controlID_t controlID)
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

HardwareParams* Manager::GetHardware(controlID_t controlID)
{
	std::lock_guard<std::mutex> guard(hardwareMutex);
	if (hardwareParams.count(controlID) != 1)
	{
		return nullptr;
	}
	return hardwareParams.at(controlID);
}

unsigned int Manager::ControlsOfHardwareType(const hardwareType_t hardwareType)
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

bool Manager::HardwareLibraryAdd(const hardwareType_t hardwareType, void* libraryHandle)
{
	std::lock_guard<std::mutex> guard(hardwareLibrariesMutex);
	if (hardwareLibraries.count(hardwareType) == 1)
	{
		return false;
	}
	hardwareLibraries[hardwareType] = libraryHandle;
	return true;
}

void* Manager::HardwareLibraryGet(const hardwareType_t hardwareType) const
{
	std::lock_guard<std::mutex> guard(hardwareLibrariesMutex);
	if (hardwareLibraries.count(hardwareType) != 1)
	{
		return nullptr;
	}
	return hardwareLibraries.at(hardwareType);
}

bool Manager::HardwareLibraryRemove(const hardwareType_t hardwareType)
{
	std::lock_guard<std::mutex> guard(hardwareLibrariesMutex);
	if (hardwareLibraries.count(hardwareType) != 1)
	{
		return false;
	}
	hardwareLibraries.erase(hardwareType);
	return true;
}

ControlInterface* Manager::GetControl(const controlID_t controlID) const
{
	std::lock_guard<std::mutex> guard(controlMutex);
	if (controls.count(controlID) != 1)
	{
		return nullptr;
	}
	return controls.at(controlID);
}

const std::string Manager::GetControlName(const controlID_t controlID)
{
	std::lock_guard<std::mutex> guard(controlMutex);
	if (controls.count(controlID) != 1)
	{
		return unknownControl;
	}
	ControlInterface* control = controls.at(controlID);
	return control->GetName();
}

const std::map<controlID_t,std::string> Manager::LocoControlListNames() const
{
	std::map<controlID_t,std::string> ret;
	std::lock_guard<std::mutex> guard(hardwareMutex);
	for (auto hardware : hardwareParams)
	{
		std::lock_guard<std::mutex> guard2(controlMutex);
		if (controls.count(hardware.second->GetControlID()) != 1)
		{
			continue;
		}
		ControlInterface* c = controls.at(hardware.second->GetControlID());
		if (c->CanHandleLocos() == false)
		{
			continue;
		}
		ret[hardware.first] = hardware.second->GetName();
	}
	return ret;
}

const std::map<controlID_t,std::string> Manager::AccessoryControlListNames() const
{
	std::map<controlID_t,std::string> ret;
	std::lock_guard<std::mutex> guard(hardwareMutex);
	for (auto hardware : hardwareParams)
	{
		std::lock_guard<std::mutex> guard2(controlMutex);
		if (controls.count(hardware.second->GetControlID()) != 1)
		{
			continue;
		}
		ControlInterface* c = controls.at(hardware.second->GetControlID());
		if (c->CanHandleAccessories() == false)
		{
			continue;
		}
		ret[hardware.first] = hardware.second->GetName();
	}
	return ret;
}

const std::map<controlID_t,std::string> Manager::FeedbackControlListNames() const
{
	std::map<controlID_t,std::string> ret;
	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		if (control.second->ControlType() != ControlTypeHardware || control.second->CanHandleFeedbacks() == false)
		{
			continue;
		}
		ret[control.first] = control.second->GetName();
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

const std::map<std::string, protocol_t> Manager::ProtocolsOfControl(const addressType_t type, const controlID_t controlID) const
{
	std::map<std::string,protocol_t> ret;
	{
		const ControlInterface* control = GetControl(controlID);
		if (control == nullptr || control->ControlType() != ControlTypeHardware)
		{
			ret[protocolSymbols[ProtocolNone]] = ProtocolNone;
			return ret;
		}

		const HardwareHandler* hardware = static_cast<const HardwareHandler*>(control);
		if (hardware->ControlID() != controlID)
		{
			ret[protocolSymbols[ProtocolNone]] = ProtocolNone;
			return ret;
		}

		std::vector<protocol_t> protocols;
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
			ret[protocolSymbols[protocol]] = protocol;
		}
	}
	return ret;
}

/***************************
* Loco                     *
***************************/

DataModel::Loco* Manager::GetLoco(const locoID_t locoID) const
{
	std::lock_guard<std::mutex> guard(locoMutex);
	if (locos.count(locoID) != 1)
	{
		return nullptr;
	}
	return locos.at(locoID);
}

Loco* Manager::GetLoco(const controlID_t controlID, const protocol_t protocol, const address_t address) const
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

const std::string& Manager::GetLocoName(const locoID_t locoID) const
{
	std::lock_guard<std::mutex> guard(locoMutex);
	if (locos.count(locoID) != 1)
	{
		return unknownLoco;
	}
	return locos.at(locoID)->GetName();
}

const map<string,locoID_t> Manager::LocoListFree() const
{
	map<string,locoID_t> out;
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

const map<string,DataModel::Loco*> Manager::LocoListByName() const
{
	map<string,DataModel::Loco*> out;
	std::lock_guard<std::mutex> guard(locoMutex);
	for(auto loco : locos)
	{
		out[loco.second->GetName()] = loco.second;
	}
	return out;
}

bool Manager::LocoSave(const locoID_t locoID,
	const string& name,
	const controlID_t controlID,
	const protocol_t protocol,
	const address_t address,
	const function_t nrOfFunctions,
	const length_t length,
	const bool pushpull,
	const locoSpeed_t maxSpeed,
	const locoSpeed_t travelSpeed,
	const locoSpeed_t reducedSpeed,
	const locoSpeed_t creepingSpeed,
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

	loco->SetName(CheckObjectName(locos, locoMutex, locoID, name.size() == 0 ? "L" : name));
	loco->SetControlID(controlID);
	loco->SetProtocol(protocol);
	loco->SetAddress(address);
	loco->SetNrOfFunctions(nrOfFunctions);
	loco->SetLength(length);
	loco->SetPushpull(pushpull);
	loco->SetMaxSpeed(maxSpeed);
	loco->SetTravelSpeed(travelSpeed);
	loco->SetReducedSpeed(reducedSpeed);
	loco->SetCreepingSpeed(creepingSpeed);
	loco->AssignSlaves(slaves);

	// save in db
	if (storage)
	{
		storage->Save(*loco);
	}
	const locoID_t locoIdSave = loco->GetID();
	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		control.second->LocoSettings(locoIdSave, name);
	}
	return true;
}

bool Manager::LocoDelete(const locoID_t locoID)
{
	Loco* loco = nullptr;
	{
		std::lock_guard<std::mutex> guard(locoMutex);
		if (locoID == LocoNone || locos.count(locoID) != 1)
		{
			return false;
		}

		loco = locos.at(locoID);
		if (loco->IsInUse())
		{
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

bool Manager::LocoProtocolAddress(const locoID_t locoID, controlID_t& controlID, protocol_t& protocol, address_t& address) const
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

void Manager::LocoSpeed(const controlType_t controlType, const controlID_t controlID, const protocol_t protocol, const address_t address, const locoSpeed_t speed)
{
	Loco* loco = GetLoco(controlID, protocol, address);
	if (loco == nullptr)
	{
		return;
	}
	LocoSpeed(controlType, loco, speed);
}

bool Manager::LocoSpeed(const controlType_t controlType, const locoID_t locoID, const locoSpeed_t speed, const bool withSlaves)
{
	Loco* loco = GetLoco(locoID);
	return LocoSpeed(controlType, loco, speed, withSlaves);
}

bool Manager::LocoSpeed(const controlType_t controlType, Loco* loco, const locoSpeed_t speed, const bool withSlaves)
{
	if (loco == nullptr)
	{
		return false;
	}
	locoSpeed_t s = speed;
	if (speed > MaxSpeed)
	{
		s = MaxSpeed;
	}
	const string& locoName = loco->GetName();
	logger->Info(Languages::TextLocoSpeedIs, locoName, s);
	loco->Speed(s, withSlaves);
	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		control.second->LocoSpeed(controlType, loco, s);
	}
	return true;
}

locoSpeed_t Manager::LocoSpeed(const locoID_t locoID) const
{
	Loco* loco = GetLoco(locoID);
	if (loco == nullptr)
	{
		return MinSpeed;
	}
	return loco->Speed();
}

void Manager::LocoDirection(const controlType_t controlType, const controlID_t controlID, const protocol_t protocol, const address_t address, const direction_t direction)
{
	Loco* loco = GetLoco(controlID, protocol, address);
	if (loco == nullptr)
	{
		return;
	}
	LocoDirection(controlType, loco, direction);
}

void Manager::LocoDirection(const controlType_t controlType, const locoID_t locoID, const direction_t direction)
{
	Loco* loco = GetLoco(locoID);
	LocoDirection(controlType, loco, direction);
}

void Manager::LocoDirection(const controlType_t controlType, Loco* loco, const direction_t direction)
{
	if (loco == nullptr)
	{
		return;
	}
	loco->SetDirection(direction);
	logger->Info(direction ? Languages::TextLocoDirectionIsRight : Languages::TextLocoDirectionIsLeft, loco->GetName());
	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		control.second->LocoDirection(controlType, loco, direction);
	}
}

void Manager::LocoFunction(const controlType_t controlType, const controlID_t controlID, const protocol_t protocol, const address_t address, const function_t function, const bool on)
{
	Loco* loco = GetLoco(controlID, protocol, address);
	if (loco == nullptr)
	{
		return;
	}
	LocoFunction(controlType, loco, function, on);
}

void Manager::LocoFunction(const controlType_t controlType, const locoID_t locoID, const function_t function, const bool on)
{
	Loco* loco = GetLoco(locoID);
	LocoFunction(controlType, loco, function, on);
}

void Manager::LocoFunction(const controlType_t controlType, Loco* loco, const function_t function, const bool on)
{
	if (loco == nullptr)
	{
		return;
	}

	loco->SetFunction(function, on);
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

void Manager::AccessoryState(const controlType_t controlType, const controlID_t controlID, const protocol_t protocol, const address_t address, const accessoryState_t state)
{
	Accessory* accessory = GetAccessory(controlID, protocol, address);
	if (accessory != nullptr)
	{
		AccessoryState(controlType, accessory, state, true);
		return;
	}

	Switch* mySwitch = GetSwitch(controlID, protocol, address);
	if (mySwitch != nullptr)
	{
		SwitchState(controlType, mySwitch, state, true);
		return;
	}

	Signal* signal = GetSignal(controlID, protocol, address);
	if (signal != nullptr)
	{
		SignalState(controlType, signal, state, true);
		return;
	}
}

bool Manager::AccessoryState(const controlType_t controlType, const accessoryID_t accessoryID, const accessoryState_t state, const bool force)
{
	if (boosterState == BoosterStop)
	{
		return false;
	}
	Accessory* accessory = GetAccessory(accessoryID);
	AccessoryState(controlType, accessory, state, force);
	return true;
}

void Manager::AccessoryState(const controlType_t controlType, Accessory* accessory, const accessoryState_t state, const bool force)
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

	accessory->SetState(state);
	const bool inverted = accessory->GetInverted();

	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		accessoryState_t tempState = (control.first >= ControlIdFirstHardware ? (state != inverted) : state);
		control.second->AccessoryState(controlType, accessory, tempState);
	}
}

Accessory* Manager::GetAccessory(const accessoryID_t accessoryID) const
{
	std::lock_guard<std::mutex> guard(accessoryMutex);
	if (accessories.count(accessoryID) != 1)
	{
		return nullptr;
	}
	return accessories.at(accessoryID);
}

Accessory* Manager::GetAccessory(const controlID_t controlID, const protocol_t protocol, const address_t address) const
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

const std::string& Manager::GetAccessoryName(const accessoryID_t accessoryID) const
{
	std::lock_guard<std::mutex> guard(accessoryMutex);
	if (accessories.count(accessoryID) != 1)
	{
		return unknownAccessory;
	}
	return accessories.at(accessoryID)->GetName();
}

bool Manager::CheckAccessoryPosition(const Accessory* accessory, const layoutPosition_t posX, const layoutPosition_t posY, const layoutPosition_t posZ, string& result) const
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

bool Manager::AccessorySave(const accessoryID_t accessoryID, const string& name, const layoutPosition_t posX, const layoutPosition_t posY, const layoutPosition_t posZ, const controlID_t controlID, const protocol_t protocol, const address_t address, const accessoryType_t type, const accessoryDuration_t duration, const bool inverted, string& result)
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

	// update existing accessory
	accessory->SetName(CheckObjectName(accessories, accessoryMutex, accessoryID, name.size() == 0 ? "A" : name));
	accessory->SetPosX(posX);
	accessory->SetPosY(posY);
	accessory->SetPosZ(posZ);
	accessory->SetControlID(controlID);
	accessory->SetProtocol(protocol);
	accessory->SetAddress(address);
	accessory->SetType(type);
	accessory->SetDuration(duration);
	accessory->SetInverted(inverted);

	// save in db
	if (storage)
	{
		storage->Save(*accessory);
	}
	accessoryID_t accessoryIdSave = accessory->GetID();
	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		control.second->AccessorySettings(accessoryIdSave, name);
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

bool Manager::AccessoryDelete(const accessoryID_t accessoryID)
{
	Accessory* accessory = nullptr;
	{
		std::lock_guard<std::mutex> guard(accessoryMutex);
		if (accessoryID == AccessoryNone || accessories.count(accessoryID) != 1)
		{
			return false;
		}

		accessory = accessories.at(accessoryID);
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

bool Manager::AccessoryRelease(const accessoryID_t accessoryID)
{
	Accessory* accessory = GetAccessory(accessoryID);
	if (accessory == nullptr)
	{
		return false;
	}
	locoID_t locoID = accessory->GetLoco();
	return accessory->Release(logger, locoID);
}

/***************************
* Feedback                 *
***************************/

void Manager::FeedbackState(const controlID_t controlID, const feedbackPin_t pin, const DataModel::Feedback::feedbackState_t state)
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

	FeedbackSave(FeedbackNone, name, VisibleNo, 0, 0, 0, controlID, pin, false, result);
}

void Manager::FeedbackState(const feedbackID_t feedbackID, const DataModel::Feedback::feedbackState_t state)
{
	Feedback* feedback = GetFeedback(feedbackID);
	if (feedback == nullptr)
	{
		return;
	}
	FeedbackState(feedback, state);
}

void Manager::FeedbackPublishState(Feedback* feedback)
{
	if (feedback == nullptr)
	{
		return;
	}
	DataModel::Feedback::feedbackState_t state = feedback->GetState();
	const string& feedbackName = feedback->GetName();
	logger->Info(state ? Languages::TextFeedbackStateIsOn : Languages::TextFeedbackStateIsOff, feedbackName);
	const feedbackID_t feedbackID = feedback->GetID();
	{
		std::lock_guard<std::mutex> guard(controlMutex);
		for (auto control : controls)
		{
			control.second->FeedbackState(feedbackName, feedbackID, state);
		}
	}
}

Feedback* Manager::GetFeedback(const feedbackID_t feedbackID) const
{
	std::lock_guard<std::mutex> guard(feedbackMutex);
	return GetFeedbackUnlocked(feedbackID);
}

Feedback* Manager::GetFeedbackUnlocked(const feedbackID_t feedbackID) const
{
	if (feedbacks.count(feedbackID) != 1)
	{
		return nullptr;
	}
	return feedbacks.at(feedbackID);
}

Feedback* Manager::GetFeedback(const controlID_t controlID, const feedbackPin_t pin) const
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

const std::string& Manager::GetFeedbackName(const feedbackID_t feedbackID) const
{
	std::lock_guard<std::mutex> guard(feedbackMutex);
	if (feedbacks.count(feedbackID) != 1)
	{
		return unknownFeedback;
	}
	return feedbacks.at(feedbackID)->GetName();
}

bool Manager::CheckFeedbackPosition(const Feedback* feedback, const layoutPosition_t posX, const layoutPosition_t posY, const layoutPosition_t posZ, string& result) const
{
	if (feedback == nullptr)
	{
		return CheckPositionFree(posX, posY, posZ, DataModel::LayoutItem::Width1, DataModel::LayoutItem::Height1, DataModel::LayoutItem::Rotation0, result);
	}

	if (feedback->GetVisible() == VisibleNo)
	{
		return true;
	}

	if (feedback->HasPosition(posX, posY, posZ))
	{
		return true;
	}
	return CheckPositionFree(posX, posY, posZ, DataModel::LayoutItem::Width1, DataModel::LayoutItem::Height1, DataModel::LayoutItem::Rotation0, result);
}

feedbackID_t Manager::FeedbackSave(const feedbackID_t feedbackID, const std::string& name, const visible_t visible, const layoutPosition_t posX, const layoutPosition_t posY, const layoutPosition_t posZ, const controlID_t controlID, const feedbackPin_t pin, const bool inverted, string& result)
{
	Feedback* feedback = GetFeedback(feedbackID);
	if (visible && !CheckFeedbackPosition(feedback, posX, posY, posZ, result))
	{
		return FeedbackNone;
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
	feedbackID_t feedbackIdSave = feedback->GetID();
	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		control.second->FeedbackSettings(feedbackIdSave, name);
	}
	return feedbackIdSave;
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

const map<string,feedbackID_t> Manager::FeedbacksOfTrack(const trackID_t trackID) const
{
	map<string,feedbackID_t> out;
	Track* track = GetTrack(trackID);
	if (track == nullptr)
	{
		return out;
	}
	vector<feedbackID_t> feedbacksOfTrack = track->GetFeedbacks();
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

bool Manager::FeedbackDelete(const feedbackID_t feedbackID)
{
	Feedback* feedback = nullptr;
	{
		std::lock_guard<std::mutex> guard(feedbackMutex);
		if (feedbackID == FeedbackNone || feedbacks.count(feedbackID) != 1)
		{
			return false;
		}

		feedback = feedbacks.at(feedbackID);
		if (feedback == nullptr)
		{
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

Track* Manager::GetTrack(const trackID_t trackID) const
{
	std::lock_guard<std::mutex> guard(trackMutex);
	if (tracks.count(trackID) != 1)
	{
		return nullptr;
	}
	return tracks.at(trackID);
}

const std::string& Manager::GetTrackName(const trackID_t trackID) const
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

const map<string,trackID_t> Manager::TrackListIdByName() const
{
	map<string,trackID_t> out;
	std::lock_guard<std::mutex> guard(trackMutex);
	for(auto track : tracks)
	{
		out[track.second->GetName()] = track.second->GetID();
	}
	return out;
}

bool Manager::CheckTrackPosition(const Track* track,
	const layoutPosition_t posX,
	const layoutPosition_t posY,
	const layoutPosition_t posZ,
	const layoutItemSize_t height,
	const DataModel::LayoutItem::layoutRotation_t rotation,
	string& result) const
{
	layoutPosition_t x1;
	layoutPosition_t y1;
	layoutPosition_t z1 = posZ;
	layoutItemSize_t w1;
	layoutItemSize_t h1;
	bool ret = DataModel::LayoutItem::MapPosition(posX, posY, DataModel::LayoutItem::Width1, height, rotation, x1, y1, w1, h1);
	if (ret == false)
	{
		result = Languages::GetText(Languages::TextUnableToCalculatePosition);
		return false;
	}

	layoutPosition_t x2 = 0;
	layoutPosition_t y2 = 0;
	layoutPosition_t z2 = 0;
	layoutItemSize_t w2 = 0;
	layoutItemSize_t h2 = 0;

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

	for(layoutPosition_t ix = x1; ix < x1 + w1; ++ix)
	{
		for(layoutPosition_t iy = y1; iy < y1 + h1; ++iy)
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

const std::vector<feedbackID_t> Manager::CleanupAndCheckFeedbacks(trackID_t trackID, std::vector<feedbackID_t>& newFeedbacks)
{
	{
		std::lock_guard<std::mutex> feedbackguard(feedbackMutex);
		for (auto feedback : feedbacks)
		{
			if (feedback.second->GetTrack() != trackID)
			{
				continue;
			}

			feedback.second->SetTrack(TrackNone);
			if (storage != nullptr)
			{
				storage->Save(*feedback.second);
			}
		}
	}

	std::vector<feedbackID_t> checkedFeedbacks;
	for (auto feedbackID : newFeedbacks)
	{
		Feedback* feedback = GetFeedback(feedbackID);
		if (feedback == nullptr)
		{
			continue;
		}
		if (feedback->GetTrack() != TrackNone)
		{
			continue;
		}
		checkedFeedbacks.push_back(feedbackID);
		feedback->SetTrack(trackID);
	}
	return checkedFeedbacks;
}

trackID_t Manager::TrackSave(const trackID_t trackID,
	const std::string& name,
	const layoutPosition_t posX,
	const layoutPosition_t posY,
	const layoutPosition_t posZ,
	const layoutItemSize_t height,
	const DataModel::LayoutItem::layoutRotation_t rotation,
	const DataModel::Track::type_t type,
	std::vector<feedbackID_t> newFeedbacks,
	const DataModel::Track::selectStreetApproach_t selectStreetApproach,
	const bool releaseWhenFree,
	string& result)
{
	Track* track = GetTrack(trackID);
	if (!CheckTrackPosition(track, posX, posY, posZ, height, rotation, result))
	{
		return TrackNone;
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

	// update existing track
	track->SetName(CheckObjectName(tracks, trackMutex, trackID, name.size() == 0 ? "T" : name));
	track->SetHeight(height);
	track->SetRotation(rotation);
	track->SetPosX(posX);
	track->SetPosY(posY);
	track->SetPosZ(posZ);
	track->SetType(type);
	track->Feedbacks(CleanupAndCheckFeedbacks(trackID, newFeedbacks));
	track->SetSelectStreetApproach(selectStreetApproach);
	track->SetReleaseWhenFree(releaseWhenFree);

	// save in db
	if (storage)
	{
		storage->Save(*track);
	}
	std::lock_guard<std::mutex> guard(controlMutex);
	trackID_t trackIdSave = track->GetID();
	for (auto control : controls)
	{
		control.second->TrackSettings(trackIdSave, name);
	}
	return trackIdSave;
}

bool Manager::TrackDelete(const trackID_t trackID)
{
	Track* track = nullptr;
	{
		std::lock_guard<std::mutex> guard(trackMutex);
		if (trackID == TrackNone || tracks.count(trackID) != 1)
		{
			return false;
		}

		track = tracks.at(trackID);
		if (track == nullptr || track->IsInUse())
		{
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
	delete track;
	return true;
}

/***************************
* Switch                   *
***************************/

bool Manager::SwitchState(const controlType_t controlType, const switchID_t switchID, const switchState_t state, const bool force)
{
	if (boosterState == BoosterStop)
	{
		return false;
	}

	Switch* mySwitch = GetSwitch(switchID);
	SwitchState(controlType, mySwitch, state, force);
	return true;
}

void Manager::SwitchState(const controlType_t controlType, Switch* mySwitch, const switchState_t state, const bool force)
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

	mySwitch->SetState(state);
	bool inverted = mySwitch->GetInverted();

	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		switchState_t tempState = (control.first >= ControlIdFirstHardware ? (state != inverted) : state);
		control.second->SwitchState(controlType, mySwitch, tempState);
	}
}

Switch* Manager::GetSwitch(const switchID_t switchID) const
{
	std::lock_guard<std::mutex> guard(switchMutex);
	if (switches.count(switchID) != 1)
	{
		return nullptr;
	}
	return switches.at(switchID);
}

Switch* Manager::GetSwitch(const controlID_t controlID, const protocol_t protocol, const address_t address) const
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

const std::string& Manager::GetSwitchName(const switchID_t switchID) const
{
	if (switches.count(switchID) != 1)
	{
		return unknownSwitch;
	}
	return switches.at(switchID)->GetName();
}

bool Manager::CheckSwitchPosition(const Switch* mySwitch, const layoutPosition_t posX, const layoutPosition_t posY, const layoutPosition_t posZ, string& result) const
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

bool Manager::SwitchSave(const switchID_t switchID,
	const string& name,
	const layoutPosition_t posX,
	const layoutPosition_t posY,
	const layoutPosition_t posZ,
	const DataModel::LayoutItem::layoutRotation_t rotation,
	const controlID_t controlID,
	const protocol_t protocol,
	const address_t address,
	const switchType_t type,
	const switchDuration_t duration,
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
	mySwitch->SetDuration(duration);
	mySwitch->SetInverted(inverted);

	// save in db
	if (storage)
	{
		storage->Save(*mySwitch);
	}
	const switchID_t switchIdSave = mySwitch->GetID();
	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		control.second->SwitchSettings(switchIdSave, name);
	}
	return true;
}

bool Manager::SwitchDelete(const switchID_t switchID)
{
	Switch* mySwitch = nullptr;
	{
		std::lock_guard<std::mutex> guard(switchMutex);
		if (switchID == SwitchNone || switches.count(switchID) != 1)
		{
			return false;
		}

		mySwitch = switches.at(switchID);
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

bool Manager::SwitchRelease(const streetID_t switchID)
{
	Switch* mySwitch = GetSwitch(switchID);
	if (mySwitch == nullptr)
	{
		return false;
	}
	locoID_t locoID = mySwitch->GetLoco();
	return mySwitch->Release(logger, locoID);
}

/***************************
* Street                   *
***************************/

bool Manager::StreetExecute(Logger::Logger* logger, const locoID_t locoID, const streetID_t streetID)
{
	Street* street = GetStreet(streetID);
	if (street == nullptr)
	{
		return false;
	}
	return street->Execute(logger, locoID);
}

void Manager::StreetExecuteAsync(Logger::Logger* logger, const streetID_t streetID)
{
	Street* street = GetStreet(streetID);
	if (street == nullptr)
	{
		return;
	}
	std::async(std::launch::async, Street::ExecuteStatic, logger, street);
}

Street* Manager::GetStreet(const streetID_t streetID) const
{
	std::lock_guard<std::mutex> guard(streetMutex);
	if (streets.count(streetID) != 1)
	{
		return nullptr;
	}
	return streets.at(streetID);
}

const string& Manager::GetStreetName(const streetID_t streetID) const
{
	std::lock_guard<std::mutex> guard(streetMutex);
	if (streets.count(streetID) != 1)
	{
		return unknownStreet;
	}
	return streets.at(streetID)->GetName();
}

bool Manager::CheckStreetPosition(const Street* street, const layoutPosition_t posX, const layoutPosition_t posY, const layoutPosition_t posZ, string& result) const
{
	if (street == nullptr)
	{
		return CheckPositionFree(posX, posY, posZ, DataModel::LayoutItem::Width1, DataModel::LayoutItem::Height1, DataModel::LayoutItem::Rotation0, result);
	}

	if (street->GetVisible() == VisibleNo)
	{
		return true;
	}

	if (street->HasPosition(posX, posY, posZ))
	{
		return true;
	}
	return CheckPositionFree(posX, posY, posZ, DataModel::LayoutItem::Width1, DataModel::LayoutItem::Height1, DataModel::LayoutItem::Rotation0, result);
}

bool Manager::StreetSave(const streetID_t streetID,
	const std::string& name,
	const delay_t delay,
	const Street::PushpullType pushpull,
	const length_t minTrainLength,
	const length_t maxTrainLength,
	const std::vector<DataModel::Relation*>& relationsAtLock,
	const std::vector<DataModel::Relation*>& relationsAtUnlock,
	const visible_t visible,
	const layoutPosition_t posX,
	const layoutPosition_t posY,
	const layoutPosition_t posZ,
	const automode_t automode,
	const trackID_t fromTrack,
	const direction_t fromDirection,
	const trackID_t toTrack,
	const direction_t toDirection,
	const Street::Speed speed,
	const feedbackID_t feedbackIdReduced,
	const feedbackID_t feedbackIdCreep,
	const feedbackID_t feedbackIdStop,
	const feedbackID_t feedbackIdOver,
	const wait_t waitAfterRelease,
	string& result)
{

	Street* street = GetStreet(streetID);
	if (visible && !CheckStreetPosition(street, posX, posY, posZ, result))
	{
		return false;
	}

	if (street == nullptr)
	{
		street = CreateAndAddObject(streets, streetMutex);
	}

	if (street == nullptr)
	{
		result = Languages::GetText(Languages::TextUnableToAddStreet);
		return false;
	}

	// remove street from old track
	Track* track = GetTrack(street->GetFromTrack());
	if (track != nullptr)
	{
		track->RemoveStreet(street);
		if (storage)
		{
			storage->Save(*track);
		}
	}

	// update existing street
	street->SetName(CheckObjectName(streets, streetMutex, streetID, name.size() == 0 ? "S" : name));
	street->SetDelay(delay);
	street->AssignRelationsAtLock(relationsAtLock);
	street->AssignRelationsAtUnlock(relationsAtUnlock);
	street->SetVisible(visible);
	street->SetPosX(posX);
	street->SetPosY(posY);
	street->SetPosZ(posZ);
	street->SetAutomode(automode);
	if (automode == AutomodeYes)
	{
		street->SetFromTrack(fromTrack);
		street->SetFromDirection(fromDirection);
		street->SetToTrack(toTrack);
		street->SetToDirection(toDirection);
		street->SetSpeed(speed);
		street->SetFeedbackIdReduced(feedbackIdReduced);
		street->SetFeedbackIdCreep(feedbackIdCreep);
		street->SetFeedbackIdStop(feedbackIdStop);
		street->SetFeedbackIdOver(feedbackIdOver);
		street->SetPushpull(pushpull);
		street->SetMinTrainLength(minTrainLength);
		street->SetMaxTrainLength(maxTrainLength);
		street->SetWaitAfterRelease(waitAfterRelease);
	}
	else
	{
		street->SetFromTrack(TrackNone);
		street->SetFromDirection(DirectionRight);
		street->SetToTrack(TrackNone);
		street->SetToDirection(DirectionLeft);
		street->SetSpeed(Street::SpeedTravel);
		street->SetFeedbackIdReduced(FeedbackNone);
		street->SetFeedbackIdCreep(FeedbackNone);
		street->SetFeedbackIdStop(FeedbackNone);
		street->SetFeedbackIdOver(FeedbackNone);
		street->SetPushpull(Street::PushpullTypeBoth);
		street->SetMinTrainLength(0);
		street->SetMaxTrainLength(0);
		street->SetWaitAfterRelease(0);
	}

	//Add new street
	track = GetTrack(fromTrack);
	if (track != nullptr)
	{
		track->AddStreet(street);
		if (storage)
		{
			storage->Save(*track);
		}
	}
	// save in db
	if (storage)
	{
		storage->Save(*street);
	}
	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		control.second->StreetSettings(street->GetID(), name);
	}
	return true;
}

const map<string,DataModel::Street*> Manager::StreetListByName() const
{
	map<string,DataModel::Street*> out;
	std::lock_guard<std::mutex> guard(streetMutex);
	for(auto street : streets)
	{
		out[street.second->GetName()] = street.second;
	}
	return out;
}

bool Manager::StreetDelete(const streetID_t streetID)
{
	Street* street = nullptr;
	{
		std::lock_guard<std::mutex> guard(streetMutex);
		if (streetID == StreetNone || streets.count(streetID) != 1)
		{
			return false;
		}

		street = streets.at(streetID);
		streets.erase(streetID);
	}

	if (storage)
	{
		storage->DeleteStreet(streetID);
	}

	const string& streetName = street->GetName();
	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		control.second->StreetDelete(streetID, streetName);
	}
	delete street;
	return true;
}

Layer* Manager::GetLayer(const layerID_t layerID) const
{
	std::lock_guard<std::mutex> guard(layerMutex);
	if (layers.count(layerID) != 1)
	{
		return nullptr;
	}
	return layers.at(layerID);
}

const map<string,layerID_t> Manager::LayerListByName() const
{
	map<string,layerID_t> list;
	std::lock_guard<std::mutex> guard(layerMutex);
	for (auto layer : layers)
	{
		list[layer.second->GetName()] = layer.first;
	}
	return list;
}

const map<string,layerID_t> Manager::LayerListByNameWithFeedback() const
{
	map<string,layerID_t> list = LayerListByName();
	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		if (!control.second->CanHandleFeedbacks())
		{
			continue;
		}
		list["| Feedbacks of " + control.second->GetName()] = -control.first;
	}
	return list;
}

bool Manager::LayerSave(const layerID_t layerID, const std::string&name, std::string& result)
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

	// update existing layer
	layer->SetName(CheckObjectName(layers, layerMutex, layerID, name.size() == 0 ? "L" : name));

	// save in db
	if (storage)
	{
		storage->Save(*layer);
	}
	const layerID_t layerIdSave = layer->GetID();
	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		control.second->LayerSettings(layerIdSave, name);
	}
	return true;
}

bool Manager::LayerDelete(const layerID_t layerID)
{
	if (layerID == LayerUndeletable || layerID == LayerNone)
	{
		return false;
	}

	Layer* layer = nullptr;
	{
		std::lock_guard<std::mutex> guard(layerMutex);
		if (layers.count(layerID) != 1)
		{
			return false;
		}

		layer = layers.at(layerID);
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

bool Manager::SignalState(const controlType_t controlType, const signalID_t signalID, const signalState_t state, const bool force)
{
	if (boosterState == BoosterStop)
	{
		return false;
	}
	Signal* signal = GetSignal(signalID);
	SignalState(controlType, signal, state, force);
	return true;
}

void Manager::SignalState(const controlType_t controlType, Signal* signal, const signalState_t state, const bool force)
{
	if (signal == nullptr)
	{
		return;
	}

	if (force == false && signal->IsInUse())
	{
		logger->Warning(Languages::TextSignalIsLocked, signal->GetName());
		return;
	}

	signal->SetState(state);
	bool inverted = signal->GetInverted();

	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		signalState_t tempState = (control.first >= ControlIdFirstHardware ? (state != inverted) : state);
		control.second->SignalState(controlType, signal, tempState);
	}
}

Signal* Manager::GetSignal(const signalID_t signalID) const
{
	std::lock_guard<std::mutex> guard(signalMutex);
	if (signals.count(signalID) != 1)
	{
		return nullptr;
	}
	return signals.at(signalID);
}

Signal* Manager::GetSignal(const controlID_t controlID, const protocol_t protocol, const address_t address) const
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

const std::string& Manager::GetSignalName(const signalID_t signalID) const
{
	if (signals.count(signalID) != 1)
	{
		return unknownSignal;
	}
	return signals.at(signalID)->GetName();
}

bool Manager::CheckSignalPosition(const Signal* signal, const layoutPosition_t posX, const layoutPosition_t posY, const layoutPosition_t posZ, string& result) const
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

bool Manager::SignalSave(const signalID_t signalID,
	const string& name,
	const layoutPosition_t posX,
	const layoutPosition_t posY,
	const layoutPosition_t posZ,
	const DataModel::LayoutItem::layoutRotation_t rotation,
	const controlID_t controlID,
	const protocol_t protocol,
	const address_t address,
	const signalType_t type,
	const signalDuration_t duration,
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

	signal->SetName(CheckObjectName(signals, signalMutex, signalID, name.size() == 0 ? "S" : name));
	signal->SetPosX(posX);
	signal->SetPosY(posY);
	signal->SetPosZ(posZ);
	signal->SetRotation(rotation);
	signal->SetControlID(controlID);
	signal->SetProtocol(protocol);
	signal->SetAddress(address);
	signal->SetType(type);
	signal->SetDuration(duration);
	signal->SetInverted(inverted);

	// save in db
	if (storage)
	{
		storage->Save(*signal);
	}
	const signalID_t signalIdSave = signal->GetID();
	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		control.second->SignalSettings(signalIdSave, name);
	}
	return true;
}

bool Manager::SignalDelete(const signalID_t signalID)
{
	Signal* signal = nullptr;
	{
		std::lock_guard<std::mutex> guard(signalMutex);
		if (signalID == SignalNone || signals.count(signalID) != 1)
		{
			return false;
		}

		signal = signals.at(signalID);
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

bool Manager::SignalRelease(const streetID_t signalID)
{
	Signal* signal = GetSignal(signalID);
	if (signal == nullptr)
	{
		return false;
	}
	locoID_t locoID = signal->GetLoco();
	return signal->Release(logger, locoID);
}

/***************************
* Automode                 *
***************************/

bool Manager::LocoIntoTrack(Logger::Logger* logger, const locoID_t locoID, const trackID_t trackID)
{
	Track* track = GetTrack(trackID);
	if (track == nullptr)
	{
		return false;
	}

	Loco* loco = GetLoco(locoID);
	if (loco == nullptr)
	{
		return false;
	}

	bool reserved = track->ReserveForce(logger, locoID);
	if (reserved == false)
	{
		return false;
	}

	reserved = loco->ToTrack(trackID);
	if (reserved == false)
	{
		track->Release(logger, locoID);
		return false;
	}

	reserved = track->Lock(logger, locoID);
	if (reserved == false)
	{
		loco->Release();
		track->Release(logger, locoID);
		return false;
	}

	string locoName = GetLocoName(locoID);
	string trackName = GetTrackName(trackID);
	logger->Info(Languages::TextLocoIsOnTrack, locoName, trackName);

	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		control.second->LocoIntoTrack(locoID, trackID, locoName, trackName);
	}
	return true;
}

bool Manager::LocoRelease(const locoID_t locoID)
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
	locoID_t locoID = loco->GetID();
	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		control.second->LocoRelease(locoID);
	}
	return true;
}

bool Manager::TrackRelease(const trackID_t trackID)
{
	Track* track = GetTrack(trackID);
	if (track == nullptr)
	{
		return false;
	}
	return track->ReleaseForce(logger, LocoNone);
}

bool Manager::LocoReleaseInTrack(const trackID_t trackID)
{
	Track* track = GetTrack(trackID);
	if (track == nullptr)
	{
		return false;
	}
	locoID_t locoID = track->GetLoco();
	track->ReleaseForce(logger, locoID);
	Loco* loco = GetLoco(locoID);
	if (loco == nullptr)
	{
		return false;
	}
	return LocoReleaseInternal(loco);
}

bool Manager::TrackStartLoco(const trackID_t trackID)
{
	Track* track = GetTrack(trackID);
	if (track == nullptr)
	{
		return false;
	}
	return LocoStart(track->GetLoco());
}

bool Manager::TrackStopLoco(const trackID_t trackID)
{
	Track* track = GetTrack(trackID);
	if (track == nullptr)
	{
		return false;
	}
	return LocoStop(track->GetLoco());
}

void Manager::TrackBlock(const trackID_t trackID, const bool blocked)
{
	Track* track = GetTrack(trackID);
	if (track == nullptr)
	{
		return;
	}
	track->SetBlocked(blocked);
	TrackPublishState(track);
}

void Manager::TrackSetLocoDirection(const trackID_t trackID, const direction_t direction)
{
	Track* track = GetTrack(trackID);
	if (track == nullptr)
	{
		return;
	}
	track->SetLocoDirection(direction);
	TrackPublishState(track);
}

void Manager::TrackPublishState(const DataModel::Track* track)
{
	const Loco* loco = GetLoco(track->GetLocoDelayed());
	const bool hasLoco = loco != nullptr;
	const string& locoName = hasLoco ? loco->GetName() : "";
	const string& trackName = track->GetName();
	const trackID_t trackID = track->GetID();
	const bool occupied = track->GetFeedbackStateDelayed() == DataModel::Feedback::FeedbackStateOccupied;
	const bool blocked = track->GetBlocked();
	const direction_t direction = track->GetLocoDirection();
	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		control.second->TrackState(trackID, trackName, occupied, blocked, direction, locoName);
	}
}

bool Manager::StreetRelease(const streetID_t streetID)
{
	Street* street = GetStreet(streetID);
	if (street == nullptr)
	{
		return false;
	}
	locoID_t locoID = street->GetLoco();
	return street->Release(logger, locoID);
}

bool Manager::LocoDestinationReached(const locoID_t locoID, const streetID_t streetID, const trackID_t trackID)
{
	std::lock_guard<std::mutex> guard(controlMutex);
	for (auto control : controls)
	{
		control.second->LocoDestinationReached(locoID, streetID, trackID);
	}
	return true;
}

bool Manager::LocoStart(const locoID_t locoID)
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
	}
	return true;
}

bool Manager::LocoStop(const locoID_t locoID)
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
		std::this_thread::sleep_for(std::chrono::seconds(1));
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
		std::this_thread::sleep_for(std::chrono::seconds(1));
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
			if (locoInManualMode)
			{
				const string& locoName = loco.second->GetName();
				std::lock_guard<std::mutex> guard(controlMutex);
				for (auto control : controls)
				{
					control.second->LocoStop(loco.first, locoName);
				}
			}
		}
	}
	return true;
}

void Manager::StopAllLocosImmediately(const controlType_t controlType)
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

bool Manager::CheckPositionFree(const layoutPosition_t posX, const layoutPosition_t posY, const layoutPosition_t posZ, string& result) const
{
	return CheckLayoutPositionFree(posX, posY, posZ, result, accessories, accessoryMutex)
		&& CheckLayoutPositionFree(posX, posY, posZ, result, tracks, trackMutex)
		&& CheckLayoutPositionFree(posX, posY, posZ, result, feedbacks, feedbackMutex)
		&& CheckLayoutPositionFree(posX, posY, posZ, result, switches, switchMutex)
		&& CheckLayoutPositionFree(posX, posY, posZ, result, streets, streetMutex)
		&& CheckLayoutPositionFree(posX, posY, posZ, result, signals, signalMutex);
}

bool Manager::CheckPositionFree(const layoutPosition_t posX,
	const layoutPosition_t posY,
	const layoutPosition_t posZ,
	const layoutItemSize_t width,
	const layoutItemSize_t height,
	const DataModel::LayoutItem::layoutRotation_t rotation,
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
	layoutPosition_t x;
	layoutPosition_t y;
	layoutPosition_t z = posZ;
	layoutItemSize_t w;
	layoutItemSize_t h;
	bool ret = DataModel::LayoutItem::MapPosition(posX, posY, width, height, rotation, x, y, w, h);
	if (ret == false)
	{
		return false;
	}
	for(layoutPosition_t ix = x; ix < x + w; ix++)
	{
		for(layoutPosition_t iy = y; iy < y + h; iy++)
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
bool Manager::CheckLayoutPositionFree(const layoutPosition_t posX, const layoutPosition_t posY, const layoutPosition_t posZ, string& result, const map<objectID_t, Type*>& layoutVector, std::mutex& mutex) const
{
	std::lock_guard<std::mutex> guard(mutex);
	for (auto layout : layoutVector)
	{
		if (layout.second->CheckPositionFree(posX, posY, posZ))
		{
			continue;
		}
		result.assign(Logger::Logger::Format(Languages::GetText(Languages::TextPositionAlreadyInUse), static_cast<int>(posX), static_cast<int>(posY), static_cast<int>(posZ), layout.second->LayoutType(), layout.second->GetName()));
		return false;
	}
	return true;
}

bool Manager::CheckAddressLoco(const protocol_t protocol, const address_t address, string& result)
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

bool Manager::CheckAddressAccessory(const protocol_t protocol, const address_t address, string& result)
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

bool Manager::CheckControlProtocolAddress(const addressType_t type, const controlID_t controlID, const protocol_t protocol, const address_t address, string& result)
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
			if (protocol > ProtocolNone && protocol <= ProtocolEnd)
			{
				protocolText = protocolSymbols[protocol] + " ";
			}
			std::vector<protocol_t> protocols;
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
				protocolsText.append(protocolSymbols[p]);
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

bool Manager::SaveSettings(const accessoryDuration_t duration,
	const bool autoAddFeedback,
	const DataModel::Track::selectStreetApproach_t selectStreetApproach,
	const DataModel::Loco::nrOfTracksToReserve_t nrOfTracksToReserve,
	const Logger::Logger::logLevel_t logLevel,
	const Languages::language_t language)
{
	this->defaultAccessoryDuration = duration;
	this->autoAddFeedback = autoAddFeedback;
	this->selectStreetApproach = selectStreetApproach;
	this->nrOfTracksToReserve = nrOfTracksToReserve;
	Logger::Logger::SetLogLevel(logLevel);
	Languages::SetDefaultLanguage(language);

	if (storage == nullptr)
	{
		return false;
	}
	storage->SaveSetting("DefaultAccessoryDuration", std::to_string(duration));
	storage->SaveSetting("AutoAddFeedback", std::to_string(autoAddFeedback));
	storage->SaveSetting("SelectStreetApproach", std::to_string(static_cast<int>(selectStreetApproach)));
	storage->SaveSetting("NrOfTracksToReserve", std::to_string(static_cast<int>(nrOfTracksToReserve)));
	storage->SaveSetting("LogLevel", std::to_string(static_cast<int>(logLevel)));
	storage->SaveSetting("Language", std::to_string(static_cast<int>(language)));
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
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
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

controlID_t Manager::GetControlForLoco() const
{
	for (auto control : controls)
	{
		if (control.second->CanHandleLocos())
		{
			return control.first;
		}
	}
	return ControlIdNone;
}

controlID_t Manager::GetControlForAccessory() const
{
	for (auto control : controls)
	{
		if (control.second->CanHandleAccessories())
		{
			return control.first;
		}
	}
	return ControlIdNone;
}

controlID_t Manager::GetControlForFeedback() const
{
	for (auto control : controls)
	{
		if (control.second->CanHandleFeedbacks())
		{
			return control.first;
		}
	}
	return ControlIdNone;
}

Hardware::HardwareParams* Manager::CreateAndAddControl()
{
	std::lock_guard<std::mutex> Guard(hardwareMutex);
	controlID_t newObjectID = ControlIdFirstHardware - 1;
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
