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

#include <mutex>
#include <string>
#include <termios.h>
#include <unistd.h>   //close & write;

#include "Logger/Logger.h"

namespace Network
{
	class Serial
	{
		public:
			Serial(Logger::Logger* logger,
				const std::string& tty,
				const unsigned int dataSpeed, // from termio (ex. B9600)
				const unsigned char dataBits,
				const char parity,
				const unsigned char stopBits,
				const bool hardwareFlowControl = false)
			:	logger(logger),
			 	tty(tty),
			 	dataSpeed(dataSpeed),
			 	dataBits(dataBits),
			 	parity(parity),
			 	stopBits(stopBits),
			 	hardwareFlowControl(hardwareFlowControl)
			{
				Init();
			}

			~Serial()
			{
				Close();
			}

			void ReInit() { Close(); Init(); }

			bool IsConnected() const { return fileHandle != -1; }

			void ClearBuffers() { tcflush(fileHandle, TCIOFLUSH); }

			ssize_t Send(const std::string& data)
			{
				return Send(reinterpret_cast<const unsigned char*>(data.c_str()), data.length());
			}

			ssize_t Send(const unsigned char data)
			{
				return Send(&data, 1);
			}

			ssize_t Send(const unsigned char* data, const size_t size)
			{
				if (IsConnected() == false)
				{
					return 0;
				}
				std::lock_guard<std::mutex> Guard(fileHandleMutex);
				return write(fileHandle, data, size);
			}

			bool Receive(std::string& data, const size_t maxData = 1024, const unsigned int timeoutS = 0, const unsigned int timeoutUS = 100000);
			ssize_t Receive(unsigned char* data, const size_t maxData, const unsigned int timeoutS = 0, const unsigned int timeoutUS = 100000);
			bool ReceiveExact(std::string& data, const size_t length, const unsigned int timeoutS = 0, const unsigned int timeoutUS = 100000);
			ssize_t ReceiveExact(unsigned char* data, const size_t length, const unsigned int timeoutS = 0, const unsigned int timeoutUS = 100000);

		private:
			void Init();
			void Close();

			Logger::Logger* logger;
			const std::string tty;
			const unsigned int dataSpeed;
			const unsigned char dataBits;
			const char parity;
			const unsigned char stopBits;
			const bool hardwareFlowControl;
			int fileHandle;
			mutable std::mutex fileHandleMutex;
	};
}
