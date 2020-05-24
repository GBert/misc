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

#include <cstdint>    //int64_t;
#include <cstdio>     //printf
#include <cstdlib>    //exit(0);
#include <cstring>    //memset
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include "Hardware/Z21.h"
#include "Utils/Utils.h"

namespace Hardware
{

	// create instance of Z21
	extern "C" Z21* create_Z21(const HardwareParams* params)
	{
		return new Z21(params);
	}

	// delete instance of Z21
	extern "C" void destroy_Z21(Z21* z21)
	{
		delete(z21);
	}

	Z21::Z21(const HardwareParams* params)
	:	HardwareInterface(params->GetManager(), params->GetControlID(), "Z21 / " + params->GetName() + " at IP " + params->GetArg1()),
	 	logger(Logger::Logger::GetLogger("Z21 " + params->GetName() + " " + params->GetArg1())),
	 	run(true),
	 	connection(logger, params->GetArg1(), Z21Port),
	 	lastProgramMode(ProgramModeMm),
	 	connected(false)
	{
		logger->Info(Languages::TextStarting, name);

		if (connection.IsConnected())
		{
			logger->Info(Languages::TextSenderSocketCreated);
		}
		else
		{
			logger->Error(Languages::TextUnableToCreateUdpSocket, params->GetArg1(), Z21Port);
		}
		receiverThread = std::thread(&Hardware::Z21::Receiver, this);
		heartBeatThread = std::thread(&Hardware::Z21::HeartBeatSender, this);
		accessorySenderThread = std::thread(&Hardware::Z21::AccessorySender, this);
	}

	Z21::~Z21()
	{
		run = false;
		SendLogOff();
		accessoryQueue.Terminate();
		connection.Terminate();
		accessorySenderThread.join();
		heartBeatThread.join();
		receiverThread.join();
		logger->Info(Languages::TextTerminatingSenderSocket);
	}

	void Z21::Booster(const BoosterState status)
	{
		logger->Info(status ? Languages::TextTurningBoosterOn : Languages::TextTurningBoosterOff);
		unsigned char buffer[7] = { 0x07, 0x00, 0x40, 0x00, 0x21, 0x80, 0xA1 };
		buffer[5] |= status;
		Send(buffer, sizeof(buffer));
	}

	unsigned char Z21::EncodeSpeed14(const Speed speed)
	{
		Speed speedInternal = speed >> 6;
		switch (speedInternal)
		{
			case MinSpeed:
				return 0x00;

			case 0x0F:
				return 0x0F;

			default:
				return speedInternal + 1;
		}
	}

	Speed Z21::DecodeSpeed14(unsigned char data)
	{
		switch (data)
		{
			case 0x00:
				return MinSpeed;

			case 0x0F:
				return MaxSpeed;

			default:
				return (data - 1) << 6;
		}
	}

	unsigned char Z21::EncodeSpeed28(const Speed speed)
	{
		Speed speedInternal = speed >> 5;
		switch (speedInternal)
		{
			case MinSpeed:
				return 0x00;

			case 0x1F:
				return 0x1F;

			default:
				++speedInternal;
				return (speedInternal >> 1 | (speedInternal & 0x01) << 4);
		}
	}

	Speed Z21::DecodeSpeed28(unsigned char data)
	{
		switch (data)
		{
			case 0x00:
				return MinSpeed;

			case 0x1F:
				return MaxSpeed;

			default:
				unsigned char internalData = ((data >> 4) & 0x01) | ((data & 0x0F) << 1);
				--internalData;
				return internalData << 5;
		}
	}

	unsigned char Z21::EncodeSpeed128(const Speed speed)
	{
		Speed speedInternal = speed >> 3;
		switch (speedInternal)
		{
			case MinSpeed:
				return 0x00;

			case 0x7F:
				return 0x7F;

			default:
				return speedInternal + 1;
		}
	}

	Speed Z21::DecodeSpeed128(unsigned char data)
	{
		switch (data)
		{
			case 0x00:
				return MinSpeed;

			case 0x7F:
				return MaxSpeed;

			default:
				return (data - 1) << 3;
		}
	}

