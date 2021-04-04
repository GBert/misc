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

#include <sstream>

#include "WebServer/ResponseCsv.h"

namespace WebServer
{
	ResponseCsv::ResponseCsv(const std::string& content)
	:	Response(),
		csvContent(content)
	{
		AddHeader("Cache-Control", "no-cache, must-revalidate");
		AddHeader("Pragma", "no-cache");
		AddHeader("Expires", "Sun, 12 Feb 2016 00:00:00 GMT");
		AddHeader("Content-Type", "text/csv; charset=utf-8");
		AddHeader("Connection", "keep-alive");
	}

	ResponseCsv::operator std::string()
	{
		std::stringstream reply;
		reply << *this;
		return reply.str();
	}

	std::ostream& operator<<(std::ostream& stream, const ResponseCsv& response)
	{
		stream << "HTTP/1.1 " << response.responseCode << " " << Response::responseTexts.at(response.responseCode) << "\r\n";
		for(auto header : response.headers)
		{
			stream << header.first << ": " << header.second << "\r\n";
		}

		stream << "Content-Length: " << response.csvContent.size();
		stream << "\r\n\r\n";
		stream << response.csvContent;
		return stream;
	}
} // namespace WebServer
