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

#include <string>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>   //close & write;


#include "HardwareInterface.h"
#include "HardwareParams.h"
#include "Logger/Logger.h"
#include "Manager.h"

namespace Hardware
{
	class RM485 : HardwareInterface
	{
		private:
			class CRC8
			{
				public:
					static uint8_t calcString(const uint8_t* const s, const uint8_t size);

					CRC8() : actualValue(0x07) {};

					void calcChar(const uint8_t c) { actualValue = crcTable[actualValue ^ c]; }
					uint8_t value() const { return actualValue; }

				private:
					static const uint8_t crcTable[256];
					uint8_t actualValue;
			};

			class RS485
			{
				public:
					RS485(const std::string& tty);
					~RS485();
					bool Send(const uint8_t address, const uint8_t command, const uint8_t* c = nullptr, const size_t length = 0);
					size_t Receive(uint8_t* c, const size_t length);

				private:
					int WriteChar(uint8_t c);
					int ReadChar(uint8_t& c);

					int fileDescriptor;

					static const uint8_t StartMessage = 0xA5;
			};

			class Communication
			{
				public:
					static const uint8_t MaxInputBytesPerModule = 16;
				
					Communication(const std::string& tty) : rs485(tty) {}

					uint8_t Version(const uint8_t address);
					ssize_t ReadAll(const uint8_t address, uint8_t* data);
					ssize_t ReadDelta(const uint8_t address, uint8_t* addresses, uint8_t* data);

				private:
					static const uint8_t MaxDeltaPerMessage = 32;
					static const uint8_t MinMessageLength = 5; // Laenge einer Message ohne Daten

					enum Commands : uint8_t
					{
						CommandVersion = 0x01,
						CommandReadAll = 0x02,
						CommandReadDelta = 0x03,
						CommandChangeAddress = 0x04, // unused but reserved
						CommandNumberOfModules = 0x05 // unused but reserved
					};

					RS485 rs485;
			};

		public:
			RM485(const HardwareParams* params);
			~RM485();

			bool CanHandleFeedback() const override { return true; }

			static void GetArgumentTypesAndHint(std::map<unsigned char,ArgumentType>& argumentTypes, std::string& hint)
			{
				argumentTypes[1] = ArgumentTypeSerialPort;
				hint = Languages::GetText(Languages::TextHintRM485);
			}

		private:
			Logger::Logger* logger;
			int ttyFileDescriptor;
			Communication communication;
			volatile bool run;
			std::thread rm485Thread;

			static const uint8_t NrOfModules = 32;
			bool rmAlive[NrOfModules];
			uint8_t data[NrOfModules * Communication::MaxInputBytesPerModule];

			uint8_t rescanAddress;
			uint8_t rescanCount;
			static const uint8_t RescanCountStart = 10;

			void ScanAddress(uint16_t address);
			void ScanBus();
			void ReadInitData();
			void ReadUpdateData();
			void RM485Worker();
	};

	extern "C" RM485* create_RM485(const HardwareParams* params);
	extern "C" void destroy_RM485(RM485* RM485);
} // namespace
