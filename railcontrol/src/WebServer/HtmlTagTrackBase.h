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

#pragma once

#include <string>

#include "DataModel/TrackBase.h"
#include "WebServer/HtmlTagLayoutItem.h"

class Manager;

namespace WebServer
{
	class HtmlTagTrackBase : public HtmlTagLayoutItem
	{
		public:
			HtmlTagTrackBase() = delete;
			HtmlTagTrackBase(const HtmlTagTrackBase&) = delete;
			HtmlTagTrackBase& operator=(const HtmlTagTrackBase&) = delete;

			virtual HtmlTag AddAttribute(const std::string& name, const std::string& value) override
			{
				childTags[0].AddAttribute(name, value);
				return *this;
			}

		protected:
			HtmlTagTrackBase(const Manager& manager,
				const ObjectType objectType,
				const DataModel::TrackType trackType,
				const DataModel::TrackBase* track,
				const DataModel::LayoutItem* layout);

			virtual ~HtmlTagTrackBase() {}

			void FinishInit();

			inline void AddOnClickMenuEntry(const std::string& text)
			{
				AddMenuEntry(onClickMenuContentDiv, text);
			}

			// FIXME: move to HtmlTagLayoutItem and use code for Accessory, Switch, Feedback and Route too.
			inline void AddOnClickMenuEntry(const Languages::TextSelector text, const std::string& onClick, const std::string& className = "")
			{
				AddMenuEntry(onClickMenuContentDiv, text, onClick, className);
			}

			// FIXME: move to HtmlTagLayoutItem and use code for Accessory, Switch, Feedback and Route too.
			inline void AddContextMenuEntry(const std::string& text)
			{
				AddMenuEntry(contextMenuContentDiv, text);
			}

			// FIXME: move to HtmlTagLayoutItem and use code for Accessory, Switch, Feedback and Route too.
			inline void AddContextMenuEntry(const Languages::TextSelector text, const std::string& onClick, const std::string& className = "")
			{
				AddMenuEntry(contextMenuContentDiv, text, onClick, className);
			}

			// FIXME: move to HtmlTagLayoutItem and use code for Accessory, Switch, Feedback and Route too.
			static void AddMenuEntry(HtmlTag& menu,
				const std::string& text);

			// FIXME: move to HtmlTagLayoutItem and use code for Accessory, Switch, Feedback and Route too.
			static void AddMenuEntry(HtmlTag& menu,
				const Languages::TextSelector text,
				const std::string& onClick,
				const std::string& className = "");

			void AddToolTip(const std::string& toolTip)
			{
				imageDiv.AddChildTag(HtmlTag("span").AddClass("tooltip").AddContent(toolTip));
			}

			const ObjectType objectType;
			const ObjectID id;
			const DataModel::TrackBase* track;
			const DataModel::LayoutItem* layout;

			std::string image;
			HtmlTag imageDiv;
			// FIXME: move to HtmlTagLayoutItem and use code for Accessory, Switch, Feedback and Route too.
			HtmlTag onClickMenuDiv;
			HtmlTag onClickMenuContentDiv;
			HtmlTag contextMenuDiv;
			HtmlTag contextMenuContentDiv;
			std::string identifier;
			std::string urlIdentifier;
	};
} // namespace WebServer

