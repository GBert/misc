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

#include "WebServer/HtmlTagTrackBase.h"

class Manager;

namespace DataModel
{
	class Signal;
}

namespace WebServer
{
	class HtmlTagSignal : public HtmlTagTrackBase
	{
		public:
			HtmlTagSignal() = delete;
			HtmlTagSignal(const HtmlTagSignal&) = delete;
			HtmlTagSignal& operator=(const HtmlTagSignal&) = delete;

			HtmlTagSignal(const Manager& manager, const DataModel::Signal* signal);

			virtual ~HtmlTagSignal()
			{
			}

			static std::string GetSignalImage(const DataModel::AccessoryState state,
				const DataModel::Signal* signal);

		private:
			static std::string GetSignalImagePlain(const DataModel::Signal* signal);
			static std::string GetStateClassText(const DataModel::AccessoryState state);
	};
} // namespace WebServer

