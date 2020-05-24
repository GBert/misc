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

#pragma once

#include <map>
#include <mutex>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>

#include "Config.h"
#include "ControlInterface.h"
#include "DataModel/AccessoryBase.h"
#include "DataModel/DataModel.h"
#include "DataModel/ObjectIdentifier.h"
#include "Hardware/HardwareParams.h"
#include "Logger/Logger.h"
#include "Storage/StorageHandler.h"

class Manager
{
	public:
		Manager() = delete;
		Manager(Config& config);
		~Manager();

		// booster
		BoosterState Booster() const { return boosterState; }
		void Booster(const ControlType controlType, const BoosterState status);

		// hardware (virt, CS2, ...)
		bool ControlSave(const ControlID& controlID,
			const HardwareType& hardwareType,
			const std::string& name,
			const std::string& arg1,
			const std::string& arg2,
			const std::string& arg3,
			const std::string& arg4,
			const std::string& arg5,
			std::string& result);
		bool ControlDelete(ControlID controlID);
		Hardware::HardwareParams* GetHardware(const ControlID controlID);
		unsigned int ControlsOfHardwareType(const HardwareType hardwareType);
		bool HardwareLibraryAdd(const HardwareType hardwareType, void* libraryHandle);
		void* HardwareLibraryGet(const HardwareType hardwareType) const;
		bool HardwareLibraryRemove(const HardwareType hardwareType);

		// control (console, web, ...)
		const std::string GetControlName(const ControlID controlID); // FIXME: => string& (reference)
		const std::map<std::string,Hardware::HardwareParams*> ControlListByName() const;
		const std::map<ControlID,std::string> LocoControlListNames() const;
		const std::map<ControlID,std::string> AccessoryControlListNames() const;
		const std::map<ControlID,std::string> FeedbackControlListNames() const;
		const std::map<ControlID,std::string> ProgramControlListNames() const;
		const std::map<std::string,Protocol> LocoProtocolsOfControl(const ControlID controlID) const { return ProtocolsOfControl(AddressTypeLoco, controlID); }
		const std::map<std::string,Protocol> AccessoryProtocolsOfControl(const ControlID controlID) const { return ProtocolsOfControl(AddressTypeAccessory, controlID); }

		DataModel::TrackBase* GetTrackBase(const DataModel::ObjectIdentifier& identifier) const;
		void TrackBasePublishState(const DataModel::TrackBase* trackBase);

		// loco
		DataModel::Loco* GetLoco(const LocoID locoID) const;
		const std::string& GetLocoName(const LocoID locoID) const;
		const std::map<LocoID,DataModel::Loco*>& locoList() const { return locos; }
		const std::map<std::string,LocoID> LocoListFree() const;
		const std::map<std::string,DataModel::Loco*> LocoListByName() const;
		bool LocoSave
		(
			const LocoID locoID,
			const std::string& name,
			const ControlID controlID,
			const Protocol protocol,
			const Address address,
			const Function nrOfFunctions,
			const Length length,
			const bool pushpull,
			const Speed maxSpeed,
			const Speed travelSpeed,
			const Speed reducedSpeed,
			const Speed creepingSpeed,
			const std::vector<DataModel::Relation*>& slaves,
			std::string& result
		);
		bool LocoDelete(const LocoID locoID);
		bool LocoProtocolAddress(const LocoID locoID, ControlID& controlID, Protocol& protocol, Address& address) const;
		void LocoSpeed(const ControlType controlType, const ControlID controlID, const Protocol protocol, const Address address, const Speed speed);
		bool LocoSpeed(const ControlType controlType, const LocoID locoID, const Speed speed, const bool withSlaves = true);
		bool LocoSpeed(const ControlType controlType, DataModel::Loco* loco, const Speed speed, const bool withSlaves = true);
		Speed LocoSpeed(const LocoID locoID) const;
		void LocoDirection(const ControlType controlType, const ControlID controlID, const Protocol protocol, const Address address, const Direction direction);
		void LocoDirection(const ControlType controlType, const LocoID locoID, const Direction direction);
		void LocoDirection(const ControlType controlType, DataModel::Loco* loco, const Direction direction);
		void LocoFunction(const ControlType controlType, const ControlID controlID, const Protocol protocol, const Address address, const Function function, const DataModel::LocoFunctions::FunctionState on);
		void LocoFunction(const ControlType controlType, const LocoID locoID, const Function function, const DataModel::LocoFunctions::FunctionState on);

