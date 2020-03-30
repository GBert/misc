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
#include "DataModel/DataModel.h"
#include "Hardware/HardwareParams.h"
#include "Logger/Logger.h"
#include "Storage/StorageHandler.h"

class Manager
{
	public:
		Manager(Config& config);
		~Manager();

		// booster
		boosterState_t Booster() const { return boosterState; }
		void Booster(const controlType_t controlType, const boosterState_t status);

		// hardware (virt, CS2, ...)
		static const std::map<hardwareType_t,std::string> HardwareListNames();
		bool ControlSave(const controlID_t& controlID,
			const hardwareType_t& hardwareType,
			const std::string& name,
			const std::string& arg1,
			const std::string& arg2,
			const std::string& arg3,
			const std::string& arg4,
			const std::string& arg5,
			std::string& result);
		bool ControlDelete(controlID_t controlID);
		Hardware::HardwareParams* GetHardware(controlID_t controlID);
		unsigned int ControlsOfHardwareType(const hardwareType_t hardwareType);
		bool HardwareLibraryAdd(const hardwareType_t hardwareType, void* libraryHandle);
		void* HardwareLibraryGet(const hardwareType_t hardwareType) const;
		bool HardwareLibraryRemove(const hardwareType_t hardwareType);

		// control (console, web, ...)
		const std::string GetControlName(const controlID_t controlID); // FIXME: => string& (reference)
		const std::map<controlID_t,Hardware::HardwareParams*> controlList() const { return hardwareParams; }
		const std::map<std::string,Hardware::HardwareParams*> ControlListByName() const;
		const std::map<controlID_t,std::string> LocoControlListNames() const;
		const std::map<controlID_t,std::string> AccessoryControlListNames() const;
		const std::map<controlID_t,std::string> FeedbackControlListNames() const;
		const std::map<std::string,protocol_t> LocoProtocolsOfControl(const controlID_t controlID) const { return ProtocolsOfControl(AddressTypeLoco, controlID); }
		const std::map<std::string,protocol_t> AccessoryProtocolsOfControl(const controlID_t controlID) const { return ProtocolsOfControl(AddressTypeAccessory, controlID); }

		// loco
		DataModel::Loco* GetLoco(const locoID_t locoID) const;
		const std::string& GetLocoName(const locoID_t locoID) const;
		const std::map<locoID_t,DataModel::Loco*>& locoList() const { return locos; }
		const std::map<std::string,locoID_t> LocoListFree() const;
		const std::map<std::string,DataModel::Loco*> LocoListByName() const;
		bool LocoSave
		(
			const locoID_t locoID,
			const std::string& name,
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
			std::string& result
		);
		bool LocoDelete(const locoID_t locoID);
		bool LocoProtocolAddress(const locoID_t locoID, controlID_t& controlID, protocol_t& protocol, address_t& address) const;
		void LocoSpeed(const controlType_t controlType, const controlID_t controlID, const protocol_t protocol, const address_t address, const locoSpeed_t speed);
		bool LocoSpeed(const controlType_t controlType, const locoID_t locoID, const locoSpeed_t speed, const bool withSlaves = true);
		bool LocoSpeed(const controlType_t controlType, DataModel::Loco* loco, const locoSpeed_t speed, const bool withSlaves = true);
		locoSpeed_t LocoSpeed(const locoID_t locoID) const;
		void LocoDirection(const controlType_t controlType, const controlID_t controlID, const protocol_t protocol, const address_t address, const direction_t direction);
		void LocoDirection(const controlType_t controlType, const locoID_t locoID, const direction_t direction);
		void LocoDirection(const controlType_t controlType, DataModel::Loco* loco, const direction_t direction);
		void LocoFunction(const controlType_t controlType, const controlID_t controlID, const protocol_t protocol, const address_t address, const function_t function, const bool on);
		void LocoFunction(const controlType_t controlType, const locoID_t locoID, const function_t function, const bool on);

		// accessory
		void AccessoryState(const controlType_t controlType, const controlID_t controlID, const protocol_t protocol, const address_t address, const accessoryState_t state);
		bool AccessoryState(const controlType_t controlType, const accessoryID_t accessoryID, const accessoryState_t state, const bool force);
		void AccessoryState(const controlType_t controlType, const accessoryID_t accessoryID, const accessoryState_t state, const bool inverted, const bool on);
		DataModel::Accessory* GetAccessory(const accessoryID_t accessoryID) const;
		const std::string& GetAccessoryName(const accessoryID_t accessoryID) const;
		const std::map<accessoryID_t,DataModel::Accessory*>& AccessoryList() const { return accessories; }
		const std::map<std::string,DataModel::Accessory*> AccessoryListByName() const;
		bool AccessorySave(const accessoryID_t accessoryID, const std::string& name, const layoutPosition_t x, const layoutPosition_t y, const layoutPosition_t z, const controlID_t controlID, const protocol_t protocol, const address_t address, const accessoryType_t type, const accessoryDuration_t timeout, const bool inverted, std::string& result);
		bool AccessoryDelete(const accessoryID_t accessoryID);
		bool AccessoryRelease(const accessoryID_t accessoryID);