	void Z21::LocoSpeed(const Protocol protocol, const Address address, const Speed speed)
	{
		if (!LocoProtocolSupported(protocol))
		{
			return;
		}
		Direction direction = locoCache.GetDirection(address);
		locoCache.SetSpeed(address, speed);
		LocoSpeedDirection(protocol, address, speed, direction);
	}

	void Z21::LocoDirection(const Protocol protocol, const Address address, const Direction direction)
	{
		if (!LocoProtocolSupported(protocol))
		{
			return;
		}
		Speed speed = locoCache.GetSpeed(address);
		locoCache.SetDirection(address, direction);
		LocoSpeedDirection(protocol, address, speed, direction);
	}

	void Z21::LocoSpeedDirection(const Protocol protocol, const Address address, const Speed speed, const Direction direction)
	{
		if (!LocoProtocolSupported(protocol))
		{
			return;
		}
		unsigned char buffer[10] = { 0x0A, 0x00, 0x40, 0x00, 0xE4 };
		switch (protocol)
		{
			case ProtocolMM1:
			case ProtocolDCC14:
				buffer[5] = 0x10;
				buffer[8] = EncodeSpeed14(speed);
				break;

			case ProtocolMM15:
			case ProtocolDCC28:
				buffer[5] = 0x12;
				buffer[8] = EncodeSpeed28(speed);
				break;

			case ProtocolMM2:
			case ProtocolDCC128:
				buffer[5] = 0x13;
				buffer[8] = EncodeSpeed128(speed);
				break;

			default:
				return;
		}
		SendSetLocoMode(address, protocol);
		Utils::Utils::ShortToDataBigEndian(address | 0xC000, buffer + 6);
		buffer[8] |=  static_cast<unsigned char>(direction) << 7;
		buffer[9] = buffer[4] ^ buffer[5] ^ buffer[6] ^ buffer[7] ^ buffer[8];
		Send(buffer, sizeof(buffer));
	}

	void Z21::LocoFunction(__attribute__ ((unused)) const Protocol protocol, const Address address, const Function function, const DataModel::LocoFunctions::FunctionState on)
	{
		if (!LocoProtocolSupported(protocol))
		{
			return;
		}
		locoCache.SetFunction(address, function, on);
		unsigned char buffer[10] = { 0x0A, 0x00, 0x40, 0x00, 0xE4, 0xF8 };
		Utils::Utils::ShortToDataBigEndian(address | 0xC000, buffer + 6);
		buffer[8] = (static_cast<unsigned char>(on == DataModel::LocoFunctions::FunctionStateOn) << 6) | (function & 0x3F);
		buffer[9] = buffer[4] ^ buffer[5] ^ buffer[6] ^ buffer[7] ^ buffer[8];
		Send(buffer, sizeof(buffer));
	}

	void Z21::LocoSpeedDirectionFunctions(const Protocol protocol, const Address address, const Speed speed, const Direction direction, std::vector<DataModel::LocoFunctions::FunctionState>& functions)
	{
		if (!LocoProtocolSupported(protocol))
		{
			return;
		}
		locoCache.SetSpeedDirectionProtocol(address, speed, direction, protocol);
		LocoSpeedDirection(protocol, address, speed, direction);
		for (size_t functionNr = 0; functionNr < functions.size(); ++functionNr)
		{
			LocoFunction(protocol, address, functionNr, functions[functionNr]);
		}
	}

	void Z21::Accessory(const Protocol protocol, const Address address, const DataModel::AccessoryState state, const DataModel::AccessoryPulseDuration duration)
	{
		if (!AccessoryProtocolSupported(protocol))
		{
			return;
		}
		AccessoryQueueEntry entry(protocol, address, state, duration);
		accessoryQueue.Enqueue(entry);
	}

	void Z21::AccessoryOn(const Protocol protocol, const Address address, const DataModel::AccessoryState state)
	{
		AccessoryOnOrOff(protocol, address, state, true);
	}

	void Z21::AccessoryOff(const Protocol protocol, const Address address, const DataModel::AccessoryState state)
	{
		AccessoryOnOrOff(protocol, address, state, false);
	}

