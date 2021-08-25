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

#include <string>

#include "WebServer/HtmlTagButtonMinus.h"
#include "WebServer/HtmlTagButtonPlus.h"
#include "WebServer/HtmlTagInput.h"
#include "WebServer/HtmlTagInputInteger.h"

namespace WebServer
{
	HtmlTagInputInteger::HtmlTagInputInteger(const std::string& name, const int value, const int min, const int max)
	:	HtmlTag("div")
	{
		std::string minString = std::to_string(min);
		std::string maxString = std::to_string(max);
		AddId("d_" + name);
		AddClass("div_integer");
		HtmlTagInput input("text", name, std::to_string(value));
		input.AddClass("integer");
		input.AddAttribute("onchange", "checkIntegerValue('" + name + "', " + minString + ", " + maxString + ");");
		AddChildTag(input);

		AddChildTag(HtmlTagButtonMinus(name, min));
		AddChildTag(HtmlTagButtonPlus(name, max));
	}
} // namespace WebServer
