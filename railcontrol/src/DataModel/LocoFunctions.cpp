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

	// FIXME: remove later 2020-10-27
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
					"<text x=\"12\" y=\"25\" fill=\"white\" font-size=\"10\">kg</text>"
					"</svg>";

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

			case LocoFunctionIconShuntingLight:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"5,3 3.5,3.5 3,5 3,35 33,35 33,5 32.5,3.5 31,3 5,3\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"/>"
					"<polyline points=\"3,18 3,35 33,35 33,18\" stroke=\"black\" stroke-width=\"1\" fill=\"black\"/>"
					"<circle r=\"2\" cx=\"8\" cy=\"29\" fill=\"darkgray\" />"
					"<circle r=\"2\" cx=\"18\" cy=\"23\" fill=\"darkgray\" />"
					"<circle r=\"2\" cx=\"28\" cy=\"29\" fill=\"darkgray\" />"
					"<circle r=\"3\" cx=\"28\" cy=\"29\" fill=\"white\" class=\"button_on\" />"
					"</svg>";

			case LocoFunctionIconBlinkingLight:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"13,18 13.1,17.1 13.3,16.3 13.7,15.5 14.2,14.8 14.8,14.2 15.5,13.7 16.3,13.3 17.1,13.1 18,13 18.9,13.1 19.7,13.3 20.5,13.7 21.2,14.2 21.8,14.8 22.3,15.5 22.7,16.3 22.9,17.1 23,18 23,25 13,25\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"10,26 10,29 26,29 26,26\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"9,18 3,18\" stroke=\"black\" stroke-width=\"2\" class=\"button_on blinking\"/>"
					"<polyline points=\"10.2,13.5 5,10.5\" stroke=\"black\" stroke-width=\"2\" class=\"button_on blinking\"/>"
					"<polyline points=\"13.5,10.2 10.5,5\" stroke=\"black\" stroke-width=\"2\" class=\"button_on blinking\"/>"
					"<polyline points=\"18,9 18,3\" stroke=\"black\" stroke-width=\"2\" class=\"button_on blinking\"/>"
					"<polyline points=\"22.5,10.2 25.5,5\" stroke=\"black\" stroke-width=\"2\" class=\"button_on blinking\"/>"
					"<polyline points=\"25.8,13.5 31,10.5\" stroke=\"black\" stroke-width=\"2\" class=\"button_on blinking\"/>"
					"<polyline points=\"27,18 33,18\" stroke=\"black\" stroke-width=\"2\" class=\"button_on blinking\"/>"
					"</svg>";

			case LocoFunctionIconInteriorLight1:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"3,3 1.5,3.5 1,5 1,33 1.5,34.5 3,35 33,35 34.5,34.5 35,33 35,5 34.5,3.5 33,3 3,3\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"/>"
					"<polyline points=\"23,5 22.9,5.9 22.7,6.7 22.3,7.5 21.8,8.2 21.2,8.8 20.5,9.3 19.7,9.7 18.9,9.9 18,10 17.1,9.9 16.3,9.7 15.5,9.3 14.8,8.8 14.2,8.2 13.7,7.5 13.3,6.7 13.1,5.9 13,5\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"25.8,9.5 31,12.5\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"22.5,12.8 25.5,18\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"18,14 18,20\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"13.5,12.8 10.5,18\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"10.2,9.5 5,12.5\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<text x=\"25\" y=\"30\" fill=\"black\" font-size=\"10\">1</text>"
					"</svg>";

			case LocoFunctionIconInteriorLight2:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"3,3 1.5,3.5 1,5 1,33 1.5,34.5 3,35 33,35 34.5,34.5 35,33 35,5 34.5,3.5 33,3 3,3\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"/>"
					"<polyline points=\"23,5 22.9,5.9 22.7,6.7 22.3,7.5 21.8,8.2 21.2,8.8 20.5,9.3 19.7,9.7 18.9,9.9 18,10 17.1,9.9 16.3,9.7 15.5,9.3 14.8,8.8 14.2,8.2 13.7,7.5 13.3,6.7 13.1,5.9 13,5\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"25.8,9.5 31,12.5\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"22.5,12.8 25.5,18\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"18,14 18,20\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"13.5,12.8 10.5,18\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"10.2,9.5 5,12.5\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<text x=\"25\" y=\"30\" fill=\"black\" font-size=\"10\">2</text>"
					"</svg>";

			case LocoFunctionIconTableLight1:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"15.5,22.3 14.8,21.8 14.2,21.2 13.7,20.5 13.3,19.7 13.1,18.9 13,18 13.1,17.1 13.3,16.3 13.7,15.5 14.2,14.8 14.8,14.2 15.5,13.7 16.3,13.3 17.1,13.1 18,13 18.9,13.1 19.7,13.3 20.5,13.7 21.2,14.2 21.8,14.8 22.3,15.5 22.7,16.3 22.9,17.1 23,18 22.9,18.9 22.7,19.7 22.3,20.5 21.8,21.2 21.2,21.8 20.5,22.3\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"17,24 19,24 19,30 24,30 24,32 12,32 12,30 17,30\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
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

			case LocoFunctionIconTableLight2:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"15,13 12,21 24,21 21,13\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"17,23 19,23 19,30 24,30 24,32 12,32 12,30 17,30\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
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

			case LocoFunctionIconTableLight3:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"14,14 14,22 22,22 22,14\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"17,24 19,24 19,30 24,30 24,32 12,32 12,30 17,30\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
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

			case LocoFunctionIconCabLight1:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"5,3 3.5,3.5 3,5 3,35 33,35 33,18 27,5 26.5,3.5 25,3 5,3\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"/>"
					"<polyline points=\"3,24 3,35 33,35 33,24\" stroke=\"black\" stroke-width=\"1\" fill=\"black\"/>"
					"<circle r=\"2\" cx=\"34\" cy=\"29\" fill=\"black\" />"
					"<polyline points=\"23,5 22.9,5.9 22.7,6.7 22.3,7.5 21.8,8.2 21.2,8.8 20.5,9.3 19.7,9.7 18.9,9.9 18,10 17.1,9.9 16.3,9.7 15.5,9.3 14.8,8.8 14.2,8.2 13.7,7.5 13.3,6.7 13.1,5.9 13,5\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"22.5,12.8 25.5,18\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"18,14 18,20\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"13.5,12.8 10.5,18\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"</svg>";

			case LocoFunctionIconCabLight2:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"11,3 9.5,3.5 9,5 3,18 3,35 33,35 33,5 32.5,3.5 31,3 11,3\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"/>"
					"<polyline points=\"3,24 3,35 33,35 33,24\" stroke=\"black\" stroke-width=\"1\" fill=\"black\"/>"
					"<circle r=\"2\" cx=\"2\" cy=\"29\" fill=\"black\" />"
					"<polyline points=\"23,5 22.9,5.9 22.7,6.7 22.3,7.5 21.8,8.2 21.2,8.8 20.5,9.3 19.7,9.7 18.9,9.9 18,10 17.1,9.9 16.3,9.7 15.5,9.3 14.8,8.8 14.2,8.2 13.7,7.5 13.3,6.7 13.1,5.9 13,5\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"22.5,12.8 25.5,18\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"18,14 18,20\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"13.5,12.8 10.5,18\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"</svg>";

			case LocoFunctionIconCabLight12:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"5,3 3.5,3.5 3,5 3,35 33,35 33,5 32.5,3.5 31,3 5,3\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"/>"
					"<polyline points=\"3,24 3,35 33,35 33,24\" stroke=\"black\" stroke-width=\"1\" fill=\"black\"/>"
					"<circle r=\"2\" cx=\"8\" cy=\"29\" fill=\"darkgray\" />"
					"<circle r=\"2\" cx=\"28\" cy=\"29\" fill=\"darkgray\" />"
					"<polyline points=\"23,5 22.9,5.9 22.7,6.7 22.3,7.5 21.8,8.2 21.2,8.8 20.5,9.3 19.7,9.7 18.9,9.9 18,10 17.1,9.9 16.3,9.7 15.5,9.3 14.8,8.8 14.2,8.2 13.7,7.5 13.3,6.7 13.1,5.9 13,5\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"22.5,12.8 25.5,18\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"18,14 18,20\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"13.5,12.8 10.5,18\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"</svg>";

			case LocoFunctionIconDriversDeskLight:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"36,13.5 24.5,13.5 12.5,21.5 12.5,36\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"/>"
					"<polyline points=\"10.2,13.5 5,10.5\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"13.5,10.2 10.5,5\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"18,9 18,3\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"</svg>";

			case LocoFunctionIconTrainDestinationIndicator:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"2.5,12.5 33.5,12.5 33.5,23.5 2.5,23.5 2.5,12.5\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"/>"
					"<text x=\"4\" y=\"21.5\" fill=\"black\" font-size=\"10\">Zürich</text>"
					"<polyline points=\"13.5,10.2 10.5,5\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"18,9 18,3\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"22.5,10.2 25.5,5\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"22.5,25.8 25.5,31\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"18,27 18,33\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"13.5,25.8 10.5,31\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"</svg>";

			case LocoFunctionIconLocomotiveNumberIndicator:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"2.5,12.5 33.5,12.5 33.5,23.5 2.5,23.5 2.5,12.5\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"/>"
					"<text x=\"3.5\" y=\"21.5\" fill=\"black\" font-size=\"10\">X4014</text>"
					"<polyline points=\"13.5,10.2 10.5,5\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"18,9 18,3\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"22.5,10.2 25.5,5\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"22.5,25.8 25.5,31\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"18,27 18,33\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"13.5,25.8 10.5,31\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"</svg>";

			case LocoFunctionIconEngineLight:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"0,5 36,5\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"/>"
					"<polyline points=\"23,5 22.9,5.9 22.7,6.7 22.3,7.5 21.8,8.2 21.2,8.8 20.5,9.3 19.7,9.7 18.9,9.9 18,10 17.1,9.9 16.3,9.7 15.5,9.3 14.8,8.8 14.2,8.2 13.7,7.5 13.3,6.7 13.1,5.9 13,5\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"25.8,9.5 31,12.5\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"22.5,12.8 25.5,18\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"18,14 18,20\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"13.5,12.8 10.5,18\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"10.2,9.5 5,12.5\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"0,21 2.6,21.2 5.1,21.9 7.5,23 9.6,24.5 11.5,26.4 13,28.5 14.1,30.9 14.8,33.4 15,36\" stroke=\"black\" stroke-width=\"2\" fill=\"none\"/>"
					"<polyline points=\"0,36 2.6,21.2\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"/>"
					"<polyline points=\"0,36 7.5,23\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"/>"
					"<polyline points=\"0,36 11.5,26.4\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"/>"
					"<polyline points=\"0,36 14.1,30.9\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"/>"
					"<polyline points=\"0,36 15,36\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"/>"
					"<circle r=\"6\" cx=\"0\" cy=\"36\" fill=\"black\" />"
					"<polyline points=\"21,36 21.2,33.4 21.9,30.9 23,28.5 24.5,26.4 26.4,24.5 28.5,23 30.9,21.9 33.4,21.2 36,21\" stroke=\"black\" stroke-width=\"2\" fill=\"none\"/>"
					"<polyline points=\"36,36 21,36\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"/>"
					"<polyline points=\"36,36 21.9,30.9\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"/>"
					"<polyline points=\"36,36 24.5,26.4\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"/>"
					"<polyline points=\"36,36 28.5,23\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"/>"
					"<polyline points=\"36,36 33.4,21.2\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"/>"
					"<circle r=\"6\" cx=\"36\" cy=\"36\" fill=\"black\" />"

					"</svg>";

			case LocoFunctionIconFireBox:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"5,3 3.5,3.5 3,5 3,33 3.5,34.5 5,35 31,35 32.5,34.5 33,33 33,5 32.5,3.5 31,3 5,3\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"/>"
					"<polyline points=\"6,35 12,31 18,30 24,31 30,35 6,35\" stroke=\"black\" stroke-width=\"0\" fill=\"black\" />"
					"<polyline points=\"6,35 5,28 7,18 6,12 9,18 9,27 18,35\" stroke=\"black\" stroke-width=\"0\" fill=\"black\" class=\"button_on fire1\"/>"
					"<polyline points=\"8,35 10,24 12,16 10,9 14,16 14,24 20,35\" stroke=\"black\" stroke-width=\"0\" fill=\"black\" class=\"button_on fire2\"/>"
					"<polyline points=\"12,35 15,20 18,12 17,6 20,12 19,20 24,35\" stroke=\"black\" stroke-width=\"0\" fill=\"black\" class=\"button_on fire3\"/>"
					"<polyline points=\"16,35 22,24 22,16 26,9 24,16 26,24 28,35\" stroke=\"black\" stroke-width=\"0\" fill=\"black\" class=\"button_on fire4\"/>"
					"<polyline points=\"18,35 27,27 27,18 30,12 29,18 31,28 30,35\" stroke=\"black\" stroke-width=\"0\" fill=\"black\" class=\"button_on fire5\"/>"
					"</svg>";

			case LocoFunctionIconStairsLight:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"0,5 36,5\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"/>"
					"<polyline points=\"23,5 22.9,5.9 22.7,6.7 22.3,7.5 21.8,8.2 21.2,8.8 20.5,9.3 19.7,9.7 18.9,9.9 18,10 17.1,9.9 16.3,9.7 15.5,9.3 14.8,8.8 14.2,8.2 13.7,7.5 13.3,6.7 13.1,5.9 13,5\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"12,35 18,35 18,29 24,29 24,23 30,23 30,17 36,17\" stroke=\"black\" stroke-width=\"2\" fill=\"none\"/>"
					"<polyline points=\"25.8,9.5 31,12.5\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"22.5,12.8 25.5,18\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"18,14 18,20\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"13.5,12.8 10.5,18\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"<polyline points=\"10.2,9.5 5,12.5\" stroke=\"black\" stroke-width=\"2\" class=\"button_on\"/>"
					"</svg>";

			case LocoFunctionIconSmokeGenerator:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"20,18 30,18 30,22 28,22 28,32 22,32 22,22 20,22 20,18\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"25,12 28.5,12.9 30,15 28.5,17.1 25,18 21.5,17.1 20,15 21.5,12.9\" stroke=\"black\" stroke-width=\"0\" fill=\"black\" class=\"button_on smoke1\"/>"
					"<polyline points=\"25,12 28.5,12.9 30,15 28.5,17.1 25,18 21.5,17.1 20,15 21.5,12.9\" stroke=\"black\" stroke-width=\"0\" fill=\"black\" class=\"button_on smoke2\"/>"
					"</svg>";

			case LocoFunctionIconTelex1:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"1,14 4,14 7,10 15,10 12,13 10,13 10,23 12,23 15,26 7,26 4,22 1,22 1,14\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"35,14 32,14 29,10 21,10 24,13 26,13 26,23 24,23 21,26 29,26 32,22 35,22 35,14\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"12,31 18,31 18,28 24,32 18,36 18,33 12,33 12,31\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"</svg>";

			case LocoFunctionIconTelex2:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"1,14 4,14 7,10 15,10 12,13 10,13 10,23 12,23 15,26 7,26 4,22 1,22 1,14\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"35,14 32,14 29,10 21,10 24,13 26,13 26,23 24,23 21,26 29,26 32,22 35,22 35,14\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"24,31 18,31 18,28 12,32 18,36 18,33 24,33 24,31\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"</svg>";

			case LocoFunctionIconTelex12:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"1,14 4,14 7,10 15,10 12,13 10,13 10,23 12,23 15,26 7,26 4,22 1,22 1,14\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"35,14 32,14 29,10 21,10 24,13 26,13 26,23 24,23 21,26 29,26 32,22 35,22 35,14\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"</svg>";

			case LocoFunctionIconPanto1:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"0,5.5 36,5.5\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"/>"
					"<polyline points=\"10,24 26,24\" stroke=\"black\" stroke-width=\"2\" fill=\"none\"/>"
					"<polyline points=\"14,24 6,21 18,18 30,21 22,24\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_off\"/>"
					"<polyline points=\"14,24 8,15 18,6 28,15 22,24\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on\"/>"
					"<polyline points=\"12,31 18,31 18,28 24,32 18,36 18,33 12,33 12,31\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"</svg>";

			case LocoFunctionIconPanto2:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"0,5.5 36,5.5\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"/>"
					"<polyline points=\"10,24 26,24\" stroke=\"black\" stroke-width=\"2\" fill=\"none\"/>"
					"<polyline points=\"14,24 6,21 18,18 30,21 22,24\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_off\"/>"
					"<polyline points=\"14,24 8,15 18,6 28,15 22,24\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on\"/>"
					"<polyline points=\"24,31 18,31 18,28 12,32 18,36 18,33 24,33 24,31\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"</svg>";

			case LocoFunctionIconPanto12:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"0,5.5 36,5.5\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"/>"
					"<polyline points=\"10,24 26,24\" stroke=\"black\" stroke-width=\"2\" fill=\"none\"/>"
					"<polyline points=\"14,24 6,21 18,18 30,21 22,24\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_off\"/>"
					"<polyline points=\"14,24 8,15 18,6 28,15 22,24\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on\"/>"
					"</svg>";

			case LocoFunctionIconUp:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"18,5 26,18 20,18 20,31 16,31 16,18 10,18\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"</svg>";

			case LocoFunctionIconDown:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"18,31 26,18 20,18 20,5 16,5 16,18 10,18\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"</svg>";

			case LocoFunctionIconUpDown1:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"18,1 25,13 20,13 20,23 25,23 18,35 11,23 16,23 16,13 11,13\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"</svg>";

			case LocoFunctionIconUpDown2:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"9,4 22.4,7.6 18.9,11.1 21.1,14 22.5,17.4 23,21 28,21 21,33 14,21 19,21 18.7,18.4 17.7,16 16.1,13.9 12.5,16.5\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"</svg>";

			case LocoFunctionIconLeft:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"5,18 18,26 18,20 31,20 31,16 18,16 18,10\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"</svg>";

			case LocoFunctionIconRight:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"31,18 18,26 18,20 5,20 5,16 18,16 18,10\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"</svg>";

			case LocoFunctionIconLeftRight:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"1,18 13,25 13,20 23,20 23,25 35,18 23,11 23,16 13,16 13,11\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"</svg>";

			case LocoFunctionIconTurnLeft:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"26.5,22.2 28.4,21 29.6,19.6 30,18 29.6,16.4 28.4,15 26.5,13.8 24,12.8 21.1,12.2 18,12 14.9,12.2 12,12.8 9.5,13.8 7.6,15 6.4,16.4 6,18 0,18 8,24 16,18 10,18 10.3,17 11.1,16 12.3,15.2 14,14.5 15.9,14.1 18,14 20.1,14.1 22,14.5 23.7,15.2 24.9,16 25.7,17 26,18 25.7,19 24.9,20 23.7,20.8\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"</svg>";

			case LocoFunctionIconTurnRight:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"20,18 28,24 36,18 30,18 29.6,16.4 28.4,15 26.5,13.8 24,12.8 21.1,12.2 18,12 14.9,12.2 12,12.8 9.5,13.8 7.6,15 6.4,16.4 6,18 6.4,19.6 7.6,21 9.5,22.2 12.3,20.8 11.1,20 10.3,19 10,18 10.3,17 11.1,16 12.3,15.2 14,14.5 15.9,14.1 18,14 20.1,14.1 22,14.5 23.7,15.2 24.9,16 25.7,17 26,18\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"</svg>";

			case LocoFunctionIconTurn:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"20,18 28,24 36,18 30,18 29.6,16.4 28.4,15 26.5,13.8 24,12.8 21.1,12.2 18,12 14.9,12.2 12,12.8 9.5,13.8 7.6,15 6.4,16.4 6,18 0,18 8,24 16,18 10,18 10.3,17 11.1,16 12.3,15.2 14,14.5 15.9,14.1 18,14 20.1,14.1 22,14.5 23.7,15.2 24.9,16 25.7,17 26,18\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"</svg>";

			case LocoFunctionIconCrane:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"3,33 29,3 3,25\" stroke=\"black\" stroke-width=\"2\" fill=\"none\"/>"
					"<polyline points=\"3,33 8,21 12,22 14,16\" stroke=\"black\" stroke-width=\"2\" fill=\"none\"/>"
					"<polyline points=\"29.5,3 29.5,18.5 30.5,19.5 30.5,20.5 29.5,21.5 28.5,20.5\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"/>"
					"</svg>";

			case LocoFunctionIconMagnet:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"20,30 14,30 10.9,29.6 8,28.4 5.5,26.5 3.6,24 2.4,21.1 2,18 2.4,14.9 3.6,12 5.5,9.5 8,7.6 10.9,6.4 14,6 20,6 20,12 14,12 12.4,12.2 11,12.8 9.8,13.8 8.8,15 8.2,16.4 8,18 8.2,19.6 8.8,21 9.8,22.2 11,23.2 12.4,23.8 14,24 20,24\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"19.5,6.5 25.4,6.5 25.5,11.5 19.5,11.5 19.5,6.5\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"/>"
					"<polyline points=\"19.5,24.5 25.4,24.5 25.5,29.5 19.5,29.5 19.5,24.5\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"/>"
					"<polyline points=\"31,6 30,16 35,16 29,30 30,20 25,20\" stroke=\"black\" stroke-width=\"0\" fill=\"black\" class=\"button_on\"/>"
					"</svg>";

			case LocoFunctionIconCraneHook:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"20.5,5.3 20.7,6.9 21.3,8.3 22.3,9.5 24,11.6 26.5,13.5 28.4,16 29.6,18.9 30,22 29.6,25.1 28.4,28 26.5,30.5 24,32.4 21.1,33.6 18,34 14.9,33.6 12,32.4 9.5,30.5 7.6,28 6.4,25.1 6,22   6,14 7,14 8,15   12.8,25 13.8,26.2 15,27.2 16.4,27.8 18,28 19.6,27.8 21,27.2 22.2,26.2 23.2,25 23.8,23.6 24,22 23.8,20.4 23.2,19 22.2,17.8 21,16.8   18,13.8 16.1,11.3 14.9,8.4 14.5,5.3\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"</svg>";

			case LocoFunctionIconFan:
				return "<svg width=\"36\" height=\"36\">"
					"<circle r=\"2\" cx=\"18\" cy=\"18\" fill=\"black\" />"
					"<polyline points=\"16,16 15,12 15,10 16,9 19,8 22,9 23,10 23,12 21,14 20,16 18,15\" stroke=\"black\" stroke-width=\"0\" fill=\"black\" class=\"button_off\"/>"
					"<polyline points=\"20,16 24,15 26,15 27,16 28,19 27,22 26,23 24,23 22,21 20,20 21,18\" stroke=\"black\" stroke-width=\"0\" fill=\"black\" class=\"button_off\"/>"
					"<polyline points=\"20,20 21,24 21,26 20,27 17,28 14,27 13,26 13,24 15,22 16,20 18,21\" stroke=\"black\" stroke-width=\"0\" fill=\"black\" class=\"button_off\"/>"
					"<polyline points=\"16,20 12,21 10,21 9,20 8,17 9,14 10,13 12,13 14,15 16,16 15,18\" stroke=\"black\" stroke-width=\"0\" fill=\"black\" class=\"button_off\"/>"
					"<polyline points=\"16,16 15,12 15,10 16,9 19,8 22,9 23,10 23,12 21,14 20,16 18,15\" stroke=\"black\" stroke-width=\"0\" fill=\"black\" class=\"button_on rotate\"/>"
					"<polyline points=\"20,16 24,15 26,15 27,16 28,19 27,22 26,23 24,23 22,21 20,20 21,18\" stroke=\"black\" stroke-width=\"0\" fill=\"black\" class=\"button_on rotate\"/>"
					"<polyline points=\"20,20 21,24 21,26 20,27 17,28 14,27 13,26 13,24 15,22 16,20 18,21\" stroke=\"black\" stroke-width=\"0\" fill=\"black\" class=\"button_on rotate\"/>"
					"<polyline points=\"16,20 12,21 10,21 9,20 8,17 9,14 10,13 12,13 14,15 16,16 15,18\" stroke=\"black\" stroke-width=\"0\" fill=\"black\" class=\"button_on rotate\"/>"
					"</svg>";


			case LocoFunctionIconBreak:
				return "<svg width=\"36\" height=\"36\">"
					"<circle r=\"6\" cx=\"18\" cy=\"18\" fill=\"black\" />"
					"<circle r=\"4.5\" cx=\"18\" cy=\"18\" fill=\"none\" stroke-with=\"1\" stroke=\"darkgray\" />"
					"<circle r=\"1\" cx=\"18\" cy=\"18\" fill=\"darkgray\" />"
					"<polyline points=\"10.2,22.5 9.5,21.1 9.1,19.6 9,18 9.1,16.4 9.5,14.9 10.2,13.5 12.6,13.5 11.9,14.5 11.4,15.6 11.1,16.8 11,18 11.1,19.2 11.4,20.4 11.9,21.5 12.6,22.5\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"\"/>"
					"<polyline points=\"25.8,13.5 26.5,14.9 26.9,16.4 27,18 26.9,19.6 26.5,21.1 25.8,22.5 23.4,22.5 24.1,21.5 24.6,20.4 24.9,19.2 25,18 24.9,16.8 24.6,15.6 24.1,14.5 23.4,13.5\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"\"/>"
					"</svg>";

			case LocoFunctionIconNoSound:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"8,15 10,15 10,21 8,21\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"11,15 18,8 18,28 11,21\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"24.9,12.2 25.8,13.5 26.5,14.9 26.9,16.4 27,18 26.9,19.6 26.5,21.1 25.8,22.5 24.9,23.8\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_off sound1\"/>"
					"<polyline points=\"28,9.6 29.3,11.5 30.2,13.6 30.8,15.7 31,18 30.8,20.3 30.2,22.4 29.3,24.5 28,26.4\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_off sound2\"/>"
					"<polyline points=\"31,7.1 32.7,9.5 34,12.2 34.7,15 35,18 34.7,21 34,23.8 32.7,26.5 31,28.9\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_off sound3\"/>"
					"<polyline points=\"25,23 35,13\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on\"/>"
					"<polyline points=\"25,13 35,23\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on\"/>"
					"</svg>";

			case LocoFunctionIconSoundGeneral:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"8,15 10,15 10,21 8,21\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"11,15 18,8 18,28 11,21\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"24.9,12.2 25.8,13.5 26.5,14.9 26.9,16.4 27,18 26.9,19.6 26.5,21.1 25.8,22.5 24.9,23.8\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound1\"/>"
					"<polyline points=\"28,9.6 29.3,11.5 30.2,13.6 30.8,15.7 31,18 30.8,20.3 30.2,22.4 29.3,24.5 28,26.4\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"31,7.1 32.7,9.5 34,12.2 34.7,15 35,18 34.7,21 34,23.8 32.7,26.5 31,28.9\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"</svg>";

			case LocoFunctionIconRunning1:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"18,20 13,27\" stroke=\"black\" stroke-width=\"2\" fill=\"black\"/>"
					"<polyline points=\"18,20 23,27\" stroke=\"black\" stroke-width=\"2\" fill=\"black\"/>"
					"<polyline points=\"11,11 12,10 18,9 24,10 25,11 25,23 24,24 12,24 11,23\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"12,12 17,12 17,15 12,15\" stroke=\"black\" stroke-width=\"0\" fill=\"darkgray\"/>"
					"<polyline points=\"19,12 24,12 24,15 19,15\" stroke=\"black\" stroke-width=\"0\" fill=\"darkgray\"/>"
					"<circle r=\"1.5\" cx=\"22\" cy=\"21\" fill=\"darkgray\" />"
					"<circle r=\"1.5\" cx=\"14\" cy=\"21\" fill=\"darkgray\" />"
					"<text x=\"15\" y=\"35\" fill=\"black\" font-size=\"10\">1</text>"
					"<polyline points=\"28,9.6 29.3,11.5 30.2,13.6 30.8,15.7 31,18 30.8,20.3 30.2,22.4 29.3,24.5 28,26.4\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"31,7.1 32.7,9.5 34,12.2 34.7,15 35,18 34.7,21 34,23.8 32.7,26.5 31,28.9\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"<polyline points=\"8,26.4 6.7,24.5 5.8,22.4 5.2,20.3 5,18 5.2,15.7 5.8,13.6 6.7,11.5 8,9.6\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"5,28.9 3.3,26.5 2,23.8 1.3,21 1,18 1.3,15 2,12.2 3.3,9.5 5,7.1\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"</svg>";

			case LocoFunctionIconRunning2:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"18,20 13,27\" stroke=\"black\" stroke-width=\"2\" fill=\"black\"/>"
					"<polyline points=\"18,20 23,27\" stroke=\"black\" stroke-width=\"2\" fill=\"black\"/>"
					"<polyline points=\"11,11 12,10 18,9 24,10 25,11 25,23 24,24 12,24 11,23\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"12,12 17,12 17,15 12,15\" stroke=\"black\" stroke-width=\"0\" fill=\"darkgray\"/>"
					"<polyline points=\"19,12 24,12 24,15 19,15\" stroke=\"black\" stroke-width=\"0\" fill=\"darkgray\"/>"
					"<circle r=\"1.5\" cx=\"22\" cy=\"21\" fill=\"darkgray\" />"
					"<circle r=\"1.5\" cx=\"14\" cy=\"21\" fill=\"darkgray\" />"
					"<text x=\"15\" y=\"35\" fill=\"black\" font-size=\"10\">2</text>"
					"<polyline points=\"28,9.6 29.3,11.5 30.2,13.6 30.8,15.7 31,18 30.8,20.3 30.2,22.4 29.3,24.5 28,26.4\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"31,7.1 32.7,9.5 34,12.2 34.7,15 35,18 34.7,21 34,23.8 32.7,26.5 31,28.9\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"<polyline points=\"8,26.4 6.7,24.5 5.8,22.4 5.2,20.3 5,18 5.2,15.7 5.8,13.6 6.7,11.5 8,9.6\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"5,28.9 3.3,26.5 2,23.8 1.3,21 1,18 1.3,15 2,12.2 3.3,9.5 5,7.1\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"</svg>";

			case LocoFunctionIconEngine1:
				return "<svg width=\"36\" height=\"36\">"
					"<circle r=\"8\" cx=\"18\" cy=\"18\" fill=\"black\" />"
					"<polyline points=\"14,18 14,9.1 15.3,8.5 16.6,8.1 18,8 19.4,8.1 20.7,8.5 22,9.1 22,18\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<text x=\"13.7\" y=\"21\" fill=\"white\" font-size=\"10\">M</text>"
					"<polyline points=\"28,9.6 29.3,11.5 30.2,13.6 30.8,15.7 31,18 30.8,20.3 30.2,22.4 29.3,24.5 28,26.4\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"31,7.1 32.7,9.5 34,12.2 34.7,15 35,18 34.7,21 34,23.8 32.7,26.5 31,28.9\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"<polyline points=\"8,26.4 6.7,24.5 5.8,22.4 5.2,20.3 5,18 5.2,15.7 5.8,13.6 6.7,11.5 8,9.6\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"5,28.9 3.3,26.5 2,23.8 1.3,21 1,18 1.3,15 2,12.2 3.3,9.5 5,7.1\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"<text x=\"15\" y=\"35\" fill=\"black\" font-size=\"10\">1</text>"
					"</svg>";

			case LocoFunctionIconEngine2:
				return "<svg width=\"36\" height=\"36\">"
					"<circle r=\"8\" cx=\"18\" cy=\"18\" fill=\"black\" />"
					"<polyline points=\"14,18 14,9.1 15.3,8.5 16.6,8.1 18,8 19.4,8.1 20.7,8.5 22,9.1 22,18\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<text x=\"13.7\" y=\"21\" fill=\"white\" font-size=\"10\">M</text>"
					"<polyline points=\"28,9.6 29.3,11.5 30.2,13.6 30.8,15.7 31,18 30.8,20.3 30.2,22.4 29.3,24.5 28,26.4\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"31,7.1 32.7,9.5 34,12.2 34.7,15 35,18 34.7,21 34,23.8 32.7,26.5 31,28.9\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"<polyline points=\"8,26.4 6.7,24.5 5.8,22.4 5.2,20.3 5,18 5.2,15.7 5.8,13.6 6.7,11.5 8,9.6\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"5,28.9 3.3,26.5 2,23.8 1.3,21 1,18 1.3,15 2,12.2 3.3,9.5 5,7.1\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"<text x=\"15\" y=\"35\" fill=\"black\" font-size=\"10\">2</text>"
					"</svg>";

			case LocoFunctionIconBreak1:
				return "<svg width=\"36\" height=\"36\">"
					"<circle r=\"6\" cx=\"12\" cy=\"18\" fill=\"black\" />"
					"<circle r=\"4.5\" cx=\"12\" cy=\"18\" fill=\"none\" stroke-with=\"1\" stroke=\"darkgray\" />"
					"<circle r=\"1\" cx=\"12\" cy=\"18\" fill=\"darkgray\" />"
					"<polyline points=\"4.2,22.5 3.5,21.1 3.1,19.6 3,18 3.1,16.4 3.5,14.9 4.2,13.5 6.6,13.5 5.9,14.5 5.4,15.6 5.1,16.8 5,18 5.1,19.2 5.4,20.4 5.9,21.5 6.6,22.5\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"\"/>"
					"<polyline points=\"19.8,13.5 20.5,14.9 20.9,16.4 21,18 20.9,19.6 20.5,21.1 19.8,22.5 17.4,22.5 18.1,21.5 18.6,20.4 18.9,19.2 19,18 18.9,16.8 18.6,15.6 18.1,14.5 17.4,13.5\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"\"/>"
					"<polyline points=\"24.9,12.2 25.8,13.5 26.5,14.9 26.9,16.4 27,18 26.9,19.6 26.5,21.1 25.8,22.5 24.9,23.8\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound1\"/>"
					"<polyline points=\"28,9.6 29.3,11.5 30.2,13.6 30.8,15.7 31,18 30.8,20.3 30.2,22.4 29.3,24.5 28,26.4\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"31,7.1 32.7,9.5 34,12.2 34.7,15 35,18 34.7,21 34,23.8 32.7,26.5 31,28.9\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"<text x=\"1\" y=\"35\" fill=\"black\" font-size=\"10\">1</text>"
					"</svg>";

			case LocoFunctionIconBreak2:
				return "<svg width=\"36\" height=\"36\">"
					"<circle r=\"6\" cx=\"12\" cy=\"18\" fill=\"black\" />"
					"<circle r=\"4.5\" cx=\"12\" cy=\"18\" fill=\"none\" stroke-with=\"1\" stroke=\"darkgray\" />"
					"<circle r=\"1\" cx=\"12\" cy=\"18\" fill=\"darkgray\" />"
					"<polyline points=\"4.2,22.5 3.5,21.1 3.1,19.6 3,18 3.1,16.4 3.5,14.9 4.2,13.5 6.6,13.5 5.9,14.5 5.4,15.6 5.1,16.8 5,18 5.1,19.2 5.4,20.4 5.9,21.5 6.6,22.5\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"\"/>"
					"<polyline points=\"19.8,13.5 20.5,14.9 20.9,16.4 21,18 20.9,19.6 20.5,21.1 19.8,22.5 17.4,22.5 18.1,21.5 18.6,20.4 18.9,19.2 19,18 18.9,16.8 18.6,15.6 18.1,14.5 17.4,13.5\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"\"/>"
					"<polyline points=\"24.9,12.2 25.8,13.5 26.5,14.9 26.9,16.4 27,18 26.9,19.6 26.5,21.1 25.8,22.5 24.9,23.8\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound1\"/>"
					"<polyline points=\"28,9.6 29.3,11.5 30.2,13.6 30.8,15.7 31,18 30.8,20.3 30.2,22.4 29.3,24.5 28,26.4\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"31,7.1 32.7,9.5 34,12.2 34.7,15 35,18 34.7,21 34,23.8 32.7,26.5 31,28.9\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"<text x=\"1\" y=\"35\" fill=\"black\" font-size=\"10\">2</text>"
					"</svg>";

			case LocoFunctionIconCurve:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"23.3,13.3 21.7,14.7 20,15.9 18.1,16.8 16.1,17.5 14.1,17.9 12,18\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"\"/>"
					"<polyline points=\"26.1,16.1 24.2,17.9 22,19.3 19.7,20.5 17.2,21.3 14.6,21.8 12,22\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"\"/>"
					"<polyline points=\"20.5,13.1 25.4,19.5\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"\"/>"
					"<polyline points=\"19,14.1 23,21.1\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"\"/>"
					"<polyline points=\"17.4,14.9 20.4,22.3\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"\"/>"
					"<polyline points=\"15.6,15.5 17.7,23.3\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"\"/>"
					"<polyline points=\"13.8,15.9 14.9,23.8\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"\"/>"
					"<polyline points=\"28,9.6 29.3,11.5 30.2,13.6 30.8,15.7 31,18 30.8,20.3 30.2,22.4 29.3,24.5 28,26.4\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"31,7.1 32.7,9.5 34,12.2 34.7,15 35,18 34.7,21 34,23.8 32.7,26.5 31,28.9\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"<polyline points=\"8,26.4 6.7,24.5 5.8,22.4 5.2,20.3 5,18 5.2,15.7 5.8,13.6 6.7,11.5 8,9.6\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"5,28.9 3.3,26.5 2,23.8 1.3,21 1,18 1.3,15 2,12.2 3.3,9.5 5,7.1\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"</svg>";

			case LocoFunctionIconHorn1:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"1,16 3,16 3,17 10,17 14,16 16,15 19,12 19,24 16,21 14,20 10,19 3,19 3,20 1,20\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"24.9,12.2 25.8,13.5 26.5,14.9 26.9,16.4 27,18 26.9,19.6 26.5,21.1 25.8,22.5 24.9,23.8\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound1\"/>"
					"<polyline points=\"28,9.6 29.3,11.5 30.2,13.6 30.8,15.7 31,18 30.8,20.3 30.2,22.4 29.3,24.5 28,26.4\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"31,7.1 32.7,9.5 34,12.2 34.7,15 35,18 34.7,21 34,23.8 32.7,26.5 31,28.9\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"<text x=\"1\" y=\"35\" fill=\"black\" font-size=\"10\">1</text>"
					"</svg>";

			case LocoFunctionIconHorn2:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"1,16 3,16 3,17 10,17 14,16 16,15 19,12 19,24 16,21 14,20 10,19 3,19 3,20 1,20\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"24.9,12.2 25.8,13.5 26.5,14.9 26.9,16.4 27,18 26.9,19.6 26.5,21.1 25.8,22.5 24.9,23.8\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound1\"/>"
					"<polyline points=\"28,9.6 29.3,11.5 30.2,13.6 30.8,15.7 31,18 30.8,20.3 30.2,22.4 29.3,24.5 28,26.4\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"31,7.1 32.7,9.5 34,12.2 34.7,15 35,18 34.7,21 34,23.8 32.7,26.5 31,28.9\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"<text x=\"1\" y=\"35\" fill=\"black\" font-size=\"10\">2</text>"
					"</svg>";

			case LocoFunctionIconWhistle1:
				return "<svg width=\"36\" height=\"36\">"
					"<circle r=\"6\" cx=\"22\" cy=\"24\" fill=\"black\" />"
					"<polyline points=\"8,18 22,18 22,22 8,22\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"12.2,11.1 13.5,10.2 14.9,9.5 16.4,9.1 18,9 19.6,9.1 21.1,9.5 22.5,10.2 23.8,11.1\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound1\"/>"
					"<polyline points=\"9.6,8 11.5,6.7 13.6,5.8 15.7,5.2 18,5 20.3,5.2 22.4,5.8 24.5,6.7 26.4,8\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"7.1,5 9.5,3.3 12.2,2 15,1.3 18,1 21,1.3 23.8,2 26.5,3.3 28.9,5\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"</svg>";

			case LocoFunctionIconWhistle2:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"16,12 20,12 20,16 18,16 20,20 20,35 16,35\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"24.9,12.2 25.8,13.5 26.5,14.9 26.9,16.4 27,18 26.9,19.6 26.5,21.1 25.8,22.5 24.9,23.8\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound1\"/>"
					"<polyline points=\"28,9.6 29.3,11.5 30.2,13.6 30.8,15.7 31,18 30.8,20.3 30.2,22.4 29.3,24.5 28,26.4\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"31,7.1 32.7,9.5 34,12.2 34.7,15 35,18 34.7,21 34,23.8 32.7,26.5 31,28.9\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"</svg>";

			case LocoFunctionIconBell:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"13,23 14,20 14,16 15,14 18,13 21,14 22,16 22,20 23,23\" stroke=\"black\" stroke-width=\"0\" fill=\"black\" class=\"bell1\"/>"
					"<polyline points=\"18,13 18,25\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"bell2\"/>"
					"<polyline points=\"28,9.6 29.3,11.5 30.2,13.6 30.8,15.7 31,18 30.8,20.3 30.2,22.4 29.3,24.5 28,26.4\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"31,7.1 32.7,9.5 34,12.2 34.7,15 35,18 34.7,21 34,23.8 32.7,26.5 31,28.9\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"<polyline points=\"8,26.4 6.7,24.5 5.8,22.4 5.2,20.3 5,18 5.2,15.7 5.8,13.6 6.7,11.5 8,9.6\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"5,28.9 3.3,26.5 2,23.8 1.3,21 1,18 1.3,15 2,12.2 3.3,9.5 5,7.1\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"</svg>";

			case LocoFunctionIconStationAnnouncement1:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"8,10 10,10 10,17 14,16 16,15 19,12 19,24 16,21 14,20 10,19 10,19 10,20 8,20\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"24.9,12.2 25.8,13.5 26.5,14.9 26.9,16.4 27,18 26.9,19.6 26.5,21.1 25.8,22.5 24.9,23.8\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound1\"/>"
					"<polyline points=\"28,9.6 29.3,11.5 30.2,13.6 30.8,15.7 31,18 30.8,20.3 30.2,22.4 29.3,24.5 28,26.4\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"31,7.1 32.7,9.5 34,12.2 34.7,15 35,18 34.7,21 34,23.8 32.7,26.5 31,28.9\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"<text x=\"1\" y=\"35\" fill=\"black\" font-size=\"10\">1</text>"
					"</svg>";

			case LocoFunctionIconStationAnnouncement2:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"8,10 10,10 10,17 14,16 16,15 19,12 19,24 16,21 14,20 10,19 10,19 10,20 8,20\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"24.9,12.2 25.8,13.5 26.5,14.9 26.9,16.4 27,18 26.9,19.6 26.5,21.1 25.8,22.5 24.9,23.8\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound1\"/>"
					"<polyline points=\"28,9.6 29.3,11.5 30.2,13.6 30.8,15.7 31,18 30.8,20.3 30.2,22.4 29.3,24.5 28,26.4\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"31,7.1 32.7,9.5 34,12.2 34.7,15 35,18 34.7,21 34,23.8 32.7,26.5 31,28.9\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"<text x=\"1\" y=\"35\" fill=\"black\" font-size=\"10\">2</text>"
					"</svg>";

			case LocoFunctionIconStationAnnouncement3:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"8,10 10,10 10,17 14,16 16,15 19,12 19,24 16,21 14,20 10,19 10,19 10,20 8,20\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"24.9,12.2 25.8,13.5 26.5,14.9 26.9,16.4 27,18 26.9,19.6 26.5,21.1 25.8,22.5 24.9,23.8\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound1\"/>"
					"<polyline points=\"28,9.6 29.3,11.5 30.2,13.6 30.8,15.7 31,18 30.8,20.3 30.2,22.4 29.3,24.5 28,26.4\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"31,7.1 32.7,9.5 34,12.2 34.7,15 35,18 34.7,21 34,23.8 32.7,26.5 31,28.9\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"<text x=\"1\" y=\"35\" fill=\"black\" font-size=\"10\">3</text>"
					"</svg>";

			case LocoFunctionIconSpeak:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"8,30 10.3,22.6 7.6,21 6.2,19 6.2,17 7.6,15 10.3,13.4 13.9,12.4 18,12 22.1,12.4 25.7,13.4 28.4,15 29.8,17 29.8,19 28.4,21 25.7,22.6 22.1,23.6 18,24 8,30\" stroke=\"black\" stroke-width=\"2\" fill=\"none\"/>"
					"</svg>";

			case LocoFunctionIconRadio:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"15,10 15,26 21,26 21,14 15,14 15,19 21,19\" stroke=\"black\" stroke-width=\"2\" fill=\"none\"/>"
					"<polyline points=\"28,9.6 29.3,11.5 30.2,13.6 30.8,15.7 31,18 30.8,20.3 30.2,22.4 29.3,24.5 28,26.4\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"31,7.1 32.7,9.5 34,12.2 34.7,15 35,18 34.7,21 34,23.8 32.7,26.5 31,28.9\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"<polyline points=\"8,26.4 6.7,24.5 5.8,22.4 5.2,20.3 5,18 5.2,15.7 5.8,13.6 6.7,11.5 8,9.6\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"5,28.9 3.3,26.5 2,23.8 1.3,21 1,18 1.3,15 2,12.2 3.3,9.5 5,7.1\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"</svg>";

			case LocoFunctionIconMusic1:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"14,22.5 15.3,22.7 16.2,23.3 16.5,24 16.2,24.8 15.3,25.3 14,25.5 12.8,25.3 11.8,24.8 11.5,24 11.8,23.3 12.8,22.7\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"22,18.5 23.3,18.7 24.2,19.3 24.5,20 24.2,20.8 23.3,21.3 22,21.5 20.8,21.3 19.8,20.8 19.5,20 19.8,19.3 20.8,18.7\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"16,24 16,14 24,10 24,20\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"/>"
					"<polyline points=\"28,9.6 29.3,11.5 30.2,13.6 30.8,15.7 31,18 30.8,20.3 30.2,22.4 29.3,24.5 28,26.4\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"31,7.1 32.7,9.5 34,12.2 34.7,15 35,18 34.7,21 34,23.8 32.7,26.5 31,28.9\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"<polyline points=\"8,26.4 6.7,24.5 5.8,22.4 5.2,20.3 5,18 5.2,15.7 5.8,13.6 6.7,11.5 8,9.6\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"5,28.9 3.3,26.5 2,23.8 1.3,21 1,18 1.3,15 2,12.2 3.3,9.5 5,7.1\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"<text x=\"16\" y=\"35\" fill=\"black\" font-size=\"10\">1</text>"
					"</svg>";

			case LocoFunctionIconMusic2:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"14,22.5 15.3,22.7 16.2,23.3 16.5,24 16.2,24.8 15.3,25.3 14,25.5 12.8,25.3 11.8,24.8 11.5,24 11.8,23.3 12.8,22.7\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"22,18.5 23.3,18.7 24.2,19.3 24.5,20 24.2,20.8 23.3,21.3 22,21.5 20.8,21.3 19.8,20.8 19.5,20 19.8,19.3 20.8,18.7\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"16,24 16,14 24,10 24,20\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"/>"
					"<polyline points=\"28,9.6 29.3,11.5 30.2,13.6 30.8,15.7 31,18 30.8,20.3 30.2,22.4 29.3,24.5 28,26.4\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"31,7.1 32.7,9.5 34,12.2 34.7,15 35,18 34.7,21 34,23.8 32.7,26.5 31,28.9\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"<polyline points=\"8,26.4 6.7,24.5 5.8,22.4 5.2,20.3 5,18 5.2,15.7 5.8,13.6 6.7,11.5 8,9.6\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"5,28.9 3.3,26.5 2,23.8 1.3,21 1,18 1.3,15 2,12.2 3.3,9.5 5,7.1\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"<text x=\"16\" y=\"35\" fill=\"black\" font-size=\"10\">2</text>"
					"</svg>";

			case LocoFunctionIconOpenDoor:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"10.5,10.5 17.5,10.5 17.5,25.5 10.5,25.5 10.5,10.5\" stroke=\"black\" stroke-width=\"1\" fill=\"none\" class=\"door_left\"/>"
					"<polyline points=\"18.5,10.5 25.5,10.5 25.5,25.5 18.5,25.5 18.5,10.5\" stroke=\"black\" stroke-width=\"1\" fill=\"none\" class=\"door_right\"/>"
					"</svg>";

			case LocoFunctionIconCloseDoor:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"4.5,10.5 11.5,10.5 11.5,25.5 4.5,25.5 4.5,10.5\" stroke=\"black\" stroke-width=\"1\" fill=\"none\" class=\"door_right\"/>"
					"<polyline points=\"24.5,10.5 31.5,10.5 31.5,25.5 24.5,25.5 24.5,10.5\" stroke=\"black\" stroke-width=\"1\" fill=\"none\" class=\"door_left\"/>"
					"</svg>";

			case LocoFunctionIconFan1:
				return "<svg width=\"36\" height=\"36\">"
					"<circle r=\"2\" cx=\"18\" cy=\"18\" fill=\"black\" />"
					"<polyline points=\"16,16 15,12 15,10 16,9 19,8 22,9 23,10 23,12 21,14 20,16 18,15\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"20,16 24,15 26,15 27,16 28,19 27,22 26,23 24,23 22,21 20,20 21,18\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"20,20 21,24 21,26 20,27 17,28 14,27 13,26 13,24 15,22 16,20 18,21\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"16,20 12,21 10,21 9,20 8,17 9,14 10,13 12,13 14,15 16,16 15,18\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<text x=\"22\" y=\"35\" fill=\"black\" font-size=\"10\">1</text>"
					"<polyline points=\"28,9.6 29.3,11.5 30.2,13.6 30.8,15.7 31,18 30.8,20.3 30.2,22.4 29.3,24.5 28,26.4\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"31,7.1 32.7,9.5 34,12.2 34.7,15 35,18 34.7,21 34,23.8 32.7,26.5 31,28.9\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"<polyline points=\"8,26.4 6.7,24.5 5.8,22.4 5.2,20.3 5,18 5.2,15.7 5.8,13.6 6.7,11.5 8,9.6\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"5,28.9 3.3,26.5 2,23.8 1.3,21 1,18 1.3,15 2,12.2 3.3,9.5 5,7.1\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"</svg>";

			case LocoFunctionIconFan2:
				return "<svg width=\"36\" height=\"36\">"
					"<circle r=\"2\" cx=\"18\" cy=\"18\" fill=\"black\" />"
					"<polyline points=\"16,16 15,12 15,10 16,9 19,8 22,9 23,10 23,12 21,14 20,16 18,15\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"20,16 24,15 26,15 27,16 28,19 27,22 26,23 24,23 22,21 20,20 21,18\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"20,20 21,24 21,26 20,27 17,28 14,27 13,26 13,24 15,22 16,20 18,21\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"16,20 12,21 10,21 9,20 8,17 9,14 10,13 12,13 14,15 16,16 15,18\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<text x=\"22\" y=\"35\" fill=\"black\" font-size=\"10\">2</text>"
					"<polyline points=\"28,9.6 29.3,11.5 30.2,13.6 30.8,15.7 31,18 30.8,20.3 30.2,22.4 29.3,24.5 28,26.4\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"31,7.1 32.7,9.5 34,12.2 34.7,15 35,18 34.7,21 34,23.8 32.7,26.5 31,28.9\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"<polyline points=\"8,26.4 6.7,24.5 5.8,22.4 5.2,20.3 5,18 5.2,15.7 5.8,13.6 6.7,11.5 8,9.6\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"5,28.9 3.3,26.5 2,23.8 1.3,21 1,18 1.3,15 2,12.2 3.3,9.5 5,7.1\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"</svg>";

			case LocoFunctionIconFan3:
				return "<svg width=\"36\" height=\"36\">"
					"<circle r=\"2\" cx=\"18\" cy=\"18\" fill=\"black\" />"
					"<polyline points=\"16,16 15,12 15,10 16,9 19,8 22,9 23,10 23,12 21,14 20,16 18,15\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"20,16 24,15 26,15 27,16 28,19 27,22 26,23 24,23 22,21 20,20 21,18\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"20,20 21,24 21,26 20,27 17,28 14,27 13,26 13,24 15,22 16,20 18,21\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"16,20 12,21 10,21 9,20 8,17 9,14 10,13 12,13 14,15 16,16 15,18\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<text x=\"22\" y=\"35\" fill=\"black\" font-size=\"10\">3</text>"
					"<polyline points=\"28,9.6 29.3,11.5 30.2,13.6 30.8,15.7 31,18 30.8,20.3 30.2,22.4 29.3,24.5 28,26.4\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"31,7.1 32.7,9.5 34,12.2 34.7,15 35,18 34.7,21 34,23.8 32.7,26.5 31,28.9\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"<polyline points=\"8,26.4 6.7,24.5 5.8,22.4 5.2,20.3 5,18 5.2,15.7 5.8,13.6 6.7,11.5 8,9.6\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"5,28.9 3.3,26.5 2,23.8 1.3,21 1,18 1.3,15 2,12.2 3.3,9.5 5,7.1\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"</svg>";

			case LocoFunctionIconShovelCoal:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"0,30 8,22 16,30\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"26,16 20,22 22,24 20,26 16,26 16,22 18,20 20,22\" stroke=\"black\" stroke-width=\"2\" fill=\"none\"/>"
					"<polyline points=\"12.2,11.1 13.5,10.2 14.9,9.5 16.4,9.1 18,9 19.6,9.1 21.1,9.5 22.5,10.2 23.8,11.1\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound1\"/>"
					"<polyline points=\"9.6,8 11.5,6.7 13.6,5.8 15.7,5.2 18,5 20.3,5.2 22.4,5.8 24.5,6.7 26.4,8\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"7.1,5 9.5,3.3 12.2,2 15,1.3 18,1 21,1.3 23.8,2 26.5,3.3 28.9,5\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"</svg>";

			case LocoFunctionIconCompressedAir:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"10,18 18,18 20,19 18,20 10,20\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"21,19 24,17\" stroke=\"black\" stroke-width=\"1\" fill=\"none\" class=\"button_on\"/>"
					"<polyline points=\"21,19 25,19\" stroke=\"black\" stroke-width=\"1\" fill=\"none\" class=\"button_on\"/>"
					"<polyline points=\"21,19 24,21\" stroke=\"black\" stroke-width=\"1\" fill=\"none\" class=\"button_on\"/>"
					"<polyline points=\"12.2,11.1 13.5,10.2 14.9,9.5 16.4,9.1 18,9 19.6,9.1 21.1,9.5 22.5,10.2 23.8,11.1\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound1\"/>"
					"<polyline points=\"9.6,8 11.5,6.7 13.6,5.8 15.7,5.2 18,5 20.3,5.2 22.4,5.8 24.5,6.7 26.4,8\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"7.1,5 9.5,3.3 12.2,2 15,1.3 18,1 21,1.3 23.8,2 26.5,3.3 28.9,5\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"</svg>";

			case LocoFunctionIconReliefValve:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"8,28 9,24 10,23 26,23 27,24 28,28 27,32 26,33 10,33 9,32 8,28\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"/>"
					"<polyline points=\"18,20 18,25\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"/>"
					"<polyline points=\"18,19 16,16\" stroke=\"black\" stroke-width=\"1\" fill=\"none\" class=\"button_on\"/>"
					"<polyline points=\"18,19 18,15\" stroke=\"black\" stroke-width=\"1\" fill=\"none\" class=\"button_on\"/>"
					"<polyline points=\"18,19 20,16\" stroke=\"black\" stroke-width=\"1\" fill=\"none\" class=\"button_on\"/>"
					"<polyline points=\"12.2,11.1 13.5,10.2 14.9,9.5 16.4,9.1 18,9 19.6,9.1 21.1,9.5 22.5,10.2 23.8,11.1\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound1\"/>"
					"<polyline points=\"9.6,8 11.5,6.7 13.6,5.8 15.7,5.2 18,5 20.3,5.2 22.4,5.8 24.5,6.7 26.4,8\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"7.1,5 9.5,3.3 12.2,2 15,1.3 18,1 21,1.3 23.8,2 26.5,3.3 28.9,5\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"</svg>";

			case LocoFunctionIconSteamBlowOut:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"14,26 14,24 26,24 26,30 14,30 14,28\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"/>"
					"<polyline points=\"2,27 16,27\" stroke=\"black\" stroke-width=\"1\" fill=\"none\" class=\"cylinder\"/>"
					"<polyline points=\"16,25 16,29\" stroke=\"black\" stroke-width=\"1\" fill=\"none\" class=\"cylinder\"/>"
					"<polyline points=\"25,22 23,19\" stroke=\"black\" stroke-width=\"1\" fill=\"none\" class=\"button_on\"/>"
					"<polyline points=\"25,22 25,18\" stroke=\"black\" stroke-width=\"1\" fill=\"none\" class=\"button_on\"/>"
					"<polyline points=\"25,22 27,19\" stroke=\"black\" stroke-width=\"1\" fill=\"none\" class=\"button_on\"/>"
					"<polyline points=\"12.2,11.1 13.5,10.2 14.9,9.5 16.4,9.1 18,9 19.6,9.1 21.1,9.5 22.5,10.2 23.8,11.1\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound1\"/>"
					"<polyline points=\"9.6,8 11.5,6.7 13.6,5.8 15.7,5.2 18,5 20.3,5.2 22.4,5.8 24.5,6.7 26.4,8\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"7.1,5 9.5,3.3 12.2,2 15,1.3 18,1 21,1.3 23.8,2 26.5,3.3 28.9,5\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"</svg>";

			case LocoFunctionIconSteamBlow:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"14,26 14,24 26,24 26,30 14,30 14,28\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"/>"
					"<polyline points=\"2,27 16,27\" stroke=\"black\" stroke-width=\"1\" fill=\"none\" class=\"cylinder\"/>"
					"<polyline points=\"16,25 16,29\" stroke=\"black\" stroke-width=\"1\" fill=\"none\" class=\"cylinder\"/>"
					"<polyline points=\"25,32 23,35\" stroke=\"black\" stroke-width=\"1\" fill=\"none\" class=\"button_on\"/>"
					"<polyline points=\"25,32 25,36\" stroke=\"black\" stroke-width=\"1\" fill=\"none\" class=\"button_on\"/>"
					"<polyline points=\"25,32 27,35\" stroke=\"black\" stroke-width=\"1\" fill=\"none\" class=\"button_on\"/>"
					"<polyline points=\"12.2,11.1 13.5,10.2 14.9,9.5 16.4,9.1 18,9 19.6,9.1 21.1,9.5 22.5,10.2 23.8,11.1\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound1\"/>"
					"<polyline points=\"9.6,8 11.5,6.7 13.6,5.8 15.7,5.2 18,5 20.3,5.2 22.4,5.8 24.5,6.7 26.4,8\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"7.1,5 9.5,3.3 12.2,2 15,1.3 18,1 21,1.3 23.8,2 26.5,3.3 28.9,5\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"</svg>";

			case LocoFunctionIconDrainValve:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"10,18 26,18\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"/>"
					"<polyline points=\"10,28 26,28\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"/>"
					"<polyline points=\"18,20 16,24\" stroke=\"black\" stroke-width=\"1\" fill=\"none\" class=\"button_on\"/>"
					"<polyline points=\"18,20 18,25\" stroke=\"black\" stroke-width=\"1\" fill=\"none\" class=\"button_on\"/>"
					"<polyline points=\"18,20 20,24\" stroke=\"black\" stroke-width=\"1\" fill=\"none\" class=\"button_on\"/>"
					"<polyline points=\"12.2,11.1 13.5,10.2 14.9,9.5 16.4,9.1 18,9 19.6,9.1 21.1,9.5 22.5,10.2 23.8,11.1\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound1\"/>"
					"<polyline points=\"9.6,8 11.5,6.7 13.6,5.8 15.7,5.2 18,5 20.3,5.2 22.4,5.8 24.5,6.7 26.4,8\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"7.1,5 9.5,3.3 12.2,2 15,1.3 18,1 21,1.3 23.8,2 26.5,3.3 28.9,5\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"</svg>";

			case LocoFunctionIconShakingRust:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"10,18 26,18\" stroke=\"black\" stroke-width=\"1\" fill=\"none\" class=\"shake\"/>"
					"<polyline points=\"10,22 26,22\" stroke=\"black\" stroke-width=\"1\" fill=\"none\" class=\"shake\"/>"
					"<polyline points=\"10,26 26,26\" stroke=\"black\" stroke-width=\"1\" fill=\"none\" class=\"shake\"/>"
					"<polyline points=\"10,30 26,30\" stroke=\"black\" stroke-width=\"1\" fill=\"none\" class=\"shake\"/>"
					"<polyline points=\"10,34 26,34\" stroke=\"black\" stroke-width=\"1\" fill=\"none\" class=\"shake\"/>"
					"<polyline points=\"10,18 10,34\" stroke=\"black\" stroke-width=\"1\" fill=\"none\" class=\"shake\"/>"
					"<polyline points=\"14,18 14,34\" stroke=\"black\" stroke-width=\"1\" fill=\"none\" class=\"shake\"/>"
					"<polyline points=\"18,18 18,34\" stroke=\"black\" stroke-width=\"1\" fill=\"none\" class=\"shake\"/>"
					"<polyline points=\"22,18 22,34\" stroke=\"black\" stroke-width=\"1\" fill=\"none\" class=\"shake\"/>"
					"<polyline points=\"26,18 26,34\" stroke=\"black\" stroke-width=\"1\" fill=\"none\" class=\"shake\"/>"
					"<polyline points=\"12.2,11.1 13.5,10.2 14.9,9.5 16.4,9.1 18,9 19.6,9.1 21.1,9.5 22.5,10.2 23.8,11.1\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound1\"/>"
					"<polyline points=\"9.6,8 11.5,6.7 13.6,5.8 15.7,5.2 18,5 20.3,5.2 22.4,5.8 24.5,6.7 26.4,8\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"7.1,5 9.5,3.3 12.2,2 15,1.3 18,1 21,1.3 23.8,2 26.5,3.3 28.9,5\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"</svg>";

			case LocoFunctionIconAirPump:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"8,27 9,22 10,21 26,21 27,22 28,27 27,32 26,33 10,33 9,32 8,27\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"/>"
					"<polyline points=\"18,18 18,23\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"/>"
					"<polyline points=\"18,24 16,28\" stroke=\"black\" stroke-width=\"1\" fill=\"none\" class=\"button_on\"/>"
					"<polyline points=\"18,24 18,29\" stroke=\"black\" stroke-width=\"1\" fill=\"none\" class=\"button_on\"/>"
					"<polyline points=\"18,24 20,28\" stroke=\"black\" stroke-width=\"1\" fill=\"none\" class=\"button_on\"/>"
					"<polyline points=\"12.2,11.1 13.5,10.2 14.9,9.5 16.4,9.1 18,9 19.6,9.1 21.1,9.5 22.5,10.2 23.8,11.1\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound1\"/>"
					"<polyline points=\"9.6,8 11.5,6.7 13.6,5.8 15.7,5.2 18,5 20.3,5.2 22.4,5.8 24.5,6.7 26.4,8\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"7.1,5 9.5,3.3 12.2,2 15,1.3 18,1 21,1.3 23.8,2 26.5,3.3 28.9,5\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"</svg>";

			case LocoFunctionIconWaterPump:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"8,27 9,22 10,21 26,21 27,22 28,27 27,32 26,33 10,33 9,32 8,27\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"/>"
					"<polyline points=\"18,18 18,23\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"/>"
					"<polyline points=\"18,24 16,28\" stroke=\"black\" stroke-width=\"1\" fill=\"none\" class=\"button_on\"/>"
					"<polyline points=\"18,24 18,29\" stroke=\"black\" stroke-width=\"1\" fill=\"none\" class=\"button_on\"/>"
					"<polyline points=\"18,24 20,28\" stroke=\"black\" stroke-width=\"1\" fill=\"none\" class=\"button_on\"/>"
					"<polyline points=\"9,29 12,31 15,29 18,31 21,29 24,31 27,29\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"/>"
					"<polyline points=\"12.2,11.1 13.5,10.2 14.9,9.5 16.4,9.1 18,9 19.6,9.1 21.1,9.5 22.5,10.2 23.8,11.1\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound1\"/>"
					"<polyline points=\"9.6,8 11.5,6.7 13.6,5.8 15.7,5.2 18,5 20.3,5.2 22.4,5.8 24.5,6.7 26.4,8\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"7.1,5 9.5,3.3 12.2,2 15,1.3 18,1 21,1.3 23.8,2 26.5,3.3 28.9,5\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"</svg>";

			case LocoFunctionIconBufferPush:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"4,24 10,24\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"buffer_push\"/>"
					"<polyline points=\"9,19 10,22 10,26 9,29\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"buffer_push\"/>"
					"<polyline points=\"26,24 32,24\" stroke=\"black\" stroke-width=\"2\" fill=\"none\"/>"
					"<polyline points=\"27,19 26,22 26,26 27,29\" stroke=\"black\" stroke-width=\"2\" fill=\"none\"/>"
					"<polyline points=\"12.2,11.1 13.5,10.2 14.9,9.5 16.4,9.1 18,9 19.6,9.1 21.1,9.5 22.5,10.2 23.8,11.1\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound1\"/>"
					"<polyline points=\"9.6,8 11.5,6.7 13.6,5.8 15.7,5.2 18,5 20.3,5.2 22.4,5.8 24.5,6.7 26.4,8\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"7.1,5 9.5,3.3 12.2,2 15,1.3 18,1 21,1.3 23.8,2 26.5,3.3 28.9,5\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"</svg>";

			case LocoFunctionIconGenerator:
				return "<svg width=\"36\" height=\"36\">"
					"<circle r=\"7\" cx=\"10\" cy=\"13\" fill=\"black\" />"
					"<circle r=\"4\" cx=\"12\" cy=\"27\" fill=\"black\" />"
					"<line x1=\"3.5\" y1=\"13\" x2=\"8.5\" y2=\"27\" stroke-width=\"1\" stroke=\"black\" />"
					"<line x1=\"16.5\" y1=\"13\" x2=\"15.5\" y2=\"27\" stroke-width=\"1\" stroke=\"black\" />"
					"<circle r=\"5\" cx=\"10\" cy=\"13\" fill=\"lightgray\" />"
					"<circle r=\"2\" cx=\"12\" cy=\"27\" fill=\"lightgray\" />"
					"<polyline points=\"24.9,12.2 25.8,13.5 26.5,14.9 26.9,16.4 27,18 26.9,19.6 26.5,21.1 25.8,22.5 24.9,23.8\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound1\"/>"
					"<polyline points=\"28,9.6 29.3,11.5 30.2,13.6 30.8,15.7 31,18 30.8,20.3 30.2,22.4 29.3,24.5 28,26.4\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"31,7.1 32.7,9.5 34,12.2 34.7,15 35,18 34.7,21 34,23.8 32.7,26.5 31,28.9\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"</svg>";

			case LocoFunctionIconGearBox:
				return "<svg width=\"36\" height=\"36\">"
					"<line x1=\"10\" y1=\"4\" x2=\"10\" y2=\"22\" stroke-width=\"2\" stroke=\"black\" />"
					"<line x1=\"5.5\" y1=\"5.2\" x2=\"14.5\" y2=\"20.8\" stroke-width=\"2\" stroke=\"black\" />"
					"<line x1=\"2.2\" y1=\"8.5\" x2=\"17.8\" y2=\"17.5\" stroke-width=\"2\" stroke=\"black\" />"
					"<line x1=\"1\" y1=\"13\" x2=\"19\" y2=\"13\" stroke-width=\"2\" stroke=\"black\" />"
					"<line x1=\"5.5\" y1=\"20.8\" x2=\"14.5\" y2=\"5.2\" stroke-width=\"2\" stroke=\"black\" />"
					"<line x1=\"2.2\" y1=\"17.5\" x2=\"17.8\" y2=\"8.5\" stroke-width=\"2\" stroke=\"black\" />"
					"<circle r=\"7\" cx=\"10\" cy=\"13\" fill=\"black\" />"
					"<circle r=\"5\" cx=\"10\" cy=\"13\" fill=\"lightgray\" />"
					"<line x1=\"6\" y1=\"27\" x2=\"18\" y2=\"27\" stroke-width=\"2\" stroke=\"black\" />"
					"<line x1=\"16.2\" y1=\"31.2\" x2=\"7.8\" y2=\"22.8\" stroke-width=\"2\" stroke=\"black\" />"
					"<line x1=\"12\" y1=\"21\" x2=\"12\" y2=\"33\" stroke-width=\"2\" stroke=\"black\" />"
					"<line x1=\"7.8\" y1=\"31.2\" x2=\"16.2\" y2=\"22.8\" stroke-width=\"2\" stroke=\"black\" />"
					"<circle r=\"4\" cx=\"12\" cy=\"27\" fill=\"black\" />"
					"<circle r=\"2\" cx=\"12\" cy=\"27\" fill=\"lightgray\" />"
					"<polyline points=\"24.9,12.2 25.8,13.5 26.5,14.9 26.9,16.4 27,18 26.9,19.6 26.5,21.1 25.8,22.5 24.9,23.8\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound1\"/>"
					"<polyline points=\"28,9.6 29.3,11.5 30.2,13.6 30.8,15.7 31,18 30.8,20.3 30.2,22.4 29.3,24.5 28,26.4\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"31,7.1 32.7,9.5 34,12.2 34.7,15 35,18 34.7,21 34,23.8 32.7,26.5 31,28.9\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"</svg>";

			case LocoFunctionIconGearUp:
				return "<svg width=\"36\" height=\"36\">"
					"<line x1=\"10\" y1=\"4\" x2=\"10\" y2=\"22\" stroke-width=\"2\" stroke=\"black\" />"
					"<line x1=\"5.5\" y1=\"5.2\" x2=\"14.5\" y2=\"20.8\" stroke-width=\"2\" stroke=\"black\" />"
					"<line x1=\"2.2\" y1=\"8.5\" x2=\"17.8\" y2=\"17.5\" stroke-width=\"2\" stroke=\"black\" />"
					"<line x1=\"1\" y1=\"13\" x2=\"19\" y2=\"13\" stroke-width=\"2\" stroke=\"black\" />"
					"<line x1=\"5.5\" y1=\"20.8\" x2=\"14.5\" y2=\"5.2\" stroke-width=\"2\" stroke=\"black\" />"
					"<line x1=\"2.2\" y1=\"17.5\" x2=\"17.8\" y2=\"8.5\" stroke-width=\"2\" stroke=\"black\" />"
					"<circle r=\"7\" cx=\"10\" cy=\"13\" fill=\"black\" />"
					"<circle r=\"5\" cx=\"10\" cy=\"13\" fill=\"lightgray\" />"
					"<line x1=\"6\" y1=\"27\" x2=\"18\" y2=\"27\" stroke-width=\"2\" stroke=\"black\" />"
					"<line x1=\"16.2\" y1=\"31.2\" x2=\"7.8\" y2=\"22.8\" stroke-width=\"2\" stroke=\"black\" />"
					"<line x1=\"12\" y1=\"21\" x2=\"12\" y2=\"33\" stroke-width=\"2\" stroke=\"black\" />"
					"<line x1=\"7.8\" y1=\"31.2\" x2=\"16.2\" y2=\"22.8\" stroke-width=\"2\" stroke=\"black\" />"
					"<circle r=\"4\" cx=\"12\" cy=\"27\" fill=\"black\" />"
					"<circle r=\"2\" cx=\"12\" cy=\"27\" fill=\"lightgray\" />"
					"<polyline points=\"22,18 36,18\" stroke=\"black\" stroke-width=\"2\" fill=\"none\"/>"
					"<polyline points=\"29,11 29,25\" stroke=\"black\" stroke-width=\"2\" fill=\"none\"/>"
					"</svg>";

			case LocoFunctionIconGearDown:
				return "<svg width=\"36\" height=\"36\">"
					"<line x1=\"10\" y1=\"4\" x2=\"10\" y2=\"22\" stroke-width=\"2\" stroke=\"black\" />"
					"<line x1=\"5.5\" y1=\"5.2\" x2=\"14.5\" y2=\"20.8\" stroke-width=\"2\" stroke=\"black\" />"
					"<line x1=\"2.2\" y1=\"8.5\" x2=\"17.8\" y2=\"17.5\" stroke-width=\"2\" stroke=\"black\" />"
					"<line x1=\"1\" y1=\"13\" x2=\"19\" y2=\"13\" stroke-width=\"2\" stroke=\"black\" />"
					"<line x1=\"5.5\" y1=\"20.8\" x2=\"14.5\" y2=\"5.2\" stroke-width=\"2\" stroke=\"black\" />"
					"<line x1=\"2.2\" y1=\"17.5\" x2=\"17.8\" y2=\"8.5\" stroke-width=\"2\" stroke=\"black\" />"
					"<circle r=\"7\" cx=\"10\" cy=\"13\" fill=\"black\" />"
					"<circle r=\"5\" cx=\"10\" cy=\"13\" fill=\"lightgray\" />"
					"<line x1=\"6\" y1=\"27\" x2=\"18\" y2=\"27\" stroke-width=\"2\" stroke=\"black\" />"
					"<line x1=\"16.2\" y1=\"31.2\" x2=\"7.8\" y2=\"22.8\" stroke-width=\"2\" stroke=\"black\" />"
					"<line x1=\"12\" y1=\"21\" x2=\"12\" y2=\"33\" stroke-width=\"2\" stroke=\"black\" />"
					"<line x1=\"7.8\" y1=\"31.2\" x2=\"16.2\" y2=\"22.8\" stroke-width=\"2\" stroke=\"black\" />"
					"<circle r=\"4\" cx=\"12\" cy=\"27\" fill=\"black\" />"
					"<circle r=\"2\" cx=\"12\" cy=\"27\" fill=\"lightgray\" />"
					"<polyline points=\"22,18 36,18\" stroke=\"black\" stroke-width=\"2\" fill=\"none\"/>"
					"</svg>";

			case LocoFunctionIconFillWater:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"2,30 8,30\" stroke=\"black\" stroke-width=\"2\" fill=\"none\"/>"
					"<polyline points=\"5,30 5,10 16,10 17.5,10.5 18,12 18,14\" stroke=\"black\" stroke-width=\"2\" fill=\"none\"/>"
					"<polyline points=\"5,14 5,6\" stroke=\"black\" stroke-width=\"4\" fill=\"none\"/>"
					"<polyline points=\"18,15 16,19\" stroke=\"black\" stroke-width=\"1\" fill=\"none\" class=\"button_on\"/>"
					"<polyline points=\"18,15 18,20\" stroke=\"black\" stroke-width=\"1\" fill=\"none\" class=\"button_on\"/>"
					"<polyline points=\"18,15 20,19\" stroke=\"black\" stroke-width=\"1\" fill=\"none\" class=\"button_on\"/>"
					"<polyline points=\"24.9,12.2 25.8,13.5 26.5,14.9 26.9,16.4 27,18 26.9,19.6 26.5,21.1 25.8,22.5 24.9,23.8\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound1\"/>"
					"<polyline points=\"28,9.6 29.3,11.5 30.2,13.6 30.8,15.7 31,18 30.8,20.3 30.2,22.4 29.3,24.5 28,26.4\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"31,7.1 32.7,9.5 34,12.2 34.7,15 35,18 34.7,21 34,23.8 32.7,26.5 31,28.9\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"</svg>";

			case LocoFunctionIconFillDiesel:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"2,30 18,30\" stroke=\"black\" stroke-width=\"2\" fill=\"none\"/>"
					"<polyline points=\"4,30 4,8 4.5,6.5 6,6 14,6 15.5,6.5 16,8 16,30\" stroke=\"black\" stroke-width=\"2\" fill=\"none\"/>"
					"<polyline points=\"6.5,9.5 13.5,9.5 13.5,14.5 6.5,14.5 6.5,9.5\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"/>"
					"<polyline points=\"16,18 18.5,20.5 18.5,26.5 19.5,27.5 20.5,27.5 21.5,26.5 21.5,17.5 16,12.5\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"/>"
					"<polyline points=\"21,18 18,15\" stroke=\"black\" stroke-width=\"2\" fill=\"none\"/>"
					"<polyline points=\"24.9,12.2 25.8,13.5 26.5,14.9 26.9,16.4 27,18 26.9,19.6 26.5,21.1 25.8,22.5 24.9,23.8\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound1\"/>"
					"<polyline points=\"28,9.6 29.3,11.5 30.2,13.6 30.8,15.7 31,18 30.8,20.3 30.2,22.4 29.3,24.5 28,26.4\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"31,7.1 32.7,9.5 34,12.2 34.7,15 35,18 34.7,21 34,23.8 32.7,26.5 31,28.9\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"</svg>";

			case LocoFunctionIconFillGas:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"0,28 16,28\" stroke=\"black\" stroke-width=\"2\" fill=\"none\"/>"
					"<polyline points=\"10,18.5 16.5,18.5 18.5,20.5 18.5,26.5 19.5,27.5 20.5,27.5 21.5,26.5 21.5,17.5 16,12.5\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"/>"
					"<circle cx=\"8\" cy=\"20\" r=\"8\" fill=\"black\" />"
					"<circle cx=\"7\" cy=\"19\" r=\"6\" fill=\"darkgray\" />"
					"<circle cx=\"6\" cy=\"18\" r=\"4\" fill=\"lightgray\" />"
					"<circle cx=\"5\" cy=\"17\" r=\"2\" fill=\"white\" />"
					"<polyline points=\"21,18 18,15\" stroke=\"black\" stroke-width=\"2\" fill=\"none\"/>"
					"<polyline points=\"24.9,12.2 25.8,13.5 26.5,14.9 26.9,16.4 27,18 26.9,19.6 26.5,21.1 25.8,22.5 24.9,23.8\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound1\"/>"
					"<polyline points=\"28,9.6 29.3,11.5 30.2,13.6 30.8,15.7 31,18 30.8,20.3 30.2,22.4 29.3,24.5 28,26.4\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"31,7.1 32.7,9.5 34,12.2 34.7,15 35,18 34.7,21 34,23.8 32.7,26.5 31,28.9\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"</svg>";

			case LocoFunctionIconSand:
				return "<svg width=\"36\" height=\"36\">"
					"<circle r=\"6\" cx=\"8\" cy=\"20\" fill=\"black\" />"
					"<circle r=\"4.5\" cx=\"8\" cy=\"20\" fill=\"none\" stroke-with=\"1\" stroke=\"darkgray\" />"
					"<circle r=\"1\" cx=\"8\" cy=\"20\" fill=\"darkgray\" />"
					"<polyline points=\"13,25 17,21 17,19 14,16 14,12 21,12 21,16 18,19 18,21 14,25\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"\"/>"
					"<polyline points=\"24.9,12.2 25.8,13.5 26.5,14.9 26.9,16.4 27,18 26.9,19.6 26.5,21.1 25.8,22.5 24.9,23.8\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound1\"/>"
					"<polyline points=\"28,9.6 29.3,11.5 30.2,13.6 30.8,15.7 31,18 30.8,20.3 30.2,22.4 29.3,24.5 28,26.4\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"31,7.1 32.7,9.5 34,12.2 34.7,15 35,18 34.7,21 34,23.8 32.7,26.5 31,28.9\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"</svg>";

			case LocoFunctionIconRailJoint:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"1,13 11,13\" stroke=\"black\" stroke-width=\"1\"/>"
					"<polyline points=\"1,23 11,23\" stroke=\"black\" stroke-width=\"1\"/>"
					"<polyline points=\"12,13 22,13\" stroke=\"black\" stroke-width=\"1\"/>"
					"<polyline points=\"12,23 22,23\" stroke=\"black\" stroke-width=\"1\"/>"
					"<polyline points=\"3,11 3,25\" stroke=\"black\" stroke-width=\"1\"/>"
					"<polyline points=\"6,11 6,25\" stroke=\"black\" stroke-width=\"1\"/>"
					"<polyline points=\"9,11 9,25\" stroke=\"black\" stroke-width=\"1\"/>"
					"<polyline points=\"14,11 14,25\" stroke=\"black\" stroke-width=\"1\"/>"
					"<polyline points=\"17,11 17,25\" stroke=\"black\" stroke-width=\"1\"/>"
					"<polyline points=\"20,11 20,25\" stroke=\"black\" stroke-width=\"1\"/>"
					"<polyline points=\"24.9,12.2 25.8,13.5 26.5,14.9 26.9,16.4 27,18 26.9,19.6 26.5,21.1 25.8,22.5 24.9,23.8\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound1\"/>"
					"<polyline points=\"28,9.6 29.3,11.5 30.2,13.6 30.8,15.7 31,18 30.8,20.3 30.2,22.4 29.3,24.5 28,26.4\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"31,7.1 32.7,9.5 34,12.2 34.7,15 35,18 34.7,21 34,23.8 32.7,26.5 31,28.9\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"</svg>";

			case LocoFunctionIconCoupler:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"1,22 4,22 7,18 15,18 12,21 10,21 10,31 12,31 15,34 7,34 4,30 1,30 1,22\" stroke=\"black\" stroke-width=\"0\" fill=\"black\" class=\"buffer_push\"/>"
					"<polyline points=\"35,22 32,22 29,18 21,18 24,21 26,21 26,31 24,31 21,34 29,34 32,30 35,30 35,22\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"12.2,11.1 13.5,10.2 14.9,9.5 16.4,9.1 18,9 19.6,9.1 21.1,9.5 22.5,10.2 23.8,11.1\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound1\"/>"
					"<polyline points=\"9.6,8 11.5,6.7 13.6,5.8 15.7,5.2 18,5 20.3,5.2 22.4,5.8 24.5,6.7 26.4,8\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"7.1,5 9.5,3.3 12.2,2 15,1.3 18,1 21,1.3 23.8,2 26.5,3.3 28.9,5\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"</svg>";

			case LocoFunctionIconPanto:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"0,8.5 20,8.5\" stroke=\"black\" stroke-width=\"1\" fill=\"none\"/>"
					"<polyline points=\"2,27 18,27\" stroke=\"black\" stroke-width=\"2\" fill=\"none\"/>"
					"<polyline points=\"6,27 2,18 10,9 18,18 14,27\" stroke=\"black\" stroke-width=\"2\" fill=\"none\"/>"
					"<polyline points=\"24.9,12.2 25.8,13.5 26.5,14.9 26.9,16.4 27,18 26.9,19.6 26.5,21.1 25.8,22.5 24.9,23.8\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound1\"/>"
					"<polyline points=\"28,9.6 29.3,11.5 30.2,13.6 30.8,15.7 31,18 30.8,20.3 30.2,22.4 29.3,24.5 28,26.4\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"31,7.1 32.7,9.5 34,12.2 34.7,15 35,18 34.7,21 34,23.8 32.7,26.5 31,28.9\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"</svg>";

			case LocoFunctionIconMainSwitch:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"4,24 12,24 24,20\" stroke=\"black\" stroke-width=\"2\" fill=\"none\"/>"
					"<polyline points=\"22,24 32,24\" stroke=\"black\" stroke-width=\"2\" fill=\"none\"/>"
					"<polyline points=\"12.2,11.1 13.5,10.2 14.9,9.5 16.4,9.1 18,9 19.6,9.1 21.1,9.5 22.5,10.2 23.8,11.1\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound1\"/>"
					"<polyline points=\"9.6,8 11.5,6.7 13.6,5.8 15.7,5.2 18,5 20.3,5.2 22.4,5.8 24.5,6.7 26.4,8\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound2\"/>"
					"<polyline points=\"7.1,5 9.5,3.3 12.2,2 15,1.3 18,1 21,1.3 23.8,2 26.5,3.3 28.9,5\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on sound3\"/>"
					"</svg>";

			case LocoFunctionIconSoundLouder:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"8,15 10,15 10,21 8,21\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"11,15 18,8 18,28 11,21\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"22,18 36,18\" stroke=\"black\" stroke-width=\"2\" fill=\"none\"/>"
					"<polyline points=\"29,11 29,25\" stroke=\"black\" stroke-width=\"2\" fill=\"none\"/>"
					"</svg>";

			case LocoFunctionIconSoundLower:
				return "<svg width=\"36\" height=\"36\">"
					"<polyline points=\"8,15 10,15 10,21 8,21\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"11,15 18,8 18,28 11,21\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"/>"
					"<polyline points=\"22,18 36,18\" stroke=\"black\" stroke-width=\"2\" fill=\"none\"/>"
					"</svg>";

			case LocoFunctionIconNoBreak:
				return "<svg width=\"36\" height=\"36\">"
					"<circle r=\"6\" cx=\"12\" cy=\"18\" fill=\"black\" />"
					"<circle r=\"4.5\" cx=\"12\" cy=\"18\" fill=\"none\" stroke-with=\"1\" stroke=\"darkgray\" />"
					"<circle r=\"1\" cx=\"12\" cy=\"18\" fill=\"darkgray\" />"
					"<polyline points=\"4.2,22.5 3.5,21.1 3.1,19.6 3,18 3.1,16.4 3.5,14.9 4.2,13.5 6.6,13.5 5.9,14.5 5.4,15.6 5.1,16.8 5,18 5.1,19.2 5.4,20.4 5.9,21.5 6.6,22.5\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"\"/>"
					"<polyline points=\"19.8,13.5 20.5,14.9 20.9,16.4 21,18 20.9,19.6 20.5,21.1 19.8,22.5 17.4,22.5 18.1,21.5 18.6,20.4 18.9,19.2 19,18 18.9,16.8 18.6,15.6 18.1,14.5 17.4,13.5\" stroke=\"black\" stroke-width=\"0\" fill=\"black\"\"/>"
					"<polyline points=\"24.9,12.2 25.8,13.5 26.5,14.9 26.9,16.4 27,18 26.9,19.6 26.5,21.1 25.8,22.5 24.9,23.8\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_off sound1\"/>"
					"<polyline points=\"28,9.6 29.3,11.5 30.2,13.6 30.8,15.7 31,18 30.8,20.3 30.2,22.4 29.3,24.5 28,26.4\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_off sound2\"/>"
					"<polyline points=\"31,7.1 32.7,9.5 34,12.2 34.7,15 35,18 34.7,21 34,23.8 32.7,26.5 31,28.9\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_off sound3\"/>"
					"<polyline points=\"25,23 35,13\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on\"/>"
					"<polyline points=\"25,13 35,23\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" class=\"button_on\"/>"
					"</svg>";

			default:
				return "<svg width=\"36\" height=\"36\">"
					"<text x=\"8\" y=\"24\" fill=\"black\" font-size=\"12\">F" + std::to_string(nr) + "</text>"
					"</svg>";
		}
	}
} // namespace DataModel
