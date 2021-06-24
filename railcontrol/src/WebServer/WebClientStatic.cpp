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

#include <algorithm>

#include "DataModel/DataModel.h"
#include "DataModel/LocoConfig.h"
#include "Hardware/HardwareHandler.h"
#include "Utils/Utils.h"
#include "WebServer/HtmlTagButton.h"
#include "WebServer/HtmlTagInputHidden.h"
#include "WebServer/HtmlTagInputIntegerWithLabel.h"
#include "WebServer/HtmlTagInputTextWithLabel.h"
#include "WebServer/HtmlTagSelectWithLabel.h"
#include "WebServer/WebClientStatic.h"

using namespace DataModel;
using LayoutRotation = DataModel::LayoutItem::LayoutRotation;
using std::map;
using std::string;
using std::to_string;
using std::vector;

namespace WebServer
{
	HtmlTag WebClientStatic::HtmlTagControlArgument(const unsigned char argNr, const ArgumentType type, const string& value)
	{
		Languages::TextSelector argumentName;
		string argumentNumber = "arg" + to_string(argNr);
		switch (type)
		{
			case ArgumentTypeIpAddress:
				argumentName = Languages::TextIPAddress;
				break;

			case ArgumentTypeSerialPort:
			{
				argumentName = Languages::TextSerialPort;
#ifdef __CYGWIN__
				vector<unsigned char> comPorts;
				bool ret = Utils::Utils::GetComPorts(comPorts);
				if (ret == false || comPorts.size() == 0)
				{
					break;
				}
				map<string,string> comPortOptions;
				for (auto comPort : comPorts)
				{
					comPortOptions["/dev/ttyS" + to_string(comPort)] = "COM" + to_string(comPort + 1);
				}
				return HtmlTagSelectWithLabel(argumentNumber, argumentName, comPortOptions, value);
#else
				break;
#endif
			}

			case ArgumentTypeS88Modules:
			{
				argumentName = Languages::TextNrOfS88Modules;
				const int valueInteger = Utils::Utils::StringToInteger(value, 0, 62);
				return HtmlTagInputIntegerWithLabel(argumentNumber, argumentName, valueInteger, 0, 62);
			}

			default:
				return HtmlTag();
		}
		return HtmlTagInputTextWithLabel(argumentNumber, argumentName, value);
	}

	HtmlTag WebClientStatic::HtmlTagControlArguments(const HardwareType hardwareType, const string& arg1, const string& arg2, const string& arg3, const string& arg4, const string& arg5)
	{
		HtmlTag div;
		std::map<unsigned char,ArgumentType> argumentTypes;
		std::string hint;
		Hardware::HardwareHandler::ArgumentTypesOfHardwareTypeAndHint(hardwareType, argumentTypes, hint);
		if (argumentTypes.count(1) == 1)
		{
			div.AddChildTag(HtmlTagControlArgument(1, argumentTypes.at(1), arg1));
		}
		if (argumentTypes.count(2) == 1)
		{
			div.AddChildTag(HtmlTagControlArgument(2, argumentTypes.at(2), arg2));
		}
		if (argumentTypes.count(3) == 1)
		{
			div.AddChildTag(HtmlTagControlArgument(3, argumentTypes.at(3), arg3));
		}
		if (argumentTypes.count(4) == 1)
		{
			div.AddChildTag(HtmlTagControlArgument(4, argumentTypes.at(4), arg4));
		}
		if (argumentTypes.count(5) == 1)
		{
			div.AddChildTag(HtmlTagControlArgument(5, argumentTypes.at(5), arg5));
		}
		if (hint.size() > 0)
		{
			div.AddChildTag(HtmlTag("div").AddContent(Languages::GetText(Languages::TextHint)).AddContent(HtmlTag("br")).AddContent(hint));
		}
		return div;
	}

	const std::map<string,HardwareType> WebClientStatic::ListHardwareNames()
	{
		std::map<string,HardwareType> hardwareList;
		hardwareList["CC-Schnitte"] = HardwareTypeCcSchnitte;
		hardwareList["Digikeijs DR5000"] = HardwareTypeDR5000;
		hardwareList["ESU Ecos &amp; Märklin CS1"] = HardwareTypeEcos;
		hardwareList["Fleischmann TwinCenter"] = HardwareTypeTwinCenter;
		hardwareList["LDT HSI-88 RS-232"] = HardwareTypeHsi88;
		hardwareList["LokStoreDigital LoDi-Rektor (beta)"] = HardwareTypeRektor;
		hardwareList["Märklin Central Station 1 (CS1)"] = HardwareTypeEcos;
		hardwareList["Märklin Central Station 2/3 (CS2/CS3) TCP"] = HardwareTypeCS2Tcp;
		hardwareList["Märklin Central Station 2/3 (CS2/CS3) UDP"] = HardwareTypeCS2Udp;
		hardwareList["Märklin Interface 6050/6051"] = HardwareTypeM6051;
		hardwareList["OpenDCC Z1"] = HardwareTypeOpenDcc;
		hardwareList["RM485"] = HardwareTypeRM485;
		hardwareList["Roco Z21"] = HardwareTypeZ21;
		hardwareList["Tams MasterControl (beta)"] = HardwareTypeMasterControl;
		hardwareList["Tams MasterControl 2 (beta)"] = HardwareTypeMasterControl2;
		hardwareList["Tams RedBox (beta)"] = HardwareTypeRedBox;
		hardwareList["Uhlenbrock Intellibox (beta)"] = HardwareTypeIntellibox;
		hardwareList["Virtual Command Station"] = HardwareTypeVirtual;
		return hardwareList;
	}

