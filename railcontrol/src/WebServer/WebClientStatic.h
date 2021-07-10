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

//#include <deque>
#include <map>
#include <string>
#include <vector>

#include "WebServer/HtmlTag.h"
#include "WebServer/HtmlTagInputHidden.h"
#include "WebServer/HtmlTagSelectWithLabel.h"

namespace WebServer
{
	class WebClientStatic
	{
		public:
			WebClientStatic() = delete;
			WebClientStatic(const WebClientStatic&) = delete;
			WebClientStatic& operator=(const WebClientStatic&) = delete;

			static HtmlTag HtmlTagSelectSelectRouteApproach(const DataModel::SelectRouteApproach selectRouteApproach,
				const bool addDefault = true);

			static HtmlTag HtmlTagControlArgument(const unsigned char argNr, const ArgumentType type, const std::string& value);

			template<class T>
			static HtmlTag HtmlTagMatchKey(const std::map<std::string,T>& matchKeyMap,
				const std::string& selectedMatchKey)
			{
				if (matchKeyMap.size() == 0)
				{
					return HtmlTagInputHidden("matchkey", "");
				}

				std::map<std::string,std::string> options;
				for (auto& matchKey : matchKeyMap)
				{
					const std::string& name = matchKey.second.GetName();
					const std::string& key = matchKey.second.GetMatchKey();
					options[key] = name + (name != key ? (" (" + key + ")") : "");
				}
				return HtmlTagSelectWithLabel("matchkey", Languages::TextNameInControl, options, selectedMatchKey);
			}


			static HtmlTag HtmlTagProtocol(const std::map<std::string,Protocol>& protocolMap,
				const Protocol selectedProtocol);

			static inline HtmlTag HtmlTagDuration(const DataModel::AccessoryPulseDuration duration)
			{
				return HtmlTagDuration(duration, Languages::TextDuration);
			}

			static HtmlTag HtmlTagDuration(const DataModel::AccessoryPulseDuration duration,
				const Languages::TextSelector label);

			static HtmlTag HtmlTagRotation(const DataModel::LayoutItem::LayoutRotation rotation);

			static HtmlTag HtmlTagNrOfTracksToReserve(const DataModel::Loco::NrOfTracksToReserve nrOfTracksToReserve);
			static HtmlTag HtmlTagLogLevel();
			static HtmlTag HtmlTagLanguage();
			static HtmlTag HtmlTagControlArguments(const HardwareType hardwareType, const std::string& arg1 = "", const std::string& arg2 = "", const std::string& arg3 = "", const std::string& arg4 = "", const std::string& arg5 = "");
			static HtmlTag HtmlTagControl(const std::map<ControlID,std::string>& controls, const ControlID controlID, const std::string& objectType, const ObjectID objectID);
			static HtmlTag HtmlTagControl(const std::string& name, const std::map<ControlID,std::string>& controls);

			static HtmlTag HtmlTagSlaveEntry(const std::string& prefix,
				const std::string& priority,
				const ObjectID objectId,
				const std::map<std::string,ObjectID>& options);

			static std::map<std::string,ObjectID> GetLocoSlaveOptions(const LocoID locoID = LocoNone);

			static const std::map<std::string,HardwareType> ListHardwareNames();

			static std::vector<ObjectID> InterpretSlaveData(const std::string& prefix, const std::map<std::string,std::string>& arguments);
	};

} // namespace WebServer

