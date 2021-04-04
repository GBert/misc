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
	static const std::string gitHash = "facdc932881666a9b52a7654e44dfea0f0a4c448";
	return gitHash;
}

time_t GetVersionInfoGitTimestamp()
{
	return 1617527466;
}

unsigned int GetVersionInfoGitDirty()
{
	return 5;
}

time_t GetVersionInfoCompileTimestamp()
{
	return 1617535240;
}

const std::string& GetVersionInfoRailControlVersion()
{
	static const std::string railControlVersion = "19";
	return railControlVersion;
}

