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

#include <stdint.h>

namespace Hardware
{
	typedef uint32_t Capability;
	enum Capabilities : Capability
	{
		CapabilityNone                    = 0x00000000,
		CapabilityLoco                    = 0x00000001,
		CapabilityAccessory               = 0x00000002,
		CapabilityFeedback                = 0x00000004,
		CapabilityProgram                 = 0x00000008,
		CapabilityProgramMmWrite          = 0x00000010,
		CapabilityProgramMmPomWrite       = 0x00000020,
		CapabilityProgramMfxRead          = 0x00000040,
		CapabilityProgramMfxWrite         = 0x00000080,
		CapabilityProgramDccRegisterRead  = 0x00000100,
		CapabilityProgramDccRegisterWrite = 0x00000200,
		CapabilityProgramDccDirectRead    = 0x00000400,
		CapabilityProgramDccDirectWrite   = 0x00000800,
		CapabilityProgramDccPomRead       = 0x00001000,
		CapabilityProgramDccPomWrite      = 0x00002000,

		CapabilityLocoDatabase            = 0x00010000,
	};

	inline Capabilities operator& (const Capabilities c1, const Capabilities c2)
	{
		return static_cast<Capabilities>(static_cast<Capability>(c1) & static_cast<Capability>(c2));
	}

	inline Capabilities operator| (const Capabilities c1, const Capabilities c2)
	{
		return static_cast<Capabilities>(static_cast<Capability>(c1) | static_cast<Capability>(c2));
	}
}
