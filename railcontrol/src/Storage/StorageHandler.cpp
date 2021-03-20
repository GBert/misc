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

#include <string>
#include <vector>

#include "Logger/Logger.h"
#include "Storage/Sqlite.h"
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
	void StorageHandler::Save(const Hardware::HardwareParams& hardwareParams)
	{
		TransactionGuard guard(this);
		sqlite.SaveHardwareParams(hardwareParams);
	}

	void StorageHandler::DeleteHardwareParams(const ControlID controlID)
	{
		TransactionGuard guard(this);
		sqlite.DeleteHardwareParams(controlID);
	}

	void StorageHandler::AllLocos(map<LocoID,DataModel::Loco*>& locos)
	{
		vector<string> objects;
		sqlite.ObjectsOfType(ObjectTypeLoco, objects);
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
		TransactionGuard guard(this);
		sqlite.DeleteRelationsFrom(DataModel::Relation::TypeLocoSlave, locoID);
		sqlite.DeleteRelationsTo(ObjectTypeLoco, locoID);
		sqlite.DeleteObject(ObjectTypeLoco, locoID);
	}

	void StorageHandler::AllAccessories(std::map<AccessoryID,DataModel::Accessory*>& accessories)
	{
		vector<string> objects;
		sqlite.ObjectsOfType(ObjectTypeAccessory, objects);
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
		TransactionGuard guard(this);
		sqlite.DeleteObject(ObjectTypeAccessory, accessoryID);
	}

	void StorageHandler::AllFeedbacks(std::map<FeedbackID,DataModel::Feedback*>& feedbacks)
	{
		vector<string> objects;
		sqlite.ObjectsOfType(ObjectTypeFeedback, objects);
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
		TransactionGuard guard(this);
		sqlite.DeleteObject(ObjectTypeFeedback, feedbackID);
	}

	void StorageHandler::AllTracks(std::map<TrackID,DataModel::Track*>& tracks)
	{
		vector<string> objects;
		sqlite.ObjectsOfType(ObjectTypeTrack, objects);
		for(auto object : objects)
		{
			Track* track = new Track(manager, object);
			if (track == nullptr)
			{
				continue;
			}
			const TrackID trackId = track->GetID();
			track->AssignSignals(RelationsFrom(DataModel::Relation::TypeTrackSignal, trackId));
			tracks[trackId] = track;
		}
	}

	void StorageHandler::DeleteTrack(const TrackID trackID)
	{
		Storage::TransactionGuard guard(this);
		sqlite.DeleteRelationsTo(ObjectTypeTrack, trackID);
		sqlite.DeleteObject(ObjectTypeTrack, trackID);
	}

	void StorageHandler::AllSwitches(std::map<SwitchID,DataModel::Switch*>& switches)
	{
		vector<string> objects;
		sqlite.ObjectsOfType(ObjectTypeSwitch, objects);
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
		TransactionGuard guard(this);
		sqlite.DeleteObject(ObjectTypeSwitch, switchID);
	}

	void StorageHandler::Save(const DataModel::Route& route)
	{
		string serialized = route.Serialize();
		TransactionGuard guard(this);
		const RouteID routeID = route.GetID();
		sqlite.SaveObject(ObjectTypeRoute, routeID, route.GetName(), serialized);
		sqlite.DeleteRelationsFrom(DataModel::Relation::TypeRouteAtLock, routeID);
		SaveRelations(route.GetRelationsAtLock());
		sqlite.DeleteRelationsFrom(DataModel::Relation::TypeRouteAtUnlock, routeID);
		SaveRelations(route.GetRelationsAtUnlock());
	}

	void StorageHandler::Save(const DataModel::Loco& loco)
	{
		string serialized = loco.Serialize();
		TransactionGuard guard(this);
		const LocoID locoID = loco.GetID();
		sqlite.SaveObject(ObjectTypeLoco, locoID, loco.GetName(), serialized);
		sqlite.DeleteRelationsFrom(DataModel::Relation::TypeLocoSlave, locoID);
		SaveRelations(loco.GetSlaves());
	}

	void StorageHandler::Save(const DataModel::Cluster& cluster)
	{
		string serialized = cluster.Serialize();
		TransactionGuard guard(this);
		const ClusterID clusterID = cluster.GetID();
		sqlite.SaveObject(ObjectTypeCluster, clusterID, cluster.GetName(), serialized);
		sqlite.DeleteRelationsFrom(DataModel::Relation::TypeClusterTrack, clusterID);
		SaveRelations(cluster.GetTracks());
		sqlite.DeleteRelationsFrom(DataModel::Relation::TypeClusterSignal, clusterID);
		SaveRelations(cluster.GetSignals());
	}

	void StorageHandler::Save(const DataModel::Track& track)
	{
		string serialized = track.Serialize();
		TransactionGuard guard(this);
		const TrackID trackId = track.GetID();
		sqlite.SaveObject(ObjectTypeTrack, trackId, track.GetName(), serialized);
		sqlite.DeleteRelationsFrom(DataModel::Relation::TypeTrackSignal, trackId);
		SaveRelations(track.GetSignals());
	}

	void StorageHandler::AllRoutes(std::map<RouteID,DataModel::Route*>& routes)
	{
		vector<string> objects;
		sqlite.ObjectsOfType(ObjectTypeRoute, objects);
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
		TransactionGuard guard(this);
		sqlite.DeleteRelationsFrom(DataModel::Relation::TypeRouteAtLock, routeID);
		sqlite.DeleteRelationsFrom(DataModel::Relation::TypeRouteAtUnlock, routeID);
		sqlite.DeleteObject(ObjectTypeRoute, routeID);
	}

	void StorageHandler::AllLayers(std::map<LayerID,DataModel::Layer*>& layers)
	{
		vector<string> objects;
		sqlite.ObjectsOfType(ObjectTypeLayer, objects);
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
		TransactionGuard guard(this);
		sqlite.DeleteObject(ObjectTypeLayer, layerID);
	}

	void StorageHandler::AllSignals(std::map<SignalID,DataModel::Signal*>& signals)
	{
		vector<string> serializedObjects;
		sqlite.ObjectsOfType(ObjectTypeSignal, serializedObjects);
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
		TransactionGuard guard(this);
		sqlite.DeleteRelationsTo(ObjectTypeSignal, signalID);
		sqlite.DeleteObject(ObjectTypeSignal, signalID);
	}

	void StorageHandler::AllClusters(std::map<ClusterID,DataModel::Cluster*>& clusters)
	{
		vector<string> serializedObjects;
		sqlite.ObjectsOfType(ObjectTypeCluster, serializedObjects);
		for(auto serializedObject : serializedObjects)
		{
			Cluster* cluster = new Cluster(serializedObject);
			if (cluster == nullptr)
			{
				continue;
			}
			const ClusterID clusterId = cluster->GetID();
			cluster->AssignTracks(RelationsFrom(DataModel::Relation::TypeClusterTrack, clusterId));
			cluster->AssignSignals(RelationsFrom(DataModel::Relation::TypeClusterSignal, clusterId));
			clusters[clusterId] = cluster;
		}
	}

	void StorageHandler::DeleteCluster(const ClusterID clusterID)
	{
		TransactionGuard guard(this);
		sqlite.DeleteObject(ObjectTypeCluster, clusterID);
	}

	void StorageHandler::SaveSetting(const std::string& key, const std::string& value)
	{
		TransactionGuard guard(this);
		sqlite.SaveSetting(key, value);
	}

	void StorageHandler::SaveRelations(const vector<DataModel::Relation*> relations)
	{
		Storage::TransactionGuard guard(this);
		for (auto relation : relations)
		{
			string serializedRelation = relation->Serialize();
			sqlite.SaveRelation(relation->GetType(), relation->ObjectID1(), relation->ObjectType2(), relation->ObjectID2(), relation->GetPriority(), serializedRelation);
		}
	}

	vector<Relation*> StorageHandler::RelationsFrom(const DataModel::Relation::Type type, const ObjectID objectID)
	{
		vector<string> relationStrings;
		sqlite.RelationsFrom(type, objectID, relationStrings);
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

