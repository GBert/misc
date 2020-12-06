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

#include <string>
#include <vector>

#include "DataModel/Cluster.h"
#include "DataModel/ObjectIdentifier.h"
#include "DataModel/TrackBase.h"
#include "DataTypes.h"
#include "Languages.h"
#include "WebServer/HtmlTag.h"
#include "WebServer/HtmlTagButton.h"
#include "WebServer/HtmlTagInputCheckboxWithLabel.h"
#include "WebServer/HtmlTagInputHidden.h"
#include "WebServer/HtmlTagTextWithLabel.h"
#include "WebServer/WebClient.h"
#include "WebServer/WebClientTrackBase.h"

using namespace DataModel;
using std::to_string;

namespace WebServer
{
	HtmlTag WebClientTrackBase::HtmlTagTabTrackFeedback(const WebClient& client,
		const std::vector<FeedbackID>& feedbacks,
		const ObjectIdentifier& objectIdentifier)
	{
		unsigned int feedbackCounter = 0;
		HtmlTag existingFeedbacks("div");
		existingFeedbacks.AddId("feedbackcontent");
		for (auto feedbackID : feedbacks)
		{
			existingFeedbacks.AddChildTag(client.HtmlTagSelectFeedbackForTrack(++feedbackCounter, objectIdentifier, feedbackID));
		}
		existingFeedbacks.AddChildTag(HtmlTag("div").AddId("div_feedback_" + to_string(feedbackCounter + 1)));

		HtmlTag feedbackContent("div");
		feedbackContent.AddId("tab_feedback");
		feedbackContent.AddClass("tab_content");
		feedbackContent.AddClass("hidden");
		feedbackContent.AddChildTag(HtmlTagInputHidden("feedbackcounter", to_string(feedbackCounter)));
		feedbackContent.AddChildTag(existingFeedbacks);
		HtmlTagButton newButton(Languages::TextNew, "newfeedback");
		newButton.AddAttribute("onclick", "addFeedback();return false;");
		newButton.AddClass("wide_button");
		feedbackContent.AddChildTag(newButton);
		feedbackContent.AddChildTag(HtmlTag("br"));
		return feedbackContent;
	}

	HtmlTag WebClientTrackBase::HtmlTagTabTrackAutomode(DataModel::SelectRouteApproach selectRouteApproach,
		const bool allowLocoTurn,
		const bool releaseWhenFree,
		const Cluster* cluster)
	{
		HtmlTag automodeContent("div");
		automodeContent.AddId("tab_automode");
		automodeContent.AddClass("tab_content");
		automodeContent.AddClass("hidden");

		automodeContent.AddChildTag(WebClient::HtmlTagSelectSelectRouteApproach(selectRouteApproach));

		automodeContent.AddChildTag(HtmlTagInputCheckboxWithLabel("allowlocoturn", Languages::TextAllowLocoTurn, "false", allowLocoTurn));

		automodeContent.AddChildTag(HtmlTagInputCheckboxWithLabel("releasewhenfree", Languages::TextReleaseWhenFree, "true", releaseWhenFree));

		if (cluster != nullptr)
		{
			automodeContent.AddChildTag(HtmlTagTextWithLabel("cluster", Languages::TextCluster, cluster->GetName()));
		}
		return automodeContent;
	}
} // namespace WebServer
