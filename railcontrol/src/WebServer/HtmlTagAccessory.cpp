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

#include "DataModel/Accessory.h"
#include "WebServer/HtmlTagAccessory.h"

using std::string;
using std::to_string;

namespace WebServer
{
	HtmlTagAccessory::HtmlTagAccessory(const DataModel::Accessory* accessory)
	:	HtmlTagLayoutItem()
	{
		DataModel::AccessoryState state = accessory->GetAccessoryState();

		unsigned int layoutPosX = accessory->GetPosX() * EdgeLength;
		unsigned int layoutPosY = accessory->GetPosY() * EdgeLength;
		const string& accessoryName = accessory->GetName();

		HtmlTag div1("div");
		string accessoryIdString = to_string(accessory->GetID());
		string id("a_" + accessoryIdString);
		div1.AddAttribute("id", id);
		div1.AddClass("layout_item");
		div1.AddClass("accessory_item");
		div1.AddClass(state == DataModel::AccessoryStateOn ? "accessory_on" : "accessory_off");
		div1.AddAttribute("style", "left:" + to_string(layoutPosX) + "px;top:" + to_string(layoutPosY) + "px;");
		div1.AddChildTag(HtmlTag("span").AddContent("&#x25A0;"));
		div1.AddChildTag(HtmlTag("span").AddClass("tooltip").AddContent(accessoryName + " (addr=" + to_string(accessory->GetAddress()) + ")"));
		div1.AddAttribute("onclick", "return onClickAccessory(" + accessoryIdString + ");");
		div1.AddAttribute("oncontextmenu", "return onContextLayoutItem(event, '" + id + "');");
		AddChildTag(div1);

		HtmlTag div2("div");
		div2.AddClass("contextmenu");
		div2.AddAttribute("id", id + "_context");
		div2.AddAttribute("style", "left:" + to_string(layoutPosX + 5) + "px;top:" + to_string(layoutPosY + 30) + "px;");
		div2.AddChildTag(HtmlTag("ul").AddClass("contextentries")
			.AddChildTag(HtmlTag("li").AddClass("contextentry").AddContent(accessoryName))
			.AddChildTag(HtmlTag("li").AddClass("contextentry").AddContent(Languages::TextReleaseAccessory).AddAttribute("onClick", "fireRequestAndForget('/?cmd=accessoryrelease&accessory=" + accessoryIdString + "');"))
			.AddChildTag(HtmlTag("li").AddClass("contextentry").AddContent(Languages::TextEditAccessory).AddAttribute("onClick", "loadPopup('/?cmd=accessoryedit&accessory=" + accessoryIdString + "');"))
			.AddChildTag(HtmlTag("li").AddClass("contextentry").AddContent(Languages::TextDeleteAccessory).AddAttribute("onClick", "loadPopup('/?cmd=accessoryaskdelete&accessory=" + accessoryIdString + "');"))
			);
		AddChildTag(div2);
	}
};
