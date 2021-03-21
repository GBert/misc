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

#include "WebServer/HtmlTagButton.h"

namespace WebServer
{
	class HtmlTagButtonPlus : public HtmlTagButton
	{
		public:
			HtmlTagButtonPlus() = delete;
			HtmlTagButtonPlus(HtmlTagButtonPlus&) = delete;
			HtmlTagButtonPlus& operator=(HtmlTagButtonPlus&) = delete;

			inline HtmlTagButtonPlus(const std::string& field, const int max)
			:	HtmlTagButton(HtmlTag("span").AddContent("+"), field + "_plus")
			{
				AddAttribute("onclick", "incrementIntegerValue('" + field + "', " + std::to_string(max) + "); return false;");
				AddClass("small_button");
			}
	};
} // namespace WebServer
