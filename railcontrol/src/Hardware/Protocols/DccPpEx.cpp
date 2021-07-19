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

#include <string>

#include "Hardware/Protocols/DccPpEx.h"

using std::string;

namespace Hardware
{
	namespace Protocols
	{
		void DccPpEx::Booster(const BoosterState status)
		{
			string buffer("<");
			logger->Info(status ? Languages::TextTurningBoosterOn : Languages::TextTurningBoosterOff);
			buffer += std::to_string(static_cast<unsigned char>(status));
			buffer += ">";
			SendInternal(buffer);
		}

		void DccPpEx::LocoSpeed(__attribute__((unused)) const Protocol protocol,
			__attribute__((unused)) const Address address,
			__attribute__((unused)) const Speed speed)
		{
		}

		void DccPpEx::LocoOrientation(__attribute__((unused)) const Protocol protocol,
			__attribute__((unused)) const Address address,
			__attribute__((unused)) const Orientation orientation)
		{
		}

		void DccPpEx::LocoFunction(__attribute__((unused)) const Protocol protocol,
			__attribute__((unused)) const Address address,
			__attribute__((unused)) const DataModel::LocoFunctionNr function,
			__attribute__((unused)) const DataModel::LocoFunctionState on)
		{
		}

		void DccPpEx::AccessoryOnOrOff(__attribute__((unused)) const Protocol protocol,
			__attribute__((unused)) const Address address,
			__attribute__((unused)) const DataModel::AccessoryState state,
			__attribute__((unused)) const bool on)
		{
		}

		void DccPpEx::ProgramWrite(__attribute__((unused)) const ProgramMode mode,
			__attribute__((unused)) const Address address,
			__attribute__((unused)) const CvNumber cv,
			__attribute__((unused)) const CvValue value)
		{
		}
	} // namespace
} // namespace