		// feedback
		void FeedbackState(const controlID_t controlID, const feedbackPin_t pin, const DataModel::Feedback::feedbackState_t state);
		void FeedbackState(const feedbackID_t feedbackID, const DataModel::Feedback::feedbackState_t state);
		void FeedbackPublishState(DataModel::Feedback* feedback);
		DataModel::Feedback* GetFeedback(const feedbackID_t feedbackID) const;
		DataModel::Feedback* GetFeedbackUnlocked(const feedbackID_t feedbackID) const;
		const std::string& GetFeedbackName(const feedbackID_t feedbackID) const;
		const std::map<feedbackID_t,DataModel::Feedback*>& FeedbackList() const { return feedbacks; }
		const std::map<std::string,DataModel::Feedback*> FeedbackListByName() const;
		const std::map<std::string,feedbackID_t> FeedbacksOfTrack(const trackID_t trackID) const;
		feedbackID_t FeedbackSave(const feedbackID_t feedbackID, const std::string& name, const visible_t visible, const layoutPosition_t posX, const layoutPosition_t posY, const layoutPosition_t posZ, const controlID_t controlID, const feedbackPin_t pin, const bool inverted,  std::string& result);
		bool FeedbackDelete(const feedbackID_t feedbackID);
		bool FeedbackExists(const feedbackID_t feedbackID) const { return feedbacks.count(feedbackID) == 1; }

		// track
		DataModel::Track* GetTrack(const trackID_t trackID) const;
		const std::string& GetTrackName(const trackID_t trackID) const;
		const std::map<trackID_t,DataModel::Track*>& TrackList() const { return tracks; }
		const std::map<std::string,DataModel::Track*> TrackListByName() const;
		const std::map<std::string,trackID_t> TrackListIdByName() const;
		trackID_t TrackSave(const trackID_t trackID,
			const std::string& name,
			const layoutPosition_t posX,
			const layoutPosition_t posY,
			const layoutPosition_t posZ,
			const layoutItemSize_t width,
			const DataModel::LayoutItem::layoutRotation_t rotation,
			const DataModel::Track::type_t trackType,
			std::vector<feedbackID_t> feedbacks,
			const DataModel::Track::selectStreetApproach_t selectStreetApproach,
			const bool releaseWhenFree,
			std::string& result);
		bool TrackDelete(const trackID_t trackID);

		// switch
		bool SwitchState(const controlType_t controlType, const switchID_t switchID, const switchState_t state, const bool force);
		DataModel::Switch* GetSwitch(const switchID_t switchID) const;
		const std::string& GetSwitchName(const switchID_t switchID) const;
		const std::map<switchID_t,DataModel::Switch*>& SwitchList() const { return switches; }
		const std::map<std::string,DataModel::Switch*> SwitchListByName() const;
		bool SwitchSave(const switchID_t switchID, const std::string& name, const layoutPosition_t x, const layoutPosition_t y, const layoutPosition_t z, const DataModel::LayoutItem::layoutRotation_t rotation, const controlID_t controlID, const protocol_t protocol, const address_t address, const switchType_t type, const switchDuration_t timeout, const bool inverted, std::string& result);
		bool SwitchDelete(const switchID_t switchID);
		bool SwitchRelease(const switchID_t switchID);

		// street
		bool StreetExecute(Logger::Logger* logger, const locoID_t locoID, const streetID_t streetID);
		void StreetExecuteAsync(Logger::Logger* logger, const streetID_t streetID);
		DataModel::Street* GetStreet(const streetID_t streetID) const;
		const std::string& GetStreetName(const streetID_t streetID) const;
		const std::map<streetID_t,DataModel::Street*>& StreetList() const { return streets; }
		const std::map<std::string,DataModel::Street*> StreetListByName() const;
		bool StreetSave(const streetID_t streetID,
			const std::string& name,
			const delay_t delay,
			const DataModel::Street::PushpullType pushpull,
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
			const DataModel::Street::Speed speed,
			const feedbackID_t feedbackIdReduced,
			const feedbackID_t feedbackIdCreep,
			const feedbackID_t feedbackIdStop,
			const feedbackID_t feedbackIdOver,
			const wait_t waitAfterRelease,
			std::string& result);
		bool StreetDelete(const streetID_t streetID);

