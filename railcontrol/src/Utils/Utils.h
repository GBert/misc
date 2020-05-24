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

#include <climits>
#include <future>
#include <map>
#include <string>
#include <vector>

namespace Logger
{
	class Logger;
}

namespace Utils
{
	class Utils
	{
		public:
			static void ReplaceString(std::string& str, const std::string& from, const std::string& to);
			static void SplitString(const std::string& str, const std::string& delimiter, std::vector<std::string>& list);
			static const std::string& GetStringMapEntry(const std::map<std::string,std::string>& map, const std::string& key, const std::string& defaultValue = "");
			static int GetIntegerMapEntry(const std::map<std::string,std::string>& map, const std::string& key, const int defaultValue = 0);
			static bool GetBoolMapEntry(const std::map<std::string,std::string>& map, const std::string& key, const bool defaultValue = false);
			static std::string ToStringWithLeadingZeros(const unsigned int number, const unsigned char chars);
			static int StringToInteger(const std::string& value) { return StringToInteger(value, 0, INT_MAX); }
			static int StringToInteger(const std::string& value, const int defaultValue, const bool hex = false);
			static int StringToInteger(const std::string& value, const int min, const int max);
			static bool StringToBool(const std::string& value);
			static void StringToUpper(std::string& s) { for (auto& c : s) c = toupper(c); }
			static void IntToDataBigEndian(const uint32_t i, unsigned char* buffer);
			static uint32_t DataBigEndianToInt(const unsigned char* buffer);
			static void ShortToDataBigEndian(const uint16_t i, unsigned char* buffer);
			static uint16_t DataBigEndianToShort(const unsigned char* buffer);
			static void IntToDataLittleEndian(const uint32_t i, unsigned char* buffer);
			static uint32_t DataLittleEndianToInt(const unsigned char* buffer);
			static void ShortToDataLittleEndian(const uint16_t i, unsigned char* buffer);
			static uint16_t DataLittleEndianToShort(const unsigned char* buffer);
			static std::string IntegerToBCD(const unsigned int input);
			static void CopyFile(Logger::Logger* logger, const std::string& from, const std::string& to);
			static void RenameFile(Logger::Logger* logger, const std::string& from, const std::string& to);
			static void SetThreadName(const std::string& name) { SetThreadName(name.c_str()); }
			static void SetThreadName(__attribute__((unused)) const char* name)
			{
#if __GLIBC__ > 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ >= 12)
				pthread_setname_np(pthread_self(), name);
#endif
			}
			static void SetMinThreadPriority();
			static std::string TimestampToDate(const time_t timestamp);
			static void SleepForSeconds(int seconds) { std::this_thread::sleep_for(std::chrono::seconds(seconds)); }
			static void SleepForMilliseconds(int milliSeconds) { std::this_thread::sleep_for(std::chrono::milliseconds(milliSeconds)); }
#ifdef __CYGWIN__
		private:
			static bool GetFilesInDir(std::vector<std::string>& filesFound, const std::string& path, const std::string& prefix);
		public:
			static bool GetComPorts(std::vector<unsigned char>& comPorts);
#endif
	};
}
