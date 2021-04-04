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

#include <ostream>
#include <string>

#include "WebServer/Response.h"

namespace WebServer
{
	class ResponseHtml : public Response
	{
		public:
			ResponseHtml() = delete;
			ResponseHtml(const ResponseHtml&) = delete;
			ResponseHtml& operator=(const ResponseHtml&) = delete;

			inline ResponseHtml(const ResponseCode responseCode)
			:	ResponseHtml(responseCode, std::to_string(responseCode) + " " + ResponseHtml::responseTexts.at(responseCode), HtmlTag("body"))
			{
			}

			inline ResponseHtml(const HtmlTag body)
			:	ResponseHtml("", body)
			{
			}

			inline ResponseHtml(const std::string& title, const HtmlTag body)
			:	ResponseHtml(Response::OK, title, body)
			{
			}

			ResponseHtml(const ResponseCode responseCode, const std::string& title, const HtmlTag body);

			virtual ~ResponseHtml()
			{
			}

			void AddAttribute(const std::string name, const std::string value);
			void AddChildTag(HtmlTag content);
			operator std::string();

			friend std::ostream& operator<<(std::ostream& stream, const ResponseHtml& response);

		protected:
			std::string title;
	};
} // namespace WebServer

