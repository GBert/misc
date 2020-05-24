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

#include "DataTypes.h"

class Manager;

namespace Hardware
{
	class HardwareParams
	{
		public:
			HardwareParams(ControlID controlID,
				HardwareType hardwareType,
				std::string name,
				std::string arg1,
				std::string arg2,
				std::string arg3,
				std::string arg4,
				std::string arg5)
			:	manager(nullptr),
			 	controlID(controlID),
				hardwareType(hardwareType),
				name(name),
				arg1(arg1),
				arg2(arg2),
				arg3(arg3),
				arg4(arg4),
				arg5(arg5)
			{
			}

			HardwareParams(Manager* manager, ControlID controlID)
			:	manager(manager),
				controlID(controlID)
			{
			}

			void SetManager(const Manager* manager) { this->manager = const_cast<Manager*>(manager); }
			Manager* GetManager() const { return manager; }
			ControlID GetControlID() const { return controlID; }
			void SetName(const std::string& name) { this->name = name; }
			const std::string& GetName() const { return name; }
			void SetHardwareType(const HardwareType hardwareType) { this->hardwareType = hardwareType; }
			HardwareType GetHardwareType() const { return hardwareType; }
			void SetArg1(const std::string& arg) { this->arg1 = arg; }
			std::string GetArg1() const { return arg1; }
			void SetArg2(const std::string& arg) { this->arg2 = arg; }
			std::string GetArg2() const { return arg2; }
			void SetArg3(const std::string& arg) { this->arg3 = arg; }
			std::string GetArg3() const { return arg3; }
			void SetArg4(const std::string& arg) { this->arg4 = arg; }
			std::string GetArg4() const { return arg4; }
			void SetArg5(const std::string& arg) { this->arg5 = arg; }
			std::string GetArg5() const { return arg5; }

		private:
			Manager* manager;
			ControlID controlID;
			HardwareType hardwareType;
			std::string name;
			std::string arg1;
			std::string arg2;
			std::string arg3;
			std::string arg4;
			std::string arg5;
	};

} // namespace Hardware

