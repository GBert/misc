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

#include <vector>

#include "DataModel/Text.h"
#include "Utils/Utils.h"
#include "WebServer/HtmlTag.h"
#include "WebServer/HtmlTagButtonCancel.h"
#include "WebServer/HtmlTagButtonCommandWide.h"
#include "WebServer/HtmlTagButtonOK.h"
#include "WebServer/HtmlTagButtonPopupWide.h"
#include "WebServer/HtmlTagInputHidden.h"
#include "WebServer/HtmlTagInputIntegerWithLabel.h"
#include "WebServer/HtmlTagInputTextWithLabel.h"
#include "WebServer/HtmlTagText.h"
#include "WebServer/WebClient.h"
#include "WebServer/WebClientText.h"

using namespace DataModel;
using LayoutPosition = DataModel::LayoutItem::LayoutPosition;
using LayoutItemSize = DataModel::LayoutItem::LayoutItemSize;
using LayoutRotation = DataModel::LayoutItem::LayoutRotation;
using std::map;
using std::string;
using std::to_string;
using std::vector;

namespace WebServer
{
	void WebClientText::HandleTextEdit(const map<string, string>& arguments)
	{
		HtmlTag content;
		TextID textID = Utils::Utils::GetIntegerMapEntry(arguments, "text", TextNone);
		string name = Languages::GetText(Languages::TextNew);
		LayoutPosition posx = Utils::Utils::GetIntegerMapEntry(arguments, "posx", 0);
		LayoutPosition posy = Utils::Utils::GetIntegerMapEntry(arguments, "posy", 0);
		LayoutPosition posz = Utils::Utils::GetIntegerMapEntry(arguments, "posz", LayerUndeletable);

		LayoutItemSize width = Utils::Utils::GetIntegerMapEntry(arguments, "width", 1);

		LayoutRotation rotation = Utils::Utils::GetIntegerMapEntry(arguments, "rotation", DataModel::LayoutItem::Rotation0);

		if (textID > TextNone)
		{
			const DataModel::Text* text = manager.GetText(textID);
			if (text != nullptr)
			{
				name = text->GetName();
				posx = text->GetPosX();
				posy = text->GetPosY();
				posz = text->GetPosZ();
				width = text->GetWidth();
				rotation = text->GetRotation();
			}
		}

		content.AddChildTag(HtmlTag("h1").AddContent(name).AddId("popup_title"));
		HtmlTag tabMenu("div");
		tabMenu.AddChildTag(client.HtmlTagTabMenuItem("main", Languages::TextBasic, true));
		tabMenu.AddChildTag(client.HtmlTagTabMenuItem("position", Languages::TextPosition));

		content.AddChildTag(tabMenu);

		HtmlTag formContent;
		formContent.AddChildTag(HtmlTagInputHidden("cmd", "textsave"));
		formContent.AddChildTag(HtmlTagInputHidden("text", to_string(textID)));

		HtmlTag mainContent("div");
		mainContent.AddId("tab_main");
		mainContent.AddClass("tab_content");
		mainContent.AddChildTag(HtmlTagInputTextWithLabel("name", Languages::TextName, name).AddAttribute("onkeyup", "updateName();"));
		mainContent.AddChildTag(HtmlTagInputIntegerWithLabel("width", Languages::TextWidth, width, DataModel::Text::MinWidth, DataModel::Text::MaxWidth));
		formContent.AddChildTag(mainContent);

		formContent.AddChildTag(client.HtmlTagTabPosition(posx, posy, posz, rotation));

		content.AddChildTag(HtmlTag("div").AddClass("popup_content").AddChildTag(HtmlTag("form").AddId("editform").AddChildTag(formContent)));
		content.AddChildTag(HtmlTagButtonCancel());
		content.AddChildTag(HtmlTagButtonOK());
		client.ReplyHtmlWithHeader(content);
	}

