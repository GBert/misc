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
#include "Network/TcpClient.h"

namespace Hardware
{
	class HardwareParams;

	namespace Protocols
	{
		class P50xEthernet: public P50x
		{
			public:
				P50xEthernet() = delete;
				P50xEthernet(const P50xEthernet&) = delete;
				P50xEthernet& operator=(const P50xEthernet&) = delete;

				inline P50xEthernet(const HardwareParams* params,
				    const std::string& controlName,
				    const P50xType type)
				:	P50x(params,
						controlName + " / " + params->GetName() + " at serial port " + params->GetArg1(),
						type),
					connection(Network::TcpClient::GetTcpClientConnection(logger, params->GetArg1(), P50xPort))
				{
				}

				virtual ~P50xEthernet()
				{
				}

			protected:
				inline ssize_t Receive(unsigned char* data, const size_t length) const override
				{
					return connection.Receive(data, length);
				}

				inline ssize_t ReceiveExact(unsigned char* data, const size_t length) const override
				{
					return connection.ReceiveExact(data, length);
				}

			private:
				inline int Send(const unsigned char* data, const size_t length) const override
				{
					return connection.Send(data, length);
				}

static const unsigned short P50xPort = 8050;

				Network::TcpConnection connection;
		};
	} // namespace
} // namespace
