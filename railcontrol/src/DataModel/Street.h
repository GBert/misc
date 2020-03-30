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
#include <vector>

#include "DataTypes.h"
#include "DataModel/LayoutItem.h"
#include "DataModel/LockableItem.h"
#include "Logger/Logger.h"

class Manager;

namespace DataModel
{
	class Loco;
	class Relation;

	class Street : public LayoutItem, public LockableItem
	{
		public:
			static const delay_t DefaultDelay = 250;

			enum PushpullType : unsigned char
			{
				PushpullTypeNo = 0,
				PushpullTypeOnly = 1,
				PushpullTypeBoth = 2
			};

			enum Speed : unsigned char
			{
				SpeedMax = 3,
				SpeedTravel = 2,
				SpeedReduced = 1,
				SpeedCreeping = 0
			};

			Street() = delete;

			Street(Manager* manager, const streetID_t streetID)
			:	LayoutItem(streetID),
			 	LockableItem(),
			 	manager(manager),
				executeAtUnlock(false),
				delay(0),
				pushpull(PushpullTypeBoth),
				minTrainLength(0),
				maxTrainLength(0),
				automode(AutomodeNo),
				fromTrack(TrackNone),
				fromDirection(DirectionRight),
				toTrack(TrackNone),
				toDirection(DirectionRight),
				speed(SpeedTravel),
				feedbackIdReduced(FeedbackNone),
				feedbackIdCreep(FeedbackNone),
				feedbackIdStop(FeedbackNone),
				feedbackIdOver(FeedbackNone),
				waitAfterRelease(0),
				lastUsed(0),
				counter(0)
			{
			}

			Street(Manager* manager, const std::string& serialized);

			~Street()
			{
				DeleteRelations(relationsAtLock);
				DeleteRelations(relationsAtUnlock);
			}

			objectType_t GetObjectType() const { return ObjectTypeStreet; }

			std::string Serialize() const override;
			bool Deserialize(const std::string& serialized) override;
			std::string LayoutType() const override { return Languages::GetText(Languages::TextStreet); };

			void DeleteRelationsAtLock() { DeleteRelations(relationsAtLock); };
			void DeleteRelationsAtUnlock() { DeleteRelations(relationsAtUnlock); };
			bool AssignRelationsAtLock(const std::vector<DataModel::Relation*>& newRelations)
			{
				return AssignRelations(relationsAtLock, newRelations);
			}
			bool AssignRelationsAtUnlock(const std::vector<DataModel::Relation*>& newRelations)
			{
				return AssignRelations(relationsAtUnlock, newRelations);
			}
			const std::vector<DataModel::Relation*>& GetRelationsAtLock() const { return relationsAtLock; };
			const std::vector<DataModel::Relation*>& GetRelationsAtUnlock() const { return relationsAtUnlock; };

			bool FromTrackDirection(Logger::Logger* logger, const trackID_t trackID, const direction_t trackDirection, const DataModel::Loco* loco, const bool allowLocoTurn);

			bool Execute(Logger::Logger* logger, const locoID_t locoID);
			static bool ExecuteStatic(Logger::Logger* logger, Street* street) { return street->Execute(logger, LocoNone); }

			bool Reserve(Logger::Logger* logger, const locoID_t locoID) override;
			bool Lock(Logger::Logger* logger, const locoID_t locoID) override;
			bool Release(Logger::Logger* logger, const locoID_t locoID) override;

			delay_t GetDelay() const { return delay; }
			void SetDelay(delay_t delay) { this->delay = delay; }
			PushpullType GetPushpull() const { return pushpull; }
			void SetPushpull(const PushpullType pushpull) { this->pushpull = pushpull; }
			length_t GetMinTrainLength() const { return minTrainLength; }
			void SetMinTrainLength(const length_t length) { this->minTrainLength = length; }
			length_t GetMaxTrainLength() const { return maxTrainLength; }
			void SetMaxTrainLength(const length_t length) { this->maxTrainLength = length; }
			time_t GetLastUsed() const { return lastUsed; }
			void SetAutomode(const automode_t automode) { this->automode = automode; }
			automode_t GetAutomode() const { return automode; }
			void SetFromTrack(const trackID_t fromTrack) { this->fromTrack = fromTrack; }
			trackID_t GetFromTrack() const { return fromTrack; }
			void SetFromDirection(const direction_t fromDirection) { this->fromDirection = fromDirection; }
			direction_t GetFromDirection() const { return fromDirection; }
			void SetToTrack(const trackID_t toTrack) { this->toTrack = toTrack; }
			trackID_t GetToTrack() const { return toTrack; };
			void SetToDirection(const direction_t toDirection) { this->toDirection = toDirection; }
			direction_t GetToDirection() const { return toDirection; }
			void SetSpeed(Speed startSpeed) { this->speed = startSpeed; }
			Speed GetSpeed() const { return speed; }
			void SetFeedbackIdReduced(const feedbackID_t feedbackIdReduced) { this->feedbackIdReduced = feedbackIdReduced; }
			feedbackID_t GetFeedbackIdReduced() const { return feedbackIdReduced; }
			void SetFeedbackIdCreep(const feedbackID_t feedbackIdCreep) { this->feedbackIdCreep = feedbackIdCreep; }
			feedbackID_t GetFeedbackIdCreep() const { return feedbackIdCreep; }
			void SetFeedbackIdStop(const feedbackID_t feedbackIdStop) { this->feedbackIdStop = feedbackIdStop; }
			feedbackID_t GetFeedbackIdStop() const { return feedbackIdStop; }
			void SetFeedbackIdOver(const feedbackID_t feedbackIdOver) { this->feedbackIdOver = feedbackIdOver; }
			feedbackID_t GetFeedbackIdOver() const { return feedbackIdOver; }
			void SetWaitAfterRelease(const wait_t wait) { this->waitAfterRelease = wait; }
			wait_t GetWaitAfterRelease() const { return waitAfterRelease; }

			static bool CompareShortest(const Street* s1, const Street* s2) { return s1->GetMinTrainLength() < s2->GetMinTrainLength(); }
			static bool CompareLastUsed(const Street* s1, const Street* s2) { return s1->GetLastUsed() < s2->GetLastUsed(); }

		private:
			bool ReleaseInternal(Logger::Logger* logger, const locoID_t locoID);
			void ReleaseInternalWithToTrack(Logger::Logger* logger, const locoID_t locoID);
			static void DeleteRelations(std::vector<DataModel::Relation*>& relations);
			bool AssignRelations(std::vector<DataModel::Relation*>& relations, const std::vector<DataModel::Relation*>& newRelations);

			Manager* manager;
			std::mutex updateMutex;
			bool executeAtUnlock;

			delay_t delay;
			std::vector<DataModel::Relation*> relationsAtLock;
			std::vector<DataModel::Relation*> relationsAtUnlock;
			PushpullType pushpull;
			length_t minTrainLength;
			length_t maxTrainLength;
			automode_t automode;
			trackID_t fromTrack;
			direction_t fromDirection;
			trackID_t toTrack;
			direction_t toDirection;

			Speed speed;
			feedbackID_t feedbackIdReduced;
			feedbackID_t feedbackIdCreep;
			feedbackID_t feedbackIdStop;
			feedbackID_t feedbackIdOver;
			wait_t waitAfterRelease;

			time_t lastUsed;
			unsigned int counter;
	};
} // namespace DataModel

