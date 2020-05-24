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

#include <cstring>    //memset
#include <fcntl.h>
#include <sstream>
#include <termios.h>
#include <unistd.h>

#include "Network/Select.h"
#include "Hardware/RM485.h"
#include "Utils/Utils.h"

using std::string;

namespace Hardware
{
	// create instance of RM485
	extern "C" RM485* create_RM485(const HardwareParams* params)
	{
		return new RM485(params);
	}

	// delete instance of RM485
	extern "C" void destroy_RM485(RM485* RM485)
	{
		delete(RM485);
	}

	RM485::RM485(const HardwareParams* params)
	:	HardwareInterface(params->GetManager(), params->GetControlID(), "RM485 / " + params->GetName() + " at serial port " + params->GetArg1()),
		logger(Logger::Logger::GetLogger("RM485 " + params->GetName() + " " + params->GetArg1())),
		communication(params->GetArg1()),
		run(true),
		rescanAddress(0),
		rescanCount(RescanCountStart)
	{
		logger->Info(Languages::TextStarting, name);

		memset(data, 0, sizeof(data));

		ttyFileDescriptor = open(params->GetArg1().c_str(), O_RDWR | O_NOCTTY);
		if (ttyFileDescriptor == -1)
		{
			logger->Error(Languages::TextUnableToOpenSerial, params->GetArg1());
			return;
		}

		rm485Thread = std::thread(&Hardware::RM485::RM485Worker, this);
	}

	RM485::~RM485()
	{
		run = false;
		if (ttyFileDescriptor < 0)
		{
			return;
		}
		rm485Thread.join();
		close(ttyFileDescriptor);
	}

	void RM485::ScanAddress(uint16_t address)
	{
		uint8_t version = communication.Version(address);
		rmAlive[address] = (version > 0);
		if (rmAlive[address] == false)
		{
			return;
		}
		logger->Info(Languages::TextRM485ModuleFound, address);
		uint16_t baseAddress = address * Communication::MaxInputBytesPerModule;
		ssize_t length = communication.ReadAll(address, data + baseAddress);
		for (uint16_t a = baseAddress; a < baseAddress + length; ++a)
		{
			if (data[a] == 0)
			{
				return;
			}
			uint8_t dataByte = data[a];
			for (uint16_t pin = baseAddress * 8 + 1; pin <= (baseAddress + 1) * 8; ++pin)
			{
				DataModel::Feedback::FeedbackState state = ((dataByte & 0x01) == 0 ? DataModel::Feedback::FeedbackStateFree : DataModel::Feedback::FeedbackStateOccupied);
				if (state == DataModel::Feedback::FeedbackStateOccupied)
				{
					logger->Info(Languages::TextFeedbackChange, ((pin - 1) & 0x07) + 1, address, state);
					manager->FeedbackState(controlID, pin, state);
				}
				dataByte >>= 1;
			}
		}
	}

	void RM485::ScanBus()
	{
		for (uint16_t address = 0; address < NrOfModules; ++address)
		{
			ScanAddress(address);
		}
	}

	void RM485::ReadUpdateData()
	{
		for (uint16_t address = 0; address < NrOfModules; ++address)
		{
			if (rmAlive[address] == false)
			{
				continue;
			}
			const uint8_t MaxDeltaBytesPerModule = 125;

			uint8_t addresses[MaxDeltaBytesPerModule];
			uint8_t newData[MaxDeltaBytesPerModule];
			ssize_t length = communication.ReadDelta(address, addresses, newData);
			for (uint8_t pos = 0; pos < length; ++pos)
			{
				uint16_t byteAddress = (address * Communication::MaxInputBytesPerModule) + addresses[pos];
				uint8_t oldData = data[byteAddress];
				uint8_t diff = oldData ^ newData[pos];
				for (uint8_t shift = 0; shift < 8; ++shift)
				{
					if ((diff >> shift) & 0x01)
					{
						DataModel::Feedback::FeedbackState state = (((newData[pos] >> shift) & 0x01) == 0 ? DataModel::Feedback::FeedbackStateFree : DataModel::Feedback::FeedbackStateOccupied);
						uint16_t pin = (byteAddress * 8) + shift + 1;
						logger->Info(Languages::TextFeedbackChange, shift + 1, byteAddress, state);
						manager->FeedbackState(controlID, pin, state);
					}
				}
				data[byteAddress] = newData[pos];
			}
		}
		Utils::Utils::SleepForMilliseconds(100);
	}

	void RM485::RM485Worker()
	{
		Utils::Utils::SetThreadName("RM485");
		ScanBus();
		while(run)
		{
			ReadUpdateData();
		}
	}

