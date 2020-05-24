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

#include <arpa/inet.h>
#include <cstdarg>    // va_* in xlog
#include <cstdio>     // printf
#include <cstdlib>    // exit(0);
#include <cstring>    // memset
#include <dirent.h>
#include <fstream>
#include <iostream>   // cout
#include <sstream>
#include <string>
#include <sys/time.h> // gettimeofday

#include "Languages.h"
#include "Logger/Logger.h"
#include "Network/Select.h"
#include "Utils/Utils.h"

using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::to_string;
using std::vector;

namespace Utils
{
	void Utils::ReplaceString(std::string& str, const std::string& from, const std::string& to)
	{
		while (true)
		{
			size_t start_pos = str.find(from);
			if (start_pos == string::npos)
			{
				return;
			}
			str.replace(start_pos, from.length(), to);
		}
	}

	void Utils::SplitString(const std::string& str_in, const std::string &delimiter, std::vector<string> &list)
	{
		size_t length_delim = delimiter.length();
		string str(str_in);
		while (true)
		{
			size_t pos = str.find(delimiter);
			list.push_back(str.substr(0, pos));
			if (pos == string::npos)
			{
				return;
			}
			str = string(str.substr(pos + length_delim, string::npos));
		}
	}

	const std::string& Utils::GetStringMapEntry(const std::map<std::string, std::string>& map, const std::string& key, const std::string& defaultValue)
	{
		if (map.count(key) == 0)
		{
			return defaultValue;
		}
		return map.at(key);
	}

	int Utils::GetIntegerMapEntry(const std::map<std::string, std::string>& map, const std::string& key, const int defaultValue)
	{
		if (map.count(key) == 0)
		{
			return defaultValue;
		}
		return Utils::Utils::StringToInteger(map.at(key), defaultValue);
	}

	bool Utils::GetBoolMapEntry(const std::map<std::string, std::string>& map, const std::string& key, const bool defaultValue)
	{
		if (map.count(key) == 0)
		{
			return defaultValue;
		}
		string value = map.at(key);
		return (value.compare("true") == 0 || value.compare("on") == 0 || value.compare("1") == 0 || key.compare(value) == 0);
	}

	string Utils::ToStringWithLeadingZeros(const unsigned int number, const unsigned char chars)
	{
		string out = to_string(number);
		while (out.length() < chars)
		{
			out.insert(0, "0");
		}
		return out;
	}

	int Utils::StringToInteger(const std::string& value, const int defaultValue, const bool hex)
	{
		size_t valueSize = value.length();
		if (valueSize == 0)
		{
			return defaultValue;
		}

		char* end;
		const char* start = value.c_str();
		long longValue = std::strtol(start, &end, hex ? 16 : 10);
		if (errno == ERANGE || start == end)
		{
			return defaultValue;
		}
		if (longValue > INT_MAX || longValue < INT_MIN)
		{
			return defaultValue;
		}
		return static_cast<int>(longValue);
	}

	int Utils::StringToInteger(const std::string& value, const int min, const int max)
	{
		int intValue = StringToInteger(value, min);

		if (intValue < min)
		{
			return min;
		}

		if (intValue > max)
		{
			return max;
		}

		return intValue;
	}

	bool Utils::StringToBool(const std::string& value)
	{

		int intValue = StringToInteger(value);
		return intValue != 0;
	}

	void Utils::IntToDataBigEndian(const uint32_t i, unsigned char* buffer)
	{
		buffer[0] = (i >> 24);
		buffer[1] = ((i >> 16) & 0xFF);
		buffer[2] = ((i >> 8) & 0xFF);
		buffer[3] = (i & 0xFF);
	}

	uint32_t Utils::DataBigEndianToInt(const unsigned char* buffer)
	{
		uint32_t i = buffer[0];
		i <<= 8;
		i |= buffer[1];
		i <<= 8;
		i |= buffer[2];
		i <<= 8;
		i |= buffer[3];
		return i;
	}

