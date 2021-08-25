/*
RailControl - Model Railway Control Software

Copyright (c) 2017-2021 Dominik (Teddy) Mahrer - www.railcontrol.org

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

#include <fstream>
#include <string>

#include "Logger/LoggerClient.h"
#include "Utils/Utils.h"

namespace Logger
{
	class LoggerClientFile : public LoggerClient
	{
		public:
			LoggerClientFile(const std::string& logFileName)
			:	logFileName(logFileName)
			{
				logFile.open(logFileName, std::fstream::out | std::fstream::app);
			}

			~LoggerClientFile()
			{
				Utils::Utils::RenameFile(nullptr, logFileName, logFileName + "." + std::to_string(time(0)));
				if (logFile.is_open() == false)
				{
					return;
				}
				logFile.close();
			}

			void Send(const std::string& s) override
			{
				if (logFile.is_open() == false)
				{
					return;
				}
				logFile << s << std::flush;
			}

		private:
			const std::string logFileName;
			std::ofstream logFile;
	};
}
