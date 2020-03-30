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

#include "DataModel/Feedback.h"
#include "DataModel/LayoutItem.h"
#include "DataModel/LockableItem.h"
#include "DataTypes.h"
#include "Logger/Logger.h"

class Manager;

namespace DataModel
{
	class Loco;
	class Street;

	class Track : public LayoutItem, public LockableItem
	{
		public:
			enum selectStreetApproach_t : unsigned char
			{
				SelectStreetSystemDefault = 0,
				SelectStreetDoNotCare = 1,
				SelectStreetRandom = 2,
				SelectStreetMinTrackLength = 3,
				SelectStreetLongestUnused = 4
			};

			enum type_t : unsigned char
			{
				TrackTypeStraight = 0,
				TrackTypeTurn = 1,
				TrackTypeEnd = 2,
				TrackTypeBridge = 3,
				TrackTypeTunnel = 4,
				TrackTypeTunnelEnd = 5,
				TrackTypeLink = 6
			};

			Track(Manager* manager, const trackID_t trackID)
			:	LayoutItem(trackID),
			 	LockableItem(),
			 	manager(manager),
			 	type(TrackTypeStraight),
			 	selectStreetApproach(SelectStreetSystemDefault),
				state(DataModel::Feedback::FeedbackStateFree),
				stateDelayed(DataModel::Feedback::FeedbackStateFree),
			 	locoDirection(DirectionRight),
			 	blocked(false),
			 	locoIdDelayed(LocoNone),
			 	releaseWhenFree(false)
			{
			}

			Track(Manager* manager, const std::string& serialized)
			:	manager(manager)
			{
				Deserialize(serialized);
			}

			objectType_t GetObjectType() const { return ObjectTypeTrack; }

			std::string Serialize() const override;
			bool Deserialize(const std::string& serialized) override;

			bool Reserve(Logger::Logger* logger, const locoID_t locoID) override;
			bool ReserveForce(Logger::Logger* logger, const locoID_t locoID);
			bool Lock(Logger::Logger* logger, const locoID_t locoID) override;
			bool Release(Logger::Logger* logger, const locoID_t locoID) override;
			bool ReleaseForce(Logger::Logger* logger, const locoID_t locoID);

			std::string LayoutType() const override { return Languages::GetText(Languages::TextTrack); };
			type_t GetType() const { return type; }
			void SetType(const type_t type) { this->type = type; }
			std::vector<feedbackID_t> GetFeedbacks() const { return feedbacks; }
			void Feedbacks(const std::vector<feedbackID_t>& feedbacks) { this->feedbacks = feedbacks; }

			bool SetFeedbackState(const feedbackID_t feedbackID, const DataModel::Feedback::feedbackState_t state);
			DataModel::Feedback::feedbackState_t GetFeedbackStateDelayed() const { return stateDelayed; };

			bool AddStreet(Street* street);
			bool RemoveStreet(Street* street);

			selectStreetApproach_t GetSelectStreetApproach() const { return selectStreetApproach; }
			void SetSelectStreetApproach(const selectStreetApproach_t selectStreetApproach) { this->selectStreetApproach = selectStreetApproach; }

			bool GetValidStreets(Logger::Logger* logger, const DataModel::Loco* loco, const bool allowLocoTurn, std::vector<Street*>& validStreets) const;
			direction_t GetLocoDirection() const { return locoDirection; }
			void SetLocoDirection(const direction_t direction) { locoDirection = direction; }
			bool GetBlocked() const { return blocked; }
			void SetBlocked(const bool blocked) { this->blocked = blocked; }
			locoID_t GetLocoDelayed() const { return this->locoIdDelayed; }
			bool GetReleaseWhenFree() const { return releaseWhenFree; }
			void SetReleaseWhenFree(const bool releaseWhenFree) { this->releaseWhenFree = releaseWhenFree; }

		private:
			bool FeedbackStateInternal(const feedbackID_t feedbackID, const DataModel::Feedback::feedbackState_t state);
			void OrderValidStreets(std::vector<DataModel::Street*>& validStreets) const;
			selectStreetApproach_t GetSelectStreetApproachCalculated() const;
			bool ReleaseForceUnlocked(Logger::Logger* logger, const locoID_t locoID);

			Manager* manager;
			mutable std::mutex updateMutex;
			type_t type;
			std::vector<feedbackID_t> feedbacks;
			selectStreetApproach_t selectStreetApproach;
			DataModel::Feedback::feedbackState_t state;
			DataModel::Feedback::feedbackState_t stateDelayed;
			std::vector<Street*> streets;
			direction_t locoDirection;
			bool blocked;
			locoID_t locoIdDelayed;
			bool releaseWhenFree;
	};
} // namespace DataModel
