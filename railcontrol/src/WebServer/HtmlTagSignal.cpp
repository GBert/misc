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

#include "DataModel/AccessoryBase.h"
#include "DataModel/Signal.h"
#include "WebServer/HtmlTagSignal.h"

using std::string;
using std::to_string;

namespace WebServer
{
	HtmlTagSignal::HtmlTagSignal(const Manager& manager, const DataModel::Signal* signal)
	:	HtmlTagTrackBase(manager, ObjectTypeSignal, DataModel::TrackTypeStraight, dynamic_cast<const DataModel::TrackBase*>(signal), dynamic_cast<const DataModel::LayoutItem*>(signal)),
		signal(signal)
	{
		DataModel::AccessoryState signalState = signal->GetAccessoryState();
		DataModel::AccessoryType type = signal->GetType();

		imageDiv.AddClass("signal_item");
		imageDiv.AddClass(signalState == DataModel::SignalStateRed ? "signal_red" : "signal_green");
		switch (type)
		{
			case DataModel::SignalTypeSimpleRight:
				image += "<polygon points=\"23,5 27,1 31,1 35,5 35,18 31,22 27,22 23,18\" fill=\"black\"/><polyline points=\"29,7 29,30\" style=\"stroke:black;stroke-width:2\"/><circle class=\"red\" cx=\"29\" cy=\"7\" r=\"4\" fill=\"darkgray\"/><circle class=\"green\" cx=\"29\" cy=\"16\" r=\"4\" fill=\"darkgray\"/>";
				break;

			case DataModel::SignalTypeSimpleLeft:
			default:
				image += "<polygon points=\"1,5 5,1 9,1 13,5 13,18 9,22 5,22 1,18\" fill=\"black\"/><polyline points=\"7,7 7,30\" style=\"stroke:black;stroke-width:2\"/><circle class=\"red\" cx=\"7\" cy=\"7\" r=\"4\" fill=\"darkgray\"/><circle class=\"green\" cx=\"7\" cy=\"16\" r=\"4\" fill=\"darkgray\"/>";
				break;
		}
		AddToolTip(signal->GetName() + " (addr=" + to_string(signal->GetAddress()) + ")");
		imageDiv.AddAttribute("onclick", "return onClickSignal(" + to_string(signal->GetID()) + ");");
		imageDiv.AddAttribute("oncontextmenu", "return onContextLayoutItem(event, '" + identifier + "');");

		AddContextMenuEntry(Languages::TextEditSignal, "loadPopup('/?cmd=signaledit&" + urlIdentifier + "');");
		AddContextMenuEntry(Languages::TextDeleteSignal, "loadPopup('/?cmd=signalaskdelete&" + urlIdentifier + "');");
		FinishInit();
	}
};
