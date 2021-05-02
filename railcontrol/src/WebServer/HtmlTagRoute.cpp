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

#include "DataModel/Route.h"
#include "WebServer/HtmlTagRoute.h"

using std::string;
using std::to_string;

namespace WebServer
{
	HtmlTagRoute::HtmlTagRoute(const DataModel::Route* route)
	:	HtmlTagLayoutItem(dynamic_cast<const DataModel::LayoutItem*>(route))
	{
		image += "<svg width=\"" + EdgeLengthString + "\" height=\"" + EdgeLengthString + "\" id=\"_img\"><polygon points=\"1,21 7,21 7,29 1,29\" fill=\"none\" stroke=\"white\"/><polygon points=\"35,7 29,7 29,15 35,15\" fill=\"none\" stroke=\"white\"/><polyline points=\"7,25 15,25 21,11 29,11\" stroke=\"white\" fill=\"none\"/></svg>";

		string routeIdString = to_string(route->GetID());
		imageDiv.AddClass("route_item");
		imageDiv.AddAttribute("onclick", "return onClickRoute(" + routeIdString + ");");

		const string& routeName = route->GetName();
		AddToolTip(routeName);
		AddContextMenuEntry(routeName);
		AddContextMenuEntry(Languages::TextReleaseRoute, "fireRequestAndForget('/?cmd=routerelease&route=" + routeIdString + "');");
		AddContextMenuEntry(Languages::TextEditRoute, "loadPopup('/?cmd=routeedit&route=" + routeIdString + "');");
		AddContextMenuEntry(Languages::TextDeleteRoute, "loadPopup('/?cmd=routeaskdelete&route=" + routeIdString + "');");
		FinishInit();
	}
} // namespace WebServer
