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

#include "WebServer/Response.h"

namespace WebServer
{
	class HtmlResponse : public Response
	{
		public:
			HtmlResponse() = delete;
			HtmlResponse(const ResponseCode responseCode)
			:	HtmlResponse(responseCode, std::to_string(responseCode) + " " + HtmlResponse::responseTexts.at(responseCode), HtmlTag("body"))
			{}

			HtmlResponse(const HtmlTag body)
			:	HtmlResponse("", body)
			{}

			HtmlResponse(const std::string& title, const HtmlTag body)
			:	HtmlResponse(Response::OK, title, body)
			{}

			HtmlResponse(const ResponseCode responseCode, const std::string& title, const HtmlTag body);
			virtual ~HtmlResponse() {};
			void AddAttribute(const std::string name, const std::string value);
			void AddChildTag(HtmlTag content);
			operator std::string();

			friend std::ostream& operator<<(std::ostream& stream, const HtmlResponse& response);

		protected:
			std::string title;
	};
}; // namespace WebServer

