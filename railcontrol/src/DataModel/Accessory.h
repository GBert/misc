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

#include "DataTypes.h"
#include "DataModel/HardwareHandle.h"
#include "DataModel/LayoutItem.h"
#include "DataModel/LockableItem.h"
#include "Languages.h"

class Manager;

namespace DataModel
{
	class Accessory : public LayoutItem, public LockableItem, public HardwareHandle
	{
		public:
			enum accessoryType : accessoryType_t
			{
				AccessoryTypeDefault = 0
			};

			enum accessoryState : accessoryState_t
			{
				AccessoryStateOff = false,
				AccessoryStateOn = true
			};

			Accessory(const accessoryID_t accessoryID)
			:	LayoutItem(accessoryID),
			 	type(AccessoryTypeDefault),
			 	state(AccessoryStateOff),
			 	duration(0),
			 	inverted(false),
			 	lastUsed(0),
				counter(0)
			{
			}

			Accessory(__attribute__((unused)) Manager* manager, const accessoryID_t accessoryID)
			:	Accessory(accessoryID)
			{
			}

			Accessory(const std::string& serialized)
			{
				Deserialize(serialized);
			}

			Accessory() : Accessory(AccessoryNone) {}

			virtual ~Accessory() {}

			virtual objectType_t GetObjectType() const { return ObjectTypeAccessory; }

			virtual std::string Serialize() const override;
			virtual bool Deserialize(const std::string& serialized) override;
			virtual std::string LayoutType() const override { return Languages::GetText(Languages::TextAccessory); }

			void SetType(accessoryType_t type) { this->type = type; }
			accessoryType_t GetType() const { return type; }
			void SetState(accessoryState_t state) { this->state = state; lastUsed = time(nullptr); ++counter; }
			accessoryState_t GetState() const { return state; }
			void SetDuration(accessoryDuration_t duration) { this->duration = duration; }
			accessoryDuration_t GetDuration() const { return duration; }

			void SetInverted(const bool inverted) { this->inverted = inverted; }
			bool GetInverted() const { return inverted; }

			time_t GetLastUsed() const { return lastUsed; }

		protected:
			std::string SerializeWithoutType() const;
			virtual bool Deserialize(const std::map<std::string,std::string>& arguments);

			accessoryType_t type;
			accessoryState_t state;
			accessoryDuration_t duration; // duration in ms after which the accessory command will be turned off on rails. 0 = no turn off / turn off must be made manually
			bool inverted;

			time_t lastUsed;
			unsigned int counter;
	};
} // namespace DataModel