		// accessory
		void AccessoryState(const ControlType controlType, const ControlID controlID, const Protocol protocol, const Address address, const DataModel::AccessoryState state);
		bool AccessoryState(const ControlType controlType, const AccessoryID accessoryID, const DataModel::AccessoryState state, const bool force);
		void AccessoryState(const ControlType controlType, const AccessoryID accessoryID, const DataModel::AccessoryState state, const bool inverted, const bool on);
		DataModel::Accessory* GetAccessory(const AccessoryID accessoryID) const;
		const std::string& GetAccessoryName(const AccessoryID accessoryID) const;
		const std::map<AccessoryID,DataModel::Accessory*>& AccessoryList() const { return accessories; }
		const std::map<std::string,DataModel::Accessory*> AccessoryListByName() const;
		bool AccessorySave(const AccessoryID accessoryID,
			const std::string& name,
			const DataModel::LayoutItem::LayoutPosition x,
			const DataModel::LayoutItem::LayoutPosition y,
			const DataModel::LayoutItem::LayoutPosition z,
			const ControlID controlID,
			const Protocol protocol,
			const Address address,
			const DataModel::AccessoryType type,
			const DataModel::AccessoryPulseDuration duration,
			const bool inverted, std::string& result);
		bool AccessoryDelete(const AccessoryID accessoryID);
		bool AccessoryRelease(const AccessoryID accessoryID);

		// feedback
		void FeedbackState(const ControlID controlID, const FeedbackPin pin, const DataModel::Feedback::FeedbackState state);
		void FeedbackState(const FeedbackID feedbackID, const DataModel::Feedback::FeedbackState state);
		void FeedbackPublishState(DataModel::Feedback* feedback);
		DataModel::Feedback* GetFeedback(const FeedbackID feedbackID) const;
		DataModel::Feedback* GetFeedbackUnlocked(const FeedbackID feedbackID) const;
		const std::string& GetFeedbackName(const FeedbackID feedbackID) const;
		const std::map<FeedbackID,DataModel::Feedback*>& FeedbackList() const { return feedbacks; }
		const std::map<std::string,DataModel::Feedback*> FeedbackListByName() const;
		const std::map<std::string,FeedbackID> FeedbacksOfTrack(const DataModel::ObjectIdentifier& identifier) const;
		FeedbackID FeedbackSave(const FeedbackID feedbackID, const std::string& name, const DataModel::LayoutItem::Visible visible, const DataModel::LayoutItem::LayoutPosition posX, const DataModel::LayoutItem::LayoutPosition posY, const DataModel::LayoutItem::LayoutPosition posZ, const ControlID controlID, const FeedbackPin pin, const bool inverted,  std::string& result);
		bool FeedbackDelete(const FeedbackID feedbackID);
		bool FeedbackExists(const FeedbackID feedbackID) const { return feedbacks.count(feedbackID) == 1; }

		// track
		DataModel::Track* GetTrack(const TrackID trackID) const;
		const std::string& GetTrackName(const TrackID trackID) const;
		const std::map<TrackID,DataModel::Track*>& TrackList() const { return tracks; }
		const std::map<std::string,DataModel::Track*> TrackListByName() const;
		const std::map<std::string,TrackID> TrackListIdByName() const;
		TrackID TrackSave(const TrackID trackID,
			const std::string& name,
			const DataModel::LayoutItem::LayoutPosition posX,
			const DataModel::LayoutItem::LayoutPosition posY,
			const DataModel::LayoutItem::LayoutPosition posZ,
			const DataModel::LayoutItem::LayoutItemSize width,
			const DataModel::LayoutItem::LayoutRotation rotation,
			const DataModel::TrackType trackType,
			const std::vector<FeedbackID>& feedbacks,
			const DataModel::SelectStreetApproach selectStreetApproach,
			const bool releaseWhenFree,
			std::string& result);
		bool TrackDelete(const TrackID trackID);
		const std::map<std::string,DataModel::ObjectIdentifier> TrackBaseListIdentifierByName() const;

