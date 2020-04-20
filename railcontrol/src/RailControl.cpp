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

#include <cstdio>		//printf
#include <cstdlib>		//exit(0);
#include <cstring>		//memset
#include <iostream>
#include <signal.h>
#include <sstream>
#include <unistd.h>		//close;
#include <vector>

#include "ArgumentHandler.h"
#include "Hardware/HardwareHandler.h"
#include "Languages.h"
#include "Logger/Logger.h"
#include "Manager.h"
#include "Network/Select.h"
#include "RailControl.h"
#include "Timestamp.h"
#include "Utils/Utils.h"

using std::vector;
using std::string;

static volatile unsigned int runRailcontrol;

static volatile unsigned char stopSignalCounter;
static const unsigned char maxStopSignalCounter = 3;

void stopRailControlSignal(int signo)
{
	Logger::Logger* logger = Logger::Logger::GetLogger("Main");
	logger->Info(Languages::TextStoppingRequestedBySignal, signo);
	runRailcontrol = false;
	if (++stopSignalCounter < maxStopSignalCounter)
	{
		return;
	}
	logger->Info(Languages::TextReceivedSignalKill, maxStopSignalCounter);
	exit(1);
}

void stopRailControlWebserver()
{
	Logger::Logger::GetLogger("Main")->Info(Languages::TextStoppingRequestedByWebClient);
	runRailcontrol = false;
}

int main (int argc, char* argv[])
{
	ArgumentHandler argumentHandler(argc, argv, 'c');
	const bool Help = argumentHandler.GetArgumentBool('h');
	if (Help == true)
	{
		std::cout << "Usage: " << argv[0] << " <options>" << std::endl;
		std::cout << "Options:" << std::endl;
		std::cout << "-d  Daemonize RailControl. Implies -s" << std::endl;
		std::cout << "-l  Omit writing a logfile" << std::endl;
		std::cout << "-h  Show this help" << std::endl;
		std::cout << "-s  Omit writing to console" << std::endl;
		return 0;
	}

	const bool Daemonize = argumentHandler.GetArgumentBool('d');
	if (Daemonize == true)
	{
		pid_t pid = fork();
		if (pid > 0)
		{
			std::cout << pid << std::endl;
			return 0;
		}
		close(STDERR_FILENO);
		close(STDOUT_FILENO);
		close(STDIN_FILENO);
	}

	stopSignalCounter = 0;
	signal(SIGINT, stopRailControlSignal);
	signal(SIGTERM, stopRailControlSignal);

	const string RailControl = "RailControl";
	const string LogFileName = "railcontrol.log";
	Utils::Utils::SetThreadName(RailControl);

	runRailcontrol = true;
	Logger::Logger* logger = Logger::Logger::GetLogger("Main");

	const bool Silent = argumentHandler.GetArgumentBool('s');
	if (Daemonize == false && Silent == false)
	{
		logger->AddConsoleLogger();
	}

	const bool OmitFileLogger = argumentHandler.GetArgumentBool('l');
	if (OmitFileLogger == false)
	{
		logger->AddFileLogger(LogFileName);
	}

	logger->Info(Languages::TextStarting, RailControl);
	logger->Info(Languages::TextVersion, Utils::Utils::TimestampToDate(GetCompileTime()));

	const string ConfigFileName = argumentHandler.GetArgumentString('c', "railcontrol.conf");
	Config config(ConfigFileName);

	Manager m(config);

	// wait for q followed by \n or SIGINT or SIGTERM
	char input = 0;

	do
	{
		if (Daemonize == true)
		{
			Utils::Utils::SleepForSeconds(1);
		}
		else
		{
			struct timeval tv;
			fd_set set;

			tv.tv_sec = 1;
			tv.tv_usec = 0;
			// Zero out the fd_set - make sure it's pristine
			FD_ZERO(&set);

			// Set the FD that we want to read
			FD_SET(STDIN_FILENO, &set); //STDIN_FILENO is 0

			// select takes the last file descriptor value + 1 in the fdset to check,
			// the fdset for reads, writes, and errors.  We are only passing in reads.
			// the last parameter is the timeout.  select will return if an FD is ready or
			// the timeout has occurred
			int ret = TEMP_FAILURE_RETRY(select(FD_SETSIZE, &set, NULL, NULL, &tv));

			// only read STDIN if there really is something to read
			if (ret > 0 && FD_ISSET(STDIN_FILENO, &set))
			{
				__attribute__((unused)) size_t unused = read(STDIN_FILENO, &input, sizeof(input));
			}
		}
	} while (input != 'q' && runRailcontrol);

	logger->Info(Languages::TextStoppingRailControl);
	Utils::Utils::RenameFile(logger, LogFileName, LogFileName + "." + std::to_string(time(0)));
	return 0;
}

