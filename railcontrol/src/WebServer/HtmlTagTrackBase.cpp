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

#include "DataTypes.h"
#include "Manager.h"
#include "WebServer/HtmlTagTrackBase.h"

using std::string;
using std::to_string;

namespace WebServer
{
	HtmlTagTrackBase::HtmlTagTrackBase(const Manager& manager,
		const DataModel::TrackBase* track,
		const DataModel::LayoutItem* layout)
	:	HtmlTagLayoutItem(layout),
		track(track)
	{

		const bool occupied = track->GetFeedbackStateDelayed() == DataModel::Feedback::FeedbackStateOccupied;

		const LocoID locoID = track->GetLocoDelayed();
		const bool reserved = locoID != LocoNone;

		const bool blocked = track->GetBlocked();

		string typeString;
		ObjectType objectType = track->GetObjectType();
		switch (objectType)
		{
			case ObjectTypeTrack:
				typeString = "track";
				break;

			case ObjectTypeSignal:
				typeString = "signal";
				break;

			default:
				return;
		}
		contextMenuDiv.AddClass(reserved ? "loco_known" : "loco_unknown");
		contextMenuDiv.AddClass(blocked ? "track_blocked" : "track_unblocked");
		contextMenuDiv.AddClass(track->GetLocoOrientation() == OrientationRight ? "orientation_right" : "orientation_left");

		const string& trackName = track->GetMyName();
		AddOnClickMenuEntry(trackName);
		AddContextMenuEntry(trackName);
		urlIdentifier = typeString + "=" + to_string(layout->GetID());

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
		const DataModel::LayoutItem::LayoutItemSize trackHeight = layout->GetHeight();
		const string layoutHeight = to_string(EdgeLength * trackHeight);

		switch (track->GetTrackType())
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
				// no break

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

			case DataModel::TrackTypeCrossingLeft:
				image = "<polygon class=\"track\" points=\"14,0 22,0 22,36 36,50, 36,58 22,44 22,72 14,72 14,36 0,22 0,14 14,28\"/>";
				break;

			case DataModel::TrackTypeCrossingRight:
				image = "<polygon class=\"track\" points=\"14,0 22,0 22,28 36,14, 36,22 22,36 22,72 14,72 14,44 0,58 0,50 14,36\"/>";
				break;

			case DataModel::TrackTypeCrossingSymetric:
				image = "<polygon class=\"track\" points=\"36,14 36,22 22,36 36,50, 36,58 18,40 0,58 0,50 14,36 0,22 0,14 18,32\"/>";
				break;

			case DataModel::TrackTypeStraight:
			default:
				image = "<polygon class=\"track\" points=\"14,0 22,0 22," + layoutHeight + " 14," + layoutHeight + "\"/>";
				if (objectType != ObjectTypeTrack)
				{
					break;
				}
				const string& orientationSign = track->GetLocoOrientation() == OrientationRight ? "&rarr; " : "&larr; ";
				const string& locoName = reserved ? orientationSign + manager.GetLocoName(locoID) : "";
				const string textPositionX = to_string(EdgeLength * trackHeight - 1);
				image += "<text class=\"loconame\" x=\"-" + textPositionX + "\" y=\"11\" id=\"" + identifier + "_text_loconame\" transform=\"rotate(270 0,0)\" font-size=\"14\">" + locoName + "</text>";
				if (track->GetShowName())
				{
					image += "<text class=\"trackname\" x=\"-" + textPositionX + "\" y=\"33\" id=\"" + identifier + "_text_trackname\" transform=\"rotate(270 0,0)\" font-size=\"14\">" + track->GetMyName() + "</text>";
				}
				break;
		}
	}
} // namespace WebServer
