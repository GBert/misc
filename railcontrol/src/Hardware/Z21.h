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

#include <arpa/inet.h>
#include <cstring>
#include <string>
#include <thread>

#include "HardwareInterface.h"
#include "HardwareParams.h"
#include "Hardware/Z21LocoCache.h"
#include "Hardware/Z21TurnoutCache.h"
#include "Logger/Logger.h"
#include "Network/UdpConnection.h"

// protocol specification at https://www.z21.eu/media/Kwc_Basic_DownloadTag_Component/47-1652-959-downloadTag/default/69bad87e/1558674980/z21-lan-protokoll.pdf

namespace Hardware
{
	class AccessoryQueueEntry
	{
		public:
			AccessoryQueueEntry()
			:	protocol(ProtocolNone),
			 	address(AddressNone),
			 	state(false),
			 	waitTime(0)
			{}

			AccessoryQueueEntry(const protocol_t protocol, const address_t address, const accessoryState_t state, const waitTime_t waitTime)
			:	protocol(protocol),
			 	address(address),
			 	state(state),
			 	waitTime(waitTime)
			{}

			protocol_t protocol;
			address_t address;
			accessoryState_t state;
			waitTime_t waitTime;
	};

	class Z21 : HardwareInterface
	{
		public:
			Z21(const HardwareParams* params);
			~Z21();

			bool CanHandleLocos() const override { return true; }
			bool CanHandleAccessories() const override { return true; }
			bool CanHandleFeedback() const override { return true; }

			void GetLocoProtocols(std::vector<protocol_t>& protocols) const override
			{
				protocols.push_back(ProtocolMM1);
				protocols.push_back(ProtocolMM15);
				protocols.push_back(ProtocolMM2);
				protocols.push_back(ProtocolDCC14);
				protocols.push_back(ProtocolDCC28);
				protocols.push_back(ProtocolDCC128);
			}

			bool LocoProtocolSupported(protocol_t protocol) const override
			{
				return (protocol == ProtocolMM1
					|| protocol == ProtocolMM15
					|| protocol == ProtocolMM2
					|| protocol == ProtocolDCC14
					|| protocol == ProtocolDCC28
					|| protocol == ProtocolDCC128);
			}

			void GetAccessoryProtocols(std::vector<protocol_t>& protocols) const override
			{
				protocols.push_back(ProtocolMM);
				protocols.push_back(ProtocolDCC);
			}

			bool AccessoryProtocolSupported(protocol_t protocol) const override
			{
				return (protocol == ProtocolMM || protocol == ProtocolDCC);
			}

			static void GetArgumentTypes(std::map<unsigned char,argumentType_t>& argumentTypes)
			{
				argumentTypes[1] = IpAddress;
			}

			void Booster(const boosterState_t status) override;
			void LocoSpeed(const protocol_t protocol, const address_t address, const locoSpeed_t speed) override;
			void LocoDirection(const protocol_t protocol, const address_t address, const direction_t direction) override;
			void LocoFunction(const protocol_t protocol, const address_t address, const function_t function, const bool on) override;
			void LocoSpeedDirectionFunctions(const protocol_t protocol, const address_t address, const locoSpeed_t speed, const direction_t direction, std::vector<bool>& functions) override;
			void Accessory(const protocol_t protocol, const address_t address, const accessoryState_t state, const waitTime_t waitTime) override;
			void AccessoryOnOrOff(const protocol_t protocol, const address_t address, const accessoryState_t state, const bool on) override;

		private:
			Logger::Logger* logger;
			volatile bool run;
			Network::UdpConnection connection;
			std::thread receiverThread;
			std::thread heartBeatThread;
			std::thread accessorySenderThread;
			Z21LocoCache locoCache;
			Z21TurnoutCache turnoutCache;

			Utils::ThreadSafeQueue<AccessoryQueueEntry> accessoryQueue;

			static const unsigned short Z21Port = 21105;
			static const unsigned int Z21CommandBufferLength = 1472; // = Max Ethernet MTU
			static const address_t MaxMMAddress = 255;

			enum broadCastFlags_t : uint32_t
			{
				BroadCastFlagBasic           = 0x00000001,
				BroadCastFlagRBus            = 0x00000002,
				BroadCastFlagRailCom         = 0x00000004,
				BroadCastFlagSystemState     = 0x00000100,
				BroadCastFlagAllLoco         = 0x00010000,
				BroadCastFlagRailComData     = 0x00040000,
				BroadCastFlagCanDetector     = 0x00080000,
				BroadCastFlagLocoNetBasic    = 0x01000000,
				BroadCastFlagLocoNetLoco     = 0x02000000,
				BroadCastFlagLocoNetSwitch   = 0x04000000,
				BroadCastFlagLocoNetDetector = 0x08000000
			};

			enum commands_t : uint8_t
			{
				CommandSetLocoMode = 0x61,
				CommandSetTurnoutMode = 0x70
			};

			enum protocolMode_t : uint8_t
			{
				ProtocolModeDCC = 0,
				ProtocolModeMM = 1
			};

			enum featureSet_t : uint8_t
			{
				FeaturesNotRestricted = 0x00,
				FeaturesStartLocked = 0x01,
				FeaturesStartUnlocked = 0x02
			};

			void LocoSpeedDirection(const protocol_t protocol, const address_t address, const locoSpeed_t speed, const direction_t direction);
			void AccessorySender();
			void HeartBeatSender();
			void Receiver();
			ssize_t InterpretData(unsigned char* buffer, size_t bufferLength);

			void SendGetSerialNumber();
			void SendGetHardwareInfo();
			void SendGetStatus();
			void SendGetCode();
			void SendLogOff();
			void SendBroadcastFlags(const broadCastFlags_t flags);
			void SendSetMode(const address_t address, const commands_t command, const protocolMode_t mode);
			void SendSetLocoModeMM(const address_t address);
			void SendSetLocoModeDCC(const address_t address);
			void SendSetTurnoutModeMM(const address_t address);
			void SendSetTurnoutModeDCC(const address_t address);
			void AccessoryOn(const protocol_t protocol, const address_t address, const accessoryState_t state);
			void AccessoryOff(const protocol_t protocol, const address_t address, const accessoryState_t state);
			int Send(const unsigned char* buffer, const size_t bufferLength);
			int Send(const char* buffer, const size_t bufferLength) { return Send(reinterpret_cast<const unsigned char*>(buffer), bufferLength); }

			unsigned char EncodeSpeed14(const locoSpeed_t speed);
			unsigned char EncodeSpeed28(const locoSpeed_t speed);
			unsigned char EncodeSpeed128(const locoSpeed_t speed);
			locoSpeed_t DecodeSpeed14(unsigned char data);
			locoSpeed_t DecodeSpeed28(unsigned char data);
			locoSpeed_t DecodeSpeed128(unsigned char data);
	};

	extern "C" Z21* create_Z21(const HardwareParams* params);
	extern "C" void destroy_Z21(Z21* z21);

} // namespace

