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

#include "ProtocolMaerklinCAN.h"

namespace Hardware
{
	void ProtocolMaerklinCAN::CreateCommandHeader(unsigned char* buffer, const CanCommand command, const CanResponse response, const CanLength length)
	{
		const CanPrio prio = 0;
		buffer[0] = (prio << 1) | (command >> 7);
		buffer[1] = (command << 1) | (response & 0x01);
		Utils::Utils::ShortToDataBigEndian(hash, buffer + 2);
		buffer[4] = length;
		int64_t* data = (int64_t*) (buffer + 5);
		*data = 0L;
	}

	void ProtocolMaerklinCAN::ParseAddressProtocol(const unsigned char* buffer, CanAddress& address, protocol_t& protocol)
	{
		address = Utils::Utils::DataBigEndianToInt(buffer + 5);
		CanAddress maskedAddress = address & 0x0000FC00;

		if (maskedAddress == 0x0000 || maskedAddress == 0x3000)
		{
			protocol = ProtocolMM;
			address &= 0x03FF;
			return;
		}

		if (maskedAddress == 0x3800 || maskedAddress == 0x3C00)
		{
			protocol = ProtocolDCC;
			address &= 0x03FF;
			return;
		}

		maskedAddress = address & 0x0000C000;
		address &= 0x3FFF;
		if (maskedAddress == 0x4000)
		{
			protocol = ProtocolMFX;
			return;
		}
		if (maskedAddress == 0xC000)
		{
			protocol = ProtocolDCC;
			return;
		}

		protocol = ProtocolNone;
		address = 0;
	}

	void ProtocolMaerklinCAN::CreateLocalIDLoco(unsigned char* buffer, const protocol_t& protocol, const address_t& address)
	{
		uint32_t localID = address;
		if (protocol == ProtocolDCC)
		{
			localID |= 0xC000;
		}
		else if (protocol == ProtocolMFX)
		{
			localID |= 0x4000;
		}
		// else expect PROTOCOL_MM2: do nothing
		Utils::Utils::IntToDataBigEndian(localID, buffer + 5);
	}

	void ProtocolMaerklinCAN::CreateLocalIDAccessory(unsigned char* buffer, const protocol_t& protocol, const address_t& address)
	{
		uint32_t localID = address - 1; // GUI-address is 1-based, protocol-address is 0-based
		if (protocol == ProtocolDCC)
		{
			localID |= 0x3800;
		}
		else
		{
			localID |= 0x3000;
		}
		Utils::Utils::IntToDataBigEndian(localID, buffer + 5);
	}

	void ProtocolMaerklinCAN::Booster(const boosterState_t status)
	{
		unsigned char buffer[CANCommandBufferLength];
		logger->Info(status ? Languages::TextTurningBoosterOn : Languages::TextTurningBoosterOff);
		CreateCommandHeader(buffer, CanCommandSystem, CanResponseCommand, 5);
		buffer[9] = status;
		Send(buffer);
	}

	void ProtocolMaerklinCAN::LocoSpeed(const protocol_t protocol, const address_t address, const locoSpeed_t speed)
	{
		unsigned char buffer[CANCommandBufferLength];
		logger->Info(Languages::TextSettingSpeedWithProtocol, protocol, address, speed);
		CreateCommandHeader(buffer, CanCommandLocoSpeed, CanResponseCommand, 6);
		CreateLocalIDLoco(buffer, protocol, address);
		Utils::Utils::ShortToDataBigEndian(speed, buffer + 9);
		Send(buffer);
	}

	void ProtocolMaerklinCAN::LocoDirection(const protocol_t protocol, const address_t address, const direction_t direction)
	{
		unsigned char buffer[CANCommandBufferLength];
		logger->Info(Languages::TextSettingDirectionWithProtocol, protocol, address, Languages::GetLeftRight(direction));
		CreateCommandHeader(buffer, CanCommandLocoDirection, CanResponseCommand, 5);
		CreateLocalIDLoco(buffer, protocol, address);
		buffer[9] = (direction ? 1 : 2);
		Send(buffer);
	}

