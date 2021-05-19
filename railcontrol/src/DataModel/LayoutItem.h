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
#include "DataModel/Object.h"

namespace DataModel
{
	class LayoutItem : public Object
	{
		public:
			enum LayoutRotation : unsigned char
			{
				Rotation0 = 0,
				Rotation90,
				Rotation180,
				Rotation270,
				RotationNotRelevant
			};

			typedef unsigned char LayoutItemSize;
			typedef signed char LayoutPosition;

			enum Visible : unsigned char
			{
				VisibleNo = 0,
				VisibleYes,
				VisibleNotRelevant
			};

			static const LayoutItemSize Width1 = 1;
			static const LayoutItemSize Height1 = 1;
			static const LayoutItemSize Height2 = 2;

			inline LayoutItem(const ObjectID objectID)
			:	Object(objectID),
				visible(VisibleYes),
				posX(0),
				posY(0),
				posZ(0),
				width(Width1),
				height(Height1),
				rotation(Rotation0)
			{
			}

			inline LayoutItem()
			:	LayoutItem(0)
			{
			}

			virtual ~LayoutItem() {}

			static bool MapPosition(const LayoutPosition posX, const LayoutPosition posY, const LayoutItemSize width, const LayoutItemSize height, const LayoutRotation rotation, LayoutPosition& x, LayoutPosition& y, LayoutItemSize& w, LayoutItemSize& h);

			virtual bool Position(LayoutPosition& x,
				LayoutPosition& y,
				LayoutPosition& z,
				LayoutItemSize& w,
				LayoutItemSize& h,
				LayoutRotation& r) const
			{
				z = posZ;
				r = rotation;
				return MapPosition(posX, posY, width, height, rotation, x, y, w, h);
			}


			virtual bool CheckPositionFree(const LayoutPosition posX, const LayoutPosition posY, const LayoutPosition posZ);
			virtual std::string Serialize() const override;
			virtual bool Deserialize(const std::string& serialized) override;
			virtual std::string GetLayoutType() const = 0;

			inline void SetVisible(const Visible visible)
			{
				this->visible = visible;
			}

			inline Visible GetVisible() const
			{
				return visible;
			}

			inline void SetPosX(const LayoutPosition x)
			{
				this->posX = x;
			}

			inline LayoutPosition GetPosX() const
			{
				return posX;
			}

			inline void SetPosY(const LayoutPosition y)
			{
				this->posY = y;
			}

			inline LayoutPosition GetPosY() const
			{
				return posY;
			}

			inline void SetPosZ(const LayoutPosition z)
			{
				this->posZ = z;
			}

			inline LayoutPosition GetPosZ() const
			{
				return posZ;
			}

			inline bool HasPosition(const LayoutPosition x,
				const LayoutPosition y,
				const LayoutPosition z) const
			{
				return posX == x && posY == y && posZ == z;
			}

			inline bool IsVisibleOnLayer(const LayoutPosition z) const
			{
				return posZ == z && visible == VisibleYes;
			}

			inline void SetWidth(const LayoutItemSize width)
			{
				this->width = width;
			}

			inline LayoutItemSize GetWidth() const
			{
				return width;
			}

			inline void SetHeight(const LayoutItemSize height)
			{
				this->height = height;
			}

			inline LayoutItemSize GetHeight() const
			{
				return height;
			}

			inline void SetRotation(const LayoutRotation rotation)
			{
				this->rotation = rotation;
			}

			inline LayoutRotation GetRotation() const
			{
				return rotation;
			}

			virtual std::string Rotation() const
			{
				return Rotation(rotation);
			}

			static std::string Rotation(LayoutRotation rotation);

		protected:
			virtual bool Deserialize(const std::map<std::string,std::string>& arguments) override;
			
		private:
			Visible visible;
			LayoutPosition posX;
			LayoutPosition posY;
			LayoutPosition posZ;
			LayoutItemSize width;
			LayoutItemSize height;
			LayoutRotation rotation;
	};
} // namespace DataModel

