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

#include <algorithm>
#include <map>
#include <vector>

#include "DataModel/Cluster.h"
#include "DataModel/ObjectIdentifier.h"
#include "DataModel/Relation.h"
#include "DataModel/Signal.h"
#include "DataModel/Track.h"
#include "WebServer/HtmlTag.h"
#include "WebServer/HtmlTagButtonCancel.h"
#include "WebServer/HtmlTagButtonOK.h"
#include "WebServer/HtmlTagButtonPopupWide.h"
#include "WebServer/HtmlTagInputHidden.h"
#include "WebServer/HtmlTagInputTextWithLabel.h"
#include "WebServer/HtmlTagSelect.h"
#include "WebServer/WebClient.h"
#include "WebServer/WebClientCluster.h"

using DataModel::Cluster;
using DataModel::ObjectIdentifier;
using DataModel::Relation;
using DataModel::Signal;
using DataModel::Track;
using std::map;
using std::string;
using std::to_string;
using std::vector;

namespace WebServer
{
	void WebClientCluster::HandleClusterList()
	{
		HtmlTag content;
		content.AddChildTag(HtmlTag("h1").AddContent(Languages::TextCluster));
		HtmlTag table("table");
		map<string,string> clusterArgument;
		const map<string,Cluster*> clusterList = manager.ClusterListByName();
		for (auto& cluster : clusterList)
		{
			HtmlTag row("tr");
			row.AddChildTag(HtmlTag("td").AddContent(cluster.first));
			const string& clusterIdString = to_string(cluster.second->GetID());
			clusterArgument["cluster"] = clusterIdString;
			row.AddChildTag(HtmlTag("td").AddChildTag(HtmlTagButtonPopupWide(Languages::TextEdit, "clusteredit_list_" + clusterIdString, clusterArgument)));
			row.AddChildTag(HtmlTag("td").AddChildTag(HtmlTagButtonPopupWide(Languages::TextDelete, "clusteraskdelete_" + clusterIdString, clusterArgument)));
			table.AddChildTag(row);
		}
		content.AddChildTag(HtmlTag("div").AddClass("popup_content").AddChildTag(table));
		content.AddChildTag(HtmlTagButtonCancel());
		content.AddChildTag(HtmlTagButtonPopupWide(Languages::TextNew, "clusteredit_0"));
		client.ReplyHtmlWithHeader(content);
	}

	void WebClientCluster::HandleClusterEdit(const map<string, string>& arguments)
	{
		HtmlTag content;
		ClusterID clusterID = Utils::Utils::GetIntegerMapEntry(arguments, "cluster", ClusterNone);
		string name = Utils::Utils::GetStringMapEntry(arguments, "name");
		vector<Relation*> tracks;
		vector<Relation*> signals;

		if (clusterID != ClusterNone)
		{
			Cluster* cluster = manager.GetCluster(clusterID);
			if (cluster != nullptr)
			{
				name = cluster->GetName();
				tracks = cluster->GetTracks();
				signals = cluster->GetSignals();
			}
		}

		content.AddChildTag(HtmlTag("h1").AddContent(name).AddId("popup_title"));
		HtmlTag tabMenu("div");
		tabMenu.AddChildTag(client.HtmlTagTabMenuItem("basic", Languages::TextBasic, true));
		tabMenu.AddChildTag(client.HtmlTagTabMenuItem("tracks", Languages::TextTracks));
		if (signals.size() > 0)
		{
			tabMenu.AddChildTag(client.HtmlTagTabMenuItem("signals", Languages::TextSignals));
		}
		content.AddChildTag(tabMenu);

		HtmlTag formContent("form");
		formContent.AddId("editform");
		formContent.AddChildTag(HtmlTagInputHidden("cmd", "clustersave"));
		formContent.AddChildTag(HtmlTagInputHidden("cluster", to_string(clusterID)));

		HtmlTag basicContent("div");
		basicContent.AddId("tab_basic");
		basicContent.AddClass("tab_content");
		basicContent.AddChildTag(HtmlTagInputTextWithLabel("name", Languages::TextName, name).AddAttribute("onkeyup", "updateName();"));
		formContent.AddChildTag(basicContent);

		formContent.AddChildTag(client.HtmlTagSlaveSelect("track", tracks, GetTrackOptions(clusterID)));

		// FIXME: remove again later 2021-02-10
		if (signals.size() > 0)
		{
			formContent.AddChildTag(client.HtmlTagSlaveSelect("signal", signals, GetSignalOptions(signals), false));
		}

		content.AddChildTag(HtmlTag("div").AddClass("popup_content").AddChildTag(formContent));
		content.AddChildTag(HtmlTagButtonCancel());
		content.AddChildTag(HtmlTagButtonOK());
		client.ReplyHtmlWithHeader(content);
	}