	void Z21::AccessoryOnOrOff(__attribute__((unused)) const Protocol protocol, const Address address, const DataModel::AccessoryState state, const bool on)
	{
		const Address zeroBasedAddress = address - 1;
		unsigned char buffer[9] = { 0x09, 0x00, 0x40, 0x00, 0x53 };
		Utils::Utils::ShortToDataBigEndian(zeroBasedAddress, buffer + 5);
		buffer[7] = 0x80 | (static_cast<unsigned char>(on) << 3) | static_cast<unsigned char>(state);
		buffer[8] = buffer[4] ^ buffer[5] ^ buffer[6] ^ buffer[7];
		Send(buffer, sizeof(buffer));
	}

	void Z21::AccessorySender()
	{
		Utils::Utils::SetThreadName("Z21 Accessory Sender");
		logger->Info(Languages::TextAccessorySenderThreadStarted);
		while (run)
		{
			AccessoryQueueEntry entry = accessoryQueue.Dequeue();
			if (entry.protocol == ProtocolNone)
			{
				// ProtocolNone is in queue when we should quit
				continue;
			}
			SendSetTurnoutMode(entry.address, entry.protocol);
			AccessoryOn(entry.protocol, entry.address, entry.state);
			Utils::Utils::SleepForMilliseconds(entry.duration);
			AccessoryOff(entry.protocol, entry.address, entry.state);
		}
		logger->Info(Languages::TextTerminatingAccessorySenderThread);
	}

	void Z21::ProgramRead(const ProgramMode mode, const Address address, const CvNumber cv)
	{
		switch (mode)
		{
			case ProgramModeDccDirect:
				logger->Info(Languages::TextProgramDccRead, cv);
				ProgramDccRead(cv);
				break;

			case ProgramModeDccPomLoco:
				logger->Info(Languages::TextProgramDccPomLocoRead, address, cv);
				ProgramDccPom(PomLoco, PomReadByte, address, cv);
				break;

			case ProgramModeDccPomAccessory:
				logger->Info(Languages::TextProgramDccPomAccessoryRead, address, cv);
				ProgramDccPom(PomAccessory, PomReadByte, address, cv);
				break;

			default:
				return;
		}
		lastProgramMode = mode;
	}

	void Z21::ProgramWrite(const ProgramMode mode, const Address address, const CvNumber cv, const CvValue value)
	{
		switch (mode)
		{
			case ProgramModeMm:
				logger->Info(Languages::TextProgramMm, cv, static_cast<int>(value));
				ProgramMm(cv, value);
				break;

			case ProgramModeDccDirect:
				logger->Info(Languages::TextProgramDccWrite, cv, static_cast<int>(value));
				ProgramDccWrite(cv, value);
				break;

			case ProgramModeDccPomLoco:
				logger->Info(Languages::TextProgramDccPomLocoWrite, address, cv, value);
				ProgramDccPom(PomLoco, PomWriteByte, address, cv, value);
				break;

			case ProgramModeDccPomAccessory:
				logger->Info(Languages::TextProgramDccPomAccessoryWrite, address, cv, value);
				ProgramDccPom(PomAccessory, PomWriteByte, address, cv, value);
				break;

			default:
				return;
		}
		lastProgramMode = mode;
	}

	void Z21::ProgramMm(const CvNumber cv, const CvValue value)
	{
		const unsigned char zeroBasedCv = static_cast<unsigned char>((cv - 1) & 0xFF);
		unsigned char buffer[10] = { 0x0A, 0x00, 0x40, 0x00, 0x24, 0xFF, 0x00, zeroBasedCv, value };
		buffer[9] = buffer[4] ^ buffer[5] ^ buffer[6] ^ buffer[7] ^ buffer[8];
		Send(buffer, sizeof(buffer));
	}

	void Z21::ProgramDccRead(const CvNumber cv)
	{
		const CvNumber zeroBasedCv = cv - 1;
		unsigned char buffer[9] = { 0x09, 0x00, 0x40, 0x00, 0x23, 0x11};
		Utils::Utils::ShortToDataBigEndian(zeroBasedCv, buffer + 6);
		buffer[8] = buffer[4] ^ buffer[5] ^ buffer[6] ^ buffer[7];
		Send(buffer, sizeof(buffer));
	}

