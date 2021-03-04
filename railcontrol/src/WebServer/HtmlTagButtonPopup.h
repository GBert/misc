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

#include <map>
#include <string>

#include "Languages.h"
#include "WebServer/HtmlTagButton.h"

namespace WebServer
{
	class HtmlTagButtonPopup : public HtmlTagButton
	{
		public:
			HtmlTagButtonPopup() = delete;

			HtmlTagButtonPopup(const std::string& value,
				const std::string& command,
				const std::map<std::string,std::string>& arguments = std::map<std::string,std::string>(),
				const std::string& tooltip = "");

			HtmlTagButtonPopup(const Languages::TextSelector value,
			const std::string& command,
			const std::map<std::string,std::string>& arguments = std::map<std::string,std::string>(),
			const std::string& tooltip = "")
			:	HtmlTagButtonPopup(Languages::GetText(value), command, arguments, tooltip)
			{}

			HtmlTagButtonPopup(const std::string& value,
			const std::string& command,
			const std::map<std::string,std::string>& arguments,
			const Languages::TextSelector tooltip)
			:	HtmlTagButtonPopup(value, command, arguments, Languages::GetText(tooltip))
			{}

			HtmlTagButtonPopup(const std::string& value,
			const std::string& command,
			const Languages::TextSelector tooltip)
			:	HtmlTagButtonPopup(value, command, std::map<std::string,std::string>(), Languages::GetText(tooltip))
			{}
	};
} // namespace WebServer