	uint8_t RM485::Communication::Version(const uint8_t address)
	{
		bool ret = rs485.Send(address, CommandVersion);
		if (ret == false)
		{
			return 0;
		}
		uint8_t buffer[4];
		size_t length = rs485.Receive(buffer, sizeof(buffer));
		if (length != 3 || buffer[0] != address + 0x80 || buffer[1] != CommandVersion)
		{
			return 0;
		}
		return buffer[2];
	}

	ssize_t RM485::Communication::ReadAll(const uint8_t address, uint8_t* data)
	{
		// we expect at least MaxInputBytesPerModules byte of space in data
		bool ret = rs485.Send(address, CommandReadAll);
		if (ret == false)
		{
			return 0;
		}
		uint8_t buffer[MaxInputBytesPerModule + 2];
		size_t length = rs485.Receive(buffer, sizeof(buffer));
		if (length < 2 || buffer[0] != address + 0x80 || buffer[1] != CommandReadAll)
		{
			return 0;
		}
		length -= 2;
		memcpy(data, buffer + 2, length);
		return length;
	}

	ssize_t RM485::Communication::ReadDelta(const uint8_t address, uint8_t* addresses, uint8_t* data)
	{
		// we expect at least MaxInputBytesPerModules byte of space in addresses/data
		bool ret = rs485.Send(address, CommandReadDelta);
		if (ret == false)
		{
			return 0;
		}
		uint8_t buffer[MinMessageLength + (2*MaxDeltaPerMessage)];
		size_t length = rs485.Receive(buffer, sizeof(buffer));
		if (length < 2 || (length + 1) >> 1 != (length >> 1) || buffer[0] != address + 0x80 || buffer[1] != CommandReadDelta)
		{
			return 0;
		}
		// align length
		length -= 2;
		length >>= 1;
		// fill argument buffer with data
		for (size_t i = 0; i < length; ++i)
		{
			size_t bufferPos = (i << 1) + 2;
			addresses[i] = buffer[bufferPos];
			data[i] = buffer[bufferPos + 1];
		}	
		return length;
	}

	RM485::RS485::RS485(const string& tty)
	:	fileDescriptor(open(tty.c_str(), O_RDWR | O_NOCTTY))
	{
		if (fileDescriptor == -1)
		{
			return;
		}
		struct termios options;
		tcgetattr(fileDescriptor, &options);
		cfsetispeed(&options, B9600);
		cfsetospeed(&options, B9600);
		options.c_iflag = 0;
		options.c_oflag = 0;
		options.c_cflag = 0;
		options.c_cflag |= CS8;     // 8 data bits
		options.c_cflag |= CLOCAL;  // ignore control lines
		options.c_cflag |= CREAD;   // enable receiver
		options.c_lflag = 0;
		options.c_cc[VMIN] = 1;     // read one byte at least
		options.c_cc[VTIME] = 1;    // timeout = 0.1s
		tcsetattr(fileDescriptor, TCSANOW, &options); // store options
		tcflush(fileDescriptor, TCIFLUSH); // clear RX buffer
	}

	RM485::RS485::~RS485()
	{
		if (fileDescriptor < 0)
		{
			return;
		}
		close(fileDescriptor);
	}

	bool RM485::RS485::Send(const uint8_t address, const uint8_t command, const uint8_t* buffer, const size_t bufferLength)
	{
		if (fileDescriptor < 0)
		{
			return false;
		}
		if (bufferLength > 252)
		{
			return false;
		}
		RM485::CRC8 crc8;
		WriteChar(StartMessage);
		crc8.calcChar(StartMessage);
		WriteChar(bufferLength + 3);
		crc8.calcChar(bufferLength + 3);
		WriteChar(address);
		crc8.calcChar(address);
		WriteChar(command);
		crc8.calcChar(command);
		for (size_t i = 0; i < bufferLength; ++i)
		{
			WriteChar(buffer[i]);
			crc8.calcChar(buffer[i]);
		}
		WriteChar(crc8.value());
		return true;
	}

	size_t RM485::RS485::Receive(uint8_t* buffer, const size_t bufferLength)
	{
		if (fileDescriptor < 0)
		{
			return 0;
		}
		RM485::CRC8 crc8;
		// read startByte
		uint8_t c;
		do
		{
			int ret = ReadChar(c);
			if (ret <= 0)
			{
				return 0;
			}
		}
		while (c != StartMessage);
		crc8.calcChar(c);

		// read lengthByte
		uint8_t length;
		int ret = ReadChar(length);
		if (ret <= 0 || length > bufferLength)
		{
			return 0;
		}
		crc8.calcChar(length);
		--length;

		// read data
		for (unsigned int i = 0; i < bufferLength; ++i)
		{
			ret = ReadChar(buffer[i]);
			if (ret <= 0)
			{
				return 0;
			}

			if (i < length)
			{
				crc8.calcChar(buffer[i]);
				continue;
			}

			// check CRC
			if (crc8.value() != buffer[i])
			{
				return 0;
			}

			return length;
		}

		// buffer overflow
		return 0;
	}