		// layer
		DataModel::Layer* GetLayer(const layerID_t layerID) const;
		const std::map<std::string,layerID_t> LayerListByName() const;
		const std::map<std::string,layerID_t> LayerListByNameWithFeedback() const;
		bool LayerSave(const layerID_t layerID, const std::string&name, std::string& result);
		bool LayerDelete(const layerID_t layerID);

		// signal
		bool SignalState(const controlType_t controlType, const signalID_t signalID, const signalState_t state, const bool force);
		void SignalState(const controlType_t controlType, DataModel::Signal* signal, const accessoryState_t state, const bool force);
		DataModel::Signal* GetSignal(const signalID_t signalID) const;
		const std::string& GetSignalName(const signalID_t signalID) const;
		const std::map<signalID_t,DataModel::Signal*>& SignalList() const { return signals; }
		const std::map<std::string,DataModel::Signal*> SignalListByName() const;
		bool SignalSave(const signalID_t signalID,
			const std::string& name,
			const layoutPosition_t x,
			const layoutPosition_t y,
			const layoutPosition_t z,
			const DataModel::LayoutItem::layoutRotation_t rotation,
			const controlID_t controlID,
			const protocol_t protocol,
			const address_t address,
			const signalType_t type,
			const signalDuration_t timeout,
			const bool inverted,
			std::string& result);
		bool SignalDelete(const signalID_t signalID);
		bool SignalRelease(const signalID_t signalID);

		// automode
		bool LocoIntoTrack(Logger::Logger* logger, const locoID_t locoID, const trackID_t trackID);
		bool LocoRelease(const locoID_t locoID);
		bool TrackRelease(const trackID_t trackID);
		bool LocoReleaseInTrack(const trackID_t trackID);
		bool TrackStartLoco(const trackID_t trackID);
		bool TrackStopLoco(const trackID_t trackID);
		void TrackBlock(const trackID_t trackID, const bool blocked);
		void TrackSetLocoDirection(const trackID_t trackID, const direction_t direction);
		void TrackPublishState(const DataModel::Track* track);
		bool StreetRelease(const streetID_t streetID);
		bool LocoDestinationReached(const locoID_t locoID, const streetID_t streetID, const trackID_t trackID);
		bool LocoStart(const locoID_t locoID);
		bool LocoStop(const locoID_t locoID);
		bool LocoStartAll();
		bool LocoStopAll();
		void StopAllLocosImmediately(const controlType_t controlType);

		// settings
		accessoryDuration_t GetDefaultAccessoryDuration() const { return defaultAccessoryDuration; }
		bool GetAutoAddFeedback() const { return autoAddFeedback; }
		DataModel::Track::selectStreetApproach_t GetSelectStreetApproach() const { return selectStreetApproach; }
		DataModel::Loco::nrOfTracksToReserve_t GetNrOfTracksToReserve() const { return nrOfTracksToReserve; }
		bool SaveSettings(const accessoryDuration_t duration,
			const bool autoAddFeedback,
			const DataModel::Track::selectStreetApproach_t selectStreetApproach,
			const DataModel::Loco::nrOfTracksToReserve_t nrOfTracksToReserve,
			const Logger::Logger::logLevel_t logLevel,
			const Languages::language_t language);

		controlID_t GetControlForLoco() const;
		controlID_t GetControlForAccessory() const;
		controlID_t GetControlForFeedback() const;

	private:
		ControlInterface* GetControl(const controlID_t controlID) const;
		DataModel::Loco* GetLoco(const controlID_t controlID, const protocol_t protocol, const address_t address) const;
		DataModel::Accessory* GetAccessory(const controlID_t controlID, const protocol_t protocol, const address_t address) const;
		DataModel::Switch* GetSwitch(const controlID_t controlID, const protocol_t protocol, const address_t address) const;
		DataModel::Feedback* GetFeedback(const controlID_t controlID, const feedbackPin_t pin) const;
		DataModel::Signal* GetSignal(const controlID_t controlID, const protocol_t protocol, const address_t address) const;

		void LocoFunction(const controlType_t controlType, DataModel::Loco* loco, const function_t function, const bool on);
		void AccessoryState(const controlType_t controlType, DataModel::Accessory* accessory, const accessoryState_t state, const bool force);
		void SwitchState(const controlType_t controlType, DataModel::Switch* mySwitch, const accessoryState_t state, const bool force);
		void FeedbackState(DataModel::Feedback* feedback, const DataModel::Feedback::feedbackState_t state)  { feedback->SetState(state); }

		// layout
		bool CheckPositionFree(const layoutPosition_t posX, const layoutPosition_t posY, const layoutPosition_t posZ, std::string& result) const;
		bool CheckPositionFree(const layoutPosition_t posX,
			const layoutPosition_t posY,
			const layoutPosition_t posZ,
			const layoutItemSize_t width,
			const layoutItemSize_t height,
			const DataModel::LayoutItem::layoutRotation_t rotation,
			std::string& result) const;

