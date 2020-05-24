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

#include <map>
#include <ostream>
#include <string>

#include "WebServer/HtmlTag.h"

namespace WebServer
{
	class Response
	{
		public:
			enum ResponseCode : unsigned short
			{
				OK = 200,
				NotFound = 404,
				NotImplemented = 501
			};

			Response() : responseCode(OK) {}
			Response(const ResponseCode responseCode, const HtmlTag& content) : responseCode(responseCode), content(content) {}
			virtual ~Response() {};
			void AddHeader(const std::string& key, const std::string& value);
			operator std::string();

			friend std::ostream& operator<<(std::ostream& stream, const Response& response);

			ResponseCode responseCode;

			typedef std::map<Response::ResponseCode,std::string> responseCodeMap;
			static const responseCodeMap responseTexts;

			std::map<const std::string,std::string> headers;
			HtmlTag content;
	};
}; // namespace WebServer