		// switch
		bool SwitchState(const ControlType controlType, const SwitchID switchID, const DataModel::AccessoryState state, const bool force);
		DataModel::Switch* GetSwitch(const SwitchID switchID) const;
		const std::string& GetSwitchName(const SwitchID switchID) const;
		const std::map<SwitchID,DataModel::Switch*>& SwitchList() const { return switches; }
		const std::map<std::string,DataModel::Switch*> SwitchListByName() const;
		bool SwitchSave(const SwitchID switchID,
			const std::string& name,
			const DataModel::LayoutItem::LayoutPosition x,
			const DataModel::LayoutItem::LayoutPosition y,
			const DataModel::LayoutItem::LayoutPosition z,
			const DataModel::LayoutItem::LayoutRotation rotation,
			const ControlID controlID,
			const Protocol protocol,
			const Address address,
			const DataModel::AccessoryType type,
			const DataModel::AccessoryPulseDuration duration,
			const bool inverted, std::string& result);
		bool SwitchDelete(const SwitchID switchID);
		bool SwitchRelease(const SwitchID switchID);

		// street
		bool StreetExecute(Logger::Logger* logger, const LocoID locoID, const StreetID streetID);
		void StreetExecuteAsync(Logger::Logger* logger, const StreetID streetID);
		DataModel::Street* GetStreet(const StreetID streetID) const;
		const std::string& GetStreetName(const StreetID streetID) const;
		const std::map<StreetID,DataModel::Street*>& StreetList() const { return streets; }
		const std::map<std::string,DataModel::Street*> StreetListByName() const;
		bool StreetSave(const StreetID streetID,
			const std::string& name,
			const Delay delay,
			const DataModel::Street::PushpullType pushpull,
			const Length minTrainLength,
			const Length maxTrainLength,
			const std::vector<DataModel::Relation*>& relationsAtLock,
			const std::vector<DataModel::Relation*>& relationsAtUnlock,
			const DataModel::LayoutItem::Visible visible,
			const DataModel::LayoutItem::LayoutPosition posX,
			const DataModel::LayoutItem::LayoutPosition posY,
			const DataModel::LayoutItem::LayoutPosition posZ,
			const Automode automode,
			const DataModel::ObjectIdentifier& fromTrack,
			const Direction fromDirection,
			const DataModel::ObjectIdentifier& toTrack,
			const Direction toDirection,
			const DataModel::Street::Speed speed,
			const FeedbackID feedbackIdReduced,
			const FeedbackID feedbackIdCreep,
			const FeedbackID feedbackIdStop,
			const FeedbackID feedbackIdOver,
			const Pause waitAfterRelease,
			std::string& result);
		bool StreetDelete(const StreetID streetID);

		// layer
		DataModel::Layer* GetLayer(const LayerID layerID) const;
		const std::map<std::string,LayerID> LayerListByName() const;
		const std::map<std::string,LayerID> LayerListByNameWithFeedback() const;
		bool LayerSave(const LayerID layerID, const std::string&name, std::string& result);
		bool LayerDelete(const LayerID layerID);

		// signal
		bool SignalState(const ControlType controlType, const SignalID signalID, const DataModel::AccessoryState state, const bool force = false);
		bool SignalState(const ControlType controlType, DataModel::Signal* signal, const DataModel::AccessoryState state, const bool force = false);
		DataModel::Signal* GetSignal(const SignalID signalID) const;
		const std::string& GetSignalName(const SignalID signalID) const;
		const std::map<SignalID,DataModel::Signal*>& SignalList() const { return signals; }
		const std::map<std::string,DataModel::Signal*> SignalListByName() const;
		bool SignalSave(const SignalID signalID,
			const std::string& name,
			const DataModel::LayoutItem::LayoutPosition x,
			const DataModel::LayoutItem::LayoutPosition y,
			const DataModel::LayoutItem::LayoutPosition z,
			const DataModel::LayoutItem::LayoutItemSize height,
			const DataModel::LayoutItem::LayoutRotation rotation,
			const std::vector<FeedbackID>& newFeedbacks,
			const DataModel::SelectStreetApproach selectStreetApproach,
			const bool releaseWhenFree,
			const ControlID controlID,
			const Protocol protocol,
			const Address address,
			const DataModel::AccessoryType type,
			const DataModel::AccessoryPulseDuration duration,
			const bool inverted,
			std::string& result);
		bool SignalDelete(const SignalID signalID);
		bool SignalRelease(const SignalID signalID);
		void SignalBlock(const SignalID signalID, const bool blocked);
		void SignalSetLocoDirection(const SignalID signalID, const Direction direction);
		void SignalPublishState(const ControlType controlType, const DataModel::Signal* signal);

