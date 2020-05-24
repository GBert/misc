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

#include "WebServer/HtmlTag.h"
#include "WebServer/HtmlTagLabel.h"
#include "WebServer/HtmlTagInputCheckbox.h"

namespace WebServer
{
	class HtmlTagInputCheckboxWithLabel : public HtmlTag
	{
		public:
			HtmlTagInputCheckboxWithLabel() = delete;
			HtmlTagInputCheckboxWithLabel(const std::string& name, const Languages::TextSelector label, const std::string& value, const bool checked)
			:	HtmlTag()
			{
				AddChildTag(HtmlTagLabel(label, name));
				AddChildTag(HtmlTagInputCheckbox(name, value, checked));
				AddChildTag(HtmlTag("br"));
			}

			virtual ~HtmlTagInputCheckboxWithLabel() {}

			virtual HtmlTag AddAttribute(const std::string& name, const std::string& value) override
			{
				childTags[1].AddAttribute(name, value);
				return *this;
			}

			virtual HtmlTag AddClass(const std::string& _class) override
			{
				childTags[1].AddClass(_class);
				return *this;
			}
	};
};

