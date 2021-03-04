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

#include <vector>
#include <map>

#include "DataModel/DataModel.h"
#include "DataTypes.h"
#include "Hardware/HardwareParams.h"
#include "Storage/Sqlite.h"
#include "Storage/StorageInterface.h"
#include "Storage/StorageParams.h"

namespace Storage
{
	// the types of the class factories
	typedef Storage::StorageInterface* CreateStorage(const StorageParams* params);
	typedef void DestroyStorage(Storage::StorageInterface*);

	class StorageHandler
	{
		public:
			inline StorageHandler(Manager* manager, const StorageParams* params)
			:	manager(manager),
				sqlite(params),
				transactionRunning(false)
			{
			}

			inline ~StorageHandler()
			{
			}

			inline void AllHardwareParams(std::map<ControlID,Hardware::HardwareParams*>& hardwareParams)
			{
				sqlite.AllHardwareParams(hardwareParams);
			}

			void DeleteHardwareParams(const ControlID controlID);
			void AllLocos(std::map<LocoID,DataModel::Loco*>& locos);
			void DeleteLoco(LocoID locoID);
			void AllAccessories(std::map<AccessoryID,DataModel::Accessory*>& accessories);
			void DeleteAccessory(AccessoryID accessoryID);
			void AllFeedbacks(std::map<FeedbackID,DataModel::Feedback*>& feedbacks);
			void DeleteFeedback(FeedbackID feedbackID);
			void AllTracks(std::map<TrackID,DataModel::Track*>& tracks);
			void DeleteTrack(TrackID trackID);
			void AllSwitches(std::map<SwitchID,DataModel::Switch*>& switches);
			void DeleteSwitch(SwitchID switchID);
			void AllRoutes(std::map<RouteID,DataModel::Route*>& routes);
			void DeleteRoute(RouteID routeID);
			void AllLayers(std::map<LayerID,DataModel::Layer*>& layers);
			void DeleteLayer(LayerID layerID);
			void AllSignals(std::map<SignalID,DataModel::Signal*>& signals);
			void DeleteSignal(SignalID signalID);
			void AllClusters(std::map<ClusterID,DataModel::Cluster*>& clusters);
			void DeleteCluster(ClusterID clusterID);
			void Save(const Hardware::HardwareParams& hardwareParams);
			void Save(const DataModel::Route& route);
			void Save(const DataModel::Loco& loco);
			void Save(const DataModel::Cluster& cluster);
			void Save(const DataModel::Track& track);

			template<class T> void Save(const T& t)
			{
				const std::string serialized = t.Serialize();
				StartTransactionInternal();
				sqlite.SaveObject(t.GetObjectType(), t.GetID(), t.GetName(), serialized);
				CommitTransactionInternal();
			}

			template <class T> static void Save(StorageHandler* storageHandler, const T* t)
			{
				storageHandler->Save(*t);
			}

			void SaveSetting(const std::string& key, const std::string& value);

			inline std::string GetSetting(const std::string& key)
			{
				return sqlite.GetSetting(key);
			}

			inline void StartTransaction()
			{
				transactionRunning = true;
				sqlite.StartTransaction();
			}

			inline void CommitTransaction()
			{
				transactionRunning = false;
				sqlite.CommitTransaction();
			}

		private:
			inline void StartTransactionInternal()
			{
				sqlite.StartTransaction();
			}

			inline void CommitTransactionInternal()
			{
				sqlite.CommitTransaction();
			}

			void SaveRelations(const std::vector<DataModel::Relation*> relations);
			std::vector<DataModel::Relation*> RelationsFrom(const DataModel::Relation::Type type, const ObjectID objectID);


			Manager* manager;
			Storage::SQLite sqlite;
			bool transactionRunning;
	};

} // namespace Storage

