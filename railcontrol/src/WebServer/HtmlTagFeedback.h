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

#include "DataTypes.h"
#include "WebServer/HtmlTagLayoutItem.h"

namespace DataModel
{
	class Feedback;
}

namespace WebServer
{
	class HtmlTagFeedback : public HtmlTagLayoutItem
	{
		public:
			HtmlTagFeedback() = delete;
			HtmlTagFeedback(const HtmlTagFeedback&) = delete;
			HtmlTagFeedback& operator=(const HtmlTagFeedback&) = delete;

			inline HtmlTagFeedback(const DataModel::Feedback* feedback, const bool onControlLayer = false)
			:	HtmlTagFeedback(feedback,
					onControlLayer ?
						((feedback->GetPin() - 1) & 0x0F) + (((feedback->GetPin() - 1) & 0x0F) >> 3):
						feedback->GetPosX(),
					onControlLayer ?
						(feedback->GetPin() - 1) >> 4 :
						feedback->GetPosY())
			{
			}

		private:
			HtmlTagFeedback(const DataModel::Feedback* feedback,
				const DataModel::LayoutItem::LayoutPosition posX,
				const DataModel::LayoutItem::LayoutPosition posY);
	};
} // namespace WebServer

