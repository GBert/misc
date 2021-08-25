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

#include "Hardware/Protocols/Z21.h"

namespace Hardware
{
	class Z21 : Protocols::Z21
	{
		public:
			Z21() = delete;
			Z21(const Z21&) = delete;
			Z21& operator=(const Z21&) = delete;

			inline Z21(const HardwareParams* params)
			:	Protocols::Z21(params, "Z21")
			{
			}
	};
} // namespace

