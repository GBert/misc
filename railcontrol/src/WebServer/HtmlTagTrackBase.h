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

			inline bool IsAttributeSet(const std::string& name)
			{
				return childTags[0].IsAttributeSet(name);
			}

		protected:
			HtmlTagTrackBase(const Manager& manager,
				const DataModel::TrackBase* track,
				const DataModel::LayoutItem* layout);

			virtual ~HtmlTagTrackBase()
			{
			}

			const DataModel::TrackBase* track;

			std::string urlIdentifier;
	};
} // namespace WebServer

