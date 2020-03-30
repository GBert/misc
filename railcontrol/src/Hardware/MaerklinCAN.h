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

#include "HardwareInterface.h"
#include "HardwareParams.h"
#include "Logger/Logger.h"

// CAN protocol specification at http://streaming.maerklin.de/public-media/cs2/cs2CAN-Protokoll-2_0.pdf

namespace Hardware
{
	class MaerklinCAN : protected HardwareInterface
	{
		public:
			MaerklinCAN() = delete;

		protected:

			typedef unsigned char canPrio_t;
			typedef unsigned char canCommand_t;
			typedef unsigned char canResponse_t;
			typedef unsigned char canLength_t;
			typedef uint32_t canAddress_t;

			MaerklinCAN(Manager* manager, controlID_t controlID, Logger::Logger* logger, std::string name)
			:	HardwareInterface(manager, controlID, name),
			 	logger(logger)
			{}

			virtual ~MaerklinCAN() {}

			void CreateBoosterCommand(unsigned char* buffer, const boosterState_t status);
			void CreateLocoSpeedCommand(unsigned char* buffer, const protocol_t protocol, const address_t address, const locoSpeed_t speed);
			void CreateLocoDirectionCommand(unsigned char* buffer, const protocol_t protocol, const address_t address, const direction_t direction);
			void CreateLocoFunctionCommand(unsigned char* buffer, const protocol_t protocol, const address_t address, const function_t function, const bool on);
			void CreateAccessoryCommand(unsigned char* buffer, const protocol_t protocol, const address_t address, const accessoryState_t state, const bool on);

			void CreateCommandHeader(unsigned char* buffer, const canPrio_t prio, const canCommand_t command, const canResponse_t response, const canLength_t length);
			void ReadCommandHeader(const unsigned char* buffer, canPrio_t& prio, canCommand_t& command, canResponse_t& response, canLength_t& length, canAddress_t& address, protocol_t& protocol);
			void CreateLocID(unsigned char* buffer, const protocol_t& protocol, const address_t& address);
			void CreateAccessoryID(unsigned char* buffer, const protocol_t& protocol, const address_t& address);
			void Parse(const unsigned char* buffer);

			static const unsigned char CANCommandBufferLength = 13;

			Logger::Logger* logger;

		private:
			static const unsigned short hash = 0x7337;
	};
} // namespace

