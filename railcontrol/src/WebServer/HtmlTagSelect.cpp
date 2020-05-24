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

#include "WebServer/HtmlTagSelect.h"

namespace WebServer
{
	HtmlTagSelect::HtmlTagSelect(const std::string& name, const std::map<std::string,std::string>& options, const std::string& defaultValue)
	:	HtmlTag("select"),
	 	commandID("s_" + name)
	{
		AddAttribute("name", name);
		AddAttribute("id", commandID);

		for (auto option : options)
		{
			HtmlTag optionTag("option");
			optionTag.AddAttribute("value", option.first);
			optionTag.AddContent(option.second);
			if (option.first.compare(defaultValue) == 0)
			{
				optionTag.AddAttribute("selected");
			}
			AddChildTag(optionTag);
		}
	}

	HtmlTagSelect::HtmlTagSelect(const std::string& name, const std::map<std::string,Languages::TextSelector>& options, const std::string& defaultValue)
	:	HtmlTag("select"),
	 	commandID("s_" + name)
	{
		AddAttribute("name", name);
		AddAttribute("id", commandID);

		for (auto option : options)
		{
			HtmlTag optionTag("option");
			optionTag.AddAttribute("value", option.first);
			optionTag.AddContent(Languages::GetText(option.second));
			if (option.first.compare(defaultValue) == 0)
			{
				optionTag.AddAttribute("selected");
			}
			AddChildTag(optionTag);
		}
	}
};
