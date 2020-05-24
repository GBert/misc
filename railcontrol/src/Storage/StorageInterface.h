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
#include <string>

#include "DataTypes.h"
#include "Hardware/HardwareParams.h"

namespace Storage
{
	class StorageInterface
	{
		public:
		  // non virtual default constructor is needed to prevent polymorphism
			StorageInterface() {};

			// pure virtual destructor prevents polymorphism in derived class
			virtual ~StorageInterface() {};

			// save control
			virtual void SaveHardwareParams(const Hardware::HardwareParams& hardwareParams) = 0;

			// read controls
			virtual void AllHardwareParams(std::map<ControlID,Hardware::HardwareParams*>& hardwareParams) = 0;

			// delete control
			virtual void DeleteHardwareParams(const ControlID controlID) = 0;

			// save datamodelobject
			virtual void SaveObject(const ObjectType objectType, const ObjectID objectID, const std::string& name, const std::string& object) = 0;

			// delete datamodelobject
			virtual void DeleteObject(const ObjectType objectType, const ObjectID objectID) = 0;

			// read datamodelobject
			virtual void ObjectsOfType(const ObjectType objectType, std::vector<std::string>& objects) = 0;

			// save datamodelrelation
			virtual void SaveRelation(const DataModel::Relation::Type type, const ObjectID objectID1, const ObjectType objectType2, const ObjectID objectID2, const Priority priority, const std::string& relation) = 0;

			// delete datamodelrelation
			virtual void DeleteRelationsFrom(const DataModel::Relation::Type type, const ObjectID objectID) = 0;

			// delete datamodelrelation
			virtual void DeleteRelationsTo(const ObjectType objectType, const ObjectID objectID) = 0;

			// read datamodelrelation
			virtual void RelationsFrom(const DataModel::Relation::Type type, const ObjectID objectID, std::vector<std::string>& relations) = 0;

			// read datamodelrelation
			virtual void RelationsTo(const ObjectType objectType, const ObjectID objectID, std::vector<std::string>& relations) = 0;

			// save setting
			virtual void SaveSetting(const std::string& key, const std::string& value) = 0;

			// read setting
			virtual std::string GetSetting(const std::string& key) = 0;

			// start transaction
			virtual void StartTransaction() {}

			// commit transaction
			virtual void CommitTransaction() {}
	};
} // namespace

