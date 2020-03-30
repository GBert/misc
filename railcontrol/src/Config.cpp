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

#include <fstream>
#include <istream> // std::ws
#include <sstream>

#include "Config.h"
#include "Logger/Logger.h"
#include "Utils/Utils.h"

using std::map;
using std::string;

Config::Config(const std::string& fileName)
{
	Logger::Logger* logger = Logger::LoggerServer::Instance().GetLogger("Config");
	// read config values
	logger->Info(Languages::TextReadingConfigFile, fileName);

	std::ifstream configFile;
	configFile.open(fileName);
	if (!configFile.is_open())
	{
		logger->Warning(Languages::TextUnableToOpenFile, fileName);
		return;
	}

	for (string line; std::getline(configFile, line); )
	{
		std::istringstream iss(line);
		string configKey;
		string eq;
		string configValue;

		if (configKey[0] == '#')
		{
			continue;
		}

		bool error = (!(iss >> configKey >> eq >> configValue >> std::ws) || eq != "=" || iss.get() != EOF);
		if (error == true)
		{
			continue;
		}

		config[configKey] = configValue;
		logger->Info(Languages::TextParameterFoundInConfigFile, configKey, configValue);
	}
	configFile.close();
}

const string& Config::getValue(const string& key, const string& defaultValue)
{
	if (config.count(key) != 1)
	{
		return defaultValue;
	}
	return config[key];
}

int Config::getValue(const string& key, const int& defaultValue)
{
	if (config.count(key) != 1)
	{
		return defaultValue;
	}
	return Utils::Utils::StringToInteger(config[key], defaultValue);
}

