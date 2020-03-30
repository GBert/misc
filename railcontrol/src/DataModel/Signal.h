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

class Manager;

namespace DataModel
{
	class Signal : public Accessory
	{
		public:
			enum signalType : signalType_t
			{
				SignalTypeSimpleLeft = 0,
				SignalTypeSimpleRight = 1
			};

			enum signalState : signalState_t
			{
				SignalStateRed = false,
				SignalStateGreen = true
			};

			Signal(Manager* manager, const signalID_t signalID)
			:	Accessory(manager, signalID),
			 	manager(manager)
			{
			}

			Signal(Manager* manager, const std::string& serialized)
			:	manager(manager)
			{
				Deserialize(serialized);
			}

			objectType_t GetObjectType() const { return ObjectTypeSignal; }

			std::string Serialize() const override;
			bool Deserialize(const std::string& serialized) override;
			std::string LayoutType() const override { return Languages::GetText(Languages::TextSignal); };

			bool Release(Logger::Logger* logger, const locoID_t locoID) override;

			signalState_t GetState() const { return static_cast<signalState_t>(state); }
			signalType_t GetType() const { return static_cast<signalType_t>(type); }

		private:
			Manager* manager;
	};
} // namespace DataModel

