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
	class Street;
	class Track;

	class Loco : public Object, public HardwareHandle
	{
		public:
			enum nrOfTracksToReserve_t : unsigned char
			{
				ReserveOne = 1,
				ReserveTwo = 2
			};

			Loco(Manager* manager, const locoID_t locoID)
			:	Object(locoID),
			 	manager(manager),
				speed(MinSpeed),
				direction(DirectionRight),
				state(LocoStateManual),
				requestManualMode(false),
				trackFrom(nullptr),
				trackFirst(nullptr),
				trackSecond(nullptr),
				streetFirst(nullptr),
				streetSecond(nullptr),
				feedbackIdFirst(FeedbackNone),
				feedbackIdReduced(FeedbackNone),
				feedbackIdCreep(FeedbackNone),
				feedbackIdStop(FeedbackNone),
				feedbackIdOver(FeedbackNone),
				feedbackIdsReached(),
				wait(0)
			{
				logger = Logger::Logger::GetLogger(GetName());
				SetNrOfFunctions(0);
			}

			Loco(Manager* manager, const std::string& serialized)
			:	Object(LocoNone),
			 	manager(manager),
				speed(MinSpeed),
				direction(DirectionRight),
				state(LocoStateManual),
				requestManualMode(false),
				trackFrom(nullptr),
				trackFirst(nullptr),
				trackSecond(nullptr),
				streetFirst(nullptr),
				streetSecond(nullptr),
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

			objectType_t GetObjectType() const { return ObjectTypeLoco; }

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

			bool ToTrack(const trackID_t trackID);
			bool Release();
			bool IsRunningFromTrack(const trackID_t trackID) const;

			void LocationReached(const feedbackID_t feedbackID);

			void Speed(const locoSpeed_t speed, const bool withSlaves);
			locoSpeed_t Speed() const { return speed; }

			void SetFunction(const function_t nr, const bool state) { functions.SetFunction(nr, state); }
			bool GetFunction(const function_t nr) const { return functions.GetFunction(nr); }
			std::vector<bool> GetFunctions() const { return functions.GetFunctions(); }
			void SetNrOfFunctions(const function_t nr) { functions.SetNrOfFunctions(nr); }
			function_t GetNrOfFunctions() const { return functions.GetNrOfFunctions(); }
			void SetDirection(const direction_t direction);
			direction_t GetDirection() const { return direction; }

			bool IsInManualMode() const { return this->state == LocoStateManual; }
			bool IsInAutoMode() const { return this->state != LocoStateManual && this->state != LocoStateTerminated; }
			bool IsInUse() const { return this->speed > 0 || this->state != LocoStateManual || this->trackFrom != nullptr || this->streetFirst != nullptr; }

			bool GetPushpull() const { return pushpull; }
			length_t GetLength() const { return length; }
			void SetLength(const length_t length) {  this->length = length; }
			locoSpeed_t GetMaxSpeed() const { return maxSpeed; }
			locoSpeed_t GetTravelSpeed() const { return travelSpeed; }
			locoSpeed_t GetReducedSpeed() const { return reducedSpeed; }
			locoSpeed_t GetCreepingSpeed() const { return creepingSpeed; }
			void SetPushpull(bool pushpull) { this->pushpull = pushpull; }
			void SetMaxSpeed(locoSpeed_t speed) { maxSpeed = speed; }
			void SetTravelSpeed(locoSpeed_t speed) { travelSpeed = speed; }
			void SetReducedSpeed(locoSpeed_t speed) { reducedSpeed = speed; }
			void SetCreepingSpeed(locoSpeed_t speed) { creepingSpeed = speed; }

			bool AssignSlaves(const std::vector<DataModel::Relation*>& newslaves);
			const std::vector<DataModel::Relation*>& GetSlaves() const { return slaves; };

		private:
			void SetMinThreadPriorityAndThreadName();
			void AutoMode();
			void SearchDestinationFirst();
			void SearchDestinationSecond();
			DataModel::Street* SearchDestination(DataModel::Track* oldToTrack, const bool allowLocoTurn);
			void FeedbackIdFirstReached();
			void FeedbackIdStopReached();
			void DeleteSlaves();
			void ForceManualMode();

			enum locoState_t : unsigned char
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

			length_t length;
			bool pushpull;
			locoSpeed_t maxSpeed;
			locoSpeed_t travelSpeed;
			locoSpeed_t reducedSpeed;
			locoSpeed_t creepingSpeed;

			locoSpeed_t speed;
			direction_t direction;

			std::vector<DataModel::Relation*> slaves;

			volatile locoState_t state;
			volatile bool requestManualMode;
			Track* trackFrom;
			Track* trackFirst;
			Track* trackSecond;
			Street* streetFirst;
			Street* streetSecond;
			volatile feedbackID_t feedbackIdFirst;
			volatile feedbackID_t feedbackIdReduced;
			volatile feedbackID_t feedbackIdCreep;
			volatile feedbackID_t feedbackIdStop;
			volatile feedbackID_t feedbackIdOver;
			Utils::ThreadSafeQueue<feedbackID_t> feedbackIdsReached;
			wait_t wait;

			LocoFunctions functions;

			Logger::Logger* logger;
	};
} // namespace DataModel
