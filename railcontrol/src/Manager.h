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

#pragma once

#include <map>
#include <mutex>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>

#include "Config.h"
#include "ControlInterface.h"
#include "DataModel/AccessoryConfig.h"
#include "DataModel/DataModel.h"
#include "DataModel/LocoConfig.h"
#include "Hardware/HardwareParams.h"
#include "Hardware/LocoCache.h"
#include "Logger/Logger.h"
#include "Storage/StorageHandler.h"

class Manager
{
	public:
		Manager() = delete;
		Manager(const Manager&) = delete;
		Manager& operator=(const Manager&) = delete;

		Manager(Config& config);
		~Manager();

		// booster
		inline BoosterState Booster() const
		{
			return boosterState;
		}

		void Booster(const ControlType controlType, const BoosterState status);

		// hardware (virt, CS2, ...)
		bool ControlSave(ControlID controlID,
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

		// control (console, web, ...)
		const std::string GetControlName(const ControlID controlID);
		const std::map<std::string,Hardware::HardwareParams*> ControlListByName() const;
		const std::map<ControlID,std::string> LocoControlListNames() const;
		const std::map<ControlID,std::string> AccessoryControlListNames() const;
		const std::map<ControlID,std::string> FeedbackControlListNames() const;
		const std::map<ControlID,std::string> ProgramControlListNames() const;

		inline const std::map<std::string,Protocol> LocoProtocolsOfControl(const ControlID controlID) const
		{
			return ProtocolsOfControl(AddressTypeLoco, controlID);
		}

		inline const std::map<std::string,Protocol> AccessoryProtocolsOfControl(const ControlID controlID) const
		{
			return ProtocolsOfControl(AddressTypeAccessory, controlID);
		}

		DataModel::TrackBase* GetTrackBase(const DataModel::ObjectIdentifier& identifier) const;
		void TrackBasePublishState(const DataModel::TrackBase* trackBase);

		// loco
		std::string GetLocoList() const;
		std::string GetRouteList() const;

		DataModel::Loco* GetLoco(const LocoID locoID) const;

		DataModel::LocoConfig GetLocoOfConfigByMatchKey(const ControlID controlId, const std::string& matchKey) const;

		DataModel::Loco* GetLocoByMatchKey(const ControlID controlId, const std::string& matchKey) const;

		void LocoRemoveMatchKey(const LocoID locoId);

		void LocoReplaceMatchKey(const LocoID locoId, const std::string& newMatchKey);

		const std::map<std::string,DataModel::LocoConfig> GetUnmatchedLocosOfControl(const ControlID controlId,
			const std::string& matchKey) const;

		const std::string& GetLocoName(const LocoID locoID) const;

		const std::map<std::string,LocoID> LocoListFree() const;

		const std::map<std::string,DataModel::LocoConfig> LocoConfigByName() const;

		const std::map<std::string,LocoID> LocoIdsByName() const;

		bool LocoSave
		(
			LocoID locoID,
			const std::string& name,
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
			const Propulsion propulsion,
			const TrainType type,
			const std::vector<DataModel::LocoFunctionEntry>& locoFunctions,
			const std::vector<DataModel::Relation*>& slaves,
			std::string& result
		);

		bool LocoDelete(const LocoID locoID,
			std::string& result);

		inline bool LocoDelete(const LocoID locoId)
		{
			std::string result;
			return LocoDelete(locoId, result);
		}

		bool LocoProtocolAddress(const LocoID locoID, ControlID& controlID, Protocol& protocol, Address& address) const;
		void LocoSpeed(const ControlType controlType, const ControlID controlID, const Protocol protocol, const Address address, const Speed speed);
		bool LocoSpeed(const ControlType controlType, const LocoID locoID, const Speed speed, const bool withSlaves = true);
		bool LocoSpeed(const ControlType controlType, DataModel::Loco* loco, const Speed speed, const bool withSlaves = true);
		Speed LocoSpeed(const LocoID locoID) const;
		void LocoOrientation(const ControlType controlType, const ControlID controlID, const Protocol protocol, const Address address, const Orientation orientation);
		void LocoOrientation(const ControlType controlType, const LocoID locoID, const Orientation orientation);
		void LocoOrientation(const ControlType controlType, DataModel::Loco* loco, const Orientation orientation);

		void LocoFunctionState(const ControlType controlType,
			const ControlID controlID,
			const Protocol protocol,
			const Address address,
			const DataModel::LocoFunctionNr function,
			const DataModel::LocoFunctionState on);

		void LocoFunctionState(const ControlType controlType,
			const LocoID locoID,
			const DataModel::LocoFunctionNr function,
			const DataModel::LocoFunctionState on);

		// accessory
		void AccessoryState(const ControlType controlType, const ControlID controlID, const Protocol protocol, const Address address, const DataModel::AccessoryState state);
		bool AccessoryState(const ControlType controlType, const AccessoryID accessoryID, const DataModel::AccessoryState state, const bool force);
		void AccessoryState(const ControlType controlType, const AccessoryID accessoryID, const DataModel::AccessoryState state, const bool inverted, const bool on);
		DataModel::Accessory* GetAccessory(const AccessoryID accessoryID) const;

		const std::map<std::string,DataModel::AccessoryConfig> GetUnmatchedAccessoriesOfControl(const ControlID controlId,
			const std::string& matchKey) const;

		const std::string& GetAccessoryName(const AccessoryID accessoryID) const;

		inline const std::map<AccessoryID,DataModel::Accessory*>& AccessoryList() const
		{
			return accessories;
		}

		const std::map<std::string,DataModel::AccessoryConfig> AccessoryConfigByName() const;

		bool AccessorySave(AccessoryID accessoryID,
			const std::string& name,
			const DataModel::LayoutItem::LayoutPosition x,
			const DataModel::LayoutItem::LayoutPosition y,
			const DataModel::LayoutItem::LayoutPosition z,
			const ControlID controlID,
			const std::string& matchKey,
			const Protocol protocol,
			const Address address,
			const DataModel::AccessoryType type,
			const DataModel::AccessoryPulseDuration duration,
			const bool inverted,
			std::string& result);

		bool AccessoryDelete(const AccessoryID accessoryID,
			std::string& result);

		bool AccessoryRelease(const AccessoryID accessoryID);

		DataModel::AccessoryConfig GetAccessoryOfConfigByMatchKey(const ControlID controlId, const std::string& matchKey) const;

		DataModel::Accessory* GetAccessoryByMatchKey(const ControlID controlId, const std::string& matchKey) const;

		void AccessoryRemoveMatchKey(const AccessoryID accessoryId);

		// feedback
		void FeedbackState(const ControlID controlID, const FeedbackPin pin, const DataModel::Feedback::FeedbackState state);
		void FeedbackState(const FeedbackID feedbackID, const DataModel::Feedback::FeedbackState state);
		void FeedbackPublishState(const DataModel::Feedback* feedback);
		DataModel::Feedback* GetFeedback(const FeedbackID feedbackID) const;
		DataModel::Feedback* GetFeedbackUnlocked(const FeedbackID feedbackID) const;
		const std::string& GetFeedbackName(const FeedbackID feedbackID) const;

		inline const std::map<FeedbackID,DataModel::Feedback*>& FeedbackList() const
		{
			return feedbacks;
		}

		const std::map<std::string,DataModel::Feedback*> FeedbackListByName() const;
		const std::map<std::string,FeedbackID> FeedbacksOfTrack(const DataModel::ObjectIdentifier& identifier) const;

		bool FeedbackSave(FeedbackID feedbackID,
			const std::string& name,
			const DataModel::LayoutItem::Visible visible,
			const DataModel::LayoutItem::LayoutPosition posX,
			const DataModel::LayoutItem::LayoutPosition posY,
			const DataModel::LayoutItem::LayoutPosition posZ,
			const ControlID controlID,
			const FeedbackPin pin,
			const bool inverted,
			std::string& result);

		bool FeedbackDelete(const FeedbackID feedbackID,
			std::string& result);

		inline bool FeedbackExists(const FeedbackID feedbackID) const
		{
			return feedbacks.count(feedbackID) == 1;
		}

		// track
		DataModel::Track* GetTrack(const TrackID trackID) const;
		const std::string& GetTrackName(const TrackID trackID) const;

		inline const std::map<TrackID,DataModel::Track*>& TrackList() const
		{
			return tracks;
		}

		const std::map<std::string,DataModel::Track*> TrackListByName() const;
		const std::map<std::string,TrackID> TrackListIdByName() const;

		bool TrackSave(const TrackID trackID,
			const std::string& name,
			const bool showName,
			const DataModel::LayoutItem::LayoutPosition posX,
			const DataModel::LayoutItem::LayoutPosition posY,
			const DataModel::LayoutItem::LayoutPosition posZ,
			const DataModel::LayoutItem::LayoutItemSize width,
			const DataModel::LayoutItem::LayoutRotation rotation,
			const DataModel::TrackType trackType,
			const std::vector<FeedbackID>& feedbacks,
			const std::vector<DataModel::Relation*>& newSignals,
			const DataModel::SelectRouteApproach selectRouteApproach,
			const bool allowLocoTurn,
			const bool releaseWhenFree,
			std::string& result);

		bool TrackDelete(const TrackID trackID,
			std::string& result);

		const std::map<std::string,DataModel::ObjectIdentifier> TrackBaseListIdentifierByName() const;

		// switch
		bool SwitchState(const ControlType controlType, const SwitchID switchID, const DataModel::AccessoryState state, const bool force);
		DataModel::Switch* GetSwitch(const SwitchID switchID) const;
		const std::string& GetSwitchName(const SwitchID switchID) const;

		inline const std::map<SwitchID,DataModel::Switch*>& SwitchList() const
		{
			return switches;
		}

		const std::map<std::string,DataModel::AccessoryConfig> SwitchConfigByName() const;

		bool SwitchSave(SwitchID switchID,
			const std::string& name,
			const DataModel::LayoutItem::LayoutPosition x,
			const DataModel::LayoutItem::LayoutPosition y,
			const DataModel::LayoutItem::LayoutPosition z,
			const DataModel::LayoutItem::LayoutRotation rotation,
			const ControlID controlID,
			const std::string& matchKey,
			const Protocol protocol,
			const Address address,
			const DataModel::AccessoryType type,
			const DataModel::AccessoryPulseDuration duration,
			const bool inverted,
			std::string& result);

		bool SwitchDelete(const SwitchID switchID,
			std::string& result);

		bool SwitchRelease(const SwitchID switchID);

		DataModel::Switch* GetSwitchByMatchKey(const ControlID controlId, const std::string& matchKey) const;
		void SwitchRemoveMatchKey(const SwitchID switchId);

		// route
		bool RouteExecute(Logger::Logger* logger, const LocoID locoID, const RouteID routeID);
		void RouteExecuteAsync(Logger::Logger* logger, const RouteID routeID);
		DataModel::Route* GetRoute(const RouteID routeID) const;
		const std::string& GetRouteName(const RouteID routeID) const;

		inline const std::map<RouteID,DataModel::Route*>& RouteList() const
		{
			return routes;
		}

		const std::map<std::string,DataModel::Route*> RouteListByName() const;
		bool RouteSave(RouteID routeID,
			const std::string& name,
			const Delay delay,
			const DataModel::Route::PushpullType pushpull,
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
			const Orientation fromOrientation,
			const DataModel::ObjectIdentifier& toTrack,
			const Orientation toOrientation,
			const DataModel::Route::Speed speed,
			const FeedbackID feedbackIdReduced,
			const FeedbackID feedbackIdCreep,
			const FeedbackID feedbackIdStop,
			const FeedbackID feedbackIdOver,
			const Pause waitAfterRelease,
			std::string& result);

		bool RouteDelete(const RouteID routeID,
			std::string& result);

		DataModel::Route* GetFirstRouteFromOrToTrackBase(const DataModel::ObjectIdentifier& identifier) const;

		inline bool HasRouteFromOrToTrackBase(const DataModel::ObjectIdentifier& identifier) const
		{
			return (GetFirstRouteFromOrToTrackBase(identifier) != nullptr);
		}

		// layer
		DataModel::Layer* GetLayer(const LayerID layerID) const;
		const std::map<std::string,LayerID> LayerListByName() const;
		const std::map<std::string,LayerID> LayerListByNameWithFeedback() const;
		bool LayerSave(const LayerID layerID, const std::string&name, std::string& result);

		bool LayerDelete(const LayerID layerID,
			std::string& result);

		// signal
		bool SignalState(const ControlType controlType, const SignalID signalID, const DataModel::AccessoryState state, const bool force = false);
		bool SignalState(const ControlType controlType, DataModel::Signal* signal, const DataModel::AccessoryState state, const bool force = false);
		DataModel::Signal* GetSignal(const SignalID signalID) const;
		const std::string& GetSignalName(const SignalID signalID) const;

		inline const std::map<SignalID,DataModel::Signal*>& SignalList() const
		{
			return signals;
		}

		const std::map<std::string,DataModel::Signal*> SignalListByName() const;
		const std::map<std::string,DataModel::AccessoryConfig> SignalConfigByName() const;

		bool SignalSave(SignalID signalID,
			const std::string& name,
			const Orientation signalOrientation,
			const DataModel::LayoutItem::LayoutPosition x,
			const DataModel::LayoutItem::LayoutPosition y,
			const DataModel::LayoutItem::LayoutPosition z,
			const DataModel::LayoutItem::LayoutItemSize height,
			const DataModel::LayoutItem::LayoutRotation rotation,
			const std::vector<FeedbackID>& newFeedbacks,
			const DataModel::SelectRouteApproach selectRouteApproach,
			const bool allowLocoTurn,
			const bool releaseWhenFree,
			const ControlID controlID,
			const std::string& matchKey,
			const Protocol protocol,
			const Address address,
			const DataModel::AccessoryType type,
			const std::map<DataModel::AccessoryState,DataModel::AddressOffset>& offsets,
			const DataModel::AccessoryPulseDuration duration,
			const bool inverted,
			std::string& result);

		bool SignalDelete(const SignalID signalID,
			std::string& result);

		void SignalPublishState(const ControlType controlType, const DataModel::Signal* signal);

		DataModel::Signal* GetSignalByMatchKey(const ControlID controlId, const std::string& matchKey) const;
		void SignalRemoveMatchKey(const SignalID signalId);

		// cluster
		DataModel::Cluster* GetCluster(const ClusterID clusterID) const;
		const std::map<std::string,DataModel::Cluster*> ClusterListByName() const;

		bool ClusterSave(ClusterID clusterID,
			const std::string& name,
			const std::vector<DataModel::Relation*>& newTracks,
			const std::vector<DataModel::Relation*>& newSignals,
			std::string& result);

		bool ClusterDelete(const ClusterID clusterID);

		// text
		DataModel::Text* GetText(const TextID textID) const;

		inline const std::map<TextID,DataModel::Text*>& TextList() const
		{
			return texts;
		}

		const std::map<std::string,DataModel::Text*> TextListByName() const;

		bool TextSave(TextID textID,
			const std::string& name,
			const DataModel::LayoutItem::LayoutPosition x,
			const DataModel::LayoutItem::LayoutPosition y,
			const DataModel::LayoutItem::LayoutPosition z,
			const DataModel::LayoutItem::LayoutItemSize width,
			const DataModel::LayoutItem::LayoutRotation rotation,
			std::string& result);

		bool TextDelete(const TextID textID,
			std::string& result);

		// automode
		bool LocoIntoTrackBase(Logger::Logger* logger, const LocoID locoID, const DataModel::ObjectIdentifier& trackIdentifier);
		bool LocoRelease(const LocoID locoID);
		bool TrackBaseRelease(const DataModel::ObjectIdentifier& objectIdentifier);
		bool LocoReleaseOnTrackBase(const DataModel::ObjectIdentifier& objectIdentifier);

		bool TrackBaseStartLoco(const DataModel::ObjectIdentifier& objectIdentifier,
			const DataModel::Loco::AutoModeType type);

		bool TrackBaseStopLoco(const DataModel::ObjectIdentifier& objectIdentifier);
		void TrackBaseBlock(const DataModel::ObjectIdentifier& objectIdentifier, const bool blocked);
		void TrackBaseSetLocoOrientation(const DataModel::ObjectIdentifier& objectIdentifier, const Orientation orientation);
		void TrackPublishState(const DataModel::Track* track);
		bool RouteRelease(const RouteID routeID);

		bool LocoDestinationReached(const DataModel::Loco* loco,
			const DataModel::Route* route,
			const DataModel::TrackBase* track);

		bool LocoStart(const LocoID locoID,
			const DataModel::Loco::AutoModeType type);

		bool LocoStop(const LocoID locoID);
		bool LocoStartAll();
		bool LocoStopAll();
		void StopAllLocosImmediately(const ControlType controlType);

		bool LocoAddTimeTable(const LocoID locoId, DataModel::ObjectIdentifier& identifier);

		// settings
		inline DataModel::AccessoryPulseDuration GetDefaultAccessoryDuration() const
		{
			return defaultAccessoryDuration;
		}

		inline bool GetAutoAddFeedback() const
		{
			return autoAddFeedback;
		}

		inline bool GetStopOnFeedbackInFreeTrack() const
		{
			return stopOnFeedbackInFreeTrack;
		}

		inline DataModel::SelectRouteApproach GetSelectRouteApproach() const
		{
			return selectRouteApproach;
		}

		inline DataModel::Loco::NrOfTracksToReserve GetNrOfTracksToReserve() const
		{
			return nrOfTracksToReserve;
		}

		bool SaveSettings(const Languages::Language language,
			const DataModel::AccessoryPulseDuration duration,
			const bool autoAddFeedback,
			const bool stopOnFeedbackInFreeTrack,
			const DataModel::SelectRouteApproach selectRouteApproach,
			const DataModel::Loco::NrOfTracksToReserve nrOfTracksToReserve,
			const Logger::Logger::Level logLevel
			);

		ControlID GetPossibleControlForLoco() const;
		ControlID GetPossibleControlForAccessory() const;
		ControlID GetPossibleControlForFeedback() const;

		void ProgramRead(const ControlID controlID, const ProgramMode mode, const Address address, const CvNumber cv);
		void ProgramWrite(const ControlID controlID, const ProgramMode mode, const Address address, const CvNumber cv, const CvValue value);
		void ProgramValue(const CvNumber cv, const CvValue value);

		inline static void ProgramDccValueStatic(Manager* manager, const CvNumber cv, const CvValue value)
		{
			manager->ProgramValue(cv, value);
		}

		bool CanHandle(const Hardware::Capabilities capability) const;
		bool CanHandle(const ControlID controlId, const Hardware::Capabilities capability) const;
		Hardware::Capabilities GetCapabilities(const ControlID controlID) const;

		bool LayoutItemRotate(const DataModel::ObjectIdentifier& identifier,
			std::string& result);

		bool LayoutItemNewPosition(const DataModel::ObjectIdentifier& identifier,
			const DataModel::LayoutItem::LayoutItemSize posX,
			const DataModel::LayoutItem::LayoutItemSize posY,
			std::string& result);

	private:
		bool ControlIsOfHardwareType(const ControlID controlID, const HardwareType hardwareType);

		ControlInterface* GetControl(const ControlID controlID) const;
		DataModel::Loco* GetLoco(const ControlID controlID, const Protocol protocol, const Address address) const;
		DataModel::Accessory* GetAccessory(const ControlID controlID, const Protocol protocol, const Address address) const;
		DataModel::Switch* GetSwitch(const ControlID controlID, const Protocol protocol, const Address address) const;
		DataModel::Feedback* GetFeedback(const ControlID controlID, const FeedbackPin pin) const;
		DataModel::Signal* GetSignal(const ControlID controlID, const Protocol protocol, const Address address) const;

		void LocoFunctionState(const ControlType controlType,
			DataModel::Loco* loco,
			const DataModel::LocoFunctionNr function,
			const DataModel::LocoFunctionState on);

		void AccessoryState(const ControlType controlType, DataModel::Accessory* accessory, const DataModel::AccessoryState state, const bool force);
		void SwitchState(const ControlType controlType, DataModel::Switch* mySwitch, const DataModel::AccessoryState state, const bool force);

		inline void FeedbackState(DataModel::Feedback* feedback, const DataModel::Feedback::FeedbackState state)
		{
			feedback->SetState(state);
		}

		bool AccessoryPosition(const AccessoryID accessoryID,
			const DataModel::LayoutItem::LayoutPosition posX,
			const DataModel::LayoutItem::LayoutPosition posY,
			std::string& result);

		bool FeedbackPosition(const FeedbackID feedbackID,
			const DataModel::LayoutItem::LayoutPosition posX,
			const DataModel::LayoutItem::LayoutPosition posY,
			std::string& result);

		bool RoutePosition(const RouteID routeID,
			const DataModel::LayoutItem::LayoutPosition posX,
			const DataModel::LayoutItem::LayoutPosition posY,
			std::string& result);

		bool SignalPosition(const SignalID signalID,
			const DataModel::LayoutItem::LayoutPosition posX,
			const DataModel::LayoutItem::LayoutPosition posY,
			std::string& result);

		bool SwitchPosition(const SwitchID switchID,
			const DataModel::LayoutItem::LayoutPosition posX,
			const DataModel::LayoutItem::LayoutPosition posY,
			std::string& result);

		bool TextPosition(const TextID textID,
			const DataModel::LayoutItem::LayoutPosition posX,
			const DataModel::LayoutItem::LayoutPosition posY,
			std::string& result);

		bool TrackPosition(const TrackID trackID,
			const DataModel::LayoutItem::LayoutPosition posX,
			const DataModel::LayoutItem::LayoutPosition posY,
			std::string& result);

		bool SignalRotate(const SignalID signalID,
			std::string& result);

		bool SwitchRotate(const SwitchID switchID,
			std::string& result);

		bool TextRotate(const TextID textID,
			std::string& result);

		bool TrackRotate(const TrackID trackID,
			std::string& result);

		void AccessorySaveAndPublishSettings(const DataModel::Accessory* const accessory);
		void FeedbackSaveAndPublishSettings(const DataModel::Feedback* const feedback);
		void RouteSaveAndPublishSettings(const DataModel::Route* const route);
		void SignalSaveAndPublishSettings(const DataModel::Signal* const signal);
		void SwitchSaveAndPublishSettings(const DataModel::Switch* const mySwitch);
		void TextSaveAndPublishSettings(const DataModel::Text* const text);
		void TrackSaveAndPublishSettings(const DataModel::Track* const track);

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

		bool CheckLayoutItemPosition(const DataModel::LayoutItem* layoutItem,
			const DataModel::LayoutItem::LayoutPosition posX,
			const DataModel::LayoutItem::LayoutPosition posY,
			const DataModel::LayoutItem::LayoutPosition posZ,
			std::string& result) const;

		bool CheckLayoutItemPosition(const DataModel::LayoutItem* item,
			const DataModel::LayoutItem::LayoutPosition posX,
			const DataModel::LayoutItem::LayoutPosition posY,
			const DataModel::LayoutItem::LayoutPosition posZ,
			const DataModel::LayoutItem::LayoutItemSize width,
			const DataModel::LayoutItem::LayoutItemSize height,
			const DataModel::LayoutItem::LayoutRotation rotation,
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

		bool LayerHasElements(const DataModel::Layer* layer,
			std::string& result);

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
			for (auto& object : objects)
			{
				if (object.second->GetName().compare(name) == 0)
				{
					return false;
				}
			}
			return true;
		}

		inline bool CheckIfNumber(const char& c)
		{
			return c >= '0' && c <= '9';
		}

		inline bool CheckIfThreeNumbers(const std::string& s)
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

		static inline void InitLocosStatic(Manager* manager)
		{
			manager->InitLocos();
		}

		void ProgramCheckBooster(const ProgramMode mode);

		bool ObjectIsPartOfRoute(const DataModel::ObjectIdentifier& identifier,
			const DataModel::Object* object,
			std::string& result);

		Logger::Logger* logger;
		volatile BoosterState boosterState;

		// FIXME: check usage of all mutexes

		// controls (Webserver & hardwareHandler. So each hardware is also added here).
		std::map<ControlID,ControlInterface*> controls;
		mutable std::mutex controlMutex;

		// hardware (virt, CS2, ...)
		std::map<ControlID,Hardware::HardwareParams*> hardwareParams;
		mutable std::mutex hardwareMutex;

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

		// route
		std::map<RouteID,DataModel::Route*> routes;
		mutable std::mutex routeMutex;

		// layer
		std::map<LayerID,DataModel::Layer*> layers;
		mutable std::mutex layerMutex;

		// signal
		std::map<SignalID,DataModel::Signal*> signals;
		mutable std::mutex signalMutex;

		// cluster
		std::map<SignalID,DataModel::Cluster*> clusters;
		mutable std::mutex clusterMutex;

		// text
		std::map<TextID,DataModel::Text*> texts;
		mutable std::mutex textMutex;

		// storage
		Storage::StorageHandler* storage;

		DataModel::AccessoryPulseDuration defaultAccessoryDuration;
		bool autoAddFeedback;
		bool stopOnFeedbackInFreeTrack;
		DataModel::SelectRouteApproach selectRouteApproach;
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
		const std::string unknownRoute;
		const std::string unknownSignal;
};
