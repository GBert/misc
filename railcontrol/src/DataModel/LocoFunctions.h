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
	class LocoFunctions : private Serializable
	{
		public:
			enum FunctionState : unsigned char
			{
				FunctionStateOff = 0,
				FunctionStateOn,
				FunctionStatePulse
			};

			LocoFunctions()
			:	count(1),
			 	states{FunctionStateOff}
			{
			}

			LocoFunctions(const std::string& serialized)
			: LocoFunctions()
			{
				Deserialize(serialized);
			}

			void SetFunction(const Function nr, const FunctionState state)
			{
				if (nr >= MaxCount)
				{
					return;
				}
				states[nr] = state;
			}

			FunctionState GetFunction(const Function nr) const
			{
				if (nr >= MaxCount)
				{
					return FunctionStateOff;
				}
				FunctionState out = states[nr];
				return out;
			}

			std::vector<FunctionState> GetFunctions() const
			{
				std::vector<FunctionState> out;
				for (Function i = 0; i < count; ++i)
				{
					out.push_back(states[i]);
				}
				return out;
			}

			void SetNrOfFunctions(const Function nr)
			{
				// externally we count the functions additional to F0
				// internally we count all the functions including F0
				count = nr + 1;
			}

			Function GetNrOfFunctions() const
			{
				return count - 1;
			}

			std::string Serialize() const override
			{
				std::string out;
				for (Function i = 0; i < count; ++i)
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
				for (Function i = 0; i < count; ++i)
				{
					switch(serialized[i])
					{
						case '1':
							states[i] = FunctionStateOn;
							break;

						case '0':
						default:
							states[i] = FunctionStateOff;
							break;
					}
				}
				return true;
			}

			static const Function MaxFunctions = 32;

		private:
			static const Function MaxCount = MaxFunctions + 1; // f0 - f32 = 33
			Function count;
			FunctionState states[MaxCount];
	};
} // namespace DataModel