	void ProtocolMaerklinCAN::LocoFunction(const protocol_t protocol, const address_t address, const function_t function, const bool on)
	{
		unsigned char buffer[CANCommandBufferLength];
		logger->Info(Languages::TextSettingFunctionWithProtocol, static_cast<int>(function), static_cast<int>(protocol), address, Languages::GetOnOff(on));
		CreateCommandHeader(buffer, CanCommandLocoFunction, CanResponseCommand, 6);
		CreateLocalIDLoco(buffer, protocol, address);
		buffer[9] = function;
		buffer[10] = on;
		Send(buffer);
	}

	void ProtocolMaerklinCAN::AccessoryOnOrOff(const protocol_t protocol, const address_t address, const accessoryState_t state, const bool on)
	{
		unsigned char buffer[CANCommandBufferLength];
		logger->Info(Languages::TextSettingAccessoryWithProtocol, static_cast<int>(protocol), address, Languages::GetGreenRed(state), Languages::GetOnOff(on));
		CreateCommandHeader(buffer, CanCommandAccessory, CanResponseCommand, 6);
		CreateLocalIDAccessory(buffer, protocol, address);
		buffer[9] = state & 0x03;
		buffer[10] = static_cast<unsigned char>(on);
		Send(buffer);
	}

	void ProtocolMaerklinCAN::ProgramRead(const ProgramMode mode, const address_t address, const CvNumber cv)
	{
		address_t addressInternal = address;
		protocol_t protocol = ProtocolNone;
		switch (mode)
		{
			case ProgramModeDccDirect:
				logger->Info(Languages::TextProgramDccRead, cv);
				protocol = ProtocolDCC;
				break;

			case ProgramModeMfx:
				logger->Info(Languages::TextProgramMfxRead, address, cv);
				protocol = ProtocolMFX;
				break;

			default:
				return;
		}
		unsigned char buffer[CANCommandBufferLength];
		CreateCommandHeader(buffer, CanCommandReadConfig, CanResponseCommand, 7);
		CreateLocalIDLoco(buffer, protocol, addressInternal);
		Utils::Utils::ShortToDataBigEndian(cv, buffer + 9);
		buffer[11] = 1;
		Send(buffer);
	}

	void ProtocolMaerklinCAN::ProgramWrite(const ProgramMode mode, const address_t address, const CvNumber cv, const CvValue value)
	{
		address_t addressInternal = address;
		protocol_t protocol = ProtocolNone;
		unsigned char controlFlags = 0;
		switch (mode)
		{
			case ProgramModeMm:
				logger->Info(Languages::TextProgramMm, cv, value);
				protocol = ProtocolMM;
				addressInternal = 80;
				break;

			case ProgramModeMmPom:
				logger->Info(Languages::TextProgramMmPom, address, cv, value);
				protocol = ProtocolMM;
				controlFlags = 1 << 7;
				break;

			case ProgramModeDccDirect:
				logger->Info(Languages::TextProgramDccWrite, cv, value);
				protocol = ProtocolDCC;
				addressInternal = 0;
				break;

			case ProgramModeDccPomLoco:
				logger->Info(Languages::TextProgramDccWrite, address, cv, value);
				protocol = ProtocolDCC;
				controlFlags = 1 << 7;
				break;

			case ProgramModeMfx:
				logger->Info(Languages::TextProgramMfxWrite, address, cv, value);
				protocol = ProtocolMFX;
				controlFlags = 1 << 7;
				break;

			default:
				return;
		}
		unsigned char buffer[CANCommandBufferLength];
		CreateCommandHeader(buffer, CanCommandWriteConfig, CanResponseCommand, 8);
		CreateLocalIDLoco(buffer, protocol, addressInternal);
		Utils::Utils::ShortToDataBigEndian(cv, buffer + 9);
		buffer[11] = value;
		buffer[12] = controlFlags;
		Send(buffer);
	}

