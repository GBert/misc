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
#include <string>

#include "DataTypes.h"
#include "DataModel/Object.h"

class Manager;

namespace DataModel
{
	class Layer : public Object
	{
		public:
			Layer(const std::string& serialized) { Deserialize(serialized); }
			Layer(__attribute__((unused)) Manager* manager, const LayerID layerID) : Object(layerID) {}

			virtual std::string Serialize() const { return "objectType=Layer;" + Object::Serialize(); }

			ObjectType GetObjectType() const { return ObjectTypeLayer; }
	};
} // namespace DataModel

