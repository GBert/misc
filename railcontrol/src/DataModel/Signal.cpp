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

#include <map>

#include "DataModel/Signal.h"
#include "Manager.h"
#include "Utils/Utils.h"

using std::map;
using std::string;

namespace DataModel
{
	std::string Signal::Serialize() const
	{
		string str = "objectType=Signal;";
		str += AccessoryBase::Serialize();
		str += ";";
		str += TrackBase::Serialize();
		str += ";";
		str += LayoutItem::Serialize();
		str += ";";
		str += LockableItem::Serialize();
		str += ";signalorientation=";
		str += std::to_string(signalOrientation);
		for (auto& stateAddress : stateAddressMap)
		{
			str += ";address";
			str += std::to_string(stateAddress.first);
			str += "=";
			str += std::to_string(stateAddress.second);
		}
		return str;
	}

	bool Signal::Deserialize(const std::string& serialized)
	{
		map<string,string> arguments;
		ParseArguments(serialized, arguments);
		string objectType = Utils::Utils::GetStringMapEntry(arguments, "objectType");
		if (objectType.compare("Signal") != 0)
		{
			return false;
		}

		AccessoryBase::Deserialize(arguments);
		TrackBase::Deserialize(arguments);
		LayoutItem::Deserialize(arguments);
		LockableItem::Deserialize(arguments);
		SetWidth(Width1);
		SetVisible(VisibleYes);
		signalOrientation = static_cast<Orientation>(Utils::Utils::GetBoolMapEntry(arguments, "signalorientation", OrientationRight));
		for (int i = 0; i < SignalStateMax; ++i)
		{
			int address = Utils::Utils::GetIntegerMapEntry(arguments, "address" + std::to_string(i), -1);
			if (address == -1)
			{
				continue;
			}
			SetStateAddressOffset(static_cast<AccessoryState>(i), address);
		}
		return true;
	}

	bool Signal::ReleaseInternal(Logger::Logger* logger, const LocoID locoID)
	{
		bool ret = LockableItem::Release(logger, locoID);
		if (ret == false)
		{
			return false;
		}

		SetAccessoryState(SignalStateStop);
		PublishState();
		return true;
	}

	void Signal::PublishState() const
	{
		manager->SignalPublishState(ControlTypeInternal, this);
	}

	void Signal::ResetStateAddressMap()
	{
		stateAddressMap.clear();
		SetStateAddressOffset(SignalStateStop, 0);
		SetStateAddressOffset(SignalStateClear, 1);
		switch(accessoryType)
		{
			case SignalTypeDeCombined:
				SetStateAddressOffset(SignalStateAspect2, 2);
				break;

			case SignalTypeChDwarf:
				SetStateAddressOffset(SignalStateAspect2, 2);
				break;

			case SignalTypeChLMain:
				SetStateAddressOffset(SignalStateAspect2, 2);
				SetStateAddressOffset(SignalStateAspect3, 3);
				SetStateAddressOffset(SignalStateAspect5, 4);
				SetStateAddressOffset(SignalStateAspect6, 5);
				break;

			case SignalTypeSimpleLeft:
			case SignalTypeSimpleRight:
			default:
				break;
		}
	}

	std::map<DataModel::AccessoryState,Signal::StateOption> Signal::GetStateOptions() const
	{
		std::map<DataModel::AccessoryState,Signal::StateOption> out;
		out.emplace(SignalStateStop, StateOption(Languages::TextSignalStateStop, GetStateAddressOffset(SignalStateStop)));
		out.emplace(SignalStateClear, StateOption(Languages::TextSignalStateClear, GetStateAddressOffset(SignalStateClear)));
		switch(accessoryType)
		{
			case SignalTypeDeCombined:
				out.emplace(SignalStateAspect2, StateOption(Languages::TextSignalStateStopExpected, GetStateAddressOffset(SignalStateAspect2)));
				break;

			case SignalTypeChDwarf:
				out.emplace(SignalStateAspect2, StateOption(Languages::TextSignalStateCaution, GetStateAddressOffset(SignalStateAspect2)));
				break;

			case SignalTypeChLMain:
				out.emplace(SignalStateAspect2, StateOption(Languages::TextSignalStateClear40, GetStateAddressOffset(SignalStateAspect2)));
				out.emplace(SignalStateAspect3, StateOption(Languages::TextSignalStateClear60, GetStateAddressOffset(SignalStateAspect3)));
				out.emplace(SignalStateAspect5, StateOption(Languages::TextSignalStateClear90, GetStateAddressOffset(SignalStateAspect5)));
				out.emplace(SignalStateAspect6, StateOption(Languages::TextSignalStateShortClear, GetStateAddressOffset(SignalStateAspect6)));
				break;

			case SignalTypeSimpleLeft:
			case SignalTypeSimpleRight:
			default:
				break;
		}
		return out;
	}
} // namespace DataModel