		template<class Type>
		bool CheckLayoutPositionFree(const layoutPosition_t posX,
			const layoutPosition_t posY,
			const layoutPosition_t posZ,
			std::string& result,
			const std::map<objectID_t, Type*>& layoutVector, std::mutex& mutex) const;

		bool CheckAccessoryPosition(const DataModel::Accessory* accessory,
			const layoutPosition_t posX,
			const layoutPosition_t posY,
			const layoutPosition_t posZ,
			std::string& result) const;

		bool CheckSwitchPosition(const DataModel::Switch* mySwitch,
			const layoutPosition_t posX,
			const layoutPosition_t posY,
			const layoutPosition_t posZ,
			std::string& result) const;

		bool CheckStreetPosition(const DataModel::Street* street,
			const layoutPosition_t posX,
			const layoutPosition_t posY,
			const layoutPosition_t posZ,
			std::string& result) const;

		bool CheckTrackPosition(const DataModel::Track* track,
			const layoutPosition_t posX,
			const layoutPosition_t posY,
			const layoutPosition_t posZ,
			const layoutItemSize_t height,
			const DataModel::LayoutItem::layoutRotation_t rotation,
			std::string& result) const;

		bool CheckFeedbackPosition(const DataModel::Feedback* feedback,
			const layoutPosition_t posX,
			const layoutPosition_t posY,
			const layoutPosition_t posZ,
			std::string& result) const;

		bool CheckSignalPosition(const DataModel::Signal* signal,
			const layoutPosition_t posX,
			const layoutPosition_t posY,
			const layoutPosition_t posZ,
			std::string& result) const;

		bool CheckAddressLoco(const protocol_t protocol, const address_t address, std::string& result);
		bool CheckAddressAccessory(const protocol_t protocol, const address_t address, std::string& result);
		bool CheckControlLocoProtocolAddress(const controlID_t controlID, const protocol_t protocol, const address_t address, std::string& result)
		{
			return CheckControlProtocolAddress(AddressTypeLoco, controlID, protocol, address, result);
		}
		bool CheckControlAccessoryProtocolAddress(const controlID_t controlID, const protocol_t protocol, const address_t address, std::string& result)
		{
			return CheckControlProtocolAddress(AddressTypeAccessory, controlID, protocol, address, result);
		}
		bool CheckControlProtocolAddress(const addressType_t type, const controlID_t controlID, const protocol_t protocol, const address_t address, std::string& result);
		const std::map<std::string,protocol_t> ProtocolsOfControl(const addressType_t type, const controlID_t) const;

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

		const std::vector<feedbackID_t> CleanupAndCheckFeedbacks(trackID_t trackID, std::vector<feedbackID_t>& newFeedbacks);
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

		void InitLocos();

		Logger::Logger* logger;
		volatile boosterState_t boosterState;

		// FIXME: check usage of all mutexes

		// controls (Webserver, console & hardwareHandler. So each hardware is also added here).
		std::map<controlID_t,ControlInterface*> controls;
		mutable std::mutex controlMutex;

		// hardware (virt, CS2, ...)
		std::map<controlID_t,Hardware::HardwareParams*> hardwareParams;
		mutable std::mutex hardwareMutex;

		std::map<hardwareType_t,void*> hardwareLibraries;
		mutable std::mutex hardwareLibrariesMutex;

		// loco
		std::map<locoID_t,DataModel::Loco*> locos;
		mutable std::mutex locoMutex;

		// accessory
		std::map<accessoryID_t,DataModel::Accessory*> accessories;
		mutable std::mutex accessoryMutex;

		// feedback
		std::map<feedbackID_t,DataModel::Feedback*> feedbacks;
		mutable std::mutex feedbackMutex;

		// track
		std::map<trackID_t,DataModel::Track*> tracks;
		mutable std::mutex trackMutex;

		// switch
		std::map<switchID_t,DataModel::Switch*> switches;
		mutable std::mutex switchMutex;

		// street
		std::map<streetID_t,DataModel::Street*> streets;
		mutable std::mutex streetMutex;

		// layer
		std::map<layerID_t,DataModel::Layer*> layers;
		mutable std::mutex layerMutex;

		// signal
		std::map<signalID_t,DataModel::Signal*> signals;
		mutable std::mutex signalMutex;

		// storage
		Storage::StorageHandler* storage;

		accessoryDuration_t defaultAccessoryDuration;
		bool autoAddFeedback;
		DataModel::Track::selectStreetApproach_t selectStreetApproach;
		DataModel::Loco::nrOfTracksToReserve_t nrOfTracksToReserve;

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
