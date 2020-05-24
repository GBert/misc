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
			HtmlTagFeedback(const DataModel::Feedback* feedback, DataModel::LayoutItem::LayoutPosition posX, DataModel::LayoutItem::LayoutPosition posY);
			HtmlTagFeedback(const DataModel::Feedback* feedback)
			:	HtmlTagFeedback(feedback, feedback->GetPosX(), feedback->GetPosY())
			{}
	};
}; // namespace WebServer

