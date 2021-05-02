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

#include "DataModel/Accessory.h"
#include "WebServer/HtmlTagAccessory.h"

using std::string;
using std::to_string;

namespace WebServer
{
	HtmlTagAccessory::HtmlTagAccessory(const DataModel::Accessory* accessory)
	:	HtmlTagLayoutItem(dynamic_cast<const DataModel::LayoutItem*>(accessory))
	{
		image += "<polygon points=\"10,10 26,10 26,26 10,26\" />";

		const DataModel::AccessoryState state = accessory->GetAccessoryState();

		string accessoryIdString = to_string(accessory->GetID());
		imageDiv.AddClass("accessory_item");
		imageDiv.AddClass(state == DataModel::AccessoryStateOn ? "accessory_on" : "accessory_off");
		imageDiv.AddAttribute("onclick", "return onClickAccessory(" + accessoryIdString + ");");

		const string& accessoryName = accessory->GetName();
		AddToolTip(accessoryName + " (addr=" + to_string(accessory->GetAddress()) + ")");
		AddContextMenuEntry(accessoryName);
		AddContextMenuEntry(Languages::TextReleaseAccessory, "fireRequestAndForget('/?cmd=accessoryrelease&accessory=" + accessoryIdString + "');");
		AddContextMenuEntry(Languages::TextEditAccessory, "loadPopup('/?cmd=accessoryedit&accessory=" + accessoryIdString + "');");
		AddContextMenuEntry(Languages::TextDeleteAccessory, "loadPopup('/?cmd=accessoryaskdelete&accessory=" + accessoryIdString + "');");
		FinishInit();
	}
} // namespace WebServer
