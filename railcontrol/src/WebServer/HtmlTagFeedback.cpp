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

#include "DataModel/Feedback.h"
#include "WebServer/HtmlTagFeedback.h"

using std::string;
using std::to_string;

namespace WebServer
{
	HtmlTagFeedback::HtmlTagFeedback(const DataModel::Feedback* feedback, DataModel::LayoutItem::LayoutPosition posX, DataModel::LayoutItem::LayoutPosition posY)
	:	HtmlTagLayoutItem()
	{
		DataModel::Feedback::FeedbackState state = feedback->GetState();

		unsigned int layoutPosX = posX * EdgeLength;
		unsigned int layoutPosY = posY * EdgeLength;
		const string& feedbackName = feedback->GetName();

		HtmlTag div1("div");
		string feedbackIdString = to_string(feedback->GetID());
		string id("f_" + feedbackIdString);
		div1.AddAttribute("id", id);
		div1.AddClass("layout_item");
		div1.AddClass("feedback_item");
		div1.AddClass(state == DataModel::Feedback::FeedbackStateOccupied ? "feedback_occupied" : "feedback_free");
		div1.AddAttribute("style", "left:" + to_string(layoutPosX) + "px;top:" + to_string(layoutPosY) + "px;");
		string image;
		image = "<svg width=\"" + EdgeLengthString + "\" height=\"" + EdgeLengthString + "\" id=\"" + id + "_img\"><circle r=\"12\" cx=\"18\" cy=\"18\" stroke=\"black\" stroke-width=\"2\" class=\"feedback\"/></svg>";
		div1.AddChildTag(HtmlTag().AddContent(image));
		div1.AddChildTag(HtmlTag("span").AddClass("tooltip").AddContent(feedbackName + " (pin=" + to_string(feedback->GetPin()) + ")"));
		div1.AddAttribute("onclick", "return onClickFeedback(" + feedbackIdString + ");");
		div1.AddAttribute("oncontextmenu", "return onContextLayoutItem(event, '" + id + "');");
		AddChildTag(div1);

		HtmlTag div2("div");
		div2.AddClass("contextmenu");
		div2.AddAttribute("id", id + "_context");
		div2.AddAttribute("style", "left:" + to_string(layoutPosX + 5) + "px;top:" + to_string(layoutPosY + 30) + "px;");
		div2.AddChildTag(HtmlTag("ul").AddClass("contextentries")
			.AddChildTag(HtmlTag("li").AddClass("contextentry").AddContent(feedbackName))
			.AddChildTag(HtmlTag("li").AddClass("contextentry").AddContent(Languages::TextEditFeedback).AddAttribute("onClick", "loadPopup('/?cmd=feedbackedit&feedback=" + feedbackIdString + "');"))
			.AddChildTag(HtmlTag("li").AddClass("contextentry").AddContent(Languages::TextDeleteFeedback).AddAttribute("onClick", "loadPopup('/?cmd=feedbackaskdelete&feedback=" + feedbackIdString + "');"))
			);
		AddChildTag(div2);
	}
};
