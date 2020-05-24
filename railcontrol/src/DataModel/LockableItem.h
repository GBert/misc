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
#include <mutex>

#include "DataTypes.h"
#include "Logger/Logger.h"

namespace DataModel
{
	class LockableItem
	{
		public:
			enum LockState : unsigned char
			{
				LockStateFree = 0,
				LockStateReserved,
				LockStateSoftLocked,
				LockStateHardLocked
			};

			LockableItem()
			:	lockState(LockStateFree),
			 	locoID(LocoNone)
			{
			}

			virtual ~LockableItem() {};

			std::string Serialize() const;
			bool Deserialize(const std::map<std::string,std::string> arguments);


			LocoID GetLoco() const { return locoID; }
			LockState GetLockState() const { return lockState; }
			virtual bool Reserve(Logger::Logger* logger, const LocoID locoID);
			virtual bool Lock(Logger::Logger* logger, const LocoID locoID);
			virtual bool Release(Logger::Logger* logger, const LocoID locoID);

			bool IsInUse() const { return lockState != LockStateFree || locoID != LocoNone; }


		private:
			std::mutex lockMutex;
			LockState lockState;
			LocoID locoID;
	};
} // namespace DataModel

