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

#include "WebServer/ResponseHtmlFull.h"

namespace WebServer
{
	ResponseHtmlFull::operator std::string()
	{
		std::stringstream reply;
		reply << *this;
		return reply.str();
	}

	std::ostream& operator<<(std::ostream& stream, const ResponseHtmlFull& response)
	{
		stream << "HTTP/1.1 " << response.responseCode << " " << ResponseHtml::responseTexts.at(response.responseCode) << "\r\n";
		for (auto& header : response.headers)
		{
			stream << header.first << ": " << header.second << "\r\n";
		}

		std::stringstream body;
		body << "<!DOCTYPE html>";

		HtmlTag head("head");
		head.AddChildTag(HtmlTag("title").AddId("title").AddContent(response.title));
		head.AddChildTag(HtmlTag("link").AddAttribute("rel", "stylesheet").AddAttribute("type", "text/css").AddAttribute("href", "/style.css"));
		head.AddChildTag(HtmlTag("script").AddAttribute("type", "application/javascript").AddAttribute("src", "/nosleep.js"));
		head.AddChildTag(HtmlTag("script").AddAttribute("type", "application/javascript").AddAttribute("src", "/javascript.js"));
		head.AddChildTag(HtmlTag("meta").AddAttribute("name", "viewport").AddAttribute("content", "width=device-width, initial-scale=1.0, minimum-scale=1.0, maximum-scale=1.0"));
		head.AddChildTag(HtmlTag("meta").AddAttribute("name", "robots").AddAttribute("content", "noindex,nofollow"));

		body << HtmlTag("html").AddChildTag(head).AddChildTag(response.content);

		std::string bodyString(body.str());

		stream << "Content-Length: " << bodyString.size();
		stream << "\r\n\r\n";
		stream << bodyString;
		return stream;
	}
} // namespace WebServer
