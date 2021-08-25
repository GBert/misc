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

#include <map>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

#include "Languages.h"
#include "Logger/Logger.h"

namespace WebServer
{
	class HtmlTag
	{
		public:
			HtmlTag& operator=(const HtmlTag&) = delete;

			inline HtmlTag()
			{
			}

			inline HtmlTag(const std::string& name)
			:	name(name)
			{
			}

			inline virtual ~HtmlTag()
			{
			}

			virtual HtmlTag AddAttribute(const std::string& name, const std::string& value = "");

			inline bool IsAttributeSet(const std::string& name)
			{
				return attributes.count(name) == 1;
			}

			inline virtual HtmlTag AddChildTag(const HtmlTag& child)
			{
				this->childTags.push_back(child);
				return *this;
			}

			inline virtual HtmlTag AddContent(const std::string& content)
			{
				this->content += content;
				return *this;
			}

			template<typename... Args>
			inline HtmlTag AddContent(const Languages::TextSelector text, Args... args)
			{
				return AddContent(Logger::Logger::Format(Languages::GetText(text), args...));
			}

			inline virtual HtmlTag AddClass(const std::string& className)
			{
				if (className.length() > 0)
				{
					classes.push_back(className);
				}
				return *this;
			}

			inline virtual HtmlTag AddId(const std::string& id)
			{
				this->id = id;
				return *this;
			}

			inline virtual size_t ContentSize() const
			{
				return content.size();
			}

			inline size_t ChildCount() const
			{
				return childTags.size();
			}

			inline operator std::string () const
			{
				std::stringstream ss;
				ss << *this;
				return ss.str();
			}

			friend std::ostream& operator<<(std::ostream& stream, const HtmlTag& tag);

		protected:
			std::string name;
			std::vector<HtmlTag> childTags;
			std::map<std::string, std::string> attributes;
			std::vector<std::string> classes;
			std::string content;
			std::string id;
	};
} // namespace WebServer

