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

#include "DataModel/Track.h"
#include "WebServer/HtmlTagTrack.h"

using std::string;
using std::to_string;

namespace WebServer
{
	HtmlTagTrack::HtmlTagTrack(const Manager& manager, const DataModel::Track* track)
	:	HtmlTagTrackBase(manager,
		dynamic_cast<const DataModel::TrackBase*>(track),
		dynamic_cast<const DataModel::LayoutItem*>(track))
	{
		AddOnClickMenuEntry(Languages::TextBlockTrack, "fireRequestAndForget('/?cmd=trackblock&" + urlIdentifier + "&blocked=true');", "track_block");
		AddOnClickMenuEntry(Languages::TextUnblockTrack, "fireRequestAndForget('/?cmd=trackblock&" + urlIdentifier + "&blocked=false');", "track_unblock");
		AddOnClickMenuEntry(Languages::TextTurnDirectionOfTravelToLeft, "fireRequestAndForget('/?cmd=trackorientation&orientation=false&" + urlIdentifier + "');", "track_left");
		AddOnClickMenuEntry(Languages::TextTurnDirectionOfTravelToRight, "fireRequestAndForget('/?cmd=trackorientation&orientation=true&" + urlIdentifier + "');", "track_right");
		AddOnClickMenuEntry(Languages::TextSetLoco, "loadPopup('/?cmd=tracksetloco&" + urlIdentifier + "');", "track_set");
		AddOnClickMenuEntry(Languages::TextStartLoco, "fireRequestAndForget('/?cmd=trackstartloco&" + urlIdentifier + "');", "track_start_loco");
		AddOnClickMenuEntry(Languages::TextStopLoco, "fireRequestAndForget('/?cmd=trackstoploco&" + urlIdentifier + "');", "track_stop_loco");

		AddContextMenuEntry(Languages::TextReleaseTrack, "fireRequestAndForget('/?cmd=trackrelease&" + urlIdentifier + "');", "track_release");
		AddContextMenuEntry(Languages::TextReleaseTrackAndLoco, "fireRequestAndForget('/?cmd=locorelease&" + urlIdentifier + "');", "track_loco_release");
		AddContextMenuEntry(Languages::TextEditTrack, "loadPopup('/?cmd=trackedit&" + urlIdentifier + "');");
		AddContextMenuEntry(Languages::TextDeleteTrack, "loadPopup('/?cmd=trackaskdelete&" + urlIdentifier + "');");
		AddToolTip(track->GetName());
		FinishInit();
	}
} // namespace WebServer
