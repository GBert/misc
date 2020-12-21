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

#ifndef AMALGAMATION
#include <dlfcn.h>              // dl*
#endif
#include <string>
#include <vector>

#include "Logger/Logger.h"
#include "Storage/StorageHandler.h"
#include "Utils/Utils.h"

using DataModel::Accessory;
using DataModel::Cluster;
using DataModel::Feedback;
using DataModel::Layer;
using DataModel::Loco;
using DataModel::Relation;
using DataModel::Route;
using DataModel::Signal;
using DataModel::Switch;
using DataModel::Track;
using std::map;
using std::string;
using std::vector;

namespace Storage
{
	StorageHandler::StorageHandler(Manager* manager, const StorageParams* params)
	:	manager(manager),
		createStorage(nullptr),
		destroyStorage(nullptr),
		instance(nullptr),
#ifndef AMALGAMATION
		dlhandle(nullptr),
#endif
		transactionRunning(false)
	{
#ifdef AMALGAMATION
		createStorage = (Storage::StorageInterface* (*)(const Storage::StorageParams*))(&create_Sqlite);
		destroyStorage = (void (*)(Storage::StorageInterface*))(&destroy_Sqlite);
#else
		// generate symbol and library names
		char* error;
		string moduleName = "Storage/" + params->module + ".so";

		Logger::Logger* logger = Logger::Logger::GetLogger("StorageHandler");
		dlhandle = dlopen(moduleName.c_str(), RTLD_LAZY);
		if (dlhandle == nullptr)
		{
			logger->Error(Languages::TextCanNotOpenLibrary, moduleName, dlerror());
			return;
		}

		// look for symbol create_*
		string createSymbol = "create_" + params->module;
		CreateStorage* newCreateStorage = (CreateStorage*) dlsym(dlhandle, createSymbol.c_str());
		error = dlerror();
		if (error)
		{
			logger->Error(Languages::TextUnableToFindSymbol, createSymbol);
			return;
		}

		// look for symbol destroy_*
		string destroySymbol = "destroy_" + params->module;
		DestroyStorage* newDestroyStorage = (DestroyStorage*) dlsym(dlhandle, destroySymbol.c_str());
		error = dlerror();
		if (error)
		{
			logger->Error(Languages::TextUnableToFindSymbol, destroySymbol);
			return;
		}

		// register  valid symbols
		createStorage = newCreateStorage;
		destroyStorage = newDestroyStorage;
#endif

		// start storage
		if (createStorage)
		{
			instance = createStorage(params);
		}
	}

	StorageHandler::~StorageHandler()
	{
		// stop storage
		if (instance)
		{
			destroyStorage(instance);
			instance = nullptr;
		}

#ifndef AMALGAMATION
		// close library
		if (dlhandle)
		{
			dlclose(dlhandle);
			dlhandle = nullptr;
		}
#endif
	}

	void StorageHandler::Save(const Hardware::HardwareParams& hardwareParams)
	{
		if (instance == nullptr)
		{
			return;
		}
		StartTransactionInternal();
		instance->SaveHardwareParams(hardwareParams);
		CommitTransactionInternal();
	}

	void StorageHandler::AllHardwareParams(std::map<ControlID,Hardware::HardwareParams*>& hardwareParams)
	{
		if (instance == nullptr)
		{
			return;
		}
		instance->AllHardwareParams(hardwareParams);
	}

	void StorageHandler::DeleteHardwareParams(const ControlID controlID)
	{
		if (instance == nullptr)
		{
			return;
		}
		StartTransactionInternal();
		instance->DeleteHardwareParams(controlID);
		CommitTransactionInternal();
	}

	void StorageHandler::AllLocos(map<LocoID,DataModel::Loco*>& locos)
	{
		if (instance == nullptr)
		{
			return;
		}
		vector<string> objects;
		instance->ObjectsOfType(ObjectTypeLoco, objects);
		for(auto object : objects)
		{
			Loco* loco = new Loco(manager, object);
			const RouteID locoID = loco->GetID();
			loco->AssignSlaves(RelationsFrom(DataModel::Relation::TypeLocoSlave, locoID));
			locos[locoID] = loco;
		}
	}

	void StorageHandler::DeleteLoco(const LocoID locoID)
	{
		if (instance == nullptr)
		{
			return;
		}
		StartTransactionInternal();
		instance->DeleteRelationsFrom(DataModel::Relation::TypeLocoSlave, locoID);
		instance->DeleteRelationsTo(ObjectTypeLoco, locoID);
		instance->DeleteObject(ObjectTypeLoco, locoID);
		CommitTransactionInternal();
	}

	void StorageHandler::AllAccessories(std::map<AccessoryID,DataModel::Accessory*>& accessories)
	{
		if (instance == nullptr)
		{
			return;
		}
		vector<string> objects;
		instance->ObjectsOfType(ObjectTypeAccessory, objects);
		for(auto object : objects)
		{
			Accessory* accessory = new Accessory(object);
			if (accessory == nullptr)
			{
				continue;
			}
			accessories[accessory->GetID()] = accessory;
		}
	}

