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

#include "DataModel/Accessory.h"
#include "DataTypes.h"

namespace DataModel
{
	class Switch : public Accessory
	{
		public:
			enum switchType : switchType_t
			{
				SwitchTypeLeft = 0,
				SwitchTypeRight
			};

			enum switchState : switchState_t
			{
				SwitchStateTurnout = false,
				SwitchStateStraight = true
			};

			Switch(Manager* manager, const switchID_t switchID)
			:	Accessory(manager, switchID)
			{
			}

			Switch(const std::string& serialized)
			{
				Deserialize(serialized);
			}

			objectType_t GetObjectType() const { return ObjectTypeSwitch; }

			std::string Serialize() const override;
			bool Deserialize(const std::string& serialized) override;
			std::string LayoutType() const override { return Languages::GetText(Languages::TextSwitch); };

			switchState_t GetState() const { return static_cast<switchState_t>(state); }
			switchType_t GetType() const { return static_cast<switchType_t>(type); }
	};

} // namespace DataModel

