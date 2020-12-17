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

#include <mutex>
#include <string>
#include <vector>

#include "DataTypes.h"
#include "DataModel/Object.h"

class Manager;

namespace DataModel
{
	class Relation;
	class Signal;
	class Track;

	class Cluster : public Object
	{
		public:
			Cluster(__attribute__((unused)) Manager* manager, const ClusterID clusterID)
			:	Object(clusterID),
				orientation(OrientationRight)
			{
			}

			Cluster(const std::string& serialized)
			:	Object(ClusterNone),
				orientation(OrientationRight)
			{
				Deserialize(serialized);
			}

			virtual ~Cluster()
			{
				DeleteTracks();
				DeleteSignals();
			}

			inline ObjectType GetObjectType() const override
			{
				return ObjectTypeCluster;
			}

			std::string Serialize() const override;
			bool Deserialize(const std::string& serialized) override;

			bool CanSetLocoOrientation(const Orientation orientation, const LocoID locoId);
			bool SetLocoOrientation(const Orientation orientation, const LocoID locoId);

			inline Orientation GetLocoOrientation() const
			{
				return orientation;
			}

			inline const std::vector<DataModel::Relation*>& GetTracks() const
			{
				return tracks;
			}

			void DeleteTracks();
			void DeleteTrack(DataModel::Track* trackToDelete);
			void AssignTracks(const std::vector<DataModel::Relation*>& newTracks);

			inline const std::vector<DataModel::Relation*>& GetSignals() const
			{
				return signals;
			}

			void DeleteSignals();
			void DeleteSignal(DataModel::Signal* signalToDelete);
			void AssignSignals(const std::vector<DataModel::Relation*>& newSignals);

		private:
			Orientation orientation;
			std::vector<DataModel::Relation*> tracks;
			std::vector<DataModel::Relation*> signals;
			mutable std::mutex orientationMutex;
	};
} // namespace DataModel
