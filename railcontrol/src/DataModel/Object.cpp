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

#include <map>
#include <sstream>
#include <string>

#include "DataModel/Object.h"
#include "Utils/Utils.h"

using std::map;
using std::stringstream;
using std::string;

namespace DataModel
{
	std::string Object::Serialize() const
	{
		stringstream ss;
		ss << "objectID=" << (int) objectID << ";name=" << name;
		return ss.str();
	}

	bool Object::Deserialize(const std::string& serialized)
	{
		map<string, string> arguments;
		ParseArguments(serialized, arguments);
		return Deserialize(arguments);
	}

	bool Object::Deserialize(const map<string, string>& arguments)
	{
		objectID = Utils::Utils::GetIntegerMapEntry(arguments, "objectID", ObjectNone);
		name = Utils::Utils::GetStringMapEntry(arguments, "name");
		return true;
	}

} // namespace DataModel