	void StorageHandler::DeleteAccessory(const AccessoryID accessoryID)
	{
		if (instance == nullptr)
		{
			return;
		}
		StartTransactionInternal();
		instance->DeleteObject(ObjectTypeAccessory, accessoryID);
		CommitTransactionInternal();
	}

	void StorageHandler::AllFeedbacks(std::map<FeedbackID,DataModel::Feedback*>& feedbacks)
	{
		if (instance == nullptr)
		{
			return;
		}
		vector<string> objects;
		instance->ObjectsOfType(ObjectTypeFeedback, objects);
		for(auto object : objects)
		{
			Feedback* feedback = new Feedback(manager, object);
			if (feedback == nullptr)
			{
				continue;
			}
			feedbacks[feedback->GetID()] = feedback;
		}
	}

	void StorageHandler::DeleteFeedback(const FeedbackID feedbackID)
	{
		if (instance == nullptr)
		{
			return;
		}
		StartTransactionInternal();
		instance->DeleteObject(ObjectTypeFeedback, feedbackID);
		CommitTransactionInternal();
	}

	void StorageHandler::AllTracks(std::map<TrackID,DataModel::Track*>& tracks)
	{
		if (instance == nullptr)
		{
			return;
		}
		vector<string> objects;
		instance->ObjectsOfType(ObjectTypeTrack, objects);
		for(auto object : objects)
		{
			Track* track = new Track(manager, object);
			if (track == nullptr)
			{
				continue;
			}
			tracks[track->GetID()] = track;
		}
	}

	void StorageHandler::DeleteTrack(const TrackID trackID)
	{
		if (instance == nullptr)
		{
			return;
		}
		StartTransactionInternal();
		instance->DeleteRelationsTo(ObjectTypeTrack, trackID);
		instance->DeleteObject(ObjectTypeTrack, trackID);
		CommitTransactionInternal();
	}

	void StorageHandler::AllSwitches(std::map<SwitchID,DataModel::Switch*>& switches)
	{
		if (instance == nullptr)
		{
			return;
		}
		vector<string> objects;
		instance->ObjectsOfType(ObjectTypeSwitch, objects);
		for(auto object : objects)
		{
			Switch* mySwitch = new Switch(object);
			if (mySwitch == nullptr)
			{
				continue;
			}
			switches[mySwitch->GetID()] = mySwitch;
		}
	}

	void StorageHandler::DeleteSwitch(const SwitchID switchID)
	{
		if (instance == nullptr)
		{
			return;
		}
		StartTransactionInternal();
		instance->DeleteObject(ObjectTypeSwitch, switchID);
		CommitTransactionInternal();
	}

	void StorageHandler::Save(const DataModel::Route& route)
	{
		if (instance == nullptr)
		{
			return;
		}
		string serialized = route.Serialize();
		StartTransactionInternal();
		const RouteID routeID = route.GetID();
		instance->SaveObject(ObjectTypeRoute, routeID, route.GetName(), serialized);
		instance->DeleteRelationsFrom(DataModel::Relation::TypeRouteAtLock, routeID);
		SaveRelations(route.GetRelationsAtLock());
		instance->DeleteRelationsFrom(DataModel::Relation::TypeRouteAtUnlock, routeID);
		SaveRelations(route.GetRelationsAtUnlock());
		CommitTransactionInternal();
	}

	void StorageHandler::Save(const DataModel::Loco& loco)
	{
		if (instance == nullptr)
		{
			return;
		}
		string serialized = loco.Serialize();
		StartTransactionInternal();
		const LocoID locoID = loco.GetID();
		instance->SaveObject(ObjectTypeLoco, locoID, loco.GetName(), serialized);
		instance->DeleteRelationsFrom(DataModel::Relation::TypeLocoSlave, locoID);
		SaveRelations(loco.GetSlaves());
		CommitTransactionInternal();
	}

	void StorageHandler::Save(const DataModel::Cluster& cluster)
	{
		if (instance == nullptr)
		{
			return;
		}
		string serialized = cluster.Serialize();
		StartTransactionInternal();
		const ClusterID clusterID = cluster.GetID();
		instance->SaveObject(ObjectTypeCluster, clusterID, cluster.GetName(), serialized);
		instance->DeleteRelationsFrom(DataModel::Relation::TypeClusterTrack, clusterID);
		SaveRelations(cluster.GetTracks());
		instance->DeleteRelationsFrom(DataModel::Relation::TypeClusterSignal, clusterID);
		SaveRelations(cluster.GetSignals());
		CommitTransactionInternal();
	}

	void StorageHandler::AllRoutes(std::map<RouteID,DataModel::Route*>& routes)
	{
		if (instance == nullptr)
		{
			return;
		}
		vector<string> objects;
		instance->ObjectsOfType(ObjectTypeRoute, objects);
		for (auto object : objects) {
			Route* route = new Route(manager, object);
			if (route == nullptr)
			{
				continue;
			}
			const RouteID routeID = route->GetID();
			route->AssignRelationsAtLock(RelationsFrom(Relation::TypeRouteAtLock, routeID));
			route->AssignRelationsAtUnlock(RelationsFrom(Relation::TypeRouteAtUnlock, routeID));
			routes[routeID] = route;
		}
	}

