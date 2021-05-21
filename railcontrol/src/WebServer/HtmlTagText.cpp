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

#include "DataModel/Text.h"
#include "WebServer/HtmlTagText.h"

using std::string;
using std::to_string;

namespace WebServer
{
	HtmlTagText::HtmlTagText(const DataModel::Text* text)
	:	HtmlTagLayoutItem(dynamic_cast<const DataModel::LayoutItem*>(text))
	{
		const string& name = text->GetName();
		image += "<text x=\"3\" y=\"22\">" + name + "</text>";

		imageDiv.AddClass("text_item");
		imageDiv.AddAttribute("onclick", "return onClickWithoutMenu(event, '" + identifier + "');");

		const string urlIdentifier = "text=" + to_string(text->GetID());

		AddContextMenuEntry(name);
		AddContextMenuEntry(Languages::TextEditText, "loadPopup('/?cmd=textedit&" + urlIdentifier + "');");
		AddContextMenuEntry(Languages::TextDeleteText, "loadPopup('/?cmd=textaskdelete&" + urlIdentifier + "');");
		FinishInit();
	}
} // namespace WebServer