	void Z21::ProgramDccWrite(const CvNumber cv, const CvValue value)
	{
		const CvNumber zeroBasedCv = cv - 1;
		unsigned char buffer[10] = { 0x0A, 0x00, 0x40, 0x00, 0x24, 0x12};
		Utils::Utils::ShortToDataBigEndian(zeroBasedCv, buffer + 6);
		buffer[8] = value;
		buffer[9] = buffer[4] ^ buffer[5] ^ buffer[6] ^ buffer[7] ^ buffer[8];
		Send(buffer, sizeof(buffer));
	}

	void Z21::ProgramDccPom(const PomDB0 db0, const PomOption option, const Address address, const CvNumber cv, const CvValue value)
	{
		Address internalAddress = address;
		if (db0 == PomAccessory)
		{
			internalAddress <<= 4;
		}
		const CvNumber OptionAndZeroBasedCv = option | ((cv - 1) & 0x03FF);
		unsigned char buffer[12] = { 0x0C, 0x00, 0x40, 0x00, 0xE6, db0 };
		Utils::Utils::ShortToDataBigEndian(internalAddress, buffer + 6);
		Utils::Utils::ShortToDataBigEndian(OptionAndZeroBasedCv, buffer + 8);
		buffer[10] = value;
		buffer[11] = buffer[4] ^ buffer[5] ^ buffer[6] ^ buffer[7] ^ buffer[8] ^ buffer[9] ^ buffer[10];
		Send(buffer, sizeof(buffer));
	}

	void Z21::StartUpConnection()
	{
		SendGetSerialNumber();
		SendGetHardwareInfo();
		SendGetCode();
		SendBroadcastFlags(static_cast<BroadCastFlag>(
			BroadCastFlagBasic
			| BroadCastFlagRBus
			| BroadCastFlagAllLoco
			| BroadCastFlagCanDetector));
		SendGetDetectorState();
	}

	void Z21::HeartBeatSender()
	{
		Utils::Utils::SetMinThreadPriority();
		Utils::Utils::SetThreadName("Z21 Heartbeat Sender");
		logger->Info(Languages::TextHeartBeatThreadStarted);
		const unsigned int counterMask = 0x07;
		unsigned int counter = counterMask;
		while(run)
		{
			Utils::Utils::SleepForSeconds(1);
			++counter;
			counter &= counterMask;
			if (counter > 0)
			{
				continue;
			}
			if (connected)
			{
				connected = false;
			}
			else
			{
				StartUpConnection();
			}
			SendGetStatus();
		}
		logger->Info(Languages::TextTerminatingHeartBeatThread);
	}

	void Z21::Receiver()
	{
		Utils::Utils::SetThreadName("Z21 Receiver");
		logger->Info(Languages::TextReceiverThreadStarted);

		unsigned char buffer[Z21CommandBufferLength];
		while(run)
		{
			ssize_t dataLength = connection.Receive(buffer, sizeof(buffer));

			if (run == false)
			{
				break;
			}

			if (dataLength < 0)
			{
				logger->Error(Languages::TextUnableToReceiveData);
				break;
			}

			if (dataLength == 0)
			{
				continue;
			}

			logger->Hex(buffer, dataLength);

			ssize_t dataRead = 0;
			while (dataRead < dataLength)
			{
				ssize_t ret = ParseData(buffer + dataRead, dataLength - dataRead);
				if (ret == -1)
				{
					break;
				}
				dataRead += ret;
			}
		}
		logger->Info(Languages::TextTerminatingReceiverThread);
	}

