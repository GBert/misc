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

#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "DataTypes.h"
#include "Logger/Logger.h"
#include "DataModel/HardwareHandle.h"
#include "DataModel/LocoFunctions.h"
#include "DataModel/Object.h"
#include "DataModel/Relation.h"
#include "Utils/ThreadSafeQueue.h"

class Manager;

namespace Hardware
{
		class LocoCacheEntry;
}

namespace DataModel
{
	class ObjectIdentifier;
	class Route;
	class TrackBase;

	class Loco : public Object, public HardwareHandle
	{
		public:
			enum NrOfTracksToReserve : unsigned char
			{
				ReserveOne = 1,
				ReserveTwo = 2
			};

			enum AutoModeType : unsigned char
			{
				AutoModeTypeFull = 0,
				AutoModeTypeTimetable
			};

			Loco() = delete;
			Loco(const Loco&) = delete;
			Loco& operator=(const Loco&) = delete;

			inline Loco(Manager* manager, const LocoID locoID)
			:	Object(locoID),
				HardwareHandle(),
				manager(manager),
				length(0),
				pushpull(false),
				maxSpeed(0),
				travelSpeed(0),
				reducedSpeed(0),
				creepingSpeed(0),
				speed(MinSpeed),
				orientation(OrientationRight),
				state(LocoStateManual),
				requestManualMode(false),
				trackFrom(nullptr),
				trackFirst(nullptr),
				trackSecond(nullptr),
				routeFirst(nullptr),
				routeSecond(nullptr),
				feedbackIdFirst(FeedbackNone),
				feedbackIdReduced(FeedbackNone),
				feedbackIdCreep(FeedbackNone),
				feedbackIdStop(FeedbackNone),
				feedbackIdOver(FeedbackNone),
				feedbackIdsReached(),
				wait(0),
				matchKey("")
			{
				logger = Logger::Logger::GetLogger(GetName());
			}

			inline Loco(Manager* manager, const std::string& serialized)
			:	Loco(manager, LocoNone)
			{
				Deserialize(serialized);
			}

			virtual ~Loco();

			inline Logger::Logger* GetLogger()
			{
				return logger;
			}

			inline ObjectType GetObjectType() const override
			{
				return ObjectTypeLoco;
			}

			std::string Serialize() const override;
			using HardwareHandle::Deserialize;
			bool Deserialize(const std::string& serialized) override;

			virtual void SetName(const std::string& name) override
			{
				Object::SetName(name);
				logger = Logger::Logger::GetLogger(name);
			}

			bool GoToAutoMode(const AutoModeType type = AutoModeTypeFull);
			void RequestManualMode();
			bool GoToManualMode();

			bool AddTimeTable(ObjectIdentifier& identifier);

			bool SetTrack(const DataModel::ObjectIdentifier& identifier);
			TrackID GetTrackId();
			bool Release();
			bool IsRunningFromTrack(const TrackID trackID) const;

			void LocationReached(const FeedbackID feedbackID);

			void SetSpeed(const Speed speed, const bool withSlaves);

			inline Speed GetSpeed() const
			{
				return speed;
			}

			inline void SetFunctionState(const DataModel::LocoFunctionNr nr,
				const DataModel::LocoFunctionState state)
			{
				functions.SetFunctionState(nr, state);
			}

			inline DataModel::LocoFunctionState GetFunctionState(const DataModel::LocoFunctionNr nr) const
			{
				return functions.GetFunctionState(nr);
			}

			inline std::vector<DataModel::LocoFunctionEntry> GetFunctionStates() const
			{
				return functions.GetFunctionStates();
			}

			inline void GetFunctions(LocoFunctionEntry* out) const
			{
				functions.GetFunctions(out);
			}

			inline void ConfigureFunctions(const std::vector<LocoFunctionEntry>& newEntries)
			{
				functions.ConfigureFunctions(newEntries);
			}

			void SetOrientation(const Orientation orientation);

			inline Orientation GetOrientation() const
			{
				return orientation;
			}

			inline bool IsInManualMode() const
			{
				return this->state == LocoStateManual;
			}

			inline bool IsInAutoMode() const
			{
				return this->state != LocoStateManual
					&& this->state != LocoStateTerminated;
			}

