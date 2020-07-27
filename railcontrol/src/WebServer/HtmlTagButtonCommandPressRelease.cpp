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

#include "Utils/Utils.h"
#include "WebServer/HtmlTagButtonCommandPressRelease.h"

namespace WebServer
{
	HtmlTagButtonCommandPressRelease::HtmlTagButtonCommandPressRelease(const std::string& value,
		const std::string& command,
		const std::map<std::string,std::string>& arguments,
		const std::string& tooltip)
	:	HtmlTagButton(value, command, tooltip)
	{
		std::string cmd = Utils::Utils::StringBeforeDelimiter(command, "_");
		std::stringstream begin;
		begin << "var url = '/?cmd=" << cmd << "&on=";
		std::stringstream end;
		for (auto argument : arguments) {

			end << "&" << argument.first << "=" << argument.second;
		}
		end <<"';fireRequestAndForget(url);return false;";
		const std::string beginString = begin.str();
		const std::string endString = end.str();
		const std::string down = beginString + "1" + endString;
		const std::string up = beginString + "0" + endString;
		AddAttribute("onmousedown", down);
		AddAttribute("onmouseup", up);
		AddAttribute("ontouchstart", down);
		AddAttribute("ontouchend", up);
	}
} // namespace WebServer
