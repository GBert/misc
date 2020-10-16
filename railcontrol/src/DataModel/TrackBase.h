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
	class Route;

	enum TrackType : unsigned char
	{
		TrackTypeStraight = 0,
		TrackTypeTurn = 1,
		TrackTypeEnd = 2,
		TrackTypeBridge = 3,
		TrackTypeTunnel = 4,
		TrackTypeTunnelEnd = 5,
		TrackTypeLink = 6,
		TrackTypeCrossingLeft = 7,
		TrackTypeCrossingRight = 8,
		TrackTypeCrossingSymetric = 9
	};

	enum SelectRouteApproach : unsigned char
	{
		SelectRouteSystemDefault = 0,
		SelectRouteDoNotCare = 1,
		SelectRouteRandom = 2,
		SelectRouteMinTrackLength = 3,
		SelectRouteLongestUnused = 4
	};

	class TrackBase
	{
		public:
			static const LayoutItem::LayoutItemSize MinLength = 1;
			static const LayoutItem::LayoutItemSize MaxLength = 100;

			TrackBase(Manager* manager)
			:	manager(manager),
				selectRouteApproach(SelectRouteSystemDefault),
				trackState(DataModel::Feedback::FeedbackStateFree),
				trackStateDelayed(DataModel::Feedback::FeedbackStateFree),
				locoOrientation(OrientationRight),
				blocked(false),
				locoIdDelayed(LocoNone),
				releaseWhenFree(false),
				showName(true)
			{}

			virtual ~TrackBase() {}

			ObjectType GetObjectType() const { return ObjectTypeTrack; }

			std::vector<FeedbackID> GetFeedbacks() const { return feedbacks; }
			void Feedbacks(const std::vector<FeedbackID>& feedbacks) { this->feedbacks = feedbacks; }

			bool SetFeedbackState(const FeedbackID feedbackID, const DataModel::Feedback::FeedbackState state);
			DataModel::Feedback::FeedbackState GetFeedbackStateDelayed() const { return trackStateDelayed; };

			bool AddRoute(Route* route);
			bool RemoveRoute(Route* route);

			SelectRouteApproach GetSelectRouteApproach() const { return selectRouteApproach; }
			void SetSelectRouteApproach(const SelectRouteApproach selectRouteApproach) { this->selectRouteApproach = selectRouteApproach; }

			bool GetValidRoutes(Logger::Logger* logger, const DataModel::Loco* loco, const bool allowLocoTurn, std::vector<Route*>& validRoutes) const;
			Orientation GetLocoOrientation() const { return locoOrientation; }
			void SetLocoOrientation(const Orientation orientation) { locoOrientation = orientation; }
			bool GetBlocked() const { return blocked; }
			void SetBlocked(const bool blocked) { this->blocked = blocked; }
			LocoID GetLocoDelayed() const { return this->locoIdDelayed; }
			bool GetReleaseWhenFree() const { return releaseWhenFree; }
			void SetReleaseWhenFree(const bool releaseWhenFree) { this->releaseWhenFree = releaseWhenFree; }
			bool GetShowName() const { return this->showName; }
			void SetShowName(const bool showName) { this->showName = showName; }

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
			void OrderValidRoutes(std::vector<DataModel::Route*>& validRoutes) const;
			SelectRouteApproach GetSelectRouteApproachCalculated() const;
			bool BaseReleaseForceUnlocked(Logger::Logger* logger, const LocoID locoID);

			mutable std::mutex updateMutex;
			std::vector<FeedbackID> feedbacks;
			SelectRouteApproach selectRouteApproach;
			DataModel::Feedback::FeedbackState trackState;
			DataModel::Feedback::FeedbackState trackStateDelayed;
			std::vector<Route*> routes;
			Orientation locoOrientation;
			bool blocked;
			LocoID locoIdDelayed;
			bool releaseWhenFree;
			bool showName;
	};
} // namespace DataModel
