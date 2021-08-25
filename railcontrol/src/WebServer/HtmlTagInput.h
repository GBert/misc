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

#include <string>

#include "WebServer/HtmlTag.h"

namespace WebServer
{
	class HtmlTagInput : public HtmlTag
	{
		public:
			HtmlTagInput() = delete;
			HtmlTagInput(const HtmlTagInput&) = delete;
			HtmlTagInput& operator=(const HtmlTagInput&) = delete;

			HtmlTagInput(const std::string& type,
				const std::string& name,
				const std::string& value = "",
				const bool disabled = false);
	};
} // namespace WebServer

