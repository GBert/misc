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

#include "DataModel/Signal.h"
#include "WebServer/HtmlTagSignal.h"

using std::string;
using std::to_string;

namespace WebServer
{
	HtmlTagSignal::HtmlTagSignal(const DataModel::Signal* signal)
	:	HtmlTagLayoutItem()
	{
		signalState_t state = signal->GetState();
		signalType_t type = signal->GetType();

		unsigned int layoutPosX = signal->GetPosX() * EdgeLength;
		unsigned int layoutPosY = signal->GetPosY() * EdgeLength;
		const string& signalName = signal->GetName();

		HtmlTag div1("div");
		string signalIdString = to_string(signal->GetID());
		string id("si_" + signalIdString);
		div1.AddAttribute("id", id);
		div1.AddClass("layout_item");
		div1.AddClass("signal_item");
		div1.AddClass(state == DataModel::Signal::SignalStateRed ? "signal_red" : "signal_green");
		div1.AddAttribute("style", "left:" + to_string(layoutPosX) + "px;top:" + to_string(layoutPosY) + "px;");
		string image;
		switch (type)
		{
			case DataModel::Signal::SignalTypeSimpleRight:
				image = "<svg width=\"" + EdgeLengthString + "\" height=\"" + EdgeLengthString + "\" id=\"" + id + "_img\" style=\"transform:rotate(" + DataModel::LayoutItem::Rotation(signal->GetRotation()) + "deg);\"><polygon points=\"14,0 22,0 22,36 14,36\" fill=\"black\" /><polygon points=\"23,5 27,1 31,1 35,5 35,18 31,22 27,22 23,18\" fill=\"black\"/><polyline points=\"29,7 29,30\" style=\"stroke:black;stroke-width:2\"/><circle class=\"red\" cx=\"29\" cy=\"7\" r=\"4\" fill=\"darkgray\"/><circle class=\"green\" cx=\"29\" cy=\"16\" r=\"4\" fill=\"darkgray\"/></svg>";
				break;

			case DataModel::Signal::SignalTypeSimpleLeft:
			default:
				image = "<svg width=\"" + EdgeLengthString + "\" height=\"" + EdgeLengthString + "\" id=\"" + id + "_img\" style=\"transform:rotate(" + DataModel::LayoutItem::Rotation(signal->GetRotation()) + "deg);\"><polygon points=\"14,0 22,0 22,36 14,36\" fill=\"black\" /><polygon points=\"1,5 5,1 9,1 13,5 13,18 9,22 5,22 1,18\" fill=\"black\"/><polyline points=\"7,7 7,30\" style=\"stroke:black;stroke-width:2\"/><circle class=\"red\" cx=\"7\" cy=\"7\" r=\"4\" fill=\"darkgray\"/><circle class=\"green\" cx=\"7\" cy=\"16\" r=\"4\" fill=\"darkgray\"/></svg>";
				break;
		}
		div1.AddChildTag(HtmlTag().AddContent(image));
		div1.AddChildTag(HtmlTag("span").AddClass("tooltip").AddContent(signalName + " (addr=" + to_string(signal->GetAddress()) + ")"));
		div1.AddAttribute("onclick", "return onClickSignal(" + signalIdString + ");");
		div1.AddAttribute("oncontextmenu", "return onContextLayoutItem(event, '" + id + "');");
		AddChildTag(div1);

		HtmlTag div2("div");
		div2.AddClass("contextmenu");
		div2.AddAttribute("id", id + "_context");
		div2.AddAttribute("style", "left:" + to_string(layoutPosX + 5) + "px;top:" + to_string(layoutPosY + 30) + "px;");
		div2.AddChildTag(HtmlTag("ul").AddClass("contextentries")
			.AddChildTag(HtmlTag("li").AddClass("contextentry").AddContent(signalName))
			.AddChildTag(HtmlTag("li").AddClass("contextentry").AddContent(Languages::TextReleaseSignal).AddAttribute("onClick", "fireRequestAndForget('/?cmd=signalrelease&signal=" + signalIdString + "');"))
			.AddChildTag(HtmlTag("li").AddClass("contextentry").AddContent(Languages::TextEditSignal).AddAttribute("onClick", "loadPopup('/?cmd=signaledit&signal=" + signalIdString + "');"))
			.AddChildTag(HtmlTag("li").AddClass("contextentry").AddContent(Languages::TextDeleteSignal).AddAttribute("onClick", "loadPopup('/?cmd=signalaskdelete&signal=" + signalIdString + "');"))
			);
		AddChildTag(div2);
	}
};
