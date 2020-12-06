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

#include <map>
#include <string>

#include "DataModel/Cluster.h"
#include "DataModel/LockableItem.h"
#include "DataModel/Relation.h"
#include "DataModel/Signal.h"
#include "DataModel/Track.h"
#include "Manager.h"
#include "Utils/Utils.h"

using std::map;
using std::string;
using std::to_string;

namespace DataModel
{
	std::string Cluster::Serialize() const
	{
		string str;
		str += "objectType=Cluster;";
		str += Object::Serialize();
		str += ";orientation=";
		str += to_string(orientation);
		return str;
	}

	bool Cluster::Deserialize(const string& serialized)
	{
		map<string,string> arguments;
		ParseArguments(serialized, arguments);
		Object::Deserialize(arguments);
		if (!arguments.count("objectType") || arguments.at("objectType").compare("Cluster") != 0)
		{
			return false;
		}
		orientation = static_cast<Orientation>(Utils::Utils::GetBoolMapEntry(arguments, "orientation", OrientationRight));
		return true;
	}

	bool Cluster::CanSetLocoOrientation(const Orientation orientation, const LocoID locoId)
	{
		std::lock_guard<std::mutex> Guard(orientationMutex);
		if (this->orientation == orientation)
		{
			return true;
		}
		for (auto relation : tracks)
		{
			Track* track = dynamic_cast<Track*>(relation->GetObject2());
			if (track == nullptr)
			{
				return false;
			}
			if (track->GetLockState() == DataModel::LockableItem::LockStateFree)
			{
				continue;
			}
			if (track->GetLoco() == locoId)
			{
				continue;
			}
			return false;
		}
		for (auto relation : signals)
		{
			Signal* signal = dynamic_cast<Signal*>(relation->GetObject2());
			if (signal == nullptr)
			{
				return false;
			}
			if (signal->GetLockState() == DataModel::LockableItem::LockStateFree)
			{
				continue;
			}
			if (signal->GetLoco() == locoId)
			{
				continue;
			}
			return false;
		}
		return true;
	}

	bool Cluster::SetLocoOrientation(const Orientation orientation, const LocoID locoId)
	{
		std::lock_guard<std::mutex> Guard(orientationMutex);
		if (this->orientation == orientation)
		{
			return true;
		}
		for (auto relation : tracks)
		{
			Track* track = dynamic_cast<Track*>(relation->GetObject2());
			if (track == nullptr)
			{
				return false;
			}
			if (track->GetLockState() == DataModel::LockableItem::LockStateFree)
			{
				continue;
			}
			if (track->GetLoco() == locoId)
			{
				continue;
			}
			return false;
		}
		for (auto relation : signals)
		{
			Signal* signal = dynamic_cast<Signal*>(relation->GetObject2());
			if (signal == nullptr)
			{
				return false;
			}
			if (signal->GetLockState() == DataModel::LockableItem::LockStateFree)
			{
				continue;
			}
			if (signal->GetLoco() == locoId)
			{
				continue;
			}
			return false;
		}
		this->orientation = orientation;
		return true;
	}

	void Cluster::DeleteTracks()
	{
		while (tracks.size() > 0)
		{
			Relation* trackRelation = tracks.back();
			Track* track = dynamic_cast<Track*>(trackRelation->GetObject2());
			if (track != nullptr)
			{
				track->SetCluster(nullptr);
			}
			tracks.pop_back();
			delete trackRelation;
		}
	}

	void Cluster::DeleteTrack(Track* trackToDelete)
	{
		for (unsigned int index = 0; index < tracks.size(); ++index)
		{
			if (tracks[index]->GetObject2() != trackToDelete)
			{
				continue;
			}
			delete tracks[index];
			tracks.erase(tracks.begin() + index);
			trackToDelete->SetCluster(nullptr);
			return;
		}
	}

	void Cluster::AssignTracks(const std::vector<DataModel::Relation*>& newTracks)
	{
		DeleteTracks();
		tracks = newTracks;
		for (auto trackRelation : tracks)
		{
			Track* track = dynamic_cast<Track*>(trackRelation->GetObject2());
			if (track != nullptr)
			{
				track->SetCluster(this);
			}
		}
	}

	void Cluster::DeleteSignals()
	{
		while (signals.size() > 0)
		{
			Relation* signalRelation = signals.back();
			Signal* signal = dynamic_cast<Signal*>(signalRelation->GetObject2());
			if (signal != nullptr)
			{
				signal->SetCluster(nullptr);
			}
			signals.pop_back();
			delete signalRelation;
		}
	}

	void Cluster::DeleteSignal(Signal* signalToDelete)
	{
		for (unsigned int index = 0; index < signals.size(); ++index)
		{
			if (signals[index]->GetObject2() != signalToDelete)
			{
				continue;
			}
			delete signals[index];
			signals.erase(signals.begin() + index);
			signalToDelete->SetCluster(nullptr);
			return;
		}
	}

	void Cluster::AssignSignals(const std::vector<DataModel::Relation*>& newSignals)
	{
		DeleteSignals();
		signals = newSignals;
		for (auto signalRelation : tracks)
		{
			Signal* signal = dynamic_cast<Signal*>(signalRelation->GetObject2());
			if (signal != nullptr)
			{
				signal->SetCluster(this);
			}
		}
	}
} // namespace DataModel
