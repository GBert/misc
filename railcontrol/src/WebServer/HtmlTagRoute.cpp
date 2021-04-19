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
		const unsigned int layoutPosX = route->GetPosX() * EdgeLength;
		const unsigned int layoutPosY = route->GetPosY() * EdgeLength;
		const string& routeName = route->GetName();

		HtmlTag div1("div");
		string routeIdString = to_string(route->GetID());
		string id("r_" + routeIdString);
		div1.AddId(id);
		div1.AddClass("layout_item");
		div1.AddClass("route_item");
		div1.AddAttribute("style", "left:" + to_string(layoutPosX) + "px;top:" + to_string(layoutPosY) + "px;");
		string image = "<svg width=\"" + EdgeLengthString + "\" height=\"" + EdgeLengthString + "\" id=\"_img\"><polygon points=\"1,21 7,21 7,29 1,29\" fill=\"none\" stroke=\"black\"/><polygon points=\"35,7 29,7 29,15 35,15\" fill=\"none\" stroke=\"black\"/><polyline points=\"7,25 15,25 21,11 29,11\" stroke=\"black\" fill=\"none\"/></svg>";
		div1.AddChildTag(HtmlTag().AddContent(image));
		div1.AddChildTag(HtmlTag("span").AddClass("tooltip").AddContent(routeName));
		div1.AddAttribute("onclick", "return onClickRoute(" + routeIdString + ");");
		div1.AddAttribute("oncontextmenu", "return showContextMenu(event, '" + id + "');");
		AddChildTag(div1);

		HtmlTag div2("div");
		div2.AddClass("contextmenu");
		div2.AddId(id + "_context");
		div2.AddAttribute("style", "left:" + to_string(layoutPosX + 5) + "px;top:" + to_string(layoutPosY + 30) + "px;");
		div2.AddChildTag(HtmlTag("ul").AddClass("contextentries")
			.AddChildTag(HtmlTag("li").AddClass("contextentry").AddContent(routeName))
			.AddChildTag(HtmlTag("li").AddClass("contextentry").AddContent(Languages::TextReleaseRoute).AddAttribute("onClick", "fireRequestAndForget('/?cmd=routerelease&route=" + routeIdString + "');"))
			.AddChildTag(HtmlTag("li").AddClass("contextentry").AddContent(Languages::TextEditRoute).AddAttribute("onClick", "loadPopup('/?cmd=routeedit&route=" + routeIdString + "');"))
			.AddChildTag(HtmlTag("li").AddClass("contextentry").AddContent(Languages::TextDeleteRoute).AddAttribute("onClick", "loadPopup('/?cmd=routeaskdelete&route=" + routeIdString + "');"))
			);
		AddChildTag(div2);
	}
} // namespace WebServer
