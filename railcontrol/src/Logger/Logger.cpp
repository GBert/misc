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

#include <iomanip>
#include <sstream>
#include <sys/time.h> // gettimeofday

#include "Logger/Logger.h"

using std::string;

namespace Logger
{
	Logger::Level Logger::logLevel = Logger::LevelInfo;

	string Logger::DateTime()
	{
		char buffer[27];
		struct timeval timestamp;
		gettimeofday(&timestamp, NULL);
		struct tm tm;
		gmtime_r(&timestamp.tv_sec, &tm);
		strftime(buffer, sizeof(buffer), "%F %T.", &tm);
		snprintf(buffer + 20, sizeof(buffer) - 20, "%06li", timestamp.tv_usec);
		return string(buffer);
	}

	void Logger::Replace(string& workString,
		const unsigned char argument,
		const std::string& value)
	{
		std::string needle = "{" + std::to_string(argument) + "}";
		size_t pos = workString.find(needle);
		if (pos == std::string::npos)
		{
			return;
		}
		workString.replace(pos, needle.size(), value);
	}

	void Logger::AsciiPart(std::stringstream& output, const unsigned char* input, const size_t size)
	{
		output << "  ";
		if (size < 8)
		{
			output << "  ";
		}
		for (size_t index = size; index < 16; ++index)
		{
			output << "   ";
		}
		for (size_t index = 0; index < size; ++index)
		{
			if (index == 8)
			{
				output << " ";
			}
			if (input[index] >= 0x20 && input[index] < 127)
			{
				output << input[index];
			}
			else
			{
				output << ".";
			}
		}
	}

	void Logger::Hex(const unsigned char* input, const size_t size)
	{
		std::stringstream output;
		size_t index;
		for (index = 0; index < size; ++index)
		{
			if ((index & 0x0F) == 0)
			{
				output << "0x" << std::setfill('0') << std::setw(4) << std::hex << index << "   ";
			}

			output << std::setfill('0') << std::setw(2) << std::hex << static_cast<unsigned int>(input[index]) << " ";

			size_t next = index + 1;
			if ((next & 0x0F) == 0)
			{
				AsciiPart(output, input + index - 15, 16);
				Debug(output.str());
				output.str(std::string());
				if (next == size)
				{
					return;
				}
				continue;
			}
			if ((next & 0x07) == 0)
			{
				output << "  ";
			}
		}
		size_t reminder = (index & 0x0F);
		AsciiPart(output, input + index - reminder, reminder);
		Debug(output.str());
	}
}

