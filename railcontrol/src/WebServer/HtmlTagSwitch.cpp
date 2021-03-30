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

#include "DataModel/Switch.h"
#include "WebServer/HtmlTagSwitch.h"

using std::string;
using std::to_string;

namespace WebServer
{
	HtmlTagSwitch::HtmlTagSwitch(const DataModel::Switch* mySwitch)
	:	HtmlTagLayoutItem()
	{
		const DataModel::AccessoryState state = mySwitch->GetAccessoryState();
		const DataModel::AccessoryType type = mySwitch->GetType();

		const unsigned int layoutPosX = mySwitch->GetPosX() * EdgeLength;
		const unsigned int layoutPosY = mySwitch->GetPosY() * EdgeLength;
		const string& switchName = mySwitch->GetName();

		HtmlTag div1("div");
		string switchIdString = to_string(mySwitch->GetID());
		string id("sw_" + switchIdString);
		div1.AddId(id);
		div1.AddClass("layout_item");
		switch (type)
		{
			default:
				div1.AddClass("switch_item");
				div1.AddAttribute("onclick", "return onClickSwitch(" + switchIdString + ");");
				break;
		}
		switch (state)
		{
			case DataModel::SwitchStateTurnout:
				div1.AddClass("switch_turnout");
				break;

			case DataModel::SwitchStateStraight:
				div1.AddClass("switch_straight");
				break;

			case DataModel::SwitchStateThird:
				div1.AddClass("switch_third");
				break;

			default:
				break;
		}
		div1.AddAttribute("style", "left:" + to_string(layoutPosX) + "px;top:" + to_string(layoutPosY) + "px;");
		string image;
		switch (type)
		{
			case DataModel::SwitchTypeLeft:
				image = "<svg width=\"" + EdgeLengthString + "\" height=\"" + EdgeLengthString + "\" id=\"" + id + "_img\" style=\"transform:rotate(" + DataModel::LayoutItem::Rotation(mySwitch->GetRotation()) + "deg);\">"
					"<polygon points=\"14,28 22,36 14,36\" fill=\"black\" />"
					"<polygon points=\"0,14 14,28 14,36 0,22\" fill=\"gray\" class=\"turnout\"/>"
					"<polygon points=\"14,0 22,0 22,36 14,28\" fill=\"gray\" class=\"straight\"/>"
					"</svg>";
				break;

			case DataModel::SwitchTypeRight:
				image = "<svg width=\"" + EdgeLengthString + "\" height=\"" + EdgeLengthString + "\" id=\"" + id + "_img\" style=\"transform:rotate(" + DataModel::LayoutItem::Rotation(mySwitch->GetRotation()) + "deg);\">"
					"<polygon points=\"22,28 22,36 14,36\" fill=\"black\" />"
					"<polygon points=\"22,28 36,14 36,22 22,36\" fill=\"gray\" class=\"turnout\"/>"
					"<polygon points=\"14,0 22,0 22,28 14,36\" fill=\"gray\" class=\"straight\"/>"
					"</svg>";
				break;

			case DataModel::SwitchTypeThreeWay:
				image = "<svg width=\"" + EdgeLengthString + "\" height=\"" + EdgeLengthString + "\" id=\"" + id + "_img\" style=\"transform:rotate(" + DataModel::LayoutItem::Rotation(mySwitch->GetRotation()) + "deg);\">"
					"<polygon points=\"18,32 22,36 14,36\" fill=\"black\" />"
					"<polygon points=\"0,14 14,28 14,36 0,22\" fill=\"gray\" class=\"turnout\"/>"
					"<polygon points=\"14,36 14,28 18,32\" fill=\"gray\" class=\"turnout straight\"/>"
					"<polygon points=\"14,0 22,0 22,28 18,32 14,28\" fill=\"gray\" class=\"straight\"/>"
					"<polygon points=\"22,28 22,36 18,32\" fill=\"gray\" class=\"straight third\"/>"
					"<polygon points=\"22,28 36,14 36,22 22,36\" fill=\"gray\" class=\"third\"/>"
					"</svg>";
				break;

			default:
				break;
		}

		div1.AddChildTag(HtmlTag().AddContent(image));
		div1.AddChildTag(HtmlTag("span").AddClass("tooltip").AddContent(switchName + " (addr=" + to_string(mySwitch->GetAddress()) + ")"));
		div1.AddAttribute("oncontextmenu", "return onContextLayoutItem(event, '" + id + "');");
		AddChildTag(div1);

		HtmlTag div2("div");
		div2.AddClass("contextmenu");
		div2.AddId(id + "_context");
		div2.AddAttribute("style", "left:" + to_string(layoutPosX + 5) + "px;top:" + to_string(layoutPosY + 30) + "px;");
		div2.AddChildTag(HtmlTag("ul").AddClass("contextentries")
			.AddChildTag(HtmlTag("li").AddClass("contextentry").AddContent(switchName))
			.AddChildTag(HtmlTag("li").AddClass("contextentry").AddContent(Languages::TextReleaseSwitch).AddAttribute("onClick", "fireRequestAndForget('/?cmd=switchrelease&switch=" + switchIdString + "');"))
			.AddChildTag(HtmlTag("li").AddClass("contextentry").AddContent(Languages::TextEditSwitch).AddAttribute("onClick", "loadPopup('/?cmd=switchedit&switch=" + switchIdString + "');"))
			.AddChildTag(HtmlTag("li").AddClass("contextentry").AddContent(Languages::TextDeleteSwitch).AddAttribute("onClick", "loadPopup('/?cmd=switchaskdelete&switch=" + switchIdString + "');"))
			);
		AddChildTag(div2);
	}
} // namespace WebServer
