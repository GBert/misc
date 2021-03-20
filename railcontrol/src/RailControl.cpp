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
#include "Version.h"
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
	std::map<std::string,char> argumentMap;
	argumentMap["config"] = 'c';
	argumentMap["daemonize"] = 'd';
	argumentMap["logfile"] = 'l';
	argumentMap["help"] = 'h';
	argumentMap["silent"] = 's';
	ArgumentHandler argumentHandler(argc, argv, argumentMap, 'c');

	const bool help = argumentHandler.GetArgumentBool('h');
	if (help == true)
	{
		std::cout << "Usage: " << argv[0] << " <options>" << std::endl;
		std::cout << "Options:" << std::endl;
		std::cout << "-c --config=ConfigFile   Read config file with file name ConfigFile (default ConfigFile: railcontrol.conf)" << std::endl;
		std::cout << "-d --daemonize           Daemonize RailControl. Implies -s" << std::endl;
		std::cout << "-l --logfile=LogFile     Write a logfile to file LogFile (default LogFile: railcontrol.log)" << std::endl;
		std::cout << "-h --help                Show this help" << std::endl;
		std::cout << "-s --silent              Omit writing to console" << std::endl;
		return EXIT_SUCCESS;
	}

	const bool daemonize = argumentHandler.GetArgumentBool('d');
	if (daemonize == true)
	{
		pid_t pid = fork();
		if (pid > 0)
		{
			return EXIT_SUCCESS;
		}
		close(STDERR_FILENO);
		close(STDOUT_FILENO);
		close(STDIN_FILENO);
	}

	stopSignalCounter = 0;
	signal(SIGINT, stopRailControlSignal);
	signal(SIGTERM, stopRailControlSignal);

	const string RailControl = "RailControl";
	Utils::Utils::SetThreadName(RailControl);

	runRailcontrol = true;
	Logger::Logger* logger = Logger::Logger::GetLogger("Main");

	const bool silent = daemonize || argumentHandler.GetArgumentBool('s');
	if (silent == false)
	{
		logger->AddConsoleLogger();
	}

	const bool fileLogger = argumentHandler.GetArgumentBool('l');
	if (fileLogger == true)
	{
		const string logFileName = argumentHandler.GetArgumentString('l', "railcontrol.log");
		logger->AddFileLogger(logFileName);
	}

	logger->Info(Languages::TextStarting, RailControl);
	logger->Info(Languages::TextVersion, GetVersionInfoRailControlVersion());
	logger->Info(Languages::TextCompileDate, Utils::Utils::TimestampToDate(GetVersionInfoCompileTimestamp()));
	logger->Info(Languages::TextGitHash, GetVersionInfoGitHash());
	logger->Info(Languages::TextGitDate, Utils::Utils::TimestampToDate(GetVersionInfoGitTimestamp()));

	const string configFileDefaultName("railcontrol.conf");
	const string configFileName = argumentHandler.GetArgumentString('c', configFileDefaultName);
	if (configFileName.compare(configFileDefaultName) == 0 && !Utils::Utils::FileExists(configFileDefaultName))
	{
		Utils::Utils::CopyFile(logger, "railcontrol.conf.dist", configFileDefaultName);
	}
	Config config(configFileName);

	Manager m(config);

	// wait for q followed by \n or SIGINT or SIGTERM
	char input = 0;

	do
	{
		if (silent == true)
		{
			Utils::Utils::SleepForSeconds(1);
		}
		else
		{
			struct timeval tv;
			tv.tv_sec = 1;
			tv.tv_usec = 0;
			fd_set set;
			FD_ZERO(&set);
			FD_SET(STDIN_FILENO, &set);
			int ret = TEMP_FAILURE_RETRY(select(FD_SETSIZE, &set, NULL, NULL, &tv));
			if (ret > 0 && FD_ISSET(STDIN_FILENO, &set))
			{
				__attribute__((unused)) size_t unused = read(STDIN_FILENO, &input, sizeof(input));
			}
		}
	} while (input != 'q' && runRailcontrol);

	logger->Info(Languages::TextStoppingRailControl);
	return EXIT_SUCCESS;
}

