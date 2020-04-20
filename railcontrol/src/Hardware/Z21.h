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
			bool CanHandleProgram() const override { return true; }
			bool CanHandleProgramMm() const override { return true; }
			bool CanHandleProgramDccDirect() const override { return true; }
			bool CanHandleProgramDccPom() const override { return true; }

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

			static void GetArgumentTypesAndHint(std::map<unsigned char,argumentType_t>& argumentTypes, std::string& hint)
			{
				argumentTypes[1] = IpAddress;
				hint = Languages::GetText(Languages::TextHintZ21);
			}

			void Booster(const boosterState_t status) override;
			void LocoSpeed(const protocol_t protocol, const address_t address, const locoSpeed_t speed) override;
			void LocoDirection(const protocol_t protocol, const address_t address, const direction_t direction) override;
			void LocoFunction(const protocol_t protocol, const address_t address, const function_t function, const bool on) override;
			void LocoSpeedDirectionFunctions(const protocol_t protocol, const address_t address, const locoSpeed_t speed, const direction_t direction, std::vector<bool>& functions) override;
			void Accessory(const protocol_t protocol, const address_t address, const accessoryState_t state, const waitTime_t waitTime) override;
			void AccessoryOnOrOff(const protocol_t protocol, const address_t address, const accessoryState_t state, const bool on) override;
			void ProgramRead(const ProgramMode mode, const address_t address, const CvNumber cv) override;
			void ProgramWrite(const ProgramMode mode, const address_t address, const CvNumber cv, const CvValue value) override;

		private:
			static const unsigned short Z21Port = 21105;
			static const unsigned int Z21CommandBufferLength = 1472; // = Max Ethernet MTU
			static const address_t MaxMMAddress = 255;

			enum BroadCastFlag : uint32_t
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

			enum Command : uint8_t
			{
				CommandSetLocoMode = 0x61,
				CommandSetTurnoutMode = 0x70
			};

			enum ProtocolMode : uint8_t
			{
				ProtocolModeDCC = 0,
				ProtocolModeMM = 1
			};

			enum FeatureSet : uint8_t
			{
				FeaturesNotRestricted = 0x00,
				FeaturesStartLocked = 0x01,
				FeaturesStartUnlocked = 0x02
			};

			enum Header : uint16_t
			{
				HeaderSerialNumber = 0x10,
				HeaderCode = 0x18,
				HeaderHardwareInfo = 0x1A,
				HeaderSeeXHeader = 0x40,
				HeaderBroadcastFlags = 0x51,
				HeaderLocoMode = 0x60,
				HeaderTurnoutMode = 0x70,
				HeaderRmBusData = 0x80,
				HeaderSystemData = 0x84,
				HeaderRailComtData = 0x88,
				HeaderLocoNetRx = 0xA0,
				HeaderLocoNetTx = 0xA1,
				HeaderLocoNetLan = 0xA2,
				HeaderLocoNetDispatch = 0xA3,
				HeaderLocoNetDetector = 0xA4,
				HeaderDetector = 0xC4
			};

			enum XHeader : uint8_t
			{
				XHeaderTurnoutInfo = 0x43,
				XHeaderSeeDB0 = 0x61,
				XHeaderStatusChanged = 0x62,
				XHeaderVersion = 0x63,
				XHeaderCvResult = 0x64,
				XHeaderBcStopped = 0x81,
				XHeaderLocoInfo = 0xEF,
				XHeaderFirmwareVersion = 0xF3
			};

			enum DB0 : uint8_t
			{
				DB0PowerOff = 0x00,
				DB0PowerOn = 0x01,
				DB0ProgrammingMode = 0x02,
				DB0ShortCircuit = 0x08,
				DB0CvShortCircuit = 0x12,
				DB0CvNack = 0x13,
				DB0UnknownCommand = 0x82,
				DB0StatusChanged = 0x22,
				DB0Version = 0x21,
				DB0FirmwareVersion = 0x0A
			};

			enum PomDB0 : uint8_t
			{
				PomLoco = 0x30,
				PomAccessory = 0x31
			};

			enum PomOption : uint16_t
			{
				PomWriteByte = 0xEC00,
				PomWriteBit = 0xE800,
				PomReadByte = 0xE400
			};

			Logger::Logger* logger;
			volatile bool run;
			Network::UdpConnection connection;
			std::thread receiverThread;
			std::thread heartBeatThread;
			std::thread accessorySenderThread;
			Z21LocoCache locoCache;
			Z21TurnoutCache turnoutCache;
			ProgramMode lastProgramMode;
			volatile bool connected;

			Utils::ThreadSafeQueue<AccessoryQueueEntry> accessoryQueue;

			void ProgramMm(const CvNumber cv, const CvValue value);
			void ProgramDccRead(const CvNumber cv);
			void ProgramDccWrite(const CvNumber cv, const CvValue value);
			void ProgramDccPom(const PomDB0 db0, const PomOption option, const address_t address, const CvNumber cv, const CvValue value = 0);

			void LocoSpeedDirection(const protocol_t protocol, const address_t address, const locoSpeed_t speed, const direction_t direction);
			void AccessorySender();
			void HeartBeatSender();
			void Receiver();
			ssize_t ParseData(const unsigned char* buffer, size_t bufferLength);
			void ParseXHeader(const unsigned char* buffer);
			void ParseDB0(const unsigned char* buffer);
			void ParseTurnoutData(const unsigned char *buffer);
			void ParseLocoData(const unsigned char* buffer);
			void ParseCvData(const unsigned char* buffer);
			void ParseDetectorData(const unsigned char* buffer);

			void StartUpConnection();
			void SendGetSerialNumber();
			void SendGetHardwareInfo();
			void SendGetStatus();
			void SendGetCode();
			void SendGetDetectorState();
			void SendLogOff();
			void SendBroadcastFlags();
			void SendBroadcastFlags(const BroadCastFlag flags);
			void SendSetMode(const address_t address, const Command command, const ProtocolMode mode);
			void SendSetLocoMode(const address_t address, const protocol_t protocol);
			void SendSetLocoModeMM(const address_t address);
			void SendSetLocoModeDCC(const address_t address);
			void SendSetTurnoutMode(const address_t address, const protocol_t protocol);
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

