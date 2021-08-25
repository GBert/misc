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

#include "DataModel/LayoutItem.h"
#include "DataTypes.h"
#include "Languages.h"

class Manager;

namespace DataModel
{
	class Text : public LayoutItem
	{
		public:
			static const LayoutItemSize MinWidth = 1;
			static const LayoutItemSize MaxWidth = 100;

			Text() = delete;
			Text(const Text&) = delete;
			Text& operator=(const Text&) = delete;

			inline Text(__attribute__((unused)) const Manager* const manager, const TextID textID)
			:	Text(textID)
			{
			}

			inline Text(const TextID textID)
			:	LayoutItem(textID)
			{
			}

			inline Text(const std::string& serialized)
			:	Text(TextNone)
			{
				Deserialize(serialized);
			}

			inline ObjectType GetObjectType() const override
			{
				return ObjectTypeText;
			}

			std::string Serialize() const override;
			bool Deserialize(const std::string& serialized) override;

			inline std::string GetLayoutType() const override
			{
				return Languages::GetText(Languages::TextText);
			}
	};
} // namespace DataModel
