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

#include "DataModel/Switch.h"
#include "WebServer/HtmlTagSwitch.h"

using std::string;
using std::to_string;

namespace WebServer
{
	HtmlTagSwitch::HtmlTagSwitch(const DataModel::Switch* mySwitch)
	:	HtmlTagLayoutItem(dynamic_cast<const DataModel::LayoutItem*>(mySwitch))
	{
		const string& switchName = mySwitch->GetName();

		string idText = to_string(mySwitch->GetID());

		imageDiv.AddClass("switch_item");

		const DataModel::AccessoryType type = mySwitch->GetType();
		switch (type)
		{
			case DataModel::SwitchTypeLeft:
				image += "<polygon points=\"15,30 21,36 15,36\" fill=\"white\" />"
					"<polygon points=\"0,15 15,30 15,36 0,21\" fill=\"gray\" class=\"turnout\"/>"
					"<polygon points=\"15,0 21,0 21,36 15,30\" fill=\"gray\" class=\"straight\"/>";
				imageDiv.AddAttribute("onclick", "return onClickSwitch(event, " + idText + ");");
				break;

			case DataModel::SwitchTypeRight:
				image += "<polygon points=\"21,30 21,36 15,36\" fill=\"white\" />"
					"<polygon points=\"21,30 36,15 36,21 21,36\" fill=\"gray\" class=\"turnout\"/>"
					"<polygon points=\"15,0 21,0 21,30 15,36\" fill=\"gray\" class=\"straight\"/>";
				imageDiv.AddAttribute("onclick", "return onClickSwitch(event, " + idText + ");");
				break;

			case DataModel::SwitchTypeThreeWay:
				image += "<polygon points=\"18,33 21,36 15,36\" fill=\"white\" />"
					"<polygon points=\"0,15 15,30 15,36 0,21\" fill=\"gray\" class=\"turnout\"/>"
					"<polygon points=\"15,36 15,30 18,33\" fill=\"gray\" class=\"turnout straight\"/>"
					"<polygon points=\"15,0 21,0 21,30 18,33 15,30\" fill=\"gray\" class=\"straight\"/>"
					"<polygon points=\"21,30 21,36 18,33\" fill=\"gray\" class=\"straight third\"/>"
					"<polygon points=\"21,30 36,15 36,21 21,36\" fill=\"gray\" class=\"third\"/>";
				AddOnClickMenuEntry(switchName);
				AddOnClickMenuEntry(Languages::TextSwitchStateLeft, "fireRequestAndForget('/?cmd=switchstate&switch=" + idText + "&state=turnout');", "menu_turnout");
				AddOnClickMenuEntry(Languages::TextSwitchStateStraight, "fireRequestAndForget('/?cmd=switchstate&switch=" + idText + "&state=straight');", "menu_straight");
				AddOnClickMenuEntry(Languages::TextSwitchStateRight, "fireRequestAndForget('/?cmd=switchstate&switch=" + idText + "&state=third');", "menu_third");
				break;

			case DataModel::SwitchTypeMaerklinLeft:
				image += "<polygon points=\"0,15 9,24 9,30 0,21\" fill=\"white\" />"
					"<polygon points=\"15,0 21,0 21,24 15,18\" fill=\"white\" />"
					"<polygon points=\"27,42 36,51 36,57 27,48\" fill=\"white\" />"
					"<polygon points=\"15,48 21,54 21,72 15,72\" fill=\"white\" />"
					"<polygon points=\"3,24 9,30 9,42 15,48 15,54 3,42\" fill=\"gray\" class=\"turnout\" />"
					"<polygon points=\"21,18 33,30 33,48 27,42 27,30 21,24\" fill=\"gray\" class=\"turnout\" />"
					"<polygon points=\"9,24 15,30 15,18 21,24 21,36 27,42 27,48 21,42 21,54 15,48 15,36 9,30\" fill=\"gray\" class=\"straight\" />";
				imageDiv.AddAttribute("onclick", "return onClickSwitch(event, " + idText + ");");
				break;

			case DataModel::SwitchTypeMaerklinRight:
				image += "<polygon points=\"0,51 9,42 9,48 0,57\" fill=\"white\" />"
					"<polygon points=\"15,0 21,0 21,18 15,24\" fill=\"white\" />"
					"<polygon points=\"27,24 36,15 36,21 27,30\" fill=\"white\" />"
					"<polygon points=\"15,54 21,48 21,72 15,72\" fill=\"white\" />"
					"<polygon points=\"3,48 3,30 15,18 15,24 9,30 9,42\" fill=\"gray\" class=\"turnout\" />"
					"<polygon points=\"21,48 27,42 27,30 33,24 33,42 21,54\" fill=\"gray\" class=\"turnout\" />"
					"<polygon points=\"9,42 15,36 15,24 21,18 21,30 27,24 27,30 21,36 21,48 15,54 15,42 9,48\" fill=\"gray\" class=\"straight\" />";
				imageDiv.AddAttribute("onclick", "return onClickSwitch(event, " + idText + ");");
				break;

			default:
				break;
		}

		string stateClass;
		const DataModel::AccessoryState state = mySwitch->GetAccessoryState();
		switch (state)
		{
			case DataModel::SwitchStateTurnout:
				stateClass = "switch_turnout";
				break;

			case DataModel::SwitchStateStraight:
				stateClass = "switch_straight";
				break;

			case DataModel::SwitchStateThird:
				stateClass = "switch_third";
				break;

			default:
				break;
		}
		imageDiv.AddClass(stateClass);
		onClickMenuDiv.AddClass(stateClass);

		AddToolTip(switchName + " (addr=" + to_string(mySwitch->GetAddress()) + ")");
		AddContextMenuEntry(switchName);
		AddContextMenuEntry(Languages::TextReleaseSwitch, "fireRequestAndForget('/?cmd=switchrelease&switch=" + idText + "');");
		AddContextMenuEntry(Languages::TextEditSwitch, "loadPopup('/?cmd=switchedit&switch=" + idText + "');");
		AddContextMenuEntry(Languages::TextDeleteSwitch, "loadPopup('/?cmd=switchaskdelete&switch=" + idText + "');");
		FinishInit();
	}
} // namespace WebServer
