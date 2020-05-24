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
#include "DataTypes.h"

class Manager;

namespace DataModel
{
	class Switch : public AccessoryBase, public LayoutItem, public LockableItem
	{
		public:
			Switch(const SwitchID switchID)
			:	AccessoryBase(),
				LayoutItem(switchID),
				LockableItem()
			{}

			Switch(__attribute__((unused)) Manager* manager, const SwitchID switchID)
			:	Switch(switchID)
			{}

			Switch(const std::string& serialized)
			{
				Deserialize(serialized);
			}

			ObjectType GetObjectType() const override { return ObjectTypeSwitch; }
			std::string GetLayoutType() const override { return Languages::GetText(Languages::TextSwitch); };

			std::string Serialize() const override;
			bool Deserialize(const std::string& serialized) override;
	};

} // namespace DataModel

