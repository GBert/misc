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

#include <string>

#include "WebServer/HtmlTagLayoutItem.h"

using std::string;
using std::to_string;

namespace WebServer
{
	const string HtmlTagLayoutItem::EdgeLengthString = std::to_string(HtmlTagLayoutItem::EdgeLength);

	HtmlTagLayoutItem::HtmlTagLayoutItem(const DataModel::LayoutItem* layout,
		const DataModel::LayoutItem::LayoutPosition posX,
		const DataModel::LayoutItem::LayoutPosition posY)
	:	layout(layout),
		imageDiv("div"),
		onClickMenuDiv("div"),
		onClickMenuContentDiv("ul"),
		contextMenuDiv("div"),
		contextMenuContentDiv("ul"),
		layoutPosX(posX * EdgeLength),
		layoutPosY(posY * EdgeLength)
	{
		switch (layout->GetObjectType())
		{
			case ObjectTypeAccessory:
				identifier = "a_";
				break;

			case ObjectTypeFeedback:
				identifier = "f_";
				break;

			case ObjectTypeRoute:
				identifier = "r_";
				break;

			case ObjectTypeTrack:
				identifier = "t_";
				break;

			case ObjectTypeSignal:
				identifier = "si_";
				break;

			case ObjectTypeSwitch:
				identifier = "sw_";
				break;

			case ObjectTypeText:
				identifier = "tx_";
				break;

			default:
				identifier = "unknown_";
				return;
		}
		identifier += to_string(layout->GetID());
		imageDiv.AddId(identifier);
		imageDiv.AddClass("layout_item");
		imageDiv.AddAttribute("style", "left:" + to_string(layoutPosX) + "px;top:" + to_string(layoutPosY) + "px;");

		string menuPosition = "left:" + to_string(layoutPosX + 5) + "px;top:" + to_string(layoutPosY + 30) + "px;";
		onClickMenuDiv.AddAttribute("style", menuPosition);
		contextMenuDiv.AddAttribute("style", menuPosition);
	}

	void HtmlTagLayoutItem::FinishInit()
	{
		const DataModel::LayoutItem::LayoutItemSize trackHeight = layout->GetHeight();
		const DataModel::LayoutItem::LayoutItemSize trackWidth = layout->GetWidth();

		int translate = 0;
		if (trackHeight > DataModel::LayoutItem::Height1 || trackWidth > DataModel::LayoutItem::Width1)
		{
			DataModel::LayoutItem::LayoutRotation rotation = layout->GetRotation();
			if (rotation == DataModel::LayoutItem::Rotation90 || rotation == DataModel::LayoutItem::Rotation270)
			{
				translate = (((trackHeight - trackWidth) * EdgeLength) / 2);
			}
			if (rotation == DataModel::LayoutItem::Rotation90)
			{
				translate = -translate;
			}
		}

		const string layoutHeight = to_string(EdgeLength * trackHeight);
		const string layoutWidth = to_string(EdgeLength * trackWidth);
		const string translateText = to_string(translate);
		imageDiv.AddChildTag(HtmlTag().AddContent("<svg width=\"" + layoutWidth + "\" height=\"" + layoutHeight + "\" id=\"" + identifier + "_img\" style=\"transform:rotate(" + DataModel::LayoutItem::Rotation(layout->GetRotation()) + "deg) translate(" + translateText + "px," + translateText + "px);\">" + image + "</svg>"));

		if (onClickMenuContentDiv.ChildCount())
		{
			imageDiv.AddAttribute("onclick", "return showOnClickMenu(event, '" + identifier + "');");
			onClickMenuContentDiv.AddClass("contextentries");
			onClickMenuDiv.AddChildTag(onClickMenuContentDiv);
			onClickMenuDiv.AddClass("contextmenu");
			onClickMenuDiv.AddId(identifier + "_onclick");
			AddChildTag(onClickMenuDiv);
		}
		if (contextMenuContentDiv.ChildCount())
		{
			imageDiv.AddAttribute("oncontextmenu", "return showContextMenu(event, '" + identifier + "');");
			contextMenuContentDiv.AddClass("contextentries");
			contextMenuDiv.AddChildTag(contextMenuContentDiv);
			contextMenuDiv.AddClass("contextmenu");
			contextMenuDiv.AddId(identifier + "_context");
			AddChildTag(contextMenuDiv);
		}
		AddChildTag(imageDiv);
	}

	void HtmlTagLayoutItem::AddMenuEntry(HtmlTag& menu,
		const string& text)
	{
		HtmlTag li("li");
		li.AddClass("contexttitle");
		li.AddContent(text);
		menu.AddChildTag(li);
	}

	void HtmlTagLayoutItem::AddMenuEntry(HtmlTag& menu,
		const Languages::TextSelector text,
		const string& onClick,
		const string& className)
	{
		HtmlTag li("li");
		li.AddClass("contextentry");
		li.AddContent(text);
		if (onClick.length() > 0)
		{
			li.AddAttribute("onClick", onClick);
		}
		if (className.length() > 0)
		{
			li.AddClass(className);
		}
		menu.AddChildTag(li);
	}
} // namespace WebServer