		// automode
		bool LocoIntoTrackBase(Logger::Logger* logger, const LocoID locoID, const DataModel::ObjectIdentifier& trackIdentifier);
		bool LocoRelease(const LocoID locoID);
		bool TrackRelease(const TrackID trackID);
		bool LocoReleaseInTrack(const TrackID trackID);
		bool LocoReleaseInSignal(const SignalID signalID);
		bool TrackStartLoco(const TrackID trackID);
		bool TrackStopLoco(const TrackID trackID);
		bool SignalStartLoco(const TrackID trackID);
		bool SignalStopLoco(const TrackID trackID);
		void TrackBlock(const TrackID trackID, const bool blocked);
		void TrackSetLocoDirection(const TrackID trackID, const Direction direction);
		void TrackPublishState(const DataModel::Track* track);
		bool StreetRelease(const StreetID streetID);
		bool LocoDestinationReached(const DataModel::Loco* loco, const DataModel::Street* street, const DataModel::TrackBase* track);
		bool LocoStart(const LocoID locoID);
		bool LocoStop(const LocoID locoID);
		bool LocoStartAll();
		bool LocoStopAll();
		void StopAllLocosImmediately(const ControlType controlType);

		// settings
		DataModel::AccessoryPulseDuration GetDefaultAccessoryDuration() const { return defaultAccessoryDuration; }
		bool GetAutoAddFeedback() const { return autoAddFeedback; }
		DataModel::SelectStreetApproach GetSelectStreetApproach() const { return selectStreetApproach; }
		DataModel::Loco::NrOfTracksToReserve GetNrOfTracksToReserve() const { return nrOfTracksToReserve; }
		bool SaveSettings(const DataModel::AccessoryPulseDuration duration,
			const bool autoAddFeedback,
			const DataModel::SelectStreetApproach selectStreetApproach,
			const DataModel::Loco::NrOfTracksToReserve nrOfTracksToReserve,
			const Logger::Logger::Level logLevel,
			const Languages::Language language);

		ControlID GetControlForLoco() const;
		ControlID GetControlForAccessory() const;
		ControlID GetControlForFeedback() const;

		void ProgramRead(const ControlID controlID, const ProgramMode mode, const Address address, const CvNumber cv);
		void ProgramWrite(const ControlID controlID, const ProgramMode mode, const Address address, const CvNumber cv, const CvValue value);
		void ProgramValue(const CvNumber cv, const CvValue value);
		static void ProgramDccValueStatic(Manager* manager, const CvNumber cv, const CvValue value)
		{
			manager->ProgramValue(cv, value);
		}

		bool CanHandleProgram();
		bool CanHandleProgramMm();
		bool CanHandleProgramDccRead();
		bool CanHandleProgramDccWrite();

	private:
		bool ControlIsOfHardwareType(const ControlID controlID, const HardwareType hardwareType);

		ControlInterface* GetControl(const ControlID controlID) const;
		DataModel::Loco* GetLoco(const ControlID controlID, const Protocol protocol, const Address address) const;
		DataModel::Accessory* GetAccessory(const ControlID controlID, const Protocol protocol, const Address address) const;
		DataModel::Switch* GetSwitch(const ControlID controlID, const Protocol protocol, const Address address) const;
		DataModel::Feedback* GetFeedback(const ControlID controlID, const FeedbackPin pin) const;
		DataModel::Signal* GetSignal(const ControlID controlID, const Protocol protocol, const Address address) const;

		void LocoFunction(const ControlType controlType, DataModel::Loco* loco, const Function function, const DataModel::LocoFunctions::FunctionState on);
		void AccessoryState(const ControlType controlType, DataModel::Accessory* accessory, const DataModel::AccessoryState state, const bool force);
		void SwitchState(const ControlType controlType, DataModel::Switch* mySwitch, const DataModel::AccessoryState state, const bool force);
		void FeedbackState(DataModel::Feedback* feedback, const DataModel::Feedback::FeedbackState state)  { feedback->SetState(state); }

