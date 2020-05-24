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
using DataModel::Track;
using DataModel::Feedback;
using DataModel::Layer;
using DataModel::Loco;
using DataModel::Relation;
using DataModel::Signal;
using DataModel::Street;
using DataModel::Switch;
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
		dlhandle(nullptr),
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
			vector<string> slavesString;
			const StreetID locoID = loco->GetID();
			instance->RelationsFrom(DataModel::Relation::TypeLocoSlave, locoID, slavesString);
			vector<Relation*> slaves;
			for (auto slaveString : slavesString)
			{
				slaves.push_back(new Relation(manager, slaveString));
			}
			loco->AssignSlaves(slaves);
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

	void StorageHandler::Save(const DataModel::Street& street)
	{
		if (instance == nullptr)
		{
			return;
		}
		string serialized = street.Serialize();
		StartTransactionInternal();
		const StreetID streetID = street.GetID();
		instance->SaveObject(ObjectTypeStreet, streetID, street.GetName(), serialized);
		instance->DeleteRelationsFrom(DataModel::Relation::TypeStreetAtLock, streetID);
		SaveRelations(street.GetRelationsAtLock());
		instance->DeleteRelationsFrom(DataModel::Relation::TypeStreetAtUnlock, streetID);
		SaveRelations(street.GetRelationsAtUnlock());
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

	void StorageHandler::AllStreets(std::map<StreetID,DataModel::Street*>& streets)
	{
		if (instance == nullptr)
		{
			return;
		}
		vector<string> objects;
		instance->ObjectsOfType(ObjectTypeStreet, objects);
		for (auto object : objects) {
			Street* street = new Street(manager, object);
			const StreetID streetID = street->GetID();
			street->AssignRelationsAtLock(RelationsFrom(Relation::TypeStreetAtLock, streetID));
			street->AssignRelationsAtUnlock(RelationsFrom(Relation::TypeStreetAtUnlock, streetID));
			streets[streetID] = street;
		}
	}

	void StorageHandler::DeleteStreet(const StreetID streetID)
	{
		if (instance == nullptr)
		{
			return;
		}
		StartTransactionInternal();
		instance->DeleteRelationsFrom(DataModel::Relation::TypeStreetAtLock, streetID);
		instance->DeleteRelationsFrom(DataModel::Relation::TypeStreetAtUnlock, streetID);
		instance->DeleteObject(ObjectTypeStreet, streetID);
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
		instance->DeleteObject(ObjectTypeSignal, signalID);
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
			output.push_back(new Relation(manager, relationString));
		}
		return output;
	}
} // namespace Storage