	void StorageHandler::DeleteRoute(const RouteID routeID)
	{
		if (instance == nullptr)
		{
			return;
		}
		StartTransactionInternal();
		instance->DeleteRelationsFrom(DataModel::Relation::TypeRouteAtLock, routeID);
		instance->DeleteRelationsFrom(DataModel::Relation::TypeRouteAtUnlock, routeID);
		instance->DeleteObject(ObjectTypeRoute, routeID);
		CommitTransactionInternal();
	}

	void StorageHandler::AllLayers(std::map<LayerID,DataModel::Layer*>& layers)
	{
		if (instance == nullptr)
		{
			return;
		}
		vector<string> objects;
		instance->ObjectsOfType(ObjectTypeLayer, objects);
		for(auto object : objects) {
			Layer* layer = new Layer(object);
			if (layer == nullptr)
			{
				continue;
			}
			layers[layer->GetID()] = layer;
		}
	}

	void StorageHandler::DeleteLayer(const LayerID layerID)
	{
		if (instance == nullptr)
		{
			return;
		}
		StartTransactionInternal();
		instance->DeleteObject(ObjectTypeLayer, layerID);
		CommitTransactionInternal();
	}

	void StorageHandler::AllSignals(std::map<SignalID,DataModel::Signal*>& signals)
	{
		if (instance == nullptr)
		{
			return;
		}
		vector<string> serializedObjects;
		instance->ObjectsOfType(ObjectTypeSignal, serializedObjects);
		for(auto serializedObject : serializedObjects)
		{
			Signal* signal = new Signal(manager, serializedObject);
			if (signal == nullptr)
			{
				continue;
			}
			signals[signal->GetID()] = signal;
		}
	}

	void StorageHandler::DeleteSignal(const SignalID signalID)
	{
		if (instance == nullptr)
		{
			return;
		}
		StartTransactionInternal();
		instance->DeleteRelationsTo(ObjectTypeSignal, signalID);
		instance->DeleteObject(ObjectTypeSignal, signalID);
		CommitTransactionInternal();
	}

	void StorageHandler::AllClusters(std::map<ClusterID,DataModel::Cluster*>& clusters)
	{
		if (instance == nullptr)
		{
			return;
		}
		vector<string> serializedObjects;
		instance->ObjectsOfType(ObjectTypeCluster, serializedObjects);
		for(auto serializedObject : serializedObjects)
		{
			Cluster* cluster = new Cluster(serializedObject);
			if (cluster == nullptr)
			{
				continue;
			}
			const ClusterID clusterID = cluster->GetID();
			cluster->AssignTracks(RelationsFrom(DataModel::Relation::TypeClusterTrack, clusterID));
			cluster->AssignSignals(RelationsFrom(DataModel::Relation::TypeClusterSignal, clusterID));
			clusters[cluster->GetID()] = cluster;
		}
	}

	void StorageHandler::DeleteCluster(const ClusterID clusterID)
	{
		if (instance == nullptr)
		{
			return;
		}
		StartTransactionInternal();
		instance->DeleteObject(ObjectTypeCluster, clusterID);
		CommitTransactionInternal();
	}

	void StorageHandler::SaveSetting(const std::string& key, const std::string& value)
	{
		if (instance == nullptr)
		{
			return;
		}
		StartTransactionInternal();
		instance->SaveSetting(key, value);
		CommitTransactionInternal();
	}

	std::string StorageHandler::GetSetting(const std::string& key)
	{
		if (instance == nullptr)
		{
			return "";
		}
		return instance->GetSetting(key);
	}

	void StorageHandler::StartTransaction()
	{
		if (instance == nullptr)
		{
			return;
		}
		transactionRunning = true;
		instance->StartTransaction();
	}

	void StorageHandler::CommitTransaction()
	{
		if (instance == nullptr)
		{
			return;
		}
		transactionRunning = false;
		instance->CommitTransaction();
	}

	void StorageHandler::StartTransactionInternal()
	{
		if (transactionRunning == true)
		{
			return;
		}
		instance->StartTransaction();
	}

	void StorageHandler::CommitTransactionInternal()
	{
		if (transactionRunning == true)
		{
			return;
		}
		instance->CommitTransaction();
	}

	void StorageHandler::SaveRelations(const vector<DataModel::Relation*> relations)
	{
		for (auto relation : relations)
		{
			string serializedRelation = relation->Serialize();
			instance->SaveRelation(relation->GetType(), relation->ObjectID1(), relation->ObjectType2(), relation->ObjectID2(), relation->GetPriority(), serializedRelation);
		}
	}

	vector<Relation*> StorageHandler::RelationsFrom(const DataModel::Relation::Type type, const ObjectID objectID)
	{
		vector<string> relationStrings;
		instance->RelationsFrom(type, objectID, relationStrings);
		vector<Relation*> output;
		for (auto relationString : relationStrings)
		{
			Relation* relation = new Relation(manager, relationString);
			if (relation == nullptr)
			{
				continue;
			}
			output.push_back(relation);
		}
		return output;
	}
} // namespace Storage