	void ProtocolMaerklinCAN::Parse(const unsigned char* buffer)
	{
		CanResponse response = ParseResponse(buffer);
		CanCommand command = ParseCommand(buffer);
		CanLength length = ParseLength(buffer);
		if (response == true)
		{
			switch (command)
			{
				case CanCommandS88Event:
					{
					// s88 event
					const char *onOff;
					DataModel::Feedback::feedbackState_t state;
					if (buffer[10])
					{
						onOff = Languages::GetText(Languages::TextOn);
						state = DataModel::Feedback::FeedbackStateOccupied;
					}
					else
					{
						onOff = Languages::GetText(Languages::TextOff);
						state = DataModel::Feedback::FeedbackStateFree;
					}
					CanAddress address = ParseAddress(buffer);
					logger->Info(Languages::TextFeedbackChange, address & 0x000F, address >> 4, onOff);
					manager->FeedbackState(controlID, address, state);
					return;
				}

				case CanCommandReadConfig:
				{
					if (length != 7)
					{
						return;
					}
					CvNumber cv = Utils::Utils::DataBigEndianToShort(buffer + 9);
					CvValue value = buffer[11];
					logger->Info(Languages::TextProgramReadValue, cv, value);
					manager->ProgramValue(cv, value);
					return;
				}

				default:
					return;
			}
		}

		if (command == CanCommandSystem && length == 5)
		{
			CanSubCommand subcmd = ParseSubCommand(buffer);
			switch (subcmd)
			{
				case CanSubCommandStop:
					// system stop
					manager->Booster(ControlTypeHardware, BoosterStop);
					break;

				case CanSubCommandGo:
					// system go
					manager->Booster(ControlTypeHardware, BoosterGo);
					break;
			}
			return;
		}

		if (command == CanCommandLocoSpeed && length == 6)
		{
			// loco speed event
			CanAddress address;
			protocol_t protocol;
			ParseAddressProtocol(buffer, address, protocol);
			locoSpeed_t speed = Utils::Utils::DataBigEndianToShort(buffer + 9);
			logger->Info(Languages::TextReceivedSpeedCommand, protocol, address, speed);
			manager->LocoSpeed(ControlTypeHardware, controlID, protocol, static_cast<address_t>(address), speed);
			return;
		}

		if (command == CanCommandLocoDirection && length == 5)
		{
			// loco direction event (implies speed=0)
			CanAddress address;
			protocol_t protocol;
			ParseAddressProtocol(buffer, address, protocol);
			direction_t direction = (buffer[9] == 1 ? DirectionRight : DirectionLeft);
			logger->Info(Languages::TextReceivedDirectionCommand, protocol, address, direction);
			manager->LocoSpeed(ControlTypeHardware, controlID, protocol, static_cast<address_t>(address), MinSpeed);
			manager->LocoDirection(ControlTypeHardware, controlID, protocol, static_cast<address_t>(address), direction);
			return;
		}

		if (command == CanCommandLocoFunction && length == 6)
		{
			// loco function event
			CanAddress address;
			protocol_t protocol;
			ParseAddressProtocol(buffer, address, protocol);
			function_t function = buffer[9];
			bool on = buffer[10] != 0;
			logger->Info(Languages::TextReceivedFunctionCommand, protocol, address, function, on);
			manager->LocoFunction(ControlTypeHardware, controlID, protocol, static_cast<address_t>(address), function, on);
			return;
		}

		if (command == CanCommandAccessory && length == 6 && buffer[10] == 1)
		{
			// accessory event
			CanAddress address;
			protocol_t protocol;
			ParseAddressProtocol(buffer, address, protocol);
			accessoryState_t state = buffer[9];
			// GUI-address is 1-based, protocol-address is 0-based
			++address;
			logger->Info(Languages::TextReceivedAccessoryCommand, protocol, address, state);
			manager->AccessoryState(ControlTypeHardware, controlID, protocol, address, state);
			return;
		}
	}
} // namespace
