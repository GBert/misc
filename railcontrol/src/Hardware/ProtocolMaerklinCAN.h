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
#include "Utils/Utils.h"

// CAN protocol specification at http://streaming.maerklin.de/public-media/cs2/cs2CAN-Protokoll-2_0.pdf

namespace Hardware
{
	class ProtocolMaerklinCAN : protected HardwareInterface
	{
		public:
			ProtocolMaerklinCAN() = delete;

			bool CanHandleLocos() const override { return true; }
			bool CanHandleAccessories() const override { return true; }
			bool CanHandleFeedback() const override { return true; }
			bool CanHandleProgram() const override { return false; }
			bool CanHandleProgramMm() const override { return true; }
			bool CanHandleProgramMfx() const override { return true; }
			bool CanHandleProgramDccDirect() const override { return true; }

			void GetLocoProtocols(std::vector<protocol_t> &protocols) const override
			{
				protocols.push_back(ProtocolMM);
				protocols.push_back(ProtocolMFX);
				protocols.push_back(ProtocolDCC);
			}

			bool LocoProtocolSupported(protocol_t protocol) const override
			{
				return (protocol == ProtocolMM || protocol == ProtocolMFX || protocol == ProtocolDCC);
			}

			void GetAccessoryProtocols(std::vector<protocol_t> &protocols) const override
			{
				protocols.push_back(ProtocolMM);
				protocols.push_back(ProtocolDCC);
			}

			bool AccessoryProtocolSupported(protocol_t protocol) const override
			{
				return (protocol == ProtocolMM || protocol == ProtocolDCC);
			}

			void Booster(const boosterState_t status) override;
			void LocoSpeed(const protocol_t protocol, const address_t address, const locoSpeed_t speed) override;
			void LocoDirection(const protocol_t protocol, const address_t address, const direction_t direction) override;
			void LocoFunction(const protocol_t protocol, const address_t address, const function_t function, const bool on) override;
			void AccessoryOnOrOff(const protocol_t protocol, const address_t address, const accessoryState_t state, const bool on) override;
			void ProgramRead(const ProgramMode mode, const address_t address, const CvNumber cv) override;
			void ProgramWrite(const ProgramMode mode, const address_t address, const CvNumber cv, const CvValue value) override;

		protected:

			enum CanCommand : unsigned char
			{
				CanCommandSystem = 0x00,
				CanCommandLocoSpeed = 0x04,
				CanCommandLocoDirection = 0x05,
				CanCommandLocoFunction = 0x06,
				CanCommandReadConfig = 0x07,
				CanCommandWriteConfig = 0x08,
				CanCommandAccessory = 0x0B,
				CanCommandS88Event = 0x11
			};
			enum CanSubCommand : unsigned char
			{
				CanSubCommandStop = 0x00,
				CanSubCommandGo = 0x01
			};
			enum CanResponse : unsigned char
			{
				CanResponseCommand = 0x00,
				CanResponseResponse = 0x01
			};
			typedef unsigned char CanPrio;
			typedef unsigned char CanLength;
			typedef uint32_t CanAddress;

			ProtocolMaerklinCAN(Manager* manager, controlID_t controlID, Logger::Logger* logger, std::string name)
			:	HardwareInterface(manager, controlID, name),
			 	logger(logger)
			{}

			virtual ~ProtocolMaerklinCAN() {}

			void Parse(const unsigned char* buffer);

			static const unsigned char CANCommandBufferLength = 13;

			Logger::Logger* logger;

		private:
			static const unsigned short hash = 0x7337;

			void CreateCommandHeader(unsigned char* buffer, const CanCommand command, const CanResponse response, const CanLength length);
			void ParseAddressProtocol(const unsigned char* buffer, CanAddress& address, protocol_t& protocol);

			CanPrio ParsePrio(const unsigned char* buffer)
			{
				return buffer[0] >> 1;
			}

			CanCommand ParseCommand(const unsigned char* buffer)
			{
				return static_cast<CanCommand>((CanCommand)(buffer[0]) << 7 | (CanCommand)(buffer[1]) >> 1);
			}

			CanSubCommand ParseSubCommand(const unsigned char* buffer)
			{
				return static_cast<CanSubCommand>(buffer[9]);
			}

			CanResponse ParseResponse(const unsigned char* buffer)
			{
				return static_cast<CanResponse>(buffer[1] & 0x01);
			}

			CanLength ParseLength(const unsigned char* buffer)
			{
				return buffer[4];
			}

			CanAddress ParseAddress(const unsigned char* buffer)
			{
				return Utils::Utils::DataBigEndianToInt(buffer + 5);
			}

			void CreateLocalIDLoco(unsigned char* buffer, const protocol_t& protocol, const address_t& address);
			void CreateLocalIDAccessory(unsigned char* buffer, const protocol_t& protocol, const address_t& address);

			virtual void Send(const unsigned char* buffer) = 0;
	};
} // namespace