	ssize_t Z21::ParseData(const unsigned char* buffer, size_t bufferLength)
	{
		unsigned short dataLength = Utils::Utils::DataLittleEndianToShort(buffer);
		if (dataLength < 4 || dataLength > bufferLength)
		{
			return -1;
		}

		uint16_t header = Utils::Utils::DataLittleEndianToShort(buffer + 2);
		switch(header)
		{
			case HeaderSerialNumber:
			{
				unsigned int serialNumber = Utils::Utils::DataLittleEndianToInt(buffer + 4);
				logger->Info(Languages::TextSerialNumberIs, serialNumber);
				break;
			}

			case HeaderCode:
				switch (buffer[4])
				{
					case FeaturesNotRestricted:
						logger->Debug(Languages::TextZ21NotRestricted);
						break;

					case FeaturesStartLocked:
						logger->Error(Languages::TextZ21StartLocked);
						break;

					case FeaturesStartUnlocked:
						logger->Debug(Languages::TextZ21StartUnlocked);
						break;

					default:
						logger->Debug(Languages::TextZ21RestrictionsUnknown);
				}
				break;

			case HeaderHardwareInfo:
			{
				unsigned int hardwareType = Utils::Utils::DataLittleEndianToInt(buffer + 4);
				const char* hardwareTypeText;
				switch (hardwareType)
				{
					case 0x00000200:
						hardwareTypeText = Languages::GetText(Languages::TextZ21Black2012);
						break;

					case 0x00000201:
						hardwareTypeText = Languages::GetText(Languages::TextZ21Black2013);
						break;

					case 0x00000202:
						hardwareTypeText = Languages::GetText(Languages::TextZ21SmartRail2012);
						break;

					case 0x00000203:
						hardwareTypeText = Languages::GetText(Languages::TextZ21White2013);
						break;

					case 0x00000204:
						hardwareTypeText = Languages::GetText(Languages::TextZ21Start2016);
						break;

					default:
						hardwareTypeText = Languages::GetText(Languages::TextZ21Unknown);
						break;
				}

				unsigned char firmwareVersionMajor = buffer[9];
				unsigned char firmwareVersionMinor = buffer[8];
				std::string firmwareVersionText = Utils::Utils::IntegerToBCD(firmwareVersionMajor) + "." + Utils::Utils::IntegerToBCD(firmwareVersionMinor);
				logger->Info(Languages::TextZ21Type, hardwareTypeText, firmwareVersionText);
				break;
			}

			case HeaderSeeXHeader:
				ParseXHeader(buffer);
				break;

			case HeaderBroadcastFlags:
				logger->Warning(Languages::TextNotImplemented, __FILE__, __LINE__);
				break;

			case HeaderLocoMode:
				logger->Warning(Languages::TextNotImplemented, __FILE__, __LINE__);
				break;

			case HeaderTurnoutMode:
				logger->Warning(Languages::TextNotImplemented, __FILE__, __LINE__);
				break;

			case HeaderRmBusData:
				logger->Warning(Languages::TextNotImplemented, __FILE__, __LINE__);
				break;

			case HeaderSystemData:
				logger->Warning(Languages::TextNotImplemented, __FILE__, __LINE__);
				break;

			case HeaderRailComtData:
				logger->Warning(Languages::TextNotImplemented, __FILE__, __LINE__);
				break;

			case HeaderLocoNetRx:
			case HeaderLocoNetTx:
			case HeaderLocoNetLan:
			case HeaderLocoNetDispatch:
			case HeaderLocoNetDetector:
				// we do not parse LocoNet data
				break;

			case HeaderDetector:
				ParseDetectorData(buffer);
				break;

			default:
				logger->Warning(Languages::TextZ21DoesNotUnderstand);
				return -1;
		}
		return dataLength;
	}

	void Z21::ParseXHeader(const unsigned char* buffer)
	{
		unsigned char xHeader = buffer[4];
		switch (xHeader)
		{
			case XHeaderTurnoutInfo:
				ParseTurnoutData(buffer);
				break;

			case XHeaderSeeDB0:
				ParseDB0(buffer);
				break;

			case XHeaderStatusChanged:
				connected = true;
				break;

			case XHeaderVersion:
				logger->Warning(Languages::TextNotImplemented, __FILE__, __LINE__);
				break;

			case XHeaderCvResult:
				ParseCvData(buffer);
				break;

			case XHeaderBcStopped:
				logger->Warning(Languages::TextNotImplemented, __FILE__, __LINE__);
				break;

			case XHeaderLocoInfo:
				ParseLocoData(buffer);
				break;

			case XHeaderFirmwareVersion:
				logger->Warning(Languages::TextNotImplemented, __FILE__, __LINE__);
				break;

			default:
				break;
		}
	}