	void Utils::ShortToDataBigEndian(const uint16_t i, unsigned char* buffer)
	{
		buffer[0] = (i >> 8);
		buffer[1] = (i & 0xFF);
	}

	uint16_t Utils::DataBigEndianToShort(const unsigned char* buffer)
	{
		uint16_t i = buffer[0];
		i <<= 8;
		i |= buffer[1];
		return i;
	}

	void Utils::IntToDataLittleEndian(const uint32_t i, unsigned char* buffer)
	{
		buffer[0] = (i & 0xFF);
		buffer[1] = ((i >> 8) & 0xFF);
		buffer[2] = ((i >> 16) & 0xFF);
		buffer[3] = (i >> 24);
	}

	uint32_t Utils::DataLittleEndianToInt(const unsigned char* buffer)
	{
		return buffer[0]
			+ (buffer[1] << 8)
			+ (buffer[2] << 16)
			+ (buffer[3] << 24);
	}

	void Utils::ShortToDataLittleEndian(const uint16_t i, unsigned char* buffer)
	{
		buffer[0] = (i & 0xFF);
		buffer[1] = (i >> 8);
	}

	uint16_t Utils::DataLittleEndianToShort(const unsigned char* buffer)
	{
		return buffer[0]
			+ (buffer[1] << 8);
	}

	std::string Utils::IntegerToBCD(const unsigned int input)
	{
		unsigned char zero = (input >> 4);
		zero &= 0x0000000F;
		zero += '0';
		unsigned char one = input;
		one &= 0x0000000F;
		one += '0';
		std::string output;
		output.append(1, zero);
		output.append(1, one);
		return output;
	}

	void Utils::CopyFile(Logger::Logger* logger, const std::string& from, const std::string& to)
	{
		logger->Info(Languages::TextCopyingFromTo, from, to);
		std::ifstream source(from, std::ios::binary);
		std::ofstream destination(to, std::ios::binary);
		destination << source.rdbuf();
		source.close();
		destination.close();
	}

	void Utils::RenameFile(Logger::Logger* logger, const std::string& from, const std::string& to)
	{
		if (logger != nullptr)
		{
			logger->Info(Languages::TextRenamingFromTo, from, to);
		}
		std::rename(from.c_str(), to.c_str());
	}

	void Utils::SetMinThreadPriority()
	{
		sched_param param;
		int policy;
		pthread_t self = pthread_self();
		pthread_getschedparam(self, &policy, &param);
		param.sched_priority = sched_get_priority_min(policy);
		pthread_setschedparam(self, policy, &param);
	}

	std::string Utils::TimestampToDate(const time_t timestamp)
	{
		struct tm *tm = localtime(&timestamp);
		char date[20];
		strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", tm);
		return date;
	}

#ifdef __CYGWIN__
	bool Utils::GetFilesInDir(vector<string>& filesFound, const string& path, const string& prefix)
	{
		bool ret = false;
		size_t prefixLength = prefix.length();
		DIR* dirp = opendir(path.c_str());
		struct dirent* dp;
		while ((dp = readdir(dirp)) != nullptr)
		{
			string file(dp->d_name);
			string prefixRead = file.substr(0, prefixLength);
			if (prefix.compare(prefixRead) != 0)
			{
				continue;
			}
			filesFound.push_back(file);
			ret = true;
		}
		closedir(dirp);
		return ret;
	}

	bool Utils::GetComPorts(std::vector<unsigned char>& comPorts)
	{
		vector<string> filesFound;
		bool ret = GetFilesInDir(filesFound, "/dev/", "ttyS");
		if (ret == false)
		{
			return false;
		}
		for (auto file : filesFound)
		{
			string comPortString = file.substr(4);
			unsigned char comPort = StringToInteger(comPortString);
			comPorts.push_back(comPort);
		}
		return true;
	}
#endif
}