	int RM485::RS485::WriteChar(uint8_t c)
	{
		// effective character write
		return write(fileDescriptor, reinterpret_cast<char*>(&c), 1);
	}

	int RM485::RS485::ReadChar(uint8_t& c)
	{
		struct timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = 50000; // timeout = 50ms
		fd_set set;
		FD_ZERO(&set);
		FD_SET(fileDescriptor, &set);
		int ret = TEMP_FAILURE_RETRY(select(FD_SETSIZE, &set, nullptr, nullptr, &timeout));
		if (ret <= 0)
		{
			return ret;
		}

		return read(fileDescriptor, reinterpret_cast<char*>(&c), 1);
	}

	uint8_t RM485::CRC8::calcString(const uint8_t* const s, const uint8_t size)
	{
		CRC8 crc8;
		for(uint8_t pos = 0; pos < size; ++pos)
		{
			crc8.calcChar(s[pos]);
		}
		return crc8.actualValue;
	}

	// crcTable from https://www.3dbrew.org/wiki/CRC-8-CCITT
	const uint8_t RM485::CRC8::crcTable[] =
	{
		0x00, 0x07, 0x0E, 0x09, 0x1C, 0x1B, 0x12, 0x15,
		0x38, 0x3F, 0x36, 0x31, 0x24, 0x23, 0x2A, 0x2D,
		0x70, 0x77, 0x7E, 0x79, 0x6C, 0x6B, 0x62, 0x65,
		0x48, 0x4F, 0x46, 0x41, 0x54, 0x53, 0x5A, 0x5D,
		0xE0, 0xE7, 0xEE, 0xE9, 0xFC, 0xFB, 0xF2, 0xF5,
		0xD8, 0xDF, 0xD6, 0xD1, 0xC4, 0xC3, 0xCA, 0xCD,
		0x90, 0x97, 0x9E, 0x99, 0x8C, 0x8B, 0x82, 0x85,
		0xA8, 0xAF, 0xA6, 0xA1, 0xB4, 0xB3, 0xBA, 0xBD,
		0xC7, 0xC0, 0xC9, 0xCE, 0xDB, 0xDC, 0xD5, 0xD2,
		0xFF, 0xF8, 0xF1, 0xF6, 0xE3, 0xE4, 0xED, 0xEA,
		0xB7, 0xB0, 0xB9, 0xBE, 0xAB, 0xAC, 0xA5, 0xA2,
		0x8F, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9D, 0x9A,
		0x27, 0x20, 0x29, 0x2E, 0x3B, 0x3C, 0x35, 0x32,
		0x1F, 0x18, 0x11, 0x16, 0x03, 0x04, 0x0D, 0x0A,
		0x57, 0x50, 0x59, 0x5E, 0x4B, 0x4C, 0x45, 0x42,
		0x6F, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7D, 0x7A,
		0x89, 0x8E, 0x87, 0x80, 0x95, 0x92, 0x9B, 0x9C,
		0xB1, 0xB6, 0xBF, 0xB8, 0xAD, 0xAA, 0xA3, 0xA4,
		0xF9, 0xFE, 0xF7, 0xF0, 0xE5, 0xE2, 0xEB, 0xEC,
		0xC1, 0xC6, 0xCF, 0xC8, 0xDD, 0xDA, 0xD3, 0xD4,
		0x69, 0x6E, 0x67, 0x60, 0x75, 0x72, 0x7B, 0x7C,
		0x51, 0x56, 0x5F, 0x58, 0x4D, 0x4A, 0x43, 0x44,
		0x19, 0x1E, 0x17, 0x10, 0x05, 0x02, 0x0B, 0x0C,
		0x21, 0x26, 0x2F, 0x28, 0x3D, 0x3A, 0x33, 0x34,
		0x4E, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5C, 0x5B,
		0x76, 0x71, 0x78, 0x7F, 0x6A, 0x6D, 0x64, 0x63,
		0x3E, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2C, 0x2B,
		0x06, 0x01, 0x08, 0x0F, 0x1A, 0x1D, 0x14, 0x13,
		0xAE, 0xA9, 0xA0, 0xA7, 0xB2, 0xB5, 0xBC, 0xBB,
		0x96, 0x91, 0x98, 0x9F, 0x8A, 0x8D, 0x84, 0x83,
		0xDE, 0xD9, 0xD0, 0xD7, 0xC2, 0xC5, 0xCC, 0xCB,
		0xE6, 0xE1, 0xE8, 0xEF, 0xFA, 0xFD, 0xF4, 0xF3
	};
} // namespace
