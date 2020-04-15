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

#include <vector>
#include <map>

#include "DataModel/DataModel.h"
#include "DataTypes.h"
#include "Hardware/HardwareParams.h"
#include "Storage/StorageInterface.h"
#include "Storage/StorageParams.h"

namespace Storage
{
	// the types of the class factories
	typedef Storage::StorageInterface* createStorage_t(const StorageParams* params);
	typedef void destroyStorage_t(Storage::StorageInterface*);

	class StorageHandler
	{
		public:
			StorageHandler(Manager* manager, const StorageParams* params);
			~StorageHandler();
			void AllHardwareParams(std::map<controlID_t,Hardware::HardwareParams*>& hardwareParams);
			void DeleteHardwareParams(const controlID_t controlID);
			void AllLocos(std::map<locoID_t,DataModel::Loco*>& locos);
			void DeleteLoco(locoID_t locoID);
			void AllAccessories(std::map<accessoryID_t,DataModel::Accessory*>& accessories);
			void DeleteAccessory(accessoryID_t accessoryID);
			void AllFeedbacks(std::map<feedbackID_t,DataModel::Feedback*>& feedbacks);
			void DeleteFeedback(feedbackID_t feedbackID);
			void AllTracks(std::map<trackID_t,DataModel::Track*>& tracks);
			void DeleteTrack(trackID_t trackID);
			void AllSwitches(std::map<switchID_t,DataModel::Switch*>& switches);
			void DeleteSwitch(switchID_t switchID);
			void AllStreets(std::map<streetID_t,DataModel::Street*>& streets);
			void DeleteStreet(streetID_t streetID);
			void AllLayers(std::map<layerID_t,DataModel::Layer*>& layers);
			void DeleteLayer(layerID_t layerID);
			void AllSignals(std::map<signalID_t,DataModel::Signal*>& signals);
			void DeleteSignal(signalID_t signalID);
			void Save(const Hardware::HardwareParams& hardwareParams);
			void Save(const DataModel::Street& street);
			void Save(const DataModel::Loco& loco);
			template<class T> void Save(const T& t)
			{
				if (instance == nullptr)
				{
					return;
				}
				const std::string serialized = t.Serialize();
				StartTransactionInternal();
				instance->SaveObject(t.GetObjectType(), t.GetID(), t.GetName(), serialized);
				CommitTransactionInternal();
			}

			template <class T> static void Save(StorageHandler* storageHandler, const T* t) { storageHandler->Save(*t); }
			void SaveSetting(const std::string& key, const std::string& value);
			std::string GetSetting(const std::string& key) ;
			void StartTransaction();
			void CommitTransaction();

		private:
			void StartTransactionInternal();
			void CommitTransactionInternal();
			void SaveRelations(const std::vector<DataModel::Relation*> relations);
			std::vector<DataModel::Relation*> RelationsFrom(const DataModel::Relation::type_t type, const objectID_t objectID);


			Manager* manager;
			createStorage_t* createStorage;
			destroyStorage_t* destroyStorage;
			Storage::StorageInterface* instance;
			void* dlhandle;
			bool transactionRunning;
	};

} // namespace Storage

