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

#include <sstream>

#include "DataModel/Street.h"
#include "WebServer/HtmlTagStreet.h"

using std::string;
using std::to_string;

namespace WebServer
{
	HtmlTagStreet::HtmlTagStreet(const DataModel::Street* street)
	:	HtmlTagLayoutItem()
	{
		unsigned int layoutPosX = street->GetPosX() * EdgeLength;
		unsigned int layoutPosY = street->GetPosY() * EdgeLength;
		const string& streetName = street->GetName();

		HtmlTag div1("div");
		string streetIdString = to_string(street->GetID());
		string id("st_" + streetIdString);
		div1.AddAttribute("id", id);
		div1.AddClass("layout_item");
		div1.AddClass("street_item");
		div1.AddAttribute("style", "left:" + to_string(layoutPosX) + "px;top:" + to_string(layoutPosY) + "px;");
		string image = "<svg width=\"" + EdgeLengthString + "\" height=\"" + EdgeLengthString + "\" id=\"_img\"><polygon points=\"1,21 7,21 7,29 1,29\" fill=\"none\" stroke=\"black\"/><polygon points=\"35,7 29,7 29,15 35,15\" fill=\"none\" stroke=\"black\"/><polyline points=\"7,25 15,25 21,11 29,11\" stroke=\"black\" fill=\"none\"/></svg>";
		div1.AddChildTag(HtmlTag().AddContent(image));
		div1.AddChildTag(HtmlTag("span").AddClass("tooltip").AddContent(streetName));
		div1.AddAttribute("onclick", "return onClickStreet(" + streetIdString + ");");
		div1.AddAttribute("oncontextmenu", "return onContextLayoutItem(event, '" + id + "');");
		AddChildTag(div1);

		HtmlTag div2("div");
		div2.AddClass("contextmenu");
		div2.AddAttribute("id", id + "_context");
		div2.AddAttribute("style", "left:" + to_string(layoutPosX + 5) + "px;top:" + to_string(layoutPosY + 30) + "px;");
		div2.AddChildTag(HtmlTag("ul").AddClass("contextentries")
			.AddChildTag(HtmlTag("li").AddClass("contextentry").AddContent(streetName))
			.AddChildTag(HtmlTag("li").AddClass("contextentry").AddContent(Languages::TextReleaseStreet).AddAttribute("onClick", "fireRequestAndForget('/?cmd=streetrelease&switch=" + streetIdString + "');"))
			.AddChildTag(HtmlTag("li").AddClass("contextentry").AddContent(Languages::TextEditStreet).AddAttribute("onClick", "loadPopup('/?cmd=streetedit&street=" + streetIdString + "');"))
			.AddChildTag(HtmlTag("li").AddClass("contextentry").AddContent(Languages::TextDeleteStreet).AddAttribute("onClick", "loadPopup('/?cmd=streetaskdelete&street=" + streetIdString + "');"))
			);
		AddChildTag(div2);
	}
};
