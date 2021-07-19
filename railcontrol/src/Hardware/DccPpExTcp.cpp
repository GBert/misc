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

#include "Hardware/DccPpExTcp.h"
#include "Utils/Utils.h"

using std::string;

namespace Hardware
{
	DccPpExTcp::DccPpExTcp(const HardwareParams* params)
	:	DccPpEx(params,
			"DCC++EX TCP / " + params->GetName() + " at IP " + params->GetArg1(),
			params->GetName()),
	 	connection(Network::TcpClient::GetTcpClientConnection(logger, params->GetArg1(), Port))
	{
		logger->Info(Languages::TextStarting, GetFullName());

		if (connection.IsConnected() == false)
		{
			logger->Error(Languages::TextUnableToCreateTcpSocket, params->GetArg1(), Port);
		}
	}

	void DccPpExTcp::Send(const string& buffer)
	{
		if (connection.Send(buffer) == -1)
		{
			logger->Error(Languages::TextUnableToSendDataToControl);
		}

		std::string dummy;
		connection.Receive(dummy);
	}
} // namespace
