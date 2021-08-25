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

#include <string>

#include "DataModel/AccessoryBase.h"
#include "DataModel/LayoutItem.h"
#include "DataModel/LockableItem.h"
#include "DataTypes.h"

class Manager;

namespace DataModel
{
	class Switch : public AccessoryBase, public LayoutItem, public LockableItem
	{
		public:
			Switch() = delete;
			Switch(const Switch&) = delete;
			Switch& operator=(const Switch&) = delete;

			Switch(const SwitchID switchID)
			:	AccessoryBase(),
				LayoutItem(switchID),
				LockableItem()
			{
			}

			Switch(__attribute__((unused)) Manager* manager, const SwitchID switchID)
			:	Switch(switchID)
			{
			}

			Switch(const std::string& serialized)
			{
				Deserialize(serialized);
			}

			ObjectType GetObjectType() const override { return ObjectTypeSwitch; }
			std::string GetLayoutType() const override { return Languages::GetText(Languages::TextSwitch); };

			std::string Serialize() const override;

			using HardwareHandle::Deserialize;
			bool Deserialize(const std::string& serialized) override;

			void SetAccessoryState(const AccessoryState state);

			virtual inline void SetType(AccessoryType type) override
			{
				AccessoryBase::SetType(type);
				SetSizeFromType();
			}

			std::map<DataModel::AccessoryState,Languages::TextSelector> GetStateOptions() const;

			static DataModel::LayoutItem::LayoutItemSize CalculateHeightFromType(AccessoryType type);

			Switch& operator=(const Hardware::AccessoryCacheEntry& accessory);

		private:
			inline void SetSizeFromType()
			{
				SetWidth(Width1);
				SetHeight(CalculateHeightFromType(GetType()));
			}
	};
} // namespace DataModel
