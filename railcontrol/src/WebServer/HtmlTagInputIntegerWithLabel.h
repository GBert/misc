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

#include "Languages.h"
#include "WebServer/HtmlTag.h"
#include "WebServer/HtmlTagLabel.h"
#include "WebServer/HtmlTagInputInteger.h"

namespace WebServer
{
	class HtmlTagInputIntegerWithLabel : public HtmlTag
	{
		public:
			HtmlTagInputIntegerWithLabel() = delete;

			HtmlTagInputIntegerWithLabel(const std::string& name, const Languages::TextSelector label, const int min, const int max)
			:	HtmlTagInputIntegerWithLabel(name, label, 0, min, max)
			{}

			template<typename... TextArgs>
			HtmlTagInputIntegerWithLabel(const std::string& name, const Languages::TextSelector label, const int value, const int min, const int max, TextArgs... textArgs)
			:	HtmlTag("div")
			{
				HtmlTag::AddClass("input_integer_with_label");
				AddChildTag(HtmlTagLabel(label, name, textArgs...));
				AddChildTag(HtmlTagInputInteger(name, value, min, max));
			}

			virtual ~HtmlTagInputIntegerWithLabel() {}

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