	HtmlTag WebClientStatic::HtmlTagMatchKey(const map<string,LocoConfig>& matchKeyMap, const string& selectedMatchKey)
	{
		if (matchKeyMap.size() == 0)
		{
			return HtmlTagInputHidden("matchkey", "");
		}

		map<string,string> options;
		for (auto& matchKey : matchKeyMap)
		{
			options[matchKey.first] = matchKey.second.GetName();
		}
		HtmlTag content;
		content.AddChildTag(HtmlTagLabel(Languages::TextNameInControl, "matchkey"));
		content.AddChildTag(HtmlTagSelect("matchkey", options, selectedMatchKey));
		return content;
	}

	HtmlTag WebClientStatic::HtmlTagProtocol(const map<string,Protocol>& protocolMap, const Protocol selectedProtocol)
	{
		size_t mapSize = protocolMap.size();
		switch (mapSize)
		{
			case 0:
				return HtmlTagInputHidden("protocol", std::to_string(ProtocolNone));

			case 1:
				return HtmlTagInputHidden("protocol", std::to_string(protocolMap.begin()->second));

			default:
				HtmlTag content;
				content.AddChildTag(HtmlTagLabel(Languages::TextProtocol, "protocol"));
				content.AddChildTag(HtmlTagSelect("protocol", protocolMap, selectedProtocol));
				return content;
		}
	}

	HtmlTag WebClientStatic::HtmlTagDuration(const DataModel::AccessoryPulseDuration duration, const Languages::TextSelector label)
	{
		std::map<string,string> durationOptions;
		durationOptions["0000"] = "0";
		durationOptions["0100"] = "100";
		durationOptions["0250"] = "250";
		durationOptions["1000"] = "1000";
		return HtmlTagSelectWithLabel("duration", label, durationOptions, Utils::Utils::ToStringWithLeadingZeros(duration, 4));
	}

	HtmlTag WebClientStatic::HtmlTagSlaveEntry(const string& prefix,
		const string& priority,
		const ObjectID objectId,
		const map<string,ObjectID>& options)
	{
		HtmlTag content("div");
		content.AddId(prefix + "_priority_" + priority);
		HtmlTagButton deleteButton(Languages::TextDelete, prefix + "_delete_" + priority);
		deleteButton.AddAttribute("onclick", "deleteElement('" + prefix + "_priority_" + priority + "');return false;");
		deleteButton.AddClass("wide_button");
		content.AddChildTag(deleteButton);

		HtmlTag contentObject("div");
		contentObject.AddId(prefix + "_object_" + priority);
		contentObject.AddClass("inline-block");

		contentObject.AddChildTag(HtmlTagSelect(prefix + "_id_" + priority, options, objectId).AddClass("select_slave_id"));
		content.AddChildTag(contentObject);
		return content;
	}

	HtmlTag WebClientStatic::HtmlTagRotation(const LayoutRotation rotation)
	{
		HtmlTag content;
		std::map<LayoutRotation, Languages::TextSelector> rotationOptions;
		rotationOptions[DataModel::LayoutItem::Rotation0] = Languages::TextNoRotation;
		rotationOptions[DataModel::LayoutItem::Rotation90] = Languages::Text90DegClockwise;
		rotationOptions[DataModel::LayoutItem::Rotation180] = Languages::Text180Deg;
		rotationOptions[DataModel::LayoutItem::Rotation270] = Languages::Text90DegAntiClockwise;
		content.AddChildTag(HtmlTagSelectWithLabel("rotation", Languages::TextRotation, rotationOptions, rotation));
		content.AddChildTag(HtmlTag("p").AddContent(Languages::GetText(Languages::TextHint)).AddContent(HtmlTag("br")).AddContent(Languages::GetText(Languages::TextHintPositionRotate)));
		return content;
	}