	void Z21::ParseDB0(const unsigned char* buffer)
	{
		switch (buffer[5])
		{
			case DB0PowerOff:
				if (buffer[6] != 0x61)
				{
					logger->Error(Languages::TextCheckSumError);
				}
				logger->Debug(Languages::TextBoosterIsTurnedOff);
				manager->Booster(ControlTypeHardware, BoosterStateStop);
				break;

			case DB0PowerOn:
				if (buffer[6] != 0x60)
				{
					logger->Error(Languages::TextCheckSumError);
				}
				logger->Debug(Languages::TextBoosterIsTurnedOn);
				manager->Booster(ControlTypeHardware, BoosterStateGo);
				break;

			case DB0ProgrammingMode:
				if (buffer[6] != 0x63)
				{
					logger->Error(Languages::TextCheckSumError);
				}
				logger->Debug(Languages::TextProgrammingMode);
				manager->Booster(ControlTypeHardware, BoosterStateStop);
				break;

			case DB0ShortCircuit:
				if (buffer[6] != 0x69)
				{
					logger->Error(Languages::TextCheckSumError);
				}
				logger->Debug(Languages::TextShortCircuit);
				manager->Booster(ControlTypeInternal, BoosterStateStop);
				break;

			case DB0CvShortCircuit:
				if (buffer[6] != 0x73)
				{
					logger->Error(Languages::TextCheckSumError);
				}
				logger->Debug(Languages::TextShortCircuit);
				manager->Booster(ControlTypeInternal, BoosterStateStop);
				break;

			case DB0CvNack:
				if (buffer[6] != 0x72)
				{
					logger->Error(Languages::TextCheckSumError);
				}
				logger->Debug(Languages::TextNoAnswerFromDecoder);
				break;

			case DB0UnknownCommand:
				logger->Warning(Languages::TextZ21DoesNotUnderstand);
				break;
		}
	}

	void Z21::ParseTurnoutData(const unsigned char* buffer)
	{
		DataModel::AccessoryState state;
		switch (buffer[7])
		{
			case 0x01:
				state = DataModel::AccessoryStateOff;
				break;

			case 0x02:
				state = DataModel::AccessoryStateOn;
				break;

			default:
				return;
		}
		const Address zeroBasedAddress = Utils::Utils::DataBigEndianToShort(buffer + 5);
		const Address address = zeroBasedAddress + 1;
		const Protocol protocol = turnoutCache.GetProtocol(address);
		manager->AccessoryState(ControlTypeHardware, controlID, protocol, address, state);
	}

