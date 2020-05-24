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

#include <string>

#include "Logger/Logger.h"
#include "WebServer/HtmlTag.h"

namespace WebServer
{
	class HtmlTagLabel : public HtmlTag
	{
		public:
			HtmlTagLabel() = delete;

			HtmlTagLabel(const Languages::TextSelector label, const std::string& reference)
			: HtmlTag("label")
			{
				std::string stringLabel = Languages::GetText(label);
				stringLabel.append(":");
				AddContent(stringLabel);
				AddAttribute("for", reference);
			};

			template<typename... Args>
			HtmlTagLabel(const Languages::TextSelector label, const std::string& reference, Args... args)
			: HtmlTag("label")
			{
				std::string stringLabel = Logger::Logger::Format(Languages::GetText(label), args...);
				stringLabel.append(":");
				AddContent(stringLabel);
				AddAttribute("for", reference);
			};
	};
};

