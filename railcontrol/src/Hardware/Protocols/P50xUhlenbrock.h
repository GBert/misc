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
#include <string>

#include "Hardware/Protocols/P50xSerial.h"
#include "Languages.h"

namespace Hardware
{
	class HardwareParams;

	namespace Protocols
	{
		class P50xUhlenbrock: public P50xSerial
		{
			public:
				P50xUhlenbrock() = delete;
				P50xUhlenbrock(const P50xUhlenbrock&) = delete;
				P50xUhlenbrock& operator=(const P50xUhlenbrock&) = delete;

				P50xUhlenbrock(const HardwareParams* params, const std::string& controlName)
				:	P50xSerial(params, controlName, TypeUhlenbrock)
				{
				}

				virtual ~P50xUhlenbrock()
				{
				}
		};
	} // namespace
} // namespace
