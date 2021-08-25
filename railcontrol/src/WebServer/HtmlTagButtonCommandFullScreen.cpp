/*
RailControl - Model Railway Control Software

Copyright (c) 2017-2021.5 Dominik (Teddy) Mahrer - www.railcontrol.org

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

#include "Languages.h"
#include "WebServer/HtmlTagButtonCommandFullScreen.h"

namespace WebServer
{
	HtmlTagButtonCommandFullScreen::HtmlTagButtonCommandFullScreen()
	:	HtmlTagButton("<svg width=\"36\" height=\"36\">"
			"<polyline points=\"9.5,9.5 26.5,26.5\" fill=\"none\" stroke=\"black\"/>"
			"<polyline points=\"9.5,26.5 26.5,9.5\" fill=\"none\" stroke=\"black\"/>"
			"<polyline points=\"9.5,14.5 9.5,9.5 14.5,9.5\" fill=\"none\" stroke=\"black\"/>"
			"<polyline points=\"21.5,9.5 26.5,9.5 26.5,14.5\" fill=\"none\" stroke=\"black\"/>"
			"<polyline points=\"26.5,21.5 26.5,26.5 21.5,26.5\" fill=\"none\" stroke=\"black\"/>"
			"<polyline points=\"14.5,26.5 9.5,26.5 9.5,21.5\" fill=\"none\" stroke=\"black\"/>"
			"</svg>", "fullscreen", Languages::GetText(Languages::TextFullScreen))
	{
		AddAttribute("onclick", "fullScreen(); return false;");
	}
} // namespace WebServer
