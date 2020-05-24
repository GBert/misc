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

#include <string>

#include "DataModel/AccessoryBase.h"
#include "DataModel/LayoutItem.h"
#include "DataModel/LockableItem.h"
#include "DataModel/TrackBase.h"
#include "DataTypes.h"

class Manager;

namespace DataModel
{
	class Signal : public AccessoryBase, public TrackBase, public LayoutItem, public LockableItem
	{
		public:
			Signal(Manager* manager, const SignalID signalID)
			:	AccessoryBase(),
				TrackBase(manager),
				LayoutItem(signalID),
				LockableItem()
			{
			}

			Signal(Manager* manager, const std::string& serialized)
			:	Signal(manager, SignalNone)
			{
				Deserialize(serialized);
			}

			ObjectType GetObjectType() const override { return ObjectTypeSignal; }
			std::string GetLayoutType() const override { return Languages::GetText(Languages::TextSignal); };

			std::string Serialize() const override;
			bool Deserialize(const std::string& serialized) override;

			bool Reserve(Logger::Logger* logger, const LocoID locoID) override
			{
				return BaseReserve(logger, locoID);
			}

			bool ReserveForce(Logger::Logger* logger, const LocoID locoID)
			{
				return BaseReserveForce(logger, locoID);
			}

			bool Lock(Logger::Logger* logger, const LocoID locoID) override
			{
				return BaseLock(logger, locoID);
			}

			bool Release(Logger::Logger* logger, const LocoID locoID) override
			{
				return BaseRelease(logger, locoID);
			}

			bool ReleaseForce(Logger::Logger* logger, const LocoID locoID)
			{
				return BaseReleaseForce(logger, locoID);
			}

		protected:
			bool ReserveInternal(Logger::Logger* logger, const LocoID locoID) override
			{
				return LockableItem::Reserve(logger, locoID);
			}

			bool LockInternal(Logger::Logger* logger, const LocoID locoID) override
			{
				return LockableItem::Lock(logger, locoID);
			}

			bool ReleaseInternal(Logger::Logger* logger, const LocoID locoID) override;

			void PublishState() const override;

			ObjectIdentifier GetObjectIdentifier() const override
			{
				return ObjectIdentifier(ObjectTypeSignal, GetID());
			}

			ObjectID GetMyID() const override
			{
				return GetID();
			}

			const std::string& GetMyName() const override
			{
				return GetName();
			}

			LocoID GetMyLoco() const override
			{
				return GetLoco();
			}

			bool IsTrackInUse() const override
			{
				return IsInUse();
			}

			LocoID GetLockedLoco() const override
			{
				return GetLoco();
			}
	};
} // namespace DataModel

