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
#include <string>

#include "DataModel/Cluster.h"
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

			TrackBase() = delete;
			TrackBase(const TrackBase&) = delete;
			TrackBase& operator=(const TrackBase&) = delete;

			inline TrackBase(Manager* manager)
			:	manager(manager),
				trackType(TrackTypeStraight),
				cluster(nullptr),
				selectRouteApproach(SelectRouteSystemDefault),
				trackState(DataModel::Feedback::FeedbackStateFree),
				trackStateDelayed(DataModel::Feedback::FeedbackStateFree),
				locoOrientation(OrientationRight),
				blocked(false),
				locoIdDelayed(LocoNone),
				allowLocoTurn(true),
				releaseWhenFree(false),
				showName(true)
			{
			}

			virtual ~TrackBase()
			{
			}

			inline TrackType GetTrackType() const
			{
				return trackType;
			}

			inline void SetTrackType(const TrackType type)
			{
				this->trackType = type;
			}

			inline std::vector<FeedbackID> GetFeedbacks() const
			{
				return feedbacks;
			}

			inline void Feedbacks(const std::vector<FeedbackID>& feedbacks)
			{
				this->feedbacks = feedbacks;
			}

			bool SetFeedbackState(const FeedbackID feedbackID, const DataModel::Feedback::FeedbackState state);

			inline DataModel::Feedback::FeedbackState GetFeedbackStateDelayed() const
			{
				return trackStateDelayed;
			}

			bool AddRoute(Route* route);
			bool RemoveRoute(Route* route);

			inline SelectRouteApproach GetSelectRouteApproach() const
			{
				return selectRouteApproach;
			}

			inline void SetSelectRouteApproach(const SelectRouteApproach selectRouteApproach)
			{
				this->selectRouteApproach = selectRouteApproach;
			}

			bool GetValidRoutes(Logger::Logger* logger,
				const DataModel::Loco* loco,
				const bool allowLocoTurn,
				std::vector<Route*>& validRoutes) const;

			inline Orientation GetLocoOrientation() const
			{
				return locoOrientation;
			}

			inline bool CanSetLocoOrientation(const Orientation orientation, const LocoID locoId)
			{
				return cluster == nullptr ? true : cluster->CanSetLocoOrientation(orientation, locoId);
			}

			bool SetLocoOrientation(const Orientation orientation);

			inline bool GetBlocked() const
			{
				return blocked;
			}

			inline void SetBlocked(const bool blocked)
			{
				this->blocked = blocked;
			}

			inline LocoID GetLocoDelayed() const
			{
				return this->locoIdDelayed;
			}

			inline bool GetReleaseWhenFree() const
			{
				return releaseWhenFree;
			}

			inline void SetReleaseWhenFree(const bool releaseWhenFree)
			{
				this->releaseWhenFree = releaseWhenFree;
			}

			inline bool GetShowName() const
			{
				return this->showName;
			}

			inline void SetShowName(const bool showName)
			{
				this->showName = showName;
			}

			inline Cluster* GetCluster() const
			{
				return cluster;
			}

			inline void SetCluster(Cluster* const cluster)
			{
				this->cluster = cluster;
			}

			inline bool GetAllowLocoTurn() const
			{
				return allowLocoTurn;
			}

			inline void SetAllowLocoTurn(bool allowLocoTurn)
			{
				this->allowLocoTurn = allowLocoTurn;
			}

			inline FeedbackID GetFirstFeedbackId()
			{
				return feedbacks.size() == 0 ? FeedbackNone : feedbacks[0];
			}

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
			virtual void StopAllSignals(__attribute__((unused)) const LocoID locoId) {}

			Manager* manager;
			TrackType trackType;

		private:
			bool FeedbackStateInternal(const FeedbackID feedbackID, const DataModel::Feedback::FeedbackState state);
			void OrderValidRoutes(std::vector<DataModel::Route*>& validRoutes) const;
			SelectRouteApproach GetSelectRouteApproachCalculated() const;
			bool BaseReleaseForceUnlocked(Logger::Logger* logger, const LocoID locoID);

			mutable std::mutex updateMutex;
			std::vector<FeedbackID> feedbacks;
			Cluster* cluster;
			SelectRouteApproach selectRouteApproach;
			DataModel::Feedback::FeedbackState trackState;
			DataModel::Feedback::FeedbackState trackStateDelayed;
			std::vector<Route*> routes;
			Orientation locoOrientation;
			bool blocked;
			LocoID locoIdDelayed;
			bool allowLocoTurn;
			bool releaseWhenFree;
			bool showName;
	};
} // namespace DataModel
