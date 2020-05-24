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

#include "DataModel/HardwareHandle.h"

namespace DataModel
{
	enum AccessoryType : unsigned char
	{
		AccessoryTypeDefault = 0,

		SignalTypeSimpleLeft = 0,
		SignalTypeSimpleRight = 1,

		SwitchTypeLeft = 0,
		SwitchTypeRight
	};

	enum AccessoryState : bool
	{
		DefaultState = false,

		AccessoryStateOff = false,
		AccessoryStateOn = true,

		SignalStateRed = false,
		SignalStateGreen = true,

		SwitchStateTurnout = false,
		SwitchStateStraight = true
	};

	typedef unsigned short AccessoryPulseDuration;
	static const AccessoryPulseDuration DefaultAccessoryPulseDuration = 100;

	class AccessoryBase : public HardwareHandle
	{
		public:
			AccessoryBase()
			:	HardwareHandle(),
				accessoryType(AccessoryTypeDefault),
				accessoryState(AccessoryStateOff),
				duration(0),
				inverted(false),
				lastUsed(0),
				counter(0)
			{
			}

			virtual ~AccessoryBase() {}

			AccessoryType GetType() const { return accessoryType; }
			void SetType(AccessoryType type) { this->accessoryType = type; }
			AccessoryState GetAccessoryState() const { return accessoryState; }
			void SetAccessoryState(AccessoryState state) { this->accessoryState = state; lastUsed = time(nullptr); ++counter; }
			AccessoryPulseDuration GetAccessoryPulseDuration() const { return duration; }
			void SetAccessoryPulseDuration(AccessoryPulseDuration duration) { this->duration = duration; }

			bool GetInverted() const { return inverted; }
			void SetInverted(const bool inverted) { this->inverted = inverted; }

			time_t GetLastUsed() const { return lastUsed; }

		protected:
			virtual std::string Serialize() const;
			virtual bool Deserialize(const std::map<std::string,std::string>& arguments);

			AccessoryType accessoryType;
			AccessoryState accessoryState;
			AccessoryPulseDuration duration; // duration in ms after which the accessory command will be turned off on rails. 0 = no turn off / turn off must be made manually
			bool inverted;

			time_t lastUsed;
			unsigned int counter;
	};
} // namespace DataModel

