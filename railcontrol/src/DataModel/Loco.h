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

			Loco(Manager* manager, const LocoID locoID)
			:	Object(locoID),
				HardwareHandle(),
				manager(manager),
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
				wait(0)
			{
				logger = Logger::Logger::GetLogger(GetName());
			}

			Loco(Manager* manager, const std::string& serialized)
			:	Object(LocoNone),
				HardwareHandle(),
			 	manager(manager),
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
				wait(0)
			{
				Deserialize(serialized);
				logger = Logger::Logger::GetLogger(GetName());
			}

			virtual ~Loco();

			Logger::Logger* GetLogger() { return logger; }

			ObjectType GetObjectType() const { return ObjectTypeLoco; }

			std::string Serialize() const override;
			bool Deserialize(const std::string& serialized) override;

			virtual void SetName(const std::string& name) override
			{
				Object::SetName(name);
				logger = Logger::Logger::GetLogger(name);
			}

			bool GoToAutoMode();
			void RequestManualMode();
			bool GoToManualMode();

			bool SetTrack(const DataModel::ObjectIdentifier& identifier);
			bool Release();
			bool IsRunningFromTrack(const TrackID trackID) const;

			void LocationReached(const FeedbackID feedbackID);

			void SetSpeed(const Speed speed, const bool withSlaves);
			Speed GetSpeed() const { return speed; }

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

			inline const DataModel::LocoFunctionEntry* GetFunctions() const
			{
				return functions.GetFunctions();
			}

			inline void ConfigureFunctions(const std::vector<LocoFunctionEntry>& newEntries)
			{
				functions.ConfigureFunctions(newEntries);
			}

			void SetOrientation(const Orientation orientation);
			Orientation GetOrientation() const { return orientation; }

			bool IsInManualMode() const { return this->state == LocoStateManual; }
			bool IsInAutoMode() const { return this->state != LocoStateManual && this->state != LocoStateTerminated; }
			bool IsInUse() const { return this->speed > 0 || this->state != LocoStateManual || this->trackFrom != nullptr || this->routeFirst != nullptr; }

			bool GetPushpull() const { return pushpull; }
			Length GetLength() const { return length; }
			void SetLength(const Length length) {  this->length = length; }
			Speed GetMaxSpeed() const { return maxSpeed; }
			Speed GetTravelSpeed() const { return travelSpeed; }
			Speed GetReducedSpeed() const { return reducedSpeed; }
			Speed GetCreepingSpeed() const { return creepingSpeed; }
			void SetPushpull(bool pushpull) { this->pushpull = pushpull; }
			void SetMaxSpeed(Speed speed) { maxSpeed = speed; }
			void SetTravelSpeed(Speed speed) { travelSpeed = speed; }
			void SetReducedSpeed(Speed speed) { reducedSpeed = speed; }
			void SetCreepingSpeed(Speed speed) { creepingSpeed = speed; }

			bool AssignSlaves(const std::vector<DataModel::Relation*>& newslaves);
			const std::vector<DataModel::Relation*>& GetSlaves() const { return slaves; };

		private:
			void SetMinThreadPriorityAndThreadName();
			void AutoMode();
			void SearchDestinationFirst();
			void SearchDestinationSecond();
			DataModel::Route* SearchDestination(DataModel::TrackBase* oldToTrack, const bool allowLocoTurn);
			void FeedbackIdFirstReached();
			void FeedbackIdStopReached();
			void DeleteSlaves();
			void ForceManualMode();

			enum LocoState : unsigned char
			{
				LocoStateManual = 0,
				LocoStateTerminated,
				LocoStateOff,
				LocoStateSearchingFirst,
				LocoStateSearchingSecond,
				LocoStateRunning,
				LocoStateStopping,
				LocoStateError
			};

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

			LocoFunctions functions;

			Logger::Logger* logger;
	};
} // namespace DataModel
