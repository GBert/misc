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
#include <vector>

#include "DataModel/LayoutItem.h"
#include "DataModel/LockableItem.h"
#include "DataModel/TrackBase.h"
#include "DataTypes.h"
#include "Logger/Logger.h"

class Manager;

namespace DataModel
{
	class Loco;
	class Route;

	class Track : public TrackBase, public LayoutItem, public LockableItem
	{
		public:
			inline Track(Manager* manager, const TrackID trackID)
			:	TrackBase(manager),
				LayoutItem(trackID),
				LockableItem(),
				trackType(TrackTypeStraight)
			{}

			inline Track(Manager* manager, const std::string& serialized)
			:	Track(manager, TrackNone)
			{
				Deserialize(serialized);
			}

			inline ObjectType GetObjectType() const override
			{
				return ObjectTypeTrack;
			}

			std::string Serialize() const override;
			bool Deserialize(const std::string& serialized) override;

			inline std::string GetLayoutType() const override
			{
				return Languages::GetText(Languages::TextTrack);
			}

			inline TrackType GetTrackType() const
			{
				return trackType;
			}

			inline void SetTrackType(const TrackType type)
			{
				this->trackType = type;
			}

			inline bool Reserve(Logger::Logger* logger, const LocoID locoID) override
			{
				return BaseReserve(logger, locoID);
			}

			inline bool ReserveForce(Logger::Logger* logger, const LocoID locoID)
			{
				return BaseReserveForce(logger, locoID);
			}

			inline bool Lock(Logger::Logger* logger, const LocoID locoID) override
			{
				return BaseLock(logger, locoID);
			}

			inline bool Release(Logger::Logger* logger, const LocoID locoID) override
			{
				return BaseRelease(logger, locoID);
			}

			inline bool ReleaseForce(Logger::Logger* logger, const LocoID locoID)
			{
				return BaseReleaseForce(logger, locoID);
			}

			inline const std::vector<DataModel::Relation*>& GetSignals() const
			{
				return signals;
			}

			void DeleteSignals();
			void DeleteSignal(DataModel::Signal* signalToDelete);
			void AssignSignals(const std::vector<DataModel::Relation*>& newSignals);

		protected:
			inline bool ReserveInternal(Logger::Logger* logger, const LocoID locoID) override
			{
				return LockableItem::Reserve(logger, locoID);
			}

			inline bool LockInternal(Logger::Logger* logger, const LocoID locoID) override
			{
				return LockableItem::Lock(logger, locoID);
			}

			inline bool ReleaseInternal(Logger::Logger* logger, const LocoID locoID) override
			{
				return LockableItem::Release(logger, locoID);
			}

			void PublishState() const override;

			inline ObjectIdentifier GetObjectIdentifier() const override
			{
				return ObjectIdentifier(ObjectTypeTrack, GetID());
			}

			inline ObjectID GetMyID() const override
			{
				return GetID();
			}

			inline const std::string& GetMyName() const override
			{
				return GetName();
			}

			inline LocoID GetMyLoco() const override
			{
				return GetLoco();
			}

			inline bool IsTrackInUse() const override
			{
				return IsInUse();
			}

			inline LocoID GetLockedLoco() const override
			{
				return GetLoco();
			}

			void StopAllSignals(const LocoID locoId) override;

		private:
			TrackType trackType;
			std::vector<DataModel::Relation*> signals;
	};
} // namespace DataModel
