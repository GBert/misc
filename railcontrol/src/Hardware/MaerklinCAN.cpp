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

#include "Hardware/MaerklinCAN.h"

namespace Hardware
{
	void MaerklinCAN::CreateCommandHeader(unsigned char* buffer, const CanPrio prio, const CanCommand command, const CanResponse response, const CanLength length)
	{
		buffer[0] = (prio << 1) | (command >> 7);
		buffer[1] = (command << 1) | (response & 0x01);
		buffer[2] = (hash >> 8);
		buffer[3] = (hash & 0xFF);
		buffer[4] = length;
	}

	void MaerklinCAN::ParseAddressProtocol(const unsigned char* buffer, CanAddress& address, protocol_t& protocol)
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

	void MaerklinCAN::CreateLocID(unsigned char* buffer, const protocol_t& protocol, const address_t& address)
	{
		uint32_t locID = address;
		if (protocol == ProtocolDCC)
		{
			locID |= 0xC000;
		}
		else if (protocol == ProtocolMFX)
		{
			locID |= 0x4000;
		}
		// else expect PROTOCOL_MM2: do nothing
		Utils::Utils::IntToDataBigEndian(locID, buffer);
	}

	void MaerklinCAN::CreateAccessoryID(unsigned char* buffer, const protocol_t& protocol, const address_t& address)
	{
		uint32_t locID = address;
		if (protocol == ProtocolDCC)
		{
			locID |= 0x3800;
		}
		else
		{
			locID |= 0x3000;
		}
		Utils::Utils::IntToDataBigEndian(locID, buffer);
	}

	void MaerklinCAN::Booster(const boosterState_t status)
	{
		unsigned char buffer[CANCommandBufferLength];
		logger->Info(status ? Languages::TextTurningBoosterOn : Languages::TextTurningBoosterOff);
		// fill up header & locid
		CreateCommandHeader(buffer, 0, CanCommandSystem, CanResponseCommand, 5);
		// set data buffer (8 bytes) to 0
		int64_t* buffer_data = (int64_t*) (buffer + 5);
		*buffer_data = 0L;
		//buffer[5-8]: 0 = all
		//buffer[9]: subcommand stop 0x01
		buffer[9] = status;
		Send(buffer);
	}

	void MaerklinCAN::LocoSpeed(const protocol_t protocol, const address_t address, const locoSpeed_t speed)
	{
		unsigned char buffer[CANCommandBufferLength];
		logger->Info(Languages::TextSettingSpeedWithProtocol, protocol, address, speed);
		// set header
		CreateCommandHeader(buffer, 0, CanCommandLocoSpeed, CanResponseCommand, 6);
		// set data buffer (8 bytes) to 0
		int64_t* buffer_data = (int64_t*) (buffer + 5);
		*buffer_data = 0L;
		// set locID
		CreateLocID(buffer + 5, protocol, address);
		// set speed
		buffer[9] = (speed >> 8);
		buffer[10] = (speed & 0xFF);
		Send(buffer);
	}

	void MaerklinCAN::LocoDirection(const protocol_t protocol, const address_t address, const direction_t direction)
	{
		unsigned char buffer[CANCommandBufferLength];
		logger->Info(Languages::TextSettingDirectionWithProtocol, protocol, address, Languages::GetLeftRight(direction));
		// set header
		CreateCommandHeader(buffer, 0, CanCommandLocoDirection, CanResponseCommand, 5);
		// set data buffer (8 bytes) to 0
		int64_t* buffer_data = (int64_t*) (buffer + 5);
		*buffer_data = 0L;
		// set locID
		CreateLocID(buffer + 5, protocol, address);
		// set speed
		buffer[9] = (direction ? 1 : 2);
		Send(buffer);
	}

	void MaerklinCAN::LocoFunction(const protocol_t protocol, const address_t address, const function_t function, const bool on)
	{
		unsigned char buffer[CANCommandBufferLength];
		logger->Info(Languages::TextSettingFunctionWithProtocol, static_cast<int>(function), static_cast<int>(protocol), address, Languages::GetOnOff(on));
		// set header
		CreateCommandHeader(buffer, 0, CanCommandLocoFunction, CanResponseCommand, 6);
		// set data buffer (8 bytes) to 0
		int64_t* buffer_data = (int64_t*) (buffer + 5);
		*buffer_data = 0L;
		// set locID
		CreateLocID(buffer + 5, protocol, address);
		buffer[9] = function;
		buffer[10] = on;
		Send(buffer);
	}

	void MaerklinCAN::AccessoryOnOrOff(const protocol_t protocol, const address_t address, const accessoryState_t state, const bool on)
	{
		unsigned char buffer[CANCommandBufferLength];
		logger->Info(Languages::TextSettingAccessoryWithProtocol, static_cast<int>(protocol), address, Languages::GetGreenRed(state), Languages::GetOnOff(on));
		// set header
		CreateCommandHeader(buffer, 0, CanCommandAccessory, CanResponseCommand, 6);
		// set data buffer (8 bytes) to 0
		int64_t* buffer_data = (int64_t*) (buffer + 5);
		*buffer_data = 0L;
		// set locID
		CreateAccessoryID(buffer + 5, protocol, address - 1); // GUI-address is 1-based, protocol-address is 0-based
		buffer[9] = state & 0x03;
		buffer[10] = static_cast<unsigned char>(on);
		Send(buffer);
	}

	void MaerklinCAN::Parse(const unsigned char* buffer)
	{
		//xlog("Receiver %i bytes received", datalen);
		//hexlog(buffer, datalen);
		CanResponse response = ParseResponse(buffer);
		CanCommand command = ParseCommand(buffer);
		if (command == CanCommandS88Event && response)
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
		}
		if (response == true)
		{
			return;
		}

		CanLength length = ParseLength(buffer);
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
