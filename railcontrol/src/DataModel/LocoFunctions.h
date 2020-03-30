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

#include "DataTypes.h"
#include "DataModel/Serializable.h"

namespace DataModel
{
	class LocoFunctions : Serializable
	{
		public:
			LocoFunctions()
			:	count(1),
			 	states{0}
			{
			}

			LocoFunctions(const std::string& serialized)
			: LocoFunctions()
			{
				Deserialize(serialized);
			}

			void SetFunction(const function_t nr, const bool state)
			{
				if (nr >= MaxCount)
				{
					return;
				}
				states[nr] = state;
			}

			bool GetFunction(const function_t nr) const
			{
				if (nr >= MaxCount)
				{
					return false;
				}
				bool out = states[nr];
				return out;
			}

			std::vector<bool> GetFunctions() const
			{
				std::vector<bool> out;
				for (function_t i = 0; i < count; ++i)
				{
					out.push_back(states[i]);
				}
				return out;
			}

			void SetNrOfFunctions(const function_t nr)
			{
				// externally we count the functions additional to F0
				// internally we count all the functions including F0
				count = nr + 1;
			}

			function_t GetNrOfFunctions() const
			{
				return count - 1;
			}

			std::string Serialize() const override
			{
				std::string out;
				for (function_t i = 0; i < count; ++i)
				{
					out += (states[i] ? "1" : "0");
				}
				return out;
			}

			bool Deserialize(const std::string& serialized) override
			{
				count = serialized.size();
				if (count > MaxCount)
				{
					count = MaxCount;
				}
				for (function_t i = 0; i < count; ++i)
				{
					states[i] = serialized[i] == '1';
				}
				return true;
			}

			static const function_t MaxFunctions = 32;
			enum locoFunctionTypes : unsigned char
			{
				Off = 0,
				On,
				Pulse
			};

		private:
			static const function_t MaxCount = MaxFunctions + 1; // f0 - f32 = 33
			function_t count;
			bool states[MaxCount];
	};
} // namespace DataModel
