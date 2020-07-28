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

#include <deque>
#include <string>

#include "DataModel/LocoFunctions.h"
#include "Utils/Utils.h"

namespace DataModel
{
	LocoFunctions::LocoFunctions()
	{
		for (LocoFunctionNr nr = 0; nr < MaxCount; ++nr)
		{
			entries[nr].nr = nr;
			entries[nr].type = LocoFunctionTypeNone;
			entries[nr].icon = LocoFunctionIconNone;
			entries[nr].timer = 0;
		}
	}

	std::string LocoFunctions::Serialize() const
	{
		std::string out;
		for (LocoFunctionNr nr = 0; nr < MaxCount; ++nr)
		{
			if (entries[nr].type == LocoFunctionTypeNone)
			{
				continue;
			}
			out += "f" + std::to_string(static_cast<unsigned int>(nr));
			out += ":" + std::to_string(static_cast<unsigned int>(entries[nr].state));
			out += ":" + std::to_string(static_cast<unsigned int>(entries[nr].type));
			out += ":" + std::to_string(static_cast<unsigned int>(entries[nr].icon));
			if (entries[nr].type != LocoFunctionTypeTimer)
			{
				continue;
			}
			out += ":" + std::to_string(static_cast<unsigned int>(entries[nr].icon));
		}
		return out;
	}

	bool LocoFunctions::Deserialize(const std::string& serialized)
	{
		size_t count = serialized.size();
		if (count == 0 || serialized[0] == 'f')
		{
			DeserializeNew(serialized);
			return true;
		}
		DeserializeOld(serialized);
		return true;
	}

	bool LocoFunctions::DeserializeNew(__attribute__((unused))  const std::string& serialized)
	{
		std::deque<std::string> functionsSerialized;
		Utils::Utils::SplitString(serialized, "f", functionsSerialized);
		for (std::string& functionSerialized : functionsSerialized)
		{
			if (functionSerialized.size() == 0)
			{
				continue;
			}
			std::deque<std::string> functionTexts;
			Utils::Utils::SplitString(functionSerialized, ":", functionTexts);
			const size_t nrOfTexts = functionTexts.size();
			if (nrOfTexts != 4 && nrOfTexts != 5)
			{
				continue;
			}
			LocoFunctionNr nr = Utils::Utils::StringToInteger(functionTexts.front());
			functionTexts.pop_front();
			entries[nr].state = static_cast<LocoFunctionState>(Utils::Utils::StringToInteger(functionTexts.front(), LocoFunctionStateOff));
			functionTexts.pop_front();
			entries[nr].type = static_cast<LocoFunctionType>(Utils::Utils::StringToInteger(functionTexts.front(), LocoFunctionTypePermanent));
			functionTexts.pop_front();
			entries[nr].icon = static_cast<LocoFunctionIcon>(Utils::Utils::StringToInteger(functionTexts.front(), LocoFunctionIconDefault));
			functionTexts.pop_front();
			if (nrOfTexts == 4)
			{
				entries[nr].timer = 0;
				continue;
			}
			entries[nr].timer = static_cast<LocoFunctionTimer>(Utils::Utils::StringToInteger(functionTexts.front(), 0));
			functionTexts.pop_front();
		}
		return true;
	}

	// FIXME: remove later
	bool LocoFunctions::DeserializeOld(const std::string& serialized)
	{
		size_t count = serialized.size();
		if (count > MaxCount)
		{
			count = MaxCount;
		}
		for (LocoFunctionNr nr = 0; nr < MaxCount; ++nr)
		{
			if (nr >= count)
			{
				entries[nr].state = LocoFunctionStateOff;
				entries[nr].type = LocoFunctionTypeNone;
				entries[nr].icon = LocoFunctionIconNone;
				entries[nr].timer = 0;
				continue;
			}
			switch (serialized[nr])
			{
				case '1':
					entries[nr].state = LocoFunctionStateOn;
					break;

				default:
					entries[nr].state = LocoFunctionStateOff;
					break;
			}
			entries[nr].type = LocoFunctionTypePermanent;
			entries[nr].icon = LocoFunctionIconDefault;
			entries[nr].timer = 0;
		}
		return true;
	}