		// layout
		bool CheckPositionFree(const DataModel::LayoutItem::LayoutPosition posX,
			const DataModel::LayoutItem::LayoutPosition posY,
			const DataModel::LayoutItem::LayoutPosition posZ,
			std::string& result) const;
		bool CheckPositionFree(const DataModel::LayoutItem::LayoutPosition posX,
			const DataModel::LayoutItem::LayoutPosition posY,
			const DataModel::LayoutItem::LayoutPosition posZ,
			const DataModel::LayoutItem::LayoutItemSize width,
			const DataModel::LayoutItem::LayoutItemSize height,
			const DataModel::LayoutItem::LayoutRotation rotation,
			std::string& result) const;

		template<class Type>
		bool CheckLayoutPositionFree(const DataModel::LayoutItem::LayoutPosition posX,
			const DataModel::LayoutItem::LayoutPosition posY,
			const DataModel::LayoutItem::LayoutPosition posZ,
			std::string& result,
			const std::map<ObjectID, Type*>& layoutVector, std::mutex& mutex) const;

		bool CheckAccessoryPosition(const DataModel::Accessory* accessory,
			const DataModel::LayoutItem::LayoutPosition posX,
			const DataModel::LayoutItem::LayoutPosition posY,
			const DataModel::LayoutItem::LayoutPosition posZ,
			std::string& result) const;

		bool CheckSwitchPosition(const DataModel::Switch* mySwitch,
			const DataModel::LayoutItem::LayoutPosition posX,
			const DataModel::LayoutItem::LayoutPosition posY,
			const DataModel::LayoutItem::LayoutPosition posZ,
			std::string& result) const;

		bool CheckStreetPosition(const DataModel::Street* street,
			const DataModel::LayoutItem::LayoutPosition posX,
			const DataModel::LayoutItem::LayoutPosition posY,
			const DataModel::LayoutItem::LayoutPosition posZ,
			std::string& result) const;

		bool CheckTrackPosition(const DataModel::Track* track,
			const DataModel::LayoutItem::LayoutPosition posX,
			const DataModel::LayoutItem::LayoutPosition posY,
			const DataModel::LayoutItem::LayoutPosition posZ,
			const DataModel::LayoutItem::LayoutItemSize height,
			const DataModel::LayoutItem::LayoutRotation rotation,
			std::string& result) const;

		bool CheckFeedbackPosition(const DataModel::Feedback* feedback,
			const DataModel::LayoutItem::LayoutPosition posX,
			const DataModel::LayoutItem::LayoutPosition posY,
			const DataModel::LayoutItem::LayoutPosition posZ,
			std::string& result) const;

		bool CheckSignalPosition(const DataModel::Signal* signal,
			const DataModel::LayoutItem::LayoutPosition posX,
			const DataModel::LayoutItem::LayoutPosition posY,
			const DataModel::LayoutItem::LayoutPosition posZ,
			std::string& result) const;

		bool CheckAddressLoco(const Protocol protocol, const Address address, std::string& result);
		bool CheckAddressAccessory(const Protocol protocol, const Address address, std::string& result);
		bool CheckControlLocoProtocolAddress(const ControlID controlID, const Protocol protocol, const Address address, std::string& result)
		{
			return CheckControlProtocolAddress(AddressTypeLoco, controlID, protocol, address, result);
		}
		bool CheckControlAccessoryProtocolAddress(const ControlID controlID, const Protocol protocol, const Address address, std::string& result)
		{
			return CheckControlProtocolAddress(AddressTypeAccessory, controlID, protocol, address, result);
		}
		bool CheckControlProtocolAddress(const AddressType type, const ControlID controlID, const Protocol protocol, const Address address, std::string& result);
		const std::map<std::string,Protocol> ProtocolsOfControl(const AddressType type, const ControlID) const;

		bool LocoReleaseInternal(DataModel::Loco* loco);

		template<class Key, class Value>
		void DeleteAllMapEntries(std::map<Key,Value*>& m, std::mutex& x)
		{
			std::lock_guard<std::mutex> Guard(x);
			while (m.size())
			{
				auto it = m.begin();
				Value* content = it->second;
				m.erase(it);
				if (storage != nullptr)
				{
					logger->Info(Languages::TextSaving, content->GetName());
					storage->Save(*content);
				}
				delete content;
			}
		}

		const std::vector<FeedbackID> CleanupAndCheckFeedbacksForTrack(const DataModel::ObjectIdentifier& identifier, const std::vector<FeedbackID>& newFeedbacks);
		void DebounceWorker();

