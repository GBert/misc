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

#include "DataTypes.h"

class Manager;

namespace Hardware
{
	class HardwareParams
	{
		public:
			inline HardwareParams(ControlID controlID,
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

			inline HardwareParams(Manager* manager,
				ControlID controlID)
			:	manager(manager),
				controlID(controlID),
				hardwareType(HardwareTypeNone)
			{
			}

			inline void SetManager(Manager* manager)
			{
				this->manager = manager;
			}

			inline Manager* GetManager() const
			{
				return manager;
			}

			inline ControlID GetControlID() const
			{
				return controlID;
			}

			inline void SetName(const std::string& name)
			{
				this->name = name;
			}

			inline const std::string& GetName() const
			{
				return name;
			}

			inline void SetHardwareType(const HardwareType hardwareType)
			{
				this->hardwareType = hardwareType;
			}

			inline HardwareType GetHardwareType() const
			{
				return hardwareType;
			}

			inline void SetArg1(const std::string& arg)
			{
				this->arg1 = arg;
			}

			inline std::string GetArg1() const
			{
				return arg1;
			}

			inline void SetArg2(const std::string& arg)
			{
				this->arg2 = arg;
			}

			inline std::string GetArg2() const
			{
				return arg2;
			}

			inline void SetArg3(const std::string& arg)
			{
				this->arg3 = arg;
			}

			inline std::string GetArg3() const
			{
				return arg3;
			}

			inline void SetArg4(const std::string& arg)
			{
				this->arg4 = arg;
			}

			inline std::string GetArg4() const
			{
				return arg4;
			}

			inline void SetArg5(const std::string& arg)
			{
				this->arg5 = arg;
			}

			inline std::string GetArg5() const
			{
				return arg5;
			}

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

