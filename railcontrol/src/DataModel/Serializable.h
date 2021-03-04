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

namespace DataModel {

	class Serializable {
		public:
			virtual ~Serializable() {};
			virtual std::string Serialize() const = 0;
			virtual bool Deserialize(const std::string& serialized) = 0;

		protected:
			static void ParseArguments(const std::string& serialized, std::map<std::string,std::string>& arguments);
	};

} // namespace DataModel

