/*
RailControl - Model Railway Control Software

Copyright (c) 2017-2020 Dominik (Teddy) Mahrer - www.railcontrol.org

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
#include <sstream>

#include "DataTypes.h"
#include "DataModel/Serializable.h"

namespace DataModel
{
	class HardwareHandle : protected Serializable
	{
		public:
			HardwareHandle(const controlID_t controlID,
				const protocol_t protocol,
				const address_t address)
			:	controlID(controlID),
				protocol(protocol),
				address(address)
			{
			}

			HardwareHandle(const std::string& serialized)
			{
				Deserialize(serialized);
			}

			HardwareHandle() {}

			virtual std::string Serialize() const override;
			virtual bool Deserialize(const std::string& serialized) override;

			void SetControlID(controlID_t controlID) { this->controlID = controlID; }
			controlID_t GetControlID() const { return controlID; }
			void SetProtocol(protocol_t protocol) { this->protocol = protocol; }
			protocol_t GetProtocol() const { return protocol; }
			void SetAddress(address_t address) { this->address = address; }
			address_t GetAddress() const { return address; }

		protected:
			virtual bool Deserialize(const std::map<std::string,std::string>& arguments);

		private:
			controlID_t controlID;
			protocol_t protocol;
			address_t address;
	};
} // namespace DataModel

