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

#include <ostream>
#include <string>

#include "HtmlResponse.h"

namespace WebServer
{
	class HtmlFullResponse : public HtmlResponse
	{
		public:
			HtmlFullResponse() = delete;
			HtmlFullResponse(const ResponseCode responseCode);
			HtmlFullResponse(const std::string& title, const HtmlTag body);
			HtmlFullResponse(const ResponseCode responseCode, const std::string& title, const HtmlTag body);
			~HtmlFullResponse() {};
			operator std::string();

			friend std::ostream& operator<<(std::ostream& stream, const HtmlFullResponse& response);
	};
}; // namespace WebServer

