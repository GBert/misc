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

#include "DataTypes.h"
#include "Manager.h"
#include "WebServer/HtmlTagTrackBase.h"

using std::string;
using std::to_string;

namespace WebServer
{
	HtmlTagTrackBase::HtmlTagTrackBase(const Manager& manager,
		const ObjectType objectType,
		const DataModel::TrackType trackType,
		const DataModel::TrackBase* track,
		const DataModel::LayoutItem* layout)
	:	HtmlTagLayoutItem(),
		objectType(objectType),
		id(track->GetMyID()),
		track(track),
		layout(layout),
		imageDiv("div"),
		menuDiv("div"),
		menuContentDiv("ul")
	{
		DataModel::LayoutItem::LayoutPosition posX;
		DataModel::LayoutItem::LayoutPosition posY;
		DataModel::LayoutItem::LayoutPosition posZ;
		DataModel::LayoutItem::LayoutItemSize w;
		DataModel::LayoutItem::LayoutItemSize h;
		DataModel::LayoutItem::LayoutRotation r;
		layout->Position(posX, posY, posZ, w, h, r);
		const unsigned int layoutPosX = posX * EdgeLength;
		const unsigned int layoutPosY = posY * EdgeLength;

		const bool occupied = track->GetFeedbackStateDelayed() == DataModel::Feedback::FeedbackStateOccupied;

		const LocoID locoID = track->GetLocoDelayed();
		const bool reserved = locoID != LocoNone;

		const bool blocked = track->GetBlocked();

		string typeString;
		switch (objectType)
		{
			case ObjectTypeTrack:
				identifier = "t_";
				typeString = "track";
				break;

			case ObjectTypeSignal:
				identifier = "si_";
				typeString = "signal";
				break;

			default:
				return;
		}
		const string idString = to_string(id);
		identifier += idString;

		menuDiv.AddClass("contextmenu");
		menuDiv.AddClass(reserved ? "loco_known" : "loco_unknown");
		menuDiv.AddClass(blocked ? "track_blocked" : "track_unblocked");
		menuDiv.AddClass(track->GetLocoDirection() == DirectionRight ? "direction_right" : "direction_left");
		menuDiv.AddAttribute("id", identifier + "_context");
		menuDiv.AddAttribute("style", "left:" + to_string(layoutPosX + 5) + "px;top:" + to_string(layoutPosY + 30) + "px;");

		const string& trackName = track->GetMyName();
		menuContentDiv.AddClass("contextentries");
		AddContextMenuEntry(trackName);
		urlIdentifier = typeString + "=" + idString;
		AddContextMenuEntry(Languages::TextBlockTrack, "fireRequestAndForget('/?cmd=trackblock&" + urlIdentifier + "&blocked=true');", "track_block");
		AddContextMenuEntry(Languages::TextUnblockTrack, "fireRequestAndForget('/?cmd=trackblock&" + urlIdentifier + "&blocked=false');", "track_unblock");
		AddContextMenuEntry(Languages::TextTurnDirectionOfTravelToLeft, "fireRequestAndForget('/?cmd=trackdirection&direction=false&" + urlIdentifier + "');", "track_left");
		AddContextMenuEntry(Languages::TextTurnDirectionOfTravelToRight, "fireRequestAndForget('/?cmd=trackdirection&direction=true&" + urlIdentifier + "');", "track_right");
		AddContextMenuEntry(Languages::TextReleaseTrack, "fireRequestAndForget('/?cmd=trackrelease&" + urlIdentifier + "');", "track_release");
		AddContextMenuEntry(Languages::TextReleaseTrackAndLoco, "fireRequestAndForget('/?cmd=locorelease&" + urlIdentifier + "');", "track_loco_release");
		AddContextMenuEntry(Languages::TextSetLoco, "loadPopup('/?cmd=tracksetloco&" + urlIdentifier + "');", "track_set");
		AddContextMenuEntry(Languages::TextStartLoco, "fireRequestAndForget('/?cmd=trackstartloco&" + urlIdentifier + "');", "track_start_loco");
		AddContextMenuEntry(Languages::TextStopLoco, "fireRequestAndForget('/?cmd=trackstoploco&" + urlIdentifier + "');", "track_stop_loco");

		imageDiv.AddAttribute("id", identifier);
		imageDiv.AddClass("layout_item");
		imageDiv.AddClass("track_item");
		string trackClass;
		if (reserved && occupied)
		{
			trackClass = "track_reserved_occupied";
		}
		else if (reserved)
		{
			trackClass = "track_reserved";
		}
		else if (occupied)
		{
			trackClass = "track_occupied";
		}
		else if (blocked)
		{
			trackClass = "track_blocked";
		}
		else
		{
			trackClass = "track_free";
		}

		imageDiv.AddClass(trackClass);
		imageDiv.AddAttribute("style", "left:" + to_string(layoutPosX) + "px;top:" + to_string(layoutPosY) + "px;");
		const DataModel::LayoutItem::LayoutItemSize trackHeight = layout->GetHeight();
		const string layoutHeight = to_string(EdgeLength * trackHeight);

		switch (trackType)
		{
			case DataModel::TrackTypeTurn:
				image = "<polygon class=\"track\" points=\"0,22 0,14 22,36 14,36\"/>";
				break;

			case DataModel::TrackTypeEnd:
				image = "<polygon class=\"track\" points=\"14,5 22,5 22," + layoutHeight + " 14," + layoutHeight + "\"/>";
				image += "<polygon class=\"track\" points=\"4,10 4,5 32,5 32,10\"/>";
				break;

			case DataModel::TrackTypeBridge:
			{
				const string l1 = to_string(EdgeLength * trackHeight - 5);
				const string l2 = to_string(EdgeLength * trackHeight - 3);
				image = "<polygon class=\"track\" points=\"14,0 22,0 22," + layoutHeight + " 14," + layoutHeight + "\"/>";
				image += "<polygon class=\"track\" points=\"10,3 12,5 12," + l1 + " 10," + l2 + "\"/>";
				image += "<polygon class=\"track\" points=\"26,3 24,5 24," + l1 + " 26," + l2 + "\"/>";
				break;
			}

			case DataModel::TrackTypeTunnel:
				image = "<polygon class=\"track\" points=\"14,0 22,0 22,12 14,12\"/>";
				image += "<polygon class=\"track\" points=\"5,1 10,1 10,10 12,12 24,12 26,10 26,1 31,1 31,3 28,3 28,11 25,14 11,14 8,11 8,3 5,3 \"/>";
				#include "Fallthrough.h"

			case DataModel::TrackTypeTunnelEnd:
			{
				const string l0 = to_string(EdgeLength * trackHeight - 14);
				const string l1 = to_string(EdgeLength * trackHeight - 12);
				const string l2 = to_string(EdgeLength * trackHeight - 11);
				const string l3 = to_string(EdgeLength * trackHeight - 10);
				const string l4 = to_string(EdgeLength * trackHeight - 8);
				const string l5 = to_string(EdgeLength * trackHeight - 3);
				const string l6 = to_string(EdgeLength * trackHeight - 1);
				image += "<polygon class=\"track\" points=\"14," + l1 + " 22," + l1 + " 22," + layoutHeight + " 14," + layoutHeight + "\"/>";
				image += "<polygon class=\"track\" points=\"5," + l6 + " 10," + l6 + " 10," + l4 + " 12," + l1 + " 24," + l1 + " 26," + l4 + " 26," + l6 + " 31," + l6 + " 31," + l5 + " 28," + l5 + " 28," + l3 + " 25," + l0 + " 11," + l0 + " 8," + l3 + " 8," + l5 + " 5," + l5 + "\"/>";
				break;
			}

			case DataModel::TrackTypeLink:
				image = "<polygon class=\"track\" points=\"14,22 22,22 22," + layoutHeight + " 14," + layoutHeight + "\"/>";
				image += "<polygon class=\"track\" points=\"18,1 4,22 32,22\"/>";
				break;

			case DataModel::TrackTypeStraight:
			default:
				const string& directionSign = track->GetLocoDirection() == DirectionRight ? "&rarr; " : "&larr; ";
				const string& locoName = reserved ? directionSign + manager.GetLocoName(locoID) : "";
				const string textPositionX = to_string(EdgeLength * trackHeight - 1);
				image = "<polygon class=\"track\" points=\"14,0 22,0 22," + layoutHeight + " 14," + layoutHeight + "\"/>";
				image += "<text class=\"loconame\" x=\"-" + textPositionX + "\" y=\"11\" id=\"" + identifier + "_text_loconame\" transform=\"rotate(270 0,0)\" font-size=\"14\">" + locoName + "</text>";
				image += "<text class=\"trackname\" x=\"-" + textPositionX + "\" y=\"33\" id=\"" + identifier + "_text_trackname\" transform=\"rotate(270 0,0)\" font-size=\"14\">" + track->GetMyName() + "</text>";
				break;
		}
	}

