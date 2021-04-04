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

#include "WebServer/ResponseHtml.h"

namespace WebServer
{
	class ResponseHtmlFull : public ResponseHtml
	{
		public:
			ResponseHtmlFull() = delete;
			ResponseHtmlFull(const ResponseHtmlFull&) = delete;
			ResponseHtmlFull& operator=(const ResponseHtmlFull&) = delete;

			inline ResponseHtmlFull(const ResponseCode responseCode)
			:	ResponseHtml(responseCode)
			{
			}

			inline ResponseHtmlFull(const std::string& title, const HtmlTag body)
			:	ResponseHtml(title, body)
			{
			}

			ResponseHtmlFull(const ResponseCode responseCode, const std::string& title, const HtmlTag body)
			:	ResponseHtml(responseCode, title, body)
			{
			}

			virtual ~ResponseHtmlFull()
			{
			}

			operator std::string();

			friend std::ostream& operator<<(std::ostream& stream, const ResponseHtmlFull& response);
	};
} // namespace WebServer

