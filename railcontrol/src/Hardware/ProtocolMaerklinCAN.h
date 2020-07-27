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

#include "DataModel/AccessoryBase.h"
#include "DataModel/LocoFunctions.h"
#include "Hardware/Capabilities.h"
#include "Hardware/LocoCache.h"
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

			inline Hardware::Capabilities GetCapabilities() const override
			{
				return Hardware::CapabilityLoco
					| Hardware::CapabilityAccessory
					| Hardware::CapabilityFeedback
					| Hardware::CapabilityProgram
					| Hardware::CapabilityProgramMmWrite
					| Hardware::CapabilityProgramMfxRead
					| Hardware::CapabilityProgramMfxWrite
					| Hardware::CapabilityProgramDccDirectRead
					| Hardware::CapabilityProgramDccDirectWrite
					| Hardware::CapabilityProgramDccPomWrite;
			}

			void GetLocoProtocols(std::vector<Protocol>& protocols) const override
			{
				protocols.push_back(ProtocolMM);
				protocols.push_back(ProtocolMFX);
				protocols.push_back(ProtocolDCC);
			}

			inline bool LocoProtocolSupported(Protocol protocol) const override
			{
				return (protocol == ProtocolMM || protocol == ProtocolMFX || protocol == ProtocolDCC);
			}

			inline void GetAccessoryProtocols(std::vector<Protocol>& protocols) const override
			{
				protocols.push_back(ProtocolMM);
				protocols.push_back(ProtocolDCC);
			}

			inline bool AccessoryProtocolSupported(Protocol protocol) const override
			{
				return (protocol == ProtocolMM || protocol == ProtocolDCC);
			}

			void Booster(const BoosterState status) override;
			void LocoSpeed(const Protocol protocol, const Address address, const Speed speed) override;
			void LocoOrientation(const Protocol protocol, const Address address, const Orientation orientation) override;

			void LocoFunction(const Protocol protocol,
				const Address address,
				const DataModel::LocoFunctionNr function,
				const DataModel::LocoFunctionState on) override;

			void AccessoryOnOrOff(const Protocol protocol, const Address address, const DataModel::AccessoryState state, const bool on) override;
			void ProgramRead(const ProgramMode mode, const Address address, const CvNumber cv) override;
			void ProgramWrite(const ProgramMode mode, const Address address, const CvNumber cv, const CvValue value) override;

		protected:
			ProtocolMaerklinCAN(HardwareParams* const params, Logger::Logger* logger, std::string name)
			:	HardwareInterface(params->GetManager(),
				params->GetControlID(), name),
				logger(logger),
				run(false),
				params(params),
				uid(Utils::Utils::HexToInteger(params->GetArg5(), 0)),
				hasCs2Master(false),
				canFileDataSize(0),
				canFileData(nullptr),
				canFileDataPointer(nullptr)
			{
				if (uid == 0)
				{
					GenerateUidHash();
				}
				else
				{
					hash = CalcHash(uid);
					logger->Debug(Languages::TextMyUidHash, params->GetArg5(), Utils::Utils::IntegerToHex(hash));
				}
			}

			void Init();

			virtual ~ProtocolMaerklinCAN();

			void Parse(const unsigned char* buffer);

			void Ping();
			void RequestLoks();

			virtual void Receiver() = 0;

			static const unsigned char CANCommandBufferLength = 13;

			Logger::Logger* logger;
			volatile bool run;

		private:
			enum CanCommand : unsigned char
			{
				CanCommandSystem = 0x00,
				CanCommandLocoSpeed = 0x04,
				CanCommandLocoDirection = 0x05,
				CanCommandLocoFunction = 0x06,
				CanCommandReadConfig = 0x07,
				CanCommandWriteConfig = 0x08,
				CanCommandAccessory = 0x0B,
				CanCommandS88Event = 0x11,
				CanCommandPing = 0x18,
				CanCommandRequestConfigData = 0x20,
				CanCommandConfigData = 0x21,
				CanCommandHello = 0x42
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

			enum CanDeviceType : uint16_t
			{
				CanDeviceGfp = 0x0000,
				CanDeviceGleisbox = 0x0010,
				CanDeviceConnect6021 = 0x0020,
				CanDeviceMs2 = 0x0030,
				CanDeviceMs2_2 = 0x0032,
				CanDeviceCs2Slave_2 = 0xeeee,
				CanDeviceWireless = 0xffe0,
				CanDeviceCs2Slave = 0xfff0,
				CanDeviceCs2Master = 0xffff
			};

//			enum CanFileType : uint8_t
//			{
//				CanFileTypeNone,
//				CanFileTypeLokinfo,
//				CanFileTypeLoknamen,
//				CanFileTypeMaginfo,
//				CanFileTypeLokdb,
//				CanFileTypeLang,
//				CanFileTypeLdbver,
//				CanFileTypeLangver,
//				CanFileTypeLoks,
//				CanFileTypeMags,
//				CanFileTypeGbs,
//				CanFileTypeFs,
//				CanFileTypeLokstat,
//				CanFileTypeMagstat,
//				CanFileTypeGbsstat,
//				CanFileTypeFsstat
//			};

			enum LocoFunctionCs2Icon : uint8_t
			{
				LocoFunctionCs2IconNone = 0,
				LocoFunctionCs2IconLight,
				LocoFunctionCs2IconInteriorLight1,
				LocoFunctionCs2IconBackLightForward,
				LocoFunctionCs2IconHeadlightHighBeamForward,
				LocoFunctionCs2IconSoundGeneral,
				LocoFunctionCs2IconPanto12,
				LocoFunctionCs2IconSmokeGenerator,
				LocoFunctionCs2IconShuntingMode,
				LocoFunctionCs2IconTelex12,
				LocoFunctionCs2IconHorn1,
				LocoFunctionCs2IconWhistle1,
				LocoFunctionCs2IconWhistle2,
				LocoFunctionCs2IconBell,
				LocoFunctionCs2IconLeftRight,
				LocoFunctionCs2IconUpDown1,
				LocoFunctionCs2IconTurnLeft,
				LocoFunctionCs2IconUpDown2,
				LocoFunctionCs2IconInertia,
				LocoFunctionCs2IconFan2,
				LocoFunctionCs2IconBreak1,
				LocoFunctionCs2IconGearBox,
				LocoFunctionCs2IconGenerator,
				LocoFunctionCs2IconRunning1,
				LocoFunctionCs2IconEngine1,
				LocoFunctionCs2IconStationAnnouncement1,
				LocoFunctionCs2IconShovelCoal,
				LocoFunctionCs2IconCloseDoor,
				LocoFunctionCs2IconOpenDoor,
				LocoFunctionCs2IconFan1,
				LocoFunctionCs2IconFan,
				LocoFunctionCs2IconFireBox,
				LocoFunctionCs2IconInteriorLight2,
				LocoFunctionCs2IconTableLight3,
				LocoFunctionCs2IconTableLight2,
				LocoFunctionCs2IconTableLight1,
				LocoFunctionCs2IconShakingRust,
				LocoFunctionCs2IconRailJoint,
				LocoFunctionCs2IconTrainNumberIndicator,
				LocoFunctionCs2IconMusic1,
				LocoFunctionCs2IconTrainDestinationIndicator,
				LocoFunctionCs2IconCabLight2,
				LocoFunctionCs2IconCabLight1,
				LocoFunctionCs2IconCoupler,
				LocoFunctionCs2IconBufferPush,
				LocoFunctionCs2IconStationAnnouncement3,
				LocoFunctionCs2IconCraneHook,
				LocoFunctionCs2IconBlinkingLight,
				LocoFunctionCs2IconCabLight12,
				LocoFunctionCs2IconCompressedAir,
				LocoFunctionCs2IconDefault,

				LocoFunctionCs2IconTelex2 = 82,
				LocoFunctionCs2IconTelex1,
				LocoFunctionCs2IconPanto2,
				LocoFunctionCs2IconPanto1,
				LocoFunctionCs2IconHeadlightReverse,
				LocoFunctionCs2IconHeadlightForward,
				LocoFunctionCs2IconUp2,
				LocoFunctionCs2IconFan3,
				LocoFunctionCs2IconEngineLight,
				LocoFunctionCs2IconSteamBlowOut,
				LocoFunctionCs2IconSteamBlow,
				LocoFunctionCs2IconCrane,
				LocoFunctionCs2IconUp1,
				LocoFunctionCs2IconDown,
				LocoFunctionCs2IconLeft,
				LocoFunctionCs2IconRight,
				LocoFunctionCs2IconTurnRight,
				LocoFunctionCs2IconMagnet,

				LocoFunctionCs2IconPanto = 101,

				LocoFunctionCs2IconRadio = 103,
				LocoFunctionCs2IconStationAnnouncement2,
				LocoFunctionCs2IconBackLightReverse,
				LocoFunctionCs2IconAirPump,
				LocoFunctionCs2IconSpeak,
				LocoFunctionCs2IconEngine2,
				LocoFunctionCs2IconNoSound,
				LocoFunctionCs2IconStairsLight,
				LocoFunctionCs2IconFillWater,
				LocoFunctionCs2IconBreak2
			};

			typedef unsigned char CanPrio;
			typedef unsigned char CanLength;
			typedef uint32_t CanUid;
			typedef uint16_t CanHash;
			typedef uint16_t CanFileCrc;

			void CreateCommandHeader(unsigned char* const buffer, const CanCommand command, const CanResponse response, const CanLength length);
			inline void ParseAddressProtocol(const unsigned char* const buffer, Address& address, Protocol& protocol)
			{
				Address input = ParseAddress(buffer);
				ParseAddressProtocol(input, address, protocol);
			}
			static void ParseAddressProtocol(const Address input, Address& address, Protocol& protocol);

			static inline CanPrio ParsePrio(const unsigned char* const buffer)
			{
				return buffer[0] >> 1;
			}

			static inline CanCommand ParseCommand(const unsigned char* const buffer)
			{
				return static_cast<CanCommand>((buffer[0] << 7) | (buffer[1] >> 1));
			}

			static inline CanSubCommand ParseSubCommand(const unsigned char* const buffer)
			{
				return static_cast<CanSubCommand>(buffer[9]);
			}

			static inline CanResponse ParseResponse(const unsigned char* const buffer)
			{
				return static_cast<CanResponse>(buffer[1] & 0x01);
			}

			static inline CanLength ParseLength(const unsigned char* const buffer)
			{
				return buffer[4];
			}

			static inline Address ParseAddress(const unsigned char* const buffer)
			{
				return static_cast<Address>(Utils::Utils::DataBigEndianToInt(buffer + 5));
			}

			static inline CanHash ParseHash(const unsigned char* const buffer)
			{
				return Utils::Utils::DataBigEndianToShort(buffer + 2);
			}

			static inline CanUid ParseUid(const unsigned char* const buffer)
			{
				return Utils::Utils::DataBigEndianToInt(buffer + 5);
			}

			void ParseCommandSystem(const unsigned char* const buffer);
			void ParseCommandLocoSpeed(const unsigned char* const buffer);
			void ParseCommandLocoDirection(const unsigned char* const buffer);
			void ParseCommandLocoFunction(const unsigned char* const buffer);
			void ParseCommandAccessory(const unsigned char* const buffer);
			void ParseCommandPing(const unsigned char* const buffer);
			void ParseCommandConfigData(const unsigned char* const buffer);
			void ParseCommandConfigDataFirst(const unsigned char* const buffer);
			void ParseCommandConfigDataNext(const unsigned char* const buffer);
			void ParseResponseS88Event(const unsigned char* const buffer);
			void ParseResponseReadConfig(const unsigned char* const buffer);
			void ParseResponsePing(const unsigned char* const buffer);

			bool ParseCs2FileKeyValue(const std::string& line, std::string& key, std::string& value);
			bool ParseCs2FileSubkeyValue(const std::string& line, std::string& key, std::string& value);
			void ParseCs2FileLocomotiveFunction(std::deque<std::string>& lines, LocoCacheEntry& cacheEntry);
			void ParseCs2FileLocomotive(std::deque<std::string>& lines);
			void ParseCs2FileLocomotivesSession(std::deque<std::string>& lines);
			void ParseCs2FileLocomotivesVersion(std::deque<std::string>& lines);
			void ParseCs2FileLocomotives(std::deque<std::string>& lines);
			void ParseCs2File(std::deque<std::string>& lines);

			static inline DataModel::LocoFunctionIcon MapLocoFunctionCs2ToRailControl(const DataModel::LocoFunctionIcon input)
			{
				return LocoFunctionMapCs2ToRailControl[input];
			}

			static CanHash CalcHash(const CanUid uid);
			void GenerateUidHash();

			void CreateLocalIDLoco(unsigned char* buffer, const Protocol& protocol, const Address& address);
			void CreateLocalIDAccessory(unsigned char* buffer, const Protocol& protocol, const Address& address);

			void Wait(const unsigned int duration) const;
			void Cs2MasterThread();

			inline void SendInternal(const unsigned char* buffer)
			{
				logger->Hex(buffer, 5 + ParseLength(buffer));
				Send(buffer);
			}
			virtual void Send(const unsigned char* buffer) = 0;

			static inline LocoFunctionCs2Icon CalculateCs2Icon(const DataModel::LocoFunctionNr nr,
				const DataModel::LocoFunctionIcon railcontrolIcon)
			{
				LocoFunctionCs2Icon icon = LocoFunctionMapRailControlToCs2[railcontrolIcon];
				if (icon == LocoFunctionCs2IconDefault)
				{
					icon = static_cast<LocoFunctionCs2Icon>(icon + nr);
				}
				return icon;
			}

			HardwareParams* const params;
			CanUid uid;
			CanHash hash;
			bool hasCs2Master;
			std::thread receiverThread;
			std::thread cs2MasterThread;

			size_t canFileDataSize;
			CanFileCrc canFileCrc;
			unsigned char* canFileData;
			unsigned char* canFileDataPointer;

			LocoCache locoCache;

			static const uint8_t MaxNrOfCs2FunctionIcons = 128;
			static const DataModel::LocoFunctionIcon LocoFunctionMapCs2ToRailControl[MaxNrOfCs2FunctionIcons];
			static const ProtocolMaerklinCAN::LocoFunctionCs2Icon LocoFunctionMapRailControlToCs2[DataModel::MaxLocoFunctionIcons];
	};
} // namespace

