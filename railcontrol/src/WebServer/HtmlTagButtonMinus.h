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
	class HtmlTagButtonMinus : public HtmlTagButton
	{
		public:
			HtmlTagButtonMinus() = delete;
			HtmlTagButtonMinus(HtmlTagButtonMinus&) = delete;
			HtmlTagButtonMinus& operator=(HtmlTagButtonMinus&) = delete;

			inline HtmlTagButtonMinus(const std::string& field, const int min)
			:	HtmlTagButton(HtmlTag("span").AddContent("-"), field + "_minus")
			{
				AddAttribute("onclick", "decrementIntegerValue('" + field + "', " + std::to_string(min) + "); return false;");
				AddClass("small_button");
			}
	};
} // namespace WebServer
