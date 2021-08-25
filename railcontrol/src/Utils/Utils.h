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

#include <chrono>
#include <climits>
#include <deque>
#include <future>
#include <map>
#include <string>
#include <sys/stat.h>
#include <thread>
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
			static void SplitString(const std::string& str, const std::string& delimiter, std::deque<std::string>& list);
			static void SplitString(const std::string& input, const std::string& delimiter, std::string& first, std::string& second);
			static std::string StringBeforeDelimiter(const std::string& input, const std::string& delimiter);
			static std::string UrlDecode(const std::string& value);
			static std::string UrlEncode(const std::string& value);

			static inline bool IsMapEntrySet(const std::map<std::string,std::string>& map, const std::string& key)
			{
				return map.count(key) != 0;
			}

			static const std::string& GetStringMapEntry(const std::map<std::string,std::string>& map, const std::string& key, const std::string& defaultValue = "");
			static int GetIntegerMapEntry(const std::map<std::string,std::string>& map, const std::string& key, const int defaultValue = 0);
			static bool GetBoolMapEntry(const std::map<std::string,std::string>& map, const std::string& key, const bool defaultValue = false);
			static std::string ToStringWithLeadingZeros(const unsigned int number, const unsigned char chars);

			static inline int StringToInteger(const std::string& value)
			{
				return StringToInteger(value, 0, INT_MAX);
			}

			static int StringToInteger(const std::string& value, const int defaultValue);
			static int StringToInteger(const std::string& value, const int min, const int max);
			static long HexToInteger(const std::string& value, const long defaultValue = 0);
			static signed char HexToChar(signed char c);
			static bool StringToBool(const std::string& value, const bool defaultValue = false);

			static void StringToUpper(std::string& s)
			{
				for (auto& c : s)
				{
					c = toupper(c);
				}
			}

			static void IntToDataBigEndian(const uint32_t i, unsigned char* buffer);
			static uint32_t DataBigEndianToInt(const unsigned char* buffer);
			static void ShortToDataBigEndian(const uint16_t i, unsigned char* buffer);
			static uint16_t DataBigEndianToShort(const unsigned char* buffer);
			static void IntToDataLittleEndian(const uint32_t i, unsigned char* buffer);
			static uint32_t DataLittleEndianToInt(const unsigned char* buffer);
			static void ShortToDataLittleEndian(const uint16_t i, unsigned char* buffer);
			static uint16_t DataLittleEndianToShort(const unsigned char* buffer);
			static std::string IntegerToBCD(const unsigned int input);
			static std::string IntegerToHex(const unsigned int input, const unsigned int size = 1);

			static inline bool FileExists(const std::string& name)
			{
				struct stat buffer;
				return (stat(name.c_str(), &buffer) == 0);
			}

			static void CopyFile(Logger::Logger* logger, const std::string& from, const std::string& to);
			static void RenameFile(Logger::Logger* logger, const std::string& from, const std::string& to);

			static inline void SetThreadName(const std::string& name)
			{
				SetThreadName(name.c_str());
			}

			static inline void SetThreadName(__attribute__((unused)) const char* name)
			{
#if __GLIBC__ > 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ >= 12)
				pthread_setname_np(pthread_self(), name);
#endif
			}
			static void SetMinThreadPriority();
			static std::string TimestampToDate(const time_t timestamp);

			static inline void SleepForSeconds(unsigned int seconds)
			{
				std::this_thread::sleep_for(std::chrono::seconds(seconds));
			}

			static inline void SleepForMilliseconds(unsigned int milliSeconds)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(milliSeconds));
			}

#ifdef __CYGWIN__
		private:
			static bool GetFilesInDir(std::vector<std::string>& filesFound, const std::string& path, const std::string& prefix);
		public:
			static bool GetComPorts(std::vector<unsigned char>& comPorts);
#endif

			static unsigned int RandInt();

			static inline void Copy8Bytes(const unsigned char* const from, unsigned char* const to)
			{
				*(reinterpret_cast<uint64_t*>(to)) = *(reinterpret_cast<const uint64_t*>(from));
			}

			static bool HostResolves(const std::string& host);
	};
}
