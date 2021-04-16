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

#include "DataTypes.h"
#include "DataModel/Serializable.h"

namespace DataModel
{
	class HardwareHandle
	{
			// simply stores controlID, protocol and base address of an object
			// this is only to prevent copying code and use a base class instead
		public:
			inline HardwareHandle()
			:	controlID(ControlNone),
			 	protocol(ProtocolNone),
			 	address(AddressNone)
			{
			}

			virtual ~HardwareHandle()
			{
			}

			inline void SetControlID(ControlID controlID)
			{
				this->controlID = controlID;
			}

			inline ControlID GetControlID() const
			{
				return controlID;
			}

			inline void SetProtocol(Protocol protocol)
			{
				this->protocol = protocol;
			}

			inline Protocol GetProtocol() const
			{
				return protocol;
			}

			inline void SetAddress(Address address)
			{
				this->address = address;
			}

			inline Address GetAddress() const
			{
				return address;
			}

		protected:
			virtual std::string Serialize() const;
			virtual bool Deserialize(const std::map<std::string,std::string>& arguments);

		private:
			ControlID controlID;
			Protocol protocol;
			Address address;
	};
} // namespace DataModel

