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

#include "DataModel/AccessoryBase.h"
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
			 	state(DataModel::DefaultState),
			 	duration(DataModel::DefaultAccessoryPulseDuration)
			{}

			AccessoryQueueEntry(const Protocol protocol, const Address address, const DataModel::AccessoryState state, const DataModel::AccessoryPulseDuration duration)
			:	protocol(protocol),
			 	address(address),
			 	state(state),
			 	duration(duration)
			{}

			Protocol protocol;
			Address address;
			DataModel::AccessoryState state;
			DataModel::AccessoryPulseDuration duration;
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

			void GetLocoProtocols(std::vector<Protocol>& protocols) const override
			{
				protocols.push_back(ProtocolMM1);
				protocols.push_back(ProtocolMM15);
				protocols.push_back(ProtocolMM2);
				protocols.push_back(ProtocolDCC14);
				protocols.push_back(ProtocolDCC28);
				protocols.push_back(ProtocolDCC128);
			}

			bool LocoProtocolSupported(Protocol protocol) const override
			{
				return (protocol == ProtocolMM1
					|| protocol == ProtocolMM15
					|| protocol == ProtocolMM2
					|| protocol == ProtocolDCC14
					|| protocol == ProtocolDCC28
					|| protocol == ProtocolDCC128);
			}

			void GetAccessoryProtocols(std::vector<Protocol>& protocols) const override
			{
				protocols.push_back(ProtocolMM);
				protocols.push_back(ProtocolDCC);
			}

			bool AccessoryProtocolSupported(Protocol protocol) const override
			{
				return (protocol == ProtocolMM || protocol == ProtocolDCC);
			}

			static void GetArgumentTypesAndHint(std::map<unsigned char,ArgumentType>& argumentTypes, std::string& hint)
			{
				argumentTypes[1] = ArgumentTypeIpAddress;
				hint = Languages::GetText(Languages::TextHintZ21);
			}

			void Booster(const BoosterState status) override;
			void LocoSpeed(const Protocol protocol, const Address address, const Speed speed) override;
			void LocoDirection(const Protocol protocol, const Address address, const Direction direction) override;
			void LocoFunction(const Protocol protocol, const Address address, const Function function, const DataModel::LocoFunctions::FunctionState on) override;
			void LocoSpeedDirectionFunctions(const Protocol protocol, const Address address, const Speed speed, const Direction direction, std::vector<DataModel::LocoFunctions::FunctionState>& functions) override;
			void Accessory(const Protocol protocol, const Address address, const DataModel::AccessoryState state, const DataModel::AccessoryPulseDuration duration) override;
			void AccessoryOnOrOff(const Protocol protocol, const Address address, const DataModel::AccessoryState state, const bool on) override;
			void ProgramRead(const ProgramMode mode, const Address address, const CvNumber cv) override;
			void ProgramWrite(const ProgramMode mode, const Address address, const CvNumber cv, const CvValue value) override;

		private:
			static const unsigned short Z21Port = 21105;
			static const unsigned int Z21CommandBufferLength = 1472; // = Max Ethernet MTU
			static const Address MaxMMAddress = 255;

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
			void ProgramDccPom(const PomDB0 db0, const PomOption option, const Address address, const CvNumber cv, const CvValue value = 0);

			void LocoSpeedDirection(const Protocol protocol, const Address address, const Speed speed, const Direction direction);
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
			void SendSetMode(const Address address, const Command command, const ProtocolMode mode);
			void SendSetLocoMode(const Address address, const Protocol protocol);
			void SendSetLocoModeMM(const Address address);
			void SendSetLocoModeDCC(const Address address);
			void SendSetTurnoutMode(const Address address, const Protocol protocol);
			void SendSetTurnoutModeMM(const Address address);
			void SendSetTurnoutModeDCC(const Address address);
			void AccessoryOn(const Protocol protocol, const Address address, const DataModel::AccessoryState state);
			void AccessoryOff(const Protocol protocol, const Address address, const DataModel::AccessoryState state);
			int Send(const unsigned char* buffer, const size_t bufferLength);
			int Send(const char* buffer, const size_t bufferLength) { return Send(reinterpret_cast<const unsigned char*>(buffer), bufferLength); }

			unsigned char EncodeSpeed14(const Speed speed);
			unsigned char EncodeSpeed28(const Speed speed);
			unsigned char EncodeSpeed128(const Speed speed);
			Speed DecodeSpeed14(unsigned char data);
			Speed DecodeSpeed28(unsigned char data);
			Speed DecodeSpeed128(unsigned char data);
	};

	extern "C" Z21* create_Z21(const HardwareParams* params);
	extern "C" void destroy_Z21(Z21* z21);

} // namespace