	void HtmlTagTrackBase::FinishInit()
	{
		const DataModel::LayoutItem::LayoutItemSize trackHeight = layout->GetHeight();
		const string layoutHeight = to_string(EdgeLength * trackHeight);

		int translate = 0;
		if (trackHeight > DataModel::LayoutItem::Height1)
		{
			DataModel::LayoutItem::LayoutRotation trackRotation = layout->GetRotation();
			if (trackRotation == DataModel::LayoutItem::Rotation90 || trackRotation == DataModel::LayoutItem::Rotation270)
			{
				translate = (((trackHeight - 1) * EdgeLength) / 2);
			}
			if (trackRotation == DataModel::LayoutItem::Rotation90)
			{
				translate = -translate;
			}
		}

		imageDiv.AddChildTag(HtmlTag().AddContent("<svg width=\"" + EdgeLengthString + "\" height=\"" + layoutHeight + "\" id=\"" + identifier + "_img\" style=\"transform:rotate(" + DataModel::LayoutItem::Rotation(layout->GetRotation()) + "deg) translate(" + to_string(translate) + "px," + to_string(translate) + "px);\">" + image + "</svg>"));
		imageDiv.AddAttribute("oncontextmenu", "return onContextLayoutItem(event, '" + identifier + "');");
		AddChildTag(imageDiv);

		menuDiv.AddChildTag(menuContentDiv);
		AddChildTag(menuDiv);
	}

	void HtmlTagTrackBase::AddContextMenuEntry(const std::string& text)
	{
		HtmlTag li("li");
		li.AddClass("contextentry");
		li.AddContent(text);
		menuContentDiv.AddChildTag(li);
	}

	void HtmlTagTrackBase::AddContextMenuEntry(const Languages::TextSelector text, const std::string& onClick, const std::string& className)
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
		menuContentDiv.AddChildTag(li);
	}

};
