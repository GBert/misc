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
			virtual HtmlTag AddAttribute(const std::string& name, const std::string& value) override
			{
				childTags[0].AddAttribute(name, value);
				return *this;
			}

		protected:
			HtmlTagTrackBase() = delete;
			HtmlTagTrackBase(const Manager& manager,
				const ObjectType objectType,
				const DataModel::TrackType trackType,
				const DataModel::TrackBase* track,
				const DataModel::LayoutItem* layout);

			virtual ~HtmlTagTrackBase() {}

			void FinishInit();
			void AddContextMenuEntry(const std::string& text);
			void AddContextMenuEntry(const Languages::TextSelector text, const std::string& onClick, const std::string& className = "");

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
			HtmlTag menuDiv;
			HtmlTag menuContentDiv;
			std::string identifier;
			std::string urlIdentifier;
	};
}; // namespace WebServer

