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

#include "DataModel/DataModel.h"
#include "Logger/Logger.h"
#include "Storage/sqlite/sqlite3.h"
#include "Storage/StorageInterface.h"
#include "Storage/StorageParams.h"
#include "Utils/Utils.h"

namespace Storage
{
	class SQLite : public StorageInterface
	{
		public:
			SQLite() = delete;
			SQLite(const StorageParams* params);
			~SQLite();

			void SaveHardwareParams(const Hardware::HardwareParams& params) override;
			void AllHardwareParams(std::map<ControlID,Hardware::HardwareParams*>& hardwareParams) override;
			void DeleteHardwareParams(const ControlID controlID) override;
			void SaveObject(const ObjectType objectType, const ObjectID objectID, const std::string& name, const std::string& object) override;
			void DeleteObject(const ObjectType objectType, const ObjectID objectID) override;
			void ObjectsOfType(const ObjectType objectType, std::vector<std::string>& objects) override;
			void SaveRelation(const DataModel::Relation::Type type, const ObjectID objectID1, const ObjectType objectType2, const ObjectID objectID2, const Priority priority, const std::string& relation) override;
			void DeleteRelationsFrom(const DataModel::Relation::Type type, const ObjectID objectID) override;
			void DeleteRelationsTo(const ObjectType objectType, const ObjectID objectID) override;
			void RelationsFrom(const DataModel::Relation::Type type, const ObjectID objectID, std::vector<std::string>& relations) override;
			void RelationsTo(const ObjectType objectType, const ObjectID objectID, std::vector<std::string>& relations) override;
			void SaveSetting(const std::string& key, const std::string& value) override;
			std::string GetSetting(const std::string& key) override;
			void StartTransaction() override;
			void CommitTransaction() override;

		private:
			sqlite3 *db;
			const std::string filename;
			Logger::Logger* logger;
			unsigned int keepBackups;

			void RemoveOldBackupFiles();
			bool Execute(const std::string& query, sqlite3_callback callback = nullptr, void* result = nullptr) { return Execute(query.c_str(), callback, result); }
			bool Execute(const char* query, sqlite3_callback callback, void* result);
			bool DropTable(const std::string table);
			bool CreateTableHardware();
			bool CreateTableObjects();
			bool CreateTableRelations(const std::string& name);
			bool CreateTableSettings();
			bool CheckTableRelations();
			bool UpdateTableRelations1();
			bool RenameTable(const std::string& oldName, const std::string& newName);

			static int CallbackTableInfo(void *v, int argc, char **argv, char **colName);
			static int CallbackListTables(void *v, int argc, char **argv, char **colName);
			static int CallbackAllHardwareParams(void *v, int argc, char **argv, char **colName);
			static int CallbackStringVector(void* v, int argc, char **argv, char **colName);
			static std::string EscapeString(const std::string& input);
	};

	extern "C" SQLite* create_Sqlite(const StorageParams* params);
	extern "C" void destroy_Sqlite(SQLite* sqlite);

} // namespace Storage