	void WebClientText::HandleTextSave(const map<string, string>& arguments)
	{
		TextID textID = Utils::Utils::GetIntegerMapEntry(arguments, "text", TextNone);
		string name = Utils::Utils::GetStringMapEntry(arguments, "name");
		LayoutPosition posX = Utils::Utils::GetIntegerMapEntry(arguments, "posx", 0);
		LayoutPosition posY = Utils::Utils::GetIntegerMapEntry(arguments, "posy", 0);
		LayoutPosition posZ = Utils::Utils::GetIntegerMapEntry(arguments, "posz", 0);
		LayoutItemSize width = Utils::Utils::GetIntegerMapEntry(arguments, "width", 1);
		LayoutRotation rotation = Utils::Utils::GetIntegerMapEntry(arguments, "rotation", DataModel::LayoutItem::Rotation0);
		string result;
		if (!manager.TextSave(textID,
			name,
			posX,
			posY,
			posZ,
			width,
			rotation,
			result))
		{
			client.ReplyResponse(WebClient::ResponseError, result);
			return;
		}

		client.ReplyResponse(WebClient::ResponseInfo, Languages::TextTextSaved, name);
	}

	void WebClientText::HandleTextList()
	{
		HtmlTag content;
		content.AddChildTag(HtmlTag("h1").AddContent(Languages::TextTexts));
		HtmlTag table("table");
		const map<string,DataModel::Text*> textList = manager.TextListByName();
		map<string,string> textArgument;
		for (auto& text : textList)
		{
			HtmlTag row("tr");
			row.AddChildTag(HtmlTag("td").AddContent(text.first));
			const string& textIdString = to_string(text.second->GetID());
			textArgument["text"] = textIdString;
			row.AddChildTag(HtmlTag("td").AddChildTag(HtmlTagButtonPopupWide(Languages::TextEdit, "textedit_list_" + textIdString, textArgument)));
			row.AddChildTag(HtmlTag("td").AddChildTag(HtmlTagButtonPopupWide(Languages::TextDelete, "textaskdelete_" + textIdString, textArgument)));
			table.AddChildTag(row);
		}
		content.AddChildTag(HtmlTag("div").AddClass("popup_content").AddChildTag(table));
		content.AddChildTag(HtmlTagButtonCancel());
		content.AddChildTag(HtmlTagButtonPopupWide(Languages::TextNew, "textedit_0"));
		client.ReplyHtmlWithHeader(content);
	}

	void WebClientText::HandleTextAskDelete(const map<string, string>& arguments)
	{
		TextID textID = Utils::Utils::GetIntegerMapEntry(arguments, "text", TextNone);

		if (textID == TextNone)
		{
			client.ReplyHtmlWithHeaderAndParagraph(Languages::TextTextDoesNotExist);
			return;
		}

		const DataModel::Text* text = manager.GetText(textID);
		if (text == nullptr)
		{
			client.ReplyHtmlWithHeaderAndParagraph(Languages::TextTextDoesNotExist);
			return;
		}

		HtmlTag content;
		const string& textName = text->GetName();
		content.AddContent(HtmlTag("h1").AddContent(Languages::TextDeleteText));
		content.AddContent(HtmlTag("p").AddContent(Languages::TextAreYouSureToDelete, textName));
		content.AddContent(HtmlTag("form").AddId("editform")
			.AddContent(HtmlTagInputHidden("cmd", "textdelete"))
			.AddContent(HtmlTagInputHidden("text", to_string(textID))
			));
		content.AddContent(HtmlTagButtonCancel());
		content.AddContent(HtmlTagButtonOK());
		client.ReplyHtmlWithHeader(content);
	}

	void WebClientText::HandleTextDelete(const map<string, string>& arguments)
	{
		TextID textID = Utils::Utils::GetIntegerMapEntry(arguments, "text", TextNone);
		const DataModel::Text* text = manager.GetText(textID);
		if (text == nullptr)
		{
			client.ReplyResponse(WebClient::ResponseError, Languages::TextTextDoesNotExist);
			return;
		}

		string name = text->GetName();
		string result;
		if (!manager.TextDelete(textID, result))
		{
			client.ReplyResponse(WebClient::ResponseError, result);
			return;
		}

		client.ReplyResponse(WebClient::ResponseInfo, Languages::TextTextDeleted, name);
	}

	void WebClientText::HandleTextGet(const map<string, string>& arguments)
	{
		TextID textID = Utils::Utils::GetIntegerMapEntry(arguments, "text");
		const DataModel::Text* text = manager.GetText(textID);
		if (text == nullptr)
		{
			client.ReplyHtmlWithHeader(HtmlTag());
			return;
		}
		client.ReplyHtmlWithHeader(HtmlTagText(text));
	}
} // namespace WebServer
