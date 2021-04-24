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

#include "Version.h"

const std::string& GetVersionInfoGitHash()
{
	static const std::string gitHash = "32726bc206855ecc4d9b2e6e7db30eb453cf7a85";
	return gitHash;
}

time_t GetVersionInfoGitTimestamp()
{
	return 1619267118;
}

unsigned int GetVersionInfoGitDirty()
{
	return 4;
}

time_t GetVersionInfoCompileTimestamp()
{
	return 1619271793;
}

const std::string& GetVersionInfoRailControlVersion()
{
	static const std::string railControlVersion = "19";
	return railControlVersion;
}