	void Z21::ParseLocoData(const unsigned char* buffer)
	{
		const Address address = Utils::Utils::DataBigEndianToShort(buffer + 5) & 0x3FFF;
		const unsigned char protocolType = buffer[7] & 0x07;
		Protocol protocol;
		const unsigned char speedData = buffer[8] & 0x7F;
		Speed newSpeed;
		Protocol storedProtocol = locoCache.GetProtocol(address);
		switch (protocolType)
		{
			case 0:
				switch (storedProtocol)
				{
					case ProtocolDCC14:
						protocol = ProtocolDCC14;
						break;

					case ProtocolMM1:
						protocol = ProtocolMM1;
						break;

					default:
						locoCache.SetProtocol(address, ProtocolDCC14);
						protocol = ProtocolDCC14;
						logger->Warning(Languages::TextActualAndStoredProtocolsDiffer, ProtocolSymbols[ProtocolDCC14], ProtocolSymbols[storedProtocol]);
						break;
				}
				newSpeed = DecodeSpeed14(speedData);
				break;

			case 2:
				switch (storedProtocol)
				{
					case ProtocolDCC28:
						protocol = ProtocolDCC28;
						break;

					case ProtocolMM15:
						protocol = ProtocolMM15;
						break;

					default:
						locoCache.SetProtocol(address, ProtocolDCC28);
						protocol = ProtocolDCC28;
						logger->Warning(Languages::TextActualAndStoredProtocolsDiffer, ProtocolSymbols[ProtocolDCC28], ProtocolSymbols[storedProtocol]);
						break;
				}
				newSpeed = DecodeSpeed28(speedData);
				break;

			case 4:
				switch (storedProtocol)
				{
					case ProtocolDCC128:
						protocol = ProtocolDCC128;
						break;

					case ProtocolMM2:
						protocol = ProtocolMM2;
						break;

					default:
						locoCache.SetProtocol(address, ProtocolDCC128);
						protocol = ProtocolDCC128;
						logger->Warning(Languages::TextActualAndStoredProtocolsDiffer, ProtocolSymbols[ProtocolDCC128], ProtocolSymbols[storedProtocol]);
						break;
				}
				newSpeed = DecodeSpeed128(speedData);
				break;

			default:
				return;
		}
		const Speed oldSpeed = locoCache.GetSpeed(address);
		if (newSpeed != oldSpeed)
		{
			locoCache.SetSpeed(address, newSpeed);
			manager->LocoSpeed(ControlTypeHardware, controlID, protocol, address, newSpeed);
		}
		const Direction newDirection = (buffer[8] >> 7) ? DirectionRight : DirectionLeft;
		const Direction oldDirection = locoCache.GetDirection(address);
		if (newDirection != oldDirection)
		{
			locoCache.SetDirection(address, newDirection);
			manager->LocoDirection(ControlTypeHardware, controlID, protocol, address, newDirection);
		}
		const uint32_t oldFunctions = locoCache.GetFunctions(address);
		const uint32_t f0 = (static_cast<uint32_t>(buffer[9]) >> 4) & 0x01;
		const uint32_t f1_4 = (static_cast<uint32_t>(buffer[9]) << 1) & 0x1E;
		const uint32_t f5_12 = static_cast<uint32_t>(buffer[10]) << 5;
		const uint32_t f13_20 = static_cast<uint32_t>(buffer[11]) << 13;
		const uint32_t f21_28 = static_cast<uint32_t>(buffer[12]) << 21;
		const uint32_t newFunctions = f0 | f1_4 | f5_12 | f13_20 | f21_28;
		if (newFunctions == oldFunctions)
		{
			return;
		}
		const uint32_t functionsDiff = newFunctions ^ oldFunctions;
		for (Function function = 0; function <= 28; ++function)
		{
			const bool stateChange = (functionsDiff >> function) & 0x01;
			if (stateChange == false)
			{
				continue;
			}
			const DataModel::LocoFunctions::FunctionState newState = ((newFunctions >> function) & 0x01 ? DataModel::LocoFunctions::FunctionStateOn : DataModel::LocoFunctions::FunctionStateOff);
			locoCache.SetFunction(address, function, newState);
			manager->LocoFunction(ControlTypeHardware, controlID, protocol, address, function, newState);
		}
	}

	void Z21::ParseCvData(const unsigned char* buffer)
	{
		if (buffer[5] != 0x14)
		{
			logger->Error(Languages::TextCheckSumError);
			return;
		}
		if (lastProgramMode == ProgramModeMm)
		{
			return;
		}
		const CvNumber cv = Utils::Utils::DataBigEndianToShort(buffer + 6) + 1;
		const CvValue value = buffer[8];
		logger->Debug(Languages::TextProgramReadValue, cv, value);
		manager->ProgramValue(cv, value);
	}

	void Z21::ParseDetectorData(const unsigned char* buffer)
	{
		FeedbackPin pin = Utils::Utils::DataLittleEndianToShort(buffer + 6);
		uint8_t port = buffer[8];
		--pin;
		pin <<= 3;
		pin += port;
		++pin;
		uint8_t type = buffer[9];
		uint16_t value1 = Utils::Utils::DataLittleEndianToShort(buffer + 10);
		DataModel::Feedback::FeedbackState state;
		switch (type)
		{
			case 0x01:
			{
				value1 >>= 12;
				value1 &= 0x0001;
				state = static_cast<DataModel::Feedback::FeedbackState>(value1);
				break;
			}

			default:
			{
				state = (value1 > 0 ? DataModel::Feedback::FeedbackStateOccupied : DataModel::Feedback::FeedbackStateFree);
				break;
			}
		}
		manager->FeedbackState(controlID, pin, state);
	}

	void Z21::SendGetSerialNumber()
	{
		char buffer[4] = { 0x04, 0x00, 0x10, 0x00 };
		Send(buffer, sizeof(buffer));
	}

	void Z21::SendGetHardwareInfo()
	{
		char buffer[4] = { 0x04, 0x00, 0x1A, 0x00 };
		Send(buffer, sizeof(buffer));
	}