			inline bool IsInUse() const
			{
				return this->speed > 0
					|| this->state != LocoStateManual
					|| this->trackFrom != nullptr
					|| this->routeFirst != nullptr;
			}

			inline bool GetPushpull() const
			{
				return pushpull;
			}

			inline Length GetLength() const
			{
				return length;
			}

			inline void SetLength(const Length length)
			{
				this->length = length;
			}

			inline Speed GetMaxSpeed() const
			{
				return maxSpeed;
			}

			inline Speed GetTravelSpeed() const
			{
				return travelSpeed;
			}

			inline Speed GetReducedSpeed() const
			{
				return reducedSpeed;
			}

			inline Speed GetCreepingSpeed() const
			{
				return creepingSpeed;
			}

			inline void SetPushpull(bool pushpull)
			{
				this->pushpull = pushpull;
			}

			inline void SetMaxSpeed(Speed speed)
			{
				maxSpeed = speed;
			}

			inline void SetTravelSpeed(Speed speed)
			{
				travelSpeed = speed;
			}

			inline void SetReducedSpeed(Speed speed)
			{
				reducedSpeed = speed;
			}

			inline void SetCreepingSpeed(Speed speed)
			{
				creepingSpeed = speed;
			}

			bool AssignSlaves(const std::vector<DataModel::Relation*>& newslaves);

			inline const std::vector<DataModel::Relation*>& GetSlaves() const
			{
				return slaves;
			}

			inline void SetMatchKey(const std::string& matchKey)
			{
				this->matchKey = matchKey;
			}

			inline void ClearMatchKey()
			{
				matchKey.clear();
			}

			inline std::string GetMatchKey() const
			{
				return matchKey;
			}

			DataModel::LocoFunctionNr GetFunctionNumberFromFunctionIcon(DataModel::LocoFunctionIcon icon) const;

			Loco& operator=(const Hardware::LocoCacheEntry& loco);

		private:
			enum LocoState : unsigned char
			{
				LocoStateManual = 0,
				LocoStateTerminated,
				LocoStateOff,
				LocoStateAutomodeGetFirst,
				LocoStateAutomodeGetSecond,
				LocoStateAutomodeRunning,
				LocoStateTimetableGetFirst,
				LocoStateTimetableGetSecond,
				LocoStateTimetableRunning,
				LocoStateStopping,
				LocoStateError
			};

			void SetMinThreadPriorityAndThreadName();
			void AutoMode();
			void SearchDestinationFirst();
			void GetDestinationFirst();
			void PrepareDestinationFirst(Route* const route, const LocoState newState);
			void SearchDestinationSecond();
			void GetDestinationSecond();
			DataModel::Route* GetDestinationFromTimeTable(const TrackBase* const track, const bool allowLocoTurn);
			void PrepareDestinationSecond(Route* const route, const LocoState newState);
			DataModel::Route* SearchDestination(const DataModel::TrackBase* const oldToTrack, const bool allowLocoTurn);
			bool ReserveRoute(const TrackBase* const track, const bool allowLocoTurn, Route* const route);
			void FeedbackIdFirstReached();
			void FeedbackIdStopReached();
			void DeleteSlaves();
			void ForceManualMode();
			bool GoToAutoModeInternal(const LocoState newState);

			Manager* manager;
			mutable std::mutex stateMutex;
			std::thread locoThread;

			Length length;
			bool pushpull;
			Speed maxSpeed;
			Speed travelSpeed;
			Speed reducedSpeed;
			Speed creepingSpeed;

			Speed speed;
			Orientation orientation;

			std::vector<DataModel::Relation*> slaves;

			volatile LocoState state;
			volatile bool requestManualMode;
			TrackBase* trackFrom;
			TrackBase* trackFirst;
			TrackBase* trackSecond;
			Route* routeFirst;
			Route* routeSecond;
			volatile FeedbackID feedbackIdFirst;
			volatile FeedbackID feedbackIdReduced;
			volatile FeedbackID feedbackIdCreep;
			volatile FeedbackID feedbackIdStop;
			volatile FeedbackID feedbackIdOver;
			Utils::ThreadSafeQueue<FeedbackID> feedbackIdsReached;
			Pause wait;
			Utils::ThreadSafeQueue<RouteID> timeTableQueue;
			std::string matchKey;

			LocoFunctions functions;


			Logger::Logger* logger;
	};
} // namespace DataModel