		template<class ID, class T>
		T* CreateAndAddObject(std::map<ID,T*>& objects, std::mutex& mutex);

		Hardware::HardwareParams* CreateAndAddControl();

		template<class ID, class T>
		bool CheckObjectName(std::map<ID,T*>& objects, const std::string& name)
		{
			for (auto object : objects)
			{
				if (object.second->GetName().compare(name) == 0)
				{
					return false;
				}
			}
			return true;
		}

		bool CheckIfNumber(const char& c) { return c >= '0' && c <= '9'; }
		bool CheckIfThreeNumbers(const std::string& s)
		{
			size_t sSize = s.size();
			return sSize >= 3
			&& CheckIfNumber(s.at(sSize-1))
			&& CheckIfNumber(s.at(sSize-2))
			&& CheckIfNumber(s.at(sSize-3));
		}

		template<class ID, class T>
		std::string CheckObjectName(std::map<ID,T*>& objects, std::mutex& mutex, const ID objectID, const std::string& name)
		{
			std::lock_guard<std::mutex> Guard(mutex);
			if (objects.count(objectID) == 1)
			{
				const T* o = objects.at(objectID);
				const std::string& oldName = o->GetName();
				if (oldName.compare(name) == 0)
				{
					return name;
				}
			}
			if (CheckObjectName(objects, name))
			{
				return name;
			}
			unsigned int counter = 0;

			const std::string baseName = CheckIfThreeNumbers(name) ? name.substr(0, name.size() - 3) : name;

			while (true)
			{
				++counter;
				std::stringstream ss;
				ss << baseName << std::setw(3) << std::setfill('0') << counter;
				std::string newName = ss.str();
				if (CheckObjectName(objects, newName))
				{
					return newName;
				}
			}
		}

		bool LocoIntoTrackBase(Logger::Logger *logger, DataModel::Loco* loco, const ObjectType objectType, DataModel::TrackBase* track);

		void InitLocos();
		static void InitLocosStatic(Manager* manager) { manager->InitLocos(); }

		void ProgramCheckBooster(const ProgramMode mode);

		Logger::Logger* logger;
		volatile BoosterState boosterState;

		// FIXME: check usage of all mutexes

		// controls (Webserver, console & hardwareHandler. So each hardware is also added here).
		std::map<ControlID,ControlInterface*> controls;
		mutable std::mutex controlMutex;

		// hardware (virt, CS2, ...)
		std::map<ControlID,Hardware::HardwareParams*> hardwareParams;
		mutable std::mutex hardwareMutex;

		std::map<HardwareType,void*> hardwareLibraries;
		mutable std::mutex hardwareLibrariesMutex;

		// loco
		std::map<LocoID,DataModel::Loco*> locos;
		mutable std::mutex locoMutex;

		// accessory
		std::map<AccessoryID,DataModel::Accessory*> accessories;
		mutable std::mutex accessoryMutex;

		// feedback
		std::map<FeedbackID,DataModel::Feedback*> feedbacks;
		mutable std::mutex feedbackMutex;

		// track
		std::map<TrackID,DataModel::Track*> tracks;
		mutable std::mutex trackMutex;

		// switch
		std::map<SwitchID,DataModel::Switch*> switches;
		mutable std::mutex switchMutex;

		// street
		std::map<StreetID,DataModel::Street*> streets;
		mutable std::mutex streetMutex;

		// layer
		std::map<LayerID,DataModel::Layer*> layers;
		mutable std::mutex layerMutex;

		// signal
		std::map<SignalID,DataModel::Signal*> signals;
		mutable std::mutex signalMutex;

		// storage
		Storage::StorageHandler* storage;

		DataModel::AccessoryPulseDuration defaultAccessoryDuration;
		bool autoAddFeedback;
		DataModel::SelectStreetApproach selectStreetApproach;
		DataModel::Loco::NrOfTracksToReserve nrOfTracksToReserve;

		volatile bool run;
		volatile bool debounceRun;
		std::thread debounceThread;

		volatile bool initLocosDone;

		const std::string unknownControl;
		const std::string unknownLoco;
		const std::string unknownAccessory;
		const std::string unknownFeedback;
		const std::string unknownTrack;
		const std::string unknownSwitch;
		const std::string unknownStreet;
		const std::string unknownSignal;
};