	std::string LocoFunctions::GetLocoFunctionIcon(const LocoFunctionNr nr, const LocoFunctionIcon icon)
	{
		switch (icon)
		{
			case LocoFunctionIconNone:
				return "<svg width=\"36\" height=\"36\" />";

			default:
				return "<svg width=\"36\" height=\"36\">"
					"<text x=\"8\" y=\"24\" fill=\"black\" font-size=\"12\">F" + std::to_string(nr) + "</text>"
					"</svg>";

			case LocoFunctionIconShuntingMode:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"5,24 5.2,21.9 5.7,19.9 6.6,18 7.8,16.3 9.3,14.8 11,13.6 12.9,12.7 14.9,12.2 17,12 19.1,12.2 21.1,12.7 23,13.6 24.7,14.8 26.2,16.3 27.4,18 28.3,19.9 28.8,21.9 29,24\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<circle r=\"3\" cx=\"11\" cy=\"24\" fill=\"black\" />"
					"<circle r=\"3\" cx=\"23\" cy=\"24\" fill=\"black\" />"
					"<circle r=\"3\" cx=\"29\" cy=\"20\" fill=\"black\" />"
					"</svg>";

			case LocoFunctionIconInertia:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"8,16 16,16 16,10 20,10 20,16 28,16 28,28 8,28\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"18,7 18.9,7 19.7,7.2 20.5,7.4 21.2,7.7 21.8,8.1 22.3,8.5 22.7,9 22.9,9.5 23,10 22.9,10.5 22.7,11 22.3,11.5 21.8,11.9 21.2,12.3 20.5,12.6 19.7,12.8 18.9,13 18,13 17.1,13 16.3,12.8 15.5,12.6 14.8,12.3 14.2,11.9 13.7,11.5 13.3,11 13.1,10.5 13,10 13.1,9.5 13.3,9 13.7,8.5 14.2,8.1 14.8,7.7 15.5,7.4 16.3,7.2 17.1,7\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<text x=\"12\" y=\"25\" fill=\"white\" font-size=\"10\">kg</text></svg>";

			case LocoFunctionIconLight:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"15.5,22.3 14.8,21.8 14.2,21.2 13.7,20.5 13.3,19.7 13.1,18.9 13,18 13.1,17.1 13.3,16.3 13.7,15.5 14.2,14.8 14.8,14.2 15.5,13.7 16.3,13.3 17.1,13.1 18,13 18.9,13.1 19.7,13.3 20.5,13.7 21.2,14.2 21.8,14.8 22.3,15.5 22.7,16.3 22.9,17.1 23,18 22.9,18.9 22.7,19.7 22.3,20.5 21.8,21.2 21.2,21.8 20.5,22.3\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"15,23 21,23 21,30 18,32 15,30\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"10.2,22 5,25.5\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"9,18 3,18\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"10.2,13.5 5,10.5\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"13.5,10.2 10.5,5\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"18,9 18,3\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"22.5,10.2 25.5,5\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"25.8,13.5 31,10.5\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"27,18 33,18\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"25.8,22.5 31,25.5\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"</svg>";

			case LocoFunctionIconHeadlightLowBeamForward:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"16,28 12,28 10.3,27.8 8.6,27.4 7,26.7 5.6,25.7 4.3,24.4 3.3,23 2.6,21.4 2.2,19.7 2,18 2.2,16.3 2.6,14.6 3.3,13 4.3,11.6 5.6,10.3 7,9.3 8.6,8.6 10.3,8.2 12,8 16,8\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"17,28 17,8 19,8 19,28\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"23,11 35,16\" stroke=\"black\" stroke-width=\"2\" fill=\"black\" class=\"button_on\"/>"
					"<polyline points=\"23,18 35,23\" stroke=\"black\" stroke-width=\"2\" fill=\"black\" class=\"button_on\"/>"
					"<polyline points=\"23,25 35,30\" stroke=\"black\" stroke-width=\"2\" fill=\"black\" class=\"button_on\"/>"
					"</svg>";

			case LocoFunctionIconHeadlightLowBeamReverse:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"20,8 24,8 25.7,8.2 27.4,8.6 29,9.3 30.4,10.3 31.7,11.6 32.7,13 33.4,14.6 33.8,16.3 34,18 33.8,19.7 33.4,21.4 32.7,23 31.7,24.4 30.4,25.7 29,26.7 27.4,27.4 25.7,27.8 24,28 20,28\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"17,28 17,8 19,8 19,28\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"13,11 1,16\" stroke=\"black\" stroke-width=\"2\" fill=\"black\" class=\"button_on\"/>"
					"<polyline points=\"13,18 1,23\" stroke=\"black\" stroke-width=\"2\" fill=\"black\" class=\"button_on\"/>"
					"<polyline points=\"13,25 1,30\" stroke=\"black\" stroke-width=\"2\" fill=\"black\" class=\"button_on\"/>"
					"</svg>";

			case LocoFunctionIconHeadlightHighBeamForward:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"16,28 12,28 10.3,27.8 8.6,27.4 7,26.7 5.6,25.7 4.3,24.4 3.3,23 2.6,21.4 2.2,19.7 2,18 2.2,16.3 2.6,14.6 3.3,13 4.3,11.6 5.6,10.3 7,9.3 8.6,8.6 10.3,8.2 12,8 16,8\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"17,28 17,8 19,8 19,28\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"23,10.5 35,10.5\" stroke=\"black\" stroke-width=\"2\" fill=\"black\" class=\"button_on\"/>"
					"<polyline points=\"23,15.5 35,15.5\" stroke=\"black\" stroke-width=\"2\" fill=\"black\" class=\"button_on\"/>"
					"<polyline points=\"23,20.5 35,20.5\" stroke=\"black\" stroke-width=\"2\" fill=\"black\" class=\"button_on\"/>"
					"<polyline points=\"23,25.5 35,25.5\" stroke=\"black\" stroke-width=\"2\" fill=\"black\" class=\"button_on\"/>"
					"</svg>";

			case LocoFunctionIconHeadlightHighBeamReverse:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"20,8 24,8 25.7,8.2 27.4,8.6 29,9.3 30.4,10.3 31.7,11.6 32.7,13 33.4,14.6 33.8,16.3 34,18 33.8,19.7 33.4,21.4 32.7,23 31.7,24.4 30.4,25.7 29,26.7 27.4,27.4 25.7,27.8 24,28 20,28\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"17,28 17,8 19,8 19,28\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"1,10.5 15,10.5\" stroke=\"black\" stroke-width=\"2\" fill=\"black\" class=\"button_on\"/>"
					"<polyline points=\"1,15.5 15,15.5\" stroke=\"black\" stroke-width=\"2\" fill=\"black\" class=\"button_on\"/>"
					"<polyline points=\"1,20.5 15,20.5\" stroke=\"black\" stroke-width=\"2\" fill=\"black\" class=\"button_on\"/>"
					"<polyline points=\"1,25.5 15,25.5\" stroke=\"black\" stroke-width=\"2\" fill=\"black\" class=\"button_on\"/>"
					"</svg>";

			case LocoFunctionIconBacklightForward:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"18,23 17.1,22.9 16.3,22.7 15.5,22.3 14.8,21.8 14.2,21.2 13.7,20.5 13.3,19.7 13.1,18.9 13,18 13.1,17.1 13.3,16.3 13.7,15.5 14.2,14.8 14.8,14.2 15.5,13.7 16.3,13.3 17.1,13.1 18,13\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"19,10 22,10 22,26 19,26\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"13.5,25.8 10.5,31\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"10.2,22 5,25.5\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"9,18 3,18\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"10.2,13.5 5,10.5\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"13.5,10.2 10.5,5\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"</svg>";

			case LocoFunctionIconBacklightReverse:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"18,13 18.9,13.1 19.7,13.3 20.5,13.7 21.2,14.2 21.8,14.8 22.3,15.5 22.7,16.3 22.9,17.1 23,18 22.9,18.9 22.7,19.7 22.3,20.5 21.8,21.2 21.2,21.8 20.5,22.3 19.7,22.7 18.9,22.9 18,23\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"14,10 17,10 17,26 14,26\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"22.5,10.2 25.5,5\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"25.8,13.5 31,10.5\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"27,18 33,18\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"25.8,22.5 31,25.5\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"22.5,25.8 25.5,31\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"</svg>";

			case LocoFunctionIconSoundGeneral:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"8,15 10,15 10,21 8,21\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"11,15 18,8 18,28 11,21\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"24.1,12.9 24.9,14 25.5,15.3 25.9,16.6 26,18 25.9,19.4 25.5,20.7 24.9,22 24.1,23.1\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound1\"/>"
					"<polyline points=\"27.2,10.3 28.4,12 29.3,13.9 29.8,15.9 30,18 29.8,20.1 29.3,22.1 28.4,24 27.2,25.7\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"30.3,7.7 31.9,10 33,12.5 33.8,15.2 34,18 33.8,20.8 33,23.5 31.9,26 30.3,28.3\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"</svg>";
		}
	}
} // namespace DataModel