	void WebClientCluster::HandleClusterSave(const map<string, string>& arguments)
	{
		ClusterID clusterID = Utils::Utils::GetIntegerMapEntry(arguments, "cluster", ClusterNone);
		string name = Utils::Utils::GetStringMapEntry(arguments, "name");

		vector<Relation*> tracks;
		{
			vector<TrackID> trackIds = client.InterpretSlaveData("track", arguments);
			for (auto trackId : trackIds)
			{
				tracks.push_back(new Relation(&manager,
					ObjectIdentifier(ObjectTypeCluster, clusterID),
					ObjectIdentifier(ObjectTypeTrack, trackId),
					Relation::TypeClusterTrack));
			}
		}

		vector<Relation*> signals;
		{
			vector<SignalID> signalIds = client.InterpretSlaveData("signal", arguments);
			for (auto signalId : signalIds)
			{
				signals.push_back(new Relation(&manager,
					ObjectIdentifier(ObjectTypeCluster, clusterID),
					ObjectIdentifier(ObjectTypeSignal, signalId),
					Relation::TypeClusterSignal));
			}
		}

		string result;
		if (!manager.ClusterSave(clusterID, name, tracks, signals, result))
		{
			client.ReplyResponse(WebClient::ResponseError, result);
			return;
		}

		client.ReplyResponse(WebClient::ResponseInfo, Languages::TextControlSaved, name);
	}

	void WebClientCluster::HandleClusterAskDelete(const map<string, string>& arguments)
	{
		ClusterID clusterID = Utils::Utils::GetIntegerMapEntry(arguments, "cluster", ClusterNone);

		if (clusterID == ControlNone)
		{
			client.ReplyHtmlWithHeaderAndParagraph(Languages::TextClusterDoesNotExist);
			return;
		}

		const Cluster* cluster = manager.GetCluster(clusterID);
		if (cluster == nullptr)
		{
			client.ReplyHtmlWithHeaderAndParagraph(Languages::TextClusterDoesNotExist);
			return;
		}

		HtmlTag content;
		content.AddContent(HtmlTag("h1").AddContent(Languages::TextDeleteCluster));
		content.AddContent(HtmlTag("p").AddContent(Languages::TextAreYouSureToDelete, cluster->GetName()));
		content.AddContent(HtmlTag("form").AddId("editform")
			.AddContent(HtmlTagInputHidden("cmd", "clusterdelete"))
			.AddContent(HtmlTagInputHidden("cluster", to_string(clusterID))
			));
		content.AddContent(HtmlTagButtonCancel());
		content.AddContent(HtmlTagButtonOK());
		client.ReplyHtmlWithHeader(content);
	}

	void WebClientCluster::HandleClusterDelete(const map<string, string>& arguments)
	{
		ClusterID clusterID = Utils::Utils::GetIntegerMapEntry(arguments, "cluster", ClusterNone);
		const Cluster* cluster = manager.GetCluster(clusterID);
		if (cluster == nullptr)
		{
			client.ReplyResponse(WebClient::ResponseError, Languages::TextClusterDoesNotExist);
			return;
		}

		string name = cluster->GetName();

		if (!manager.ClusterDelete(clusterID))
		{
			client.ReplyResponse(WebClient::ResponseError, Languages::TextClusterDoesNotExist);
			return;
		}

		client.ReplyResponse(WebClient::ResponseInfo, Languages::TextClusterDeleted, name);
	}

	map<string,ObjectID> WebClientCluster::GetTrackOptions(const ClusterID clusterId) const
	{
		map<string, ObjectID> trackOptions;

		map<string, Track*> allTracks = manager.TrackListByName();
		for (auto& track : allTracks)
		{
			Cluster* clusterOfTrack = track.second->GetCluster();
			if (clusterOfTrack != nullptr && clusterOfTrack->GetID() != clusterId)
			{
				continue;
			}
			trackOptions[track.first] = track.second->GetID();
		}
		return trackOptions;
	}

	// FIXME: remove again later 2021-02-10
	map<string,ObjectID> WebClientCluster::GetSignalOptions(const vector<Relation*>& signals) const
	{
		map<string, ObjectID> signalOptions;

		for (auto signalRelation : signals)
		{
			Signal* signal = dynamic_cast<Signal*>(signalRelation->GetObject2());
			if (signal == nullptr)
			{
				continue;
			}
			signalOptions[signal->GetName()] = signal->GetID();
		}
		return signalOptions;
	}
} // namespace WebServer
