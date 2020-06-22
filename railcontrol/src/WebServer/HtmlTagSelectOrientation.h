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

#include <map>

#include "Languages.h"
#include "WebServer/HtmlTag.h"
#include "WebServer/HtmlTagSelect.h"

namespace WebServer
{
	class HtmlTagSelectOrientation : public HtmlTag
	{
		public:
			HtmlTagSelectOrientation() = delete;

			HtmlTagSelectOrientation(const std::string& name, const Orientation defaultValue)
			{
				std::map<Orientation,Languages::TextSelector> orientations;
				orientations[OrientationLeft] = Languages::TextLeft;
				orientations[OrientationRight] = Languages::TextRight;
				AddChildTag(HtmlTagSelect(name, orientations, defaultValue));
			}

			virtual ~HtmlTagSelectOrientation() {}

			virtual HtmlTag AddAttribute(const std::string& name, const std::string& value) override
			{
				childTags[0].AddAttribute(name, value);
				return *this;
			}

			virtual HtmlTag AddClass(const std::string& className) override
			{
				childTags[0].AddClass(className);
				return *this;
			}
	};
} // namespace WebServer

