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

#include <map>
#include <sstream>
#include <string>

#include "DataModel/LayoutItem.h"
#include "Utils/Utils.h"

using std::map;
using std::stringstream;
using std::string;

namespace DataModel
{
	bool LayoutItem::MapPosition(const LayoutPosition posX,
			const LayoutPosition posY,
			const LayoutItemSize width,
			const LayoutItemSize height,
			const LayoutRotation rotation,
			LayoutPosition& x,
			LayoutPosition& y,
			LayoutItemSize& w,
			LayoutItemSize& h)
	{
		x = posX;
		y = posY;
		switch (rotation)
		{
			case Rotation0:
			case Rotation180:
				w = width;
				h = height;
				return true;

			case Rotation90:
			case Rotation270:
				w = height;
				h = width;
				return true;

			default:
				return false;
		}
	}

	bool LayoutItem::CheckPositionFree(const LayoutPosition posX, const LayoutPosition posY, const LayoutPosition posZ)
	{
		if (this->visible == false)
		{
			return true;
		}

		if (this->posZ != posZ)
		{
			return true;
		}

		LayoutPosition x;
		LayoutPosition y;
		LayoutItemSize w;
		LayoutItemSize h;
		bool ret = MapPosition(this->posX, this->posY, this->width, this->height, this->rotation, x, y, w, h);
		if (ret == false)
		{
			return false;
		}

		for(LayoutPosition ix = x; ix < x + w; ix++)
		{
			for(LayoutPosition iy = y; iy < y + h; iy++)
			{
				if (ix == posX && iy == posY)
				{
					return false;
				}
			}
		}
		return true;
	}

	std::string LayoutItem::Serialize() const
	{
		stringstream ss;
		ss << Object::Serialize()
			<< ";visible=" << static_cast<int>(visible)
			<< ";posX=" << static_cast<int>(posX)
			<< ";posY=" << static_cast<int>(posY)
			<< ";posZ=" << static_cast<int>(posZ)
			<< ";width=" << static_cast<int>(width)
			<< ";height=" << static_cast<int>(height)
			<< ";rotation=" << static_cast<int>(rotation);
		return ss.str();
	}

	bool LayoutItem::Deserialize(const std::string& serialized)
	{
		map<string,string> arguments;
		ParseArguments(serialized, arguments);
		return Deserialize(arguments);
	}

	bool LayoutItem::Deserialize(const map<string,string>& arguments)
	{
		Object::Deserialize(arguments);
		visible = static_cast<Visible>(Utils::Utils::GetIntegerMapEntry(arguments, "visible"));
		if (visible > VisibleYes)
		{
			visible = VisibleYes;
		}
		posX = Utils::Utils::GetIntegerMapEntry(arguments, "posX", 0);
		posY = Utils::Utils::GetIntegerMapEntry(arguments, "posY", 0);
		posZ = Utils::Utils::GetIntegerMapEntry(arguments, "posZ", 0);
		width = Utils::Utils::GetIntegerMapEntry(arguments, "width", Width1);
		height = Utils::Utils::GetIntegerMapEntry(arguments, "height", Height1);
		rotation = static_cast<LayoutRotation>(Utils::Utils::GetIntegerMapEntry(arguments, "rotation", Rotation0));
		if (rotation > Rotation270)
		{
			rotation = Rotation0;
		}
		return true;
	}

	std::string LayoutItem::Rotation(LayoutRotation rotation)
	{
		switch (rotation)
		{
			case Rotation90:
				return "90";

			case Rotation180:
				return "180";

			case Rotation270:
				return "270";

			case Rotation0:
			default:
				return "0";
		}
	}
} // namespace DataModel