	HtmlTag WebClientStatic::HtmlTagSelectSelectRouteApproach(const DataModel::SelectRouteApproach selectRouteApproach, const bool addDefault)
	{
		map<DataModel::SelectRouteApproach,Languages::TextSelector> options;
		if (addDefault)
		{
			options[DataModel::SelectRouteSystemDefault] = Languages::TextSystemDefault;
		}
		options[DataModel::SelectRouteDoNotCare] = Languages::TextDoNotCare;
		options[DataModel::SelectRouteRandom] = Languages::TextRandom;
		options[DataModel::SelectRouteMinTrackLength] = Languages::TextMinTrackLength;
		options[DataModel::SelectRouteLongestUnused] = Languages::TextLongestUnused;
		return HtmlTagSelectWithLabel("selectrouteapproach", Languages::TextSelectRouteBy, options, selectRouteApproach);
	}

	HtmlTag WebClientStatic::HtmlTagNrOfTracksToReserve(const DataModel::Loco::NrOfTracksToReserve nrOfTracksToReserve)
	{
		map<DataModel::Loco::NrOfTracksToReserve,string> options;
		options[DataModel::Loco::ReserveOne] = "1";
		options[DataModel::Loco::ReserveTwo] = "2";
		return HtmlTagSelectWithLabel("nroftrackstoreserve", Languages::TextNrOfTracksToReserve, options, nrOfTracksToReserve);
	}

	HtmlTag WebClientStatic::HtmlTagLogLevel()
	{
		map<Logger::Logger::Level,Languages::TextSelector> options;
		options[Logger::Logger::LevelOff] = Languages::TextOff;
		options[Logger::Logger::LevelError] = Languages::TextError;
		options[Logger::Logger::LevelWarning] = Languages::TextWarning;
		options[Logger::Logger::LevelInfo] = Languages::TextInfo;
		options[Logger::Logger::LevelDebug] = Languages::TextDebug;
		return HtmlTagSelectWithLabel("loglevel", Languages::TextLogLevel, options, Logger::Logger::GetLogLevel());
	}

	HtmlTag WebClientStatic::HtmlTagLanguage()
	{
		map<Languages::Language,Languages::TextSelector> options;
		options[Languages::EN] = Languages::TextEnglish;
		options[Languages::DE] = Languages::TextGerman;
		options[Languages::ES] = Languages::TextSpanish;
		return HtmlTagSelectWithLabel("language", Languages::TextLanguage, options, Languages::GetDefaultLanguage());
	}

	HtmlTag WebClientStatic::HtmlTagControl(const std::map<ControlID,string>& controls, const ControlID controlID, const string& objectType, const ObjectID objectID)
	{
		ControlID controlIdMutable = controlID;
		if (controls.size() == 0)
		{
			return HtmlTagInputTextWithLabel("control", Languages::TextControl, Languages::GetText(Languages::TextConfigureControlFirst));
		}
		bool controlIdValid = false;
		if (controlIdMutable != ControlIdNone)
		{
			for (auto& control : controls)
			{
				if (control.first != controlIdMutable)
				{
					continue;
				}
				controlIdValid = true;
				break;
			}
		}
		if (!controlIdValid)
		{
			controlIdMutable = controls.begin()->first;
		}
		if (controls.size() == 1)
		{
			return HtmlTagInputHidden("control", to_string(controlIdMutable));
		}
		std::map<string, string> controlOptions;
		for (auto& control : controls)
		{
			controlOptions[to_string(control.first)] = control.second;
		}
		return HtmlTagSelectWithLabel("control", Languages::TextControl, controlOptions, to_string(controlIdMutable)).AddAttribute("onchange", "loadProtocol('" + objectType + "', " + to_string(objectID) + ")");
	}

	HtmlTag WebClientStatic::HtmlTagControl(const string& name, const std::map<ControlID,string>& controls)
	{
		ControlID controlIdFirst = controls.begin()->first;
		if (controls.size() == 1)
		{
			return HtmlTagInputHidden("s_" + name, to_string(controlIdFirst));
		}
		return HtmlTagSelectWithLabel(name, Languages::TextControl, controls, controlIdFirst).AddAttribute("onchange", "loadProgramModeSelector();");
	}

	vector<ObjectID> WebClientStatic::InterpretSlaveData(const string& prefix, const map<string, string>& arguments)
	{
		vector<ObjectID> ids;
		unsigned int count = Utils::Utils::GetIntegerMapEntry(arguments, prefix + "counter", 0);
		for (unsigned int index = 1; index <= count; ++index)
		{
			string indexAsString = to_string(index);
			ObjectID id = Utils::Utils::GetIntegerMapEntry(arguments, prefix + "_id_" + indexAsString, TrackNone);
			if (id == TrackNone)
			{
				continue;
			}
			ids.push_back(id);
		}

		std::sort(ids.begin(), ids.end());
		ids.erase(std::unique(ids.begin(), ids.end()), ids.end());
		return ids;
	}
} // namespace WebServer
