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

#include <fcntl.h>
#include <termios.h>

#include "Network/Select.h"
#include "Network/Serial.h"

namespace Network
{
	void Serial::Init()
	{
		fileHandle = open(tty.c_str(), O_RDWR | O_NOCTTY);
		if (!IsConnected())
		{
			logger->Error(Languages::TextUnableToOpenSerial, tty);
			return;
		}

		struct termios options;
		options.c_cflag = 0;
		options.c_cc[VMIN] = 1;     // read one byte at least
		options.c_cc[VTIME] = 0;    // timeout disabled
		options.c_lflag = 0;
		options.c_iflag = 0;
		options.c_oflag = 0;
		cfsetispeed(&options, dataSpeed);
		cfsetospeed(&options, dataSpeed);
		switch (dataBits)
		{
			case 5:
				options.c_cflag |= CS5;     // 5 data bits
				break;

			case 6:
				options.c_cflag |= CS6;     // 6 data bits
				break;

			case 7:
				options.c_cflag |= CS7;     // 7 data bits
				break;

			case 8:
			default:
				options.c_cflag |= CS8;     // 8 data bits
				break;
		}

		if (stopBits == 2)
		{
			options.c_cflag |= CSTOPB; // 2 stop bit
		}
		// else 1 stop bit

		switch (parity)
		{
			case 'E':
			case 'e':
				options.c_cflag |= PARENB; // even parity
				break;

			case 'O':
			case 'o':
				options.c_cflag |= PARENB;
				options.c_cflag |= PARODD; // odd parity
				break;

			// default: no parity
		}

		// CSIZE not set: no datasize
		if (hardwareFlowControl == true)
		{
			options.c_cflag |= CRTSCTS;  // hardware flow control
		}
		options.c_cflag |= CLOCAL;  // ignore control lines
		options.c_cflag |= CREAD;   // enable receiver
		tcsetattr(fileHandle, TCSANOW, &options); // store options

		ClearBuffers();
	}

	void Serial::Close()
	 {
		if (!IsConnected())
		{
			return;
		}
		close(fileHandle);
		fileHandle = -1;
	 }

	bool Serial::Receive(std::string& data, const size_t maxData, const unsigned int timeoutS, const unsigned int timeoutUS)
	{
		const size_t dataBufferSize = 1024;
		unsigned char dataBuffer[dataBufferSize];
		const size_t max = maxData > dataBufferSize ? dataBufferSize : maxData;
		ssize_t ret = Receive(dataBuffer, max, timeoutS, timeoutUS);
		if (ret <= 0)
		{
			return false;
		}
		data.append(reinterpret_cast<char*>(dataBuffer), ret);
		return true;
	}

	ssize_t Serial::Receive(unsigned char* data, const size_t maxData, const unsigned int timeoutS, const unsigned int timeoutUS)
	{
		if (!IsConnected())
		{
			return -1;
		}
		fd_set set;
		FD_ZERO(&set);
		FD_SET(fileHandle, &set);
		struct timeval tvTimeout;
		tvTimeout.tv_sec = timeoutS;
		tvTimeout.tv_usec = timeoutUS;

		ssize_t ret = TEMP_FAILURE_RETRY(select(FD_SETSIZE, &set, NULL, NULL, &tvTimeout));
		if (ret <= 0)
		{
			return -1;
		}
		ret = read(fileHandle, data, maxData);
		if (ret <= 0)
		{
			return -1;
		}
		return ret;
	}

	bool Serial::ReceiveExact(std::string& data, const size_t length, const unsigned int timeoutS, const unsigned int timeoutUS)
	{
		size_t startSize = data.length();
		size_t endSize = startSize + length;
		while (endSize > data.length())
		{
			bool ret = Receive(data, endSize - data.length(), timeoutS, timeoutUS);
			if (ret == false)
			{
				return false;
			}
		}
		return true;
	}

	ssize_t Serial::ReceiveExact(unsigned char* data, const size_t length, const unsigned int timeoutS, const unsigned int timeoutUS)
	{
		size_t actualSize = 0;
		size_t endSize = length;
		while (actualSize < endSize)
		{
			ssize_t ret = Receive(data + actualSize, endSize - actualSize, timeoutS, timeoutUS);
			if (ret <= 0)
			{
				return actualSize;
			}
			actualSize += ret;
		}
		return actualSize;
	}
}
