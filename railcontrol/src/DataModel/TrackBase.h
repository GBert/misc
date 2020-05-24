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
#include <string>

#include "DataModel/Feedback.h"
#include "Logger/Logger.h"

namespace DataModel
{
	class Loco;
	class Street;

	enum TrackType : unsigned char
	{
		TrackTypeStraight = 0,
		TrackTypeTurn = 1,
		TrackTypeEnd = 2,
		TrackTypeBridge = 3,
		TrackTypeTunnel = 4,
		TrackTypeTunnelEnd = 5,
		TrackTypeLink = 6
	};

	enum SelectStreetApproach : unsigned char
	{
		SelectStreetSystemDefault = 0,
		SelectStreetDoNotCare = 1,
		SelectStreetRandom = 2,
		SelectStreetMinTrackLength = 3,
		SelectStreetLongestUnused = 4
	};

	class TrackBase
	{
		public:
			static const LayoutItem::LayoutItemSize MinLength = 1;
			static const LayoutItem::LayoutItemSize MaxLength = 100;

			TrackBase(Manager* manager)
			:	manager(manager),
				selectStreetApproach(SelectStreetSystemDefault),
				trackState(DataModel::Feedback::FeedbackStateFree),
				trackStateDelayed(DataModel::Feedback::FeedbackStateFree),
				locoDirection(DirectionRight),
				blocked(false),
				locoIdDelayed(LocoNone),
				releaseWhenFree(false)
			{}

			virtual ~TrackBase() {}

			ObjectType GetObjectType() const { return ObjectTypeTrack; }

			std::vector<FeedbackID> GetFeedbacks() const { return feedbacks; }
			void Feedbacks(const std::vector<FeedbackID>& feedbacks) { this->feedbacks = feedbacks; }

			bool SetFeedbackState(const FeedbackID feedbackID, const DataModel::Feedback::FeedbackState state);
			DataModel::Feedback::FeedbackState GetFeedbackStateDelayed() const { return trackStateDelayed; };

			bool AddStreet(Street* street);
			bool RemoveStreet(Street* street);

			SelectStreetApproach GetSelectStreetApproach() const { return selectStreetApproach; }
			void SetSelectStreetApproach(const SelectStreetApproach selectStreetApproach) { this->selectStreetApproach = selectStreetApproach; }

			bool GetValidStreets(Logger::Logger* logger, const DataModel::Loco* loco, const bool allowLocoTurn, std::vector<Street*>& validStreets) const;
			Direction GetLocoDirection() const { return locoDirection; }
			void SetLocoDirection(const Direction direction) { locoDirection = direction; }
			bool GetBlocked() const { return blocked; }
			void SetBlocked(const bool blocked) { this->blocked = blocked; }
			LocoID GetLocoDelayed() const { return this->locoIdDelayed; }
			bool GetReleaseWhenFree() const { return releaseWhenFree; }
			void SetReleaseWhenFree(const bool releaseWhenFree) { this->releaseWhenFree = releaseWhenFree; }

			virtual ObjectIdentifier GetObjectIdentifier() const = 0;
			virtual ObjectID GetMyID() const = 0;
			virtual const std::string& GetMyName() const = 0;
			virtual LocoID GetMyLoco() const = 0;
			virtual bool IsTrackInUse() const = 0;

			bool BaseReserve(Logger::Logger* logger, const LocoID locoID);
			bool BaseReserveForce(Logger::Logger* logger, const LocoID locoID);
			bool BaseLock(Logger::Logger* logger, const LocoID locoID);
			bool BaseRelease(Logger::Logger* logger, const LocoID locoID);
			bool BaseReleaseForce(Logger::Logger* logger, const LocoID locoID);

		protected:
			std::string Serialize() const;
			bool Deserialize(const std::map<std::string, std::string> arguments);

			virtual bool ReserveInternal(Logger::Logger* logger, const LocoID locoID) = 0;
			virtual bool LockInternal(Logger::Logger* logger, const LocoID locoID) = 0;
			virtual bool ReleaseInternal(Logger::Logger* logger, const LocoID locoID) = 0;
			virtual void PublishState() const = 0;
			virtual LocoID GetLockedLoco() const = 0;

			Manager* manager;

		private:
			bool FeedbackStateInternal(const FeedbackID feedbackID, const DataModel::Feedback::FeedbackState state);
			void OrderValidStreets(std::vector<DataModel::Street*>& validStreets) const;
			SelectStreetApproach GetSelectStreetApproachCalculated() const;
			bool BaseReleaseForceUnlocked(Logger::Logger* logger, const LocoID locoID);

			mutable std::mutex updateMutex;
			std::vector<FeedbackID> feedbacks;
			SelectStreetApproach selectStreetApproach;
			DataModel::Feedback::FeedbackState trackState;
			DataModel::Feedback::FeedbackState trackStateDelayed;
			std::vector<Street*> streets;
			Direction locoDirection;
			bool blocked;
			LocoID locoIdDelayed;
			bool releaseWhenFree;
	};
} // namespace DataModel
