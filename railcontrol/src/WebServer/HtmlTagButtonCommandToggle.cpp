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

#include <sstream>
#include <vector>

#include "Utils/Utils.h"
#include "WebServer/HtmlTagButtonCommandToggle.h"

namespace WebServer
{
	HtmlTagButtonCommandToggle::HtmlTagButtonCommandToggle(const std::string& value, const std::string& command, const bool on, const std::map<std::string,std::string>& arguments)
	:	HtmlTagButton(value, command)
	{
		AddClass(on == true ? "button_on" : "button_off");

		std::vector<std::string> parts;
		Utils::Utils::SplitString(command, "_", parts);
		std::stringstream ss;
		ss <<
			"var on = !document.getElementById('" << commandID << "').classList.contains('button_on');"
			"var theUrl = '/?cmd=" << parts[0] << "&on=' + (on ? '1' : '0') + '";

		for (auto argument : arguments) {

			ss << "&" << argument.first << "=" << argument.second;
		}
		ss <<"';"
			"fireRequestAndForget(theUrl);"
			"return false;";
		AddAttribute("onclick", ss.str());
	}
};
