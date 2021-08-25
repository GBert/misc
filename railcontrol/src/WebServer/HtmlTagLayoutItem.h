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

#include "DataModel/LayoutItem.h"
#include "DataModel/ObjectIdentifier.h"
#include "WebServer/HtmlTag.h"

using std::string;
using std::to_string;

namespace WebServer
{
	class HtmlTagLayoutItem : public HtmlTag
	{
		protected:
			HtmlTagLayoutItem() = delete;
			HtmlTagLayoutItem(const HtmlTagLayoutItem&) = delete;
			HtmlTagLayoutItem& operator=(const HtmlTagLayoutItem&) = delete;

			inline HtmlTagLayoutItem(const DataModel::LayoutItem* layout)
			:	HtmlTagLayoutItem(layout, layout->GetPosX(), layout->GetPosY())
			{
			}

			HtmlTagLayoutItem(const DataModel::LayoutItem* layout,
				const DataModel::LayoutItem::LayoutPosition posX,
				const DataModel::LayoutItem::LayoutPosition posY);

			virtual ~HtmlTagLayoutItem()
			{
			}

			void FinishInit();

			inline void AddOnClickMenuEntry(const std::string& text)
			{
				AddMenuEntry(onClickMenuContentDiv, text);
			}

			inline void AddOnClickMenuEntry(const Languages::TextSelector text, const std::string& onClick, const std::string& className = "")
			{
				AddMenuEntry(onClickMenuContentDiv, text, onClick, className);
			}

			inline void AddContextMenuEntry(const std::string& text)
			{
				AddMenuEntry(contextMenuContentDiv, text);
			}

			inline void AddContextMenuEntry(const Languages::TextSelector text, const std::string& onClick, const std::string& className = "")
			{
				AddMenuEntry(contextMenuContentDiv, text, onClick, className);
			}

			void AddToolTip(const std::string& toolTip)
			{
				imageDiv.AddChildTag(HtmlTag("span").AddClass("tooltip").AddContent(toolTip));
			}

			static const unsigned char EdgeLength = 36;
			static const std::string EdgeLengthString;

			const DataModel::LayoutItem* layout;

			string identifier;
			std::string image;
			HtmlTag imageDiv;
			HtmlTag onClickMenuDiv;
			HtmlTag onClickMenuContentDiv;
			HtmlTag contextMenuDiv;
			HtmlTag contextMenuContentDiv;

			unsigned int layoutPosX;
			unsigned int layoutPosY;

		private:
			static void AddMenuEntry(HtmlTag& menu,
				const std::string& text);

			static void AddMenuEntry(HtmlTag& menu,
				const Languages::TextSelector text,
				const std::string& onClick,
				const std::string& className = "");
	};
} // namespace WebServer

