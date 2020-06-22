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
#include "DataModel/ObjectIdentifier.h"
#include "Logger/Logger.h"

class Manager;

namespace DataModel
{
	class Loco;
	class Relation;

	class Route : public LayoutItem, public LockableItem
	{
		public:
			static const Delay DefaultDelay = 250;

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

			Route() = delete;

			Route(Manager* manager, const RouteID routeID)
			:	LayoutItem(routeID),
			 	LockableItem(),
			 	manager(manager),
				executeAtUnlock(false),
				delay(0),
				pushpull(PushpullTypeBoth),
				minTrainLength(0),
				maxTrainLength(0),
				automode(AutomodeNo),
				fromTrack(),
				fromOrientation(OrientationRight),
				toTrack(),
				toOrientation(OrientationRight),
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

			Route(Manager* manager, const std::string& serialized);

			~Route()
			{
				DeleteRelations(relationsAtLock);
				DeleteRelations(relationsAtUnlock);
			}

			ObjectType GetObjectType() const { return ObjectTypeRoute; }

			std::string Serialize() const override;
			bool Deserialize(const std::string& serialized) override;
			std::string GetLayoutType() const override { return Languages::GetText(Languages::TextRoute); };

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

			bool FromTrackOrientation(Logger::Logger* logger, const DataModel::ObjectIdentifier& identifier, const Orientation trackOrientation, const DataModel::Loco* loco, const bool allowLocoTurn);

			bool Execute(Logger::Logger* logger, const LocoID locoID);
			static bool ExecuteStatic(Logger::Logger* logger, Route* route) { return route->Execute(logger, LocoNone); }

			bool Reserve(Logger::Logger* logger, const LocoID locoID) override;
			bool Lock(Logger::Logger* logger, const LocoID locoID) override;
			bool Release(Logger::Logger* logger, const LocoID locoID) override;

			Delay GetDelay() const { return delay; }
			void SetDelay(Delay delay) { this->delay = delay; }
			PushpullType GetPushpull() const { return pushpull; }
			void SetPushpull(const PushpullType pushpull) { this->pushpull = pushpull; }
			Length GetMinTrainLength() const { return minTrainLength; }
			void SetMinTrainLength(const Length length) { this->minTrainLength = length; }
			Length GetMaxTrainLength() const { return maxTrainLength; }
			void SetMaxTrainLength(const Length length) { this->maxTrainLength = length; }
			time_t GetLastUsed() const { return lastUsed; }
			void SetAutomode(const Automode automode) { this->automode = automode; }
			Automode GetAutomode() const { return automode; }
			void SetFromTrack(const ObjectIdentifier& fromTrack) { this->fromTrack = fromTrack; }
			const ObjectIdentifier& GetFromTrack() const { return fromTrack; }
			void SetFromOrientation(const Orientation fromOrientation) { this->fromOrientation = fromOrientation; }
			Orientation GetFromOrientation() const { return fromOrientation; }
			void SetToTrack(const ObjectIdentifier& toTrack) { this->toTrack = toTrack; }
			const ObjectIdentifier& GetToTrack() const { return toTrack; };
			void SetToOrientation(const Orientation toOrientation) { this->toOrientation = toOrientation; }
			Orientation GetToOrientation() const { return toOrientation; }
			void SetSpeed(Speed startSpeed) { this->speed = startSpeed; }
			Speed GetSpeed() const { return speed; }
			void SetFeedbackIdReduced(const FeedbackID feedbackIdReduced) { this->feedbackIdReduced = feedbackIdReduced; }
			FeedbackID GetFeedbackIdReduced() const { return feedbackIdReduced; }
			void SetFeedbackIdCreep(const FeedbackID feedbackIdCreep) { this->feedbackIdCreep = feedbackIdCreep; }
			FeedbackID GetFeedbackIdCreep() const { return feedbackIdCreep; }
			void SetFeedbackIdStop(const FeedbackID feedbackIdStop) { this->feedbackIdStop = feedbackIdStop; }
			FeedbackID GetFeedbackIdStop() const { return feedbackIdStop; }
			void SetFeedbackIdOver(const FeedbackID feedbackIdOver) { this->feedbackIdOver = feedbackIdOver; }
			FeedbackID GetFeedbackIdOver() const { return feedbackIdOver; }
			void SetWaitAfterRelease(const Pause wait) { this->waitAfterRelease = wait; }
			Pause GetWaitAfterRelease() const { return waitAfterRelease; }

			static bool CompareShortest(const Route* s1, const Route* s2) { return s1->GetMinTrainLength() < s2->GetMinTrainLength(); }
			static bool CompareLastUsed(const Route* s1, const Route* s2) { return s1->GetLastUsed() < s2->GetLastUsed(); }

		private:
			bool ReleaseInternal(Logger::Logger* logger, const LocoID locoID);
			void ReleaseInternalWithToTrack(Logger::Logger* logger, const LocoID locoID);
			static void DeleteRelations(std::vector<DataModel::Relation*>& relations);
			bool AssignRelations(std::vector<DataModel::Relation*>& relations, const std::vector<DataModel::Relation*>& newRelations);

			Manager* manager;
			std::mutex updateMutex;
			bool executeAtUnlock;

			Delay delay;
			std::vector<DataModel::Relation*> relationsAtLock;
			std::vector<DataModel::Relation*> relationsAtUnlock;
			PushpullType pushpull;
			Length minTrainLength;
			Length maxTrainLength;
			Automode automode;
			ObjectIdentifier fromTrack;
			Orientation fromOrientation;
			ObjectIdentifier toTrack;
			Orientation toOrientation;

			Speed speed;
			FeedbackID feedbackIdReduced;
			FeedbackID feedbackIdCreep;
			FeedbackID feedbackIdStop;
			FeedbackID feedbackIdOver;
			Pause waitAfterRelease;

			time_t lastUsed;
			unsigned int counter;
	};
} // namespace DataModel

