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

#include "WebServer/HtmlTagInputSliderLocoSpeed.h"

namespace WebServer
{
	HtmlTagInputSliderLocoSpeed::HtmlTagInputSliderLocoSpeed(const std::string& name, const unsigned int min, const unsigned int max, const unsigned int value, const LocoID locoID)
	: HtmlTagInputSlider(name, min, max, value)
	{
		const std::string locoIdString = std::to_string(locoID);
		const std::string reference = "locospeed_" + locoIdString;
		AddId(reference);
		AddClass("slider");
		const std::string script = "locoSpeedSliderChange(" + locoIdString + "); return false;";
		AddAttribute("onchange", script);
		AddAttribute("oninput", script);
	}
} // namespace WebServer
