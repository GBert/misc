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
#include "Utils/Utils.h"

namespace Hardware
{
	void MaerklinCAN::CreateCommandHeader(unsigned char* buffer, const canPrio_t prio, const canCommand_t command, const canResponse_t response, const canLength_t length)
	{
		buffer[0] = (prio << 1) | (command >> 7);
		buffer[1] = (command << 1) | (response & 0x01);
		buffer[2] = (hash >> 8);
		buffer[3] = (hash & 0xFF);
		buffer[4] = length;
	}

	void MaerklinCAN::ReadCommandHeader(const unsigned char* buffer, canPrio_t& prio, canCommand_t& command, canResponse_t& response, canLength_t& length, canAddress_t& address, protocol_t& protocol)
	{
		prio = buffer[0] >> 1;
		command = (canCommand_t)(buffer[0]) << 7 | (canCommand_t)(buffer[1]) >> 1;
		response = buffer[1] & 0x01;
		length = buffer[4];
		address = Utils::Utils::DataBigEndianToInt(buffer + 5);
		canAddress_t maskedAddress = address & 0x0000FC00;
		address &= 0x03FF;

		switch (maskedAddress)
		{
			case 0x3800:
			case 0x3C00:
			case 0xC000:
				protocol = ProtocolDCC;
				return;

			case 0x4000:
				protocol = ProtocolMFX;
				return;

			default:
				protocol = ProtocolMM2;
				return;
		}
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

	void MaerklinCAN::CreateBoosterCommand(unsigned char* buffer, const boosterState_t status)
	{
		logger->Info(status ? Languages::TextTurningBoosterOn : Languages::TextTurningBoosterOff);
		// fill up header & locid
		CreateCommandHeader(buffer, 0, 0x00, 0, 5);
		// set data buffer (8 bytes) to 0
		int64_t* buffer_data = (int64_t*) (buffer + 5);
		*buffer_data = 0L;
		//buffer[5-8]: 0 = all
		//buffer[9]: subcommand stop 0x01
		buffer[9] = status;
	}

	void MaerklinCAN::CreateLocoSpeedCommand(unsigned char* buffer, const protocol_t protocol, const address_t address, const locoSpeed_t speed)
	{
		logger->Info(Languages::TextSettingSpeedWithProtocol, protocol, address, speed);
		// set header
		CreateCommandHeader(buffer, 0, 0x04, 0, 6);
		// set data buffer (8 bytes) to 0
		int64_t* buffer_data = (int64_t*) (buffer + 5);
		*buffer_data = 0L;
		// set locID
		CreateLocID(buffer + 5, protocol, address);
		// set speed
		buffer[9] = (speed >> 8);
		buffer[10] = (speed & 0xFF);
	}

	void MaerklinCAN::CreateLocoDirectionCommand(unsigned char* buffer, const protocol_t protocol, const address_t address, const direction_t direction)
	{
		logger->Info(Languages::TextSettingDirectionWithProtocol, protocol, address, Languages::GetLeftRight(direction));
		// set header
		CreateCommandHeader(buffer, 0, 0x05, 0, 5);
		// set data buffer (8 bytes) to 0
		int64_t* buffer_data = (int64_t*) (buffer + 5);
		*buffer_data = 0L;
		// set locID
		CreateLocID(buffer + 5, protocol, address);
		// set speed
		buffer[9] = (direction ? 1 : 2);
	}

	void MaerklinCAN::CreateLocoFunctionCommand(unsigned char* buffer, const protocol_t protocol, const address_t address, const function_t function, const bool on)
	{
		logger->Info(Languages::TextSettingFunctionWithProtocol, static_cast<int>(function), static_cast<int>(protocol), address, Languages::GetOnOff(on));
		// set header
		CreateCommandHeader(buffer, 0, 0x06, 0, 6);
		// set data buffer (8 bytes) to 0
		int64_t* buffer_data = (int64_t*) (buffer + 5);
		*buffer_data = 0L;
		// set locID
		CreateLocID(buffer + 5, protocol, address);
		buffer[9] = function;
		buffer[10] = on;
	}

	void MaerklinCAN::CreateAccessoryCommand(unsigned char* buffer, const protocol_t protocol, const address_t address, const accessoryState_t state, const bool on)
	{
		logger->Info(Languages::TextSettingAccessoryWithProtocol, static_cast<int>(protocol), address, Languages::GetGreenRed(state), Languages::GetOnOff(on));
		// set header
		CreateCommandHeader(buffer, 0, 0x0B, 0, 6);
		// set data buffer (8 bytes) to 0
		int64_t* buffer_data = (int64_t*) (buffer + 5);
		*buffer_data = 0L;
		// set locID
		CreateAccessoryID(buffer + 5, protocol, address - 1); // GUI-address is 1-based, protocol-address is 0-based
		buffer[9] = state & 0x03;
		buffer[10] = static_cast<unsigned char>(on);
	}

	void MaerklinCAN::Parse(const unsigned char* buffer)
	{
		//xlog("Receiver %i bytes received", datalen);
		//hexlog(buffer, datalen);
		canPrio_t prio;
		canCommand_t command;
		canResponse_t response;
		canLength_t length;
		canAddress_t address;
		protocol_t protocol;
		ReadCommandHeader(buffer, prio, command, response, length, address, protocol);
		if (command == 0x11 && response)
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
			logger->Info(Languages::TextFeedbackChange, address & 0x000F, address >> 4, onOff);
			manager->FeedbackState(controlID, address, state);
		}
		else if (command == 0x00 && !response && length == 5)
		{
			unsigned char subcmd = buffer[9];
			switch (subcmd)
			{
				case 0x00:
					// system stop
					manager->Booster(ControlTypeHardware, BoosterStop);
					break;

				case 0x01:
					// system go
					manager->Booster(ControlTypeHardware, BoosterGo);
					break;
			}
		}
		else if (command == 0x04 && !response && length == 6)
		{
			// speed event
			locoSpeed_t speed = Utils::Utils::DataBigEndianToShort(buffer + 9);
			logger->Info(Languages::TextReceivedSpeedCommand, protocol, address, speed);
			manager->LocoSpeed(ControlTypeHardware, controlID, protocol, static_cast<address_t>(address), speed);
		}
		else if (command == 0x05 && !response && length == 5)
		{
			// direction event (implies speed=0)
			direction_t direction = (buffer[9] == 1 ? DirectionRight : DirectionLeft);
			logger->Info(Languages::TextReceivedDirectionCommand, protocol, address, direction);
			manager->LocoSpeed(ControlTypeHardware, controlID, protocol, static_cast<address_t>(address), MinSpeed);
			manager->LocoDirection(ControlTypeHardware, controlID, protocol, static_cast<address_t>(address), direction);
		}
		else if (command == 0x06 && !response && length == 6)
		{
			// function event
			function_t function = buffer[9];
			bool on = buffer[10] != 0;
			logger->Info(Languages::TextReceivedFunctionCommand, protocol, address, function, on);
			manager->LocoFunction(ControlTypeHardware, controlID, protocol, static_cast<address_t>(address), function, on);
		}
		else if (command == 0x0B && !response && length == 6 && buffer[10] == 1)
		{
			// accessory event
			accessoryState_t state = buffer[9];
			// GUI-address is 1-based, protocol-address is 0-based
			++address;
			logger->Info(Languages::TextReceivedAccessoryCommand, protocol, address, state);
			manager->AccessoryState(ControlTypeHardware, controlID, protocol, address, state);
		}
	}
} // namespace
