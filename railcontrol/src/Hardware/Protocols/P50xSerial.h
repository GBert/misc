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

#include <string>

#include "Hardware/Protocols/P50x.h"
#include "Logger/Logger.h"
#include "Network/Serial.h"

namespace Hardware
{
	class HardwareParams;

	namespace Protocols
	{
		class P50xSerial: public P50x
		{
			public:
				P50xSerial() = delete;
				P50xSerial(const P50xSerial&) = delete;
				P50xSerial& operator=(const P50xSerial&) = delete;

				inline P50xSerial(const HardwareParams* const params,
				    const std::string& controlName,
				    const P50xType type)
				:	P50x(params,
						controlName + " / " + params->GetName() + " at serial port " + params->GetArg1(),
						type),
					serialLine(logger, params->GetArg1(), B19200, 8, 'N', 2)
				{
				}

				virtual ~P50xSerial()
				{
				}

			protected:
				inline int Send(const unsigned char* data, const size_t length) const override
				{
					return serialLine.Send(data, length);
				}

				inline ssize_t Receive(unsigned char* data, const size_t length) const override
				{
					return serialLine.Receive(data, length, 1, 0);
				}

				inline ssize_t ReceiveExact(unsigned char* data, const size_t length) const override
				{
					return serialLine.ReceiveExact(data, length, 1, 0);
				}

			private:
				mutable Network::Serial serialLine;
		};
	} // namespace
} // namespace