	void Z21::SendGetStatus()
	{
		char buffer[7] = { 0x07, 0x00, 0x40, 0x00, 0x21, 0x24, 0x05 };
		Send(buffer, sizeof(buffer));
	}

	void Z21::SendGetCode()
	{
		char buffer[4] = { 0x04, 0x00, 0x18, 0x00 };
		Send(buffer, sizeof(buffer));
	}

	void Z21::SendGetDetectorState()
	{
		unsigned char buffer[7] = { 0x07, 0x00, 0xC4, 0x00, 0x00, 0x00, 0xD0 };
		Send(buffer, sizeof(buffer));
	}

	void Z21::SendLogOff()
	{
		char buffer[4] = { 0x04, 0x00, 0x30, 0x00 };
		Send(buffer, sizeof(buffer));
	}

	void Z21::SendBroadcastFlags()
	{
		SendBroadcastFlags(static_cast<BroadCastFlag>(
			BroadCastFlagBasic
			| BroadCastFlagRBus
			| BroadCastFlagAllLoco
			| BroadCastFlagCanDetector));
	}

	void Z21::SendBroadcastFlags(const BroadCastFlag flags)
	{
		unsigned char buffer[8] = { 0x08, 0x00, 0x50, 0x00 };
		Utils::Utils::IntToDataLittleEndian(flags, buffer + 4);
		Send(buffer, sizeof(buffer));
	}

	void Z21::SendSetMode(const Address address, const Command command, const ProtocolMode mode)
	{
		switch (command)
		{
			case CommandSetLocoMode:
			case CommandSetTurnoutMode:
				break;

			default:
				return;
		}

		switch (mode)
		{
			case ProtocolModeMM:
				if (address > MaxMMAddress)
				{
					return;
				}
				break;

			case ProtocolModeDCC:
				break;

			default:
				return;
		}

		unsigned char buffer[7] = { 0x07, 0x00, command, 0x00, 0x00, 0x00, mode };
		Utils::Utils::ShortToDataBigEndian(address, buffer + 4);
		Send(buffer, sizeof(buffer));
	}

	void Z21::SendSetLocoMode(const Address address, const Protocol protocol)
	{
		const Protocol storedProtocol = locoCache.GetProtocol(address);
		if (storedProtocol == protocol)
		{
			return;
		}
		switch (protocol)
		{
			case ProtocolMM1:
			case ProtocolMM15:
			case ProtocolMM2:
				SendSetLocoModeMM(address);
				break;

			case ProtocolDCC14:
			case ProtocolDCC28:
			case ProtocolDCC128:
				SendSetLocoModeDCC(address);
				break;

			default:
				return;
		}
		locoCache.SetProtocol(address, protocol);
	}

	void Z21::SendSetLocoModeMM(const Address address)
	{
		SendSetMode(address, CommandSetLocoMode, ProtocolModeMM);
	}

	void Z21::SendSetLocoModeDCC(const Address address)
	{
		SendSetMode(address, CommandSetLocoMode, ProtocolModeDCC);
	}

	void Z21::SendSetTurnoutMode(const Address address, const Protocol protocol)
	{
		Protocol storedProtocol = turnoutCache.GetProtocol(address);
		if (storedProtocol == protocol)
		{
			return;
		}
		switch (protocol)
		{
			case ProtocolMM:
				SendSetTurnoutModeMM(address);
				break;

			case ProtocolDCC:
				SendSetTurnoutModeDCC(address);
				break;

			default:
				return;
		}
		turnoutCache.SetProtocol(address, protocol);
	}

	void Z21::SendSetTurnoutModeMM(const Address address)
	{
		const Address zeroBasedAddress = address - 1;
		SendSetMode(zeroBasedAddress, CommandSetTurnoutMode, ProtocolModeMM);
	}

	void Z21::SendSetTurnoutModeDCC(const Address address)
	{
		const Address zeroBasedAddress = address - 1;
		SendSetMode(zeroBasedAddress, CommandSetTurnoutMode, ProtocolModeDCC);
	}

	int Z21::Send(const unsigned char* buffer, const size_t bufferLength)
	{
		logger->Hex(buffer, bufferLength);
		return connection.Send(buffer, bufferLength);
	}
} // namespace
