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

#include <algorithm>
#include <map>
#include <string>

#include "DataModel/Loco.h"
#include "DataModel/Track.h"
#include "Hardware/LocoCache.h"
#include "Manager.h"
#include "Utils/Utils.h"

using std::map;
using std::string;
using std::to_string;
using std::vector;

namespace DataModel
{
	Loco::~Loco()
	{
		while (true)
		{
			{
				std::lock_guard<std::mutex> Guard(stateMutex);
				if (state == LocoStateManual)
				{
					break;
				}
			}
			logger->Info(Languages::TextWaitingUntilHasStopped, GetName());
			Utils::Utils::SleepForSeconds(1);
		}
		DeleteSlaves();
	}

	std::string Loco::Serialize() const
	{
		string str;
		str += "objectType=Loco;";
		str += Object::Serialize();
		str += ";";
		str += HardwareHandle::Serialize();
		str += ";functions=";
		str += functions.Serialize();
		str += ";orientation=";
		str += to_string(orientation);
		if (trackFrom != nullptr)
		{
			str += ";track=";
			str += trackFrom->GetObjectIdentifier();
		}
		str += ";length=";
		str += to_string(length);
		str += ";pushpull=";
		str += to_string(pushpull);
		str += ";maxspeed=";
		str += to_string(maxSpeed);
		str += ";travelspeed=";
		str += to_string(travelSpeed);
		str += ";reducedspeed=";
		str += to_string(reducedSpeed);
		str += ";creepingspeed=";
		str += to_string(creepingSpeed);
		str += ";matchkey=";
		str += matchKey;
		return str;
	}

	bool Loco::Deserialize(const std::string& serialized)
	{
		map<string,string> arguments;
		ParseArguments(serialized, arguments);
		Object::Deserialize(arguments);
		if (!arguments.count("objectType") || arguments.at("objectType").compare("Loco") != 0)
		{
			return false;
		}
		HardwareHandle::Deserialize(arguments);
		ObjectIdentifier trackIdentifier = Utils::Utils::GetStringMapEntry(arguments, "track");
		if (trackIdentifier.GetObjectID() == ObjectNone)
		{
			trackIdentifier = static_cast<ObjectID>(Utils::Utils::GetIntegerMapEntry(arguments, "trackID", TrackNone));
			if (trackIdentifier.GetObjectID() != ObjectNone)
			{
				trackIdentifier = ObjectTypeTrack;
			}
		}
		trackFrom = manager->GetTrackBase(trackIdentifier);
		functions.Deserialize(Utils::Utils::GetStringMapEntry(arguments, "functions", "0"));
		orientation = (Utils::Utils::GetStringMapEntry(arguments, "direction", "right").compare("right") == 0 ? OrientationRight : OrientationLeft); // FIXME: remove later 2020-10-27
		orientation = (static_cast<Orientation>(Utils::Utils::GetBoolMapEntry(arguments, "orientation", orientation)));
		length = static_cast<Length>(Utils::Utils::GetIntegerMapEntry(arguments, "length", 0));
		pushpull = Utils::Utils::GetBoolMapEntry(arguments, "commuter", false);  // FIXME: remove later 2020-10-27
		pushpull = Utils::Utils::GetBoolMapEntry(arguments, "pushpull", pushpull);
		maxSpeed = Utils::Utils::GetIntegerMapEntry(arguments, "maxspeed", MaxSpeed);
		travelSpeed = Utils::Utils::GetIntegerMapEntry(arguments, "travelspeed", DefaultTravelSpeed);
		reducedSpeed = Utils::Utils::GetIntegerMapEntry(arguments, "reducedspeed", DefaultReducedSpeed);
		creepingSpeed = Utils::Utils::GetIntegerMapEntry(arguments, "creepspeed", DefaultCreepingSpeed);
		creepingSpeed = Utils::Utils::GetIntegerMapEntry(arguments, "creepingspeed", creepingSpeed);
		matchKey = Utils::Utils::GetStringMapEntry(arguments, "matchkey");
		return true;
	}

	bool Loco::SetTrack(const ObjectIdentifier& identifier)
	{
		std::lock_guard<std::mutex> Guard(stateMutex);
		// there must not be set a track
		if (this->trackFrom != nullptr)
		{
			return false;
		}
		this->trackFrom = manager->GetTrackBase(identifier);
		return true;
	}

	bool Loco::Release()
	{
		manager->LocoSpeed(ControlTypeInternal, this, MinSpeed);
		ForceManualMode();
		std::lock_guard<std::mutex> Guard(stateMutex);

		if (routeFirst != nullptr)
		{
			routeFirst->Release(logger, GetID());
			routeFirst = nullptr;
		}
		if (routeSecond != nullptr)
		{
			routeSecond->Release(logger, GetID());
			routeSecond = nullptr;
		}
		if (trackFrom != nullptr)
		{
			trackFrom->BaseRelease(logger, GetID());
			trackFrom = nullptr;
		}
		if (trackFirst != nullptr)
		{
			trackFirst->BaseRelease(logger, GetID());
			trackFirst = nullptr;
		}
		if (trackSecond != nullptr)
		{
			trackSecond->BaseRelease(logger, GetID());
			trackSecond = nullptr;
		}
		feedbackIdOver = FeedbackNone;
		feedbackIdStop = FeedbackNone;
		feedbackIdCreep = FeedbackNone;
		feedbackIdReduced = FeedbackNone;
		feedbackIdFirst = FeedbackNone;
		return true;
	}

	bool Loco::IsRunningFromTrack(const TrackID trackID) const
	{
		std::lock_guard<std::mutex> Guard(stateMutex);
		return trackFirst != nullptr && trackFrom != nullptr && trackFrom->GetMyID() == trackID;
	}

	bool Loco::GoToAutoMode()
	{
		std::lock_guard<std::mutex> Guard(stateMutex);
		if (trackFrom == nullptr)
		{
			logger->Warning(Languages::TextCanNotStartNotOnTrack, GetName());
			return false;
		}
		if (state == LocoStateError)
		{
			logger->Warning(Languages::TextCanNotStartInErrorState, GetName());
			return false;
		}
		if (state == LocoStateTerminated)
		{
			locoThread.join();
			state = LocoStateManual;
		}
		if (state != LocoStateManual)
		{
			logger->Info(Languages::TextCanNotStartAlreadyRunning, GetName());
			return false;
		}

		state = LocoStateSearchingFirst;
		locoThread = std::thread(&DataModel::Loco::AutoMode, this);

		return true;
	}

	void Loco::RequestManualMode()
	{
		if (state == LocoStateManual || state == LocoStateTerminated)
		{
			return;
		}
		requestManualMode = true;
	}

	bool Loco::GoToManualMode()
	{
		if (state == LocoStateManual)
		{
			return true;
		}
		if (state != LocoStateTerminated)
		{
			return false;
		}
		locoThread.join();
		state = LocoStateManual;
		return true;
	}

	void Loco::ForceManualMode()
	{
		{
			std::lock_guard<std::mutex> Guard(stateMutex);
			switch (state)
			{
				case LocoStateManual:
					return;

				case LocoStateTerminated:
					break;

				default:
					state = LocoStateOff;
					break;
			}
		}
		locoThread.join();
		state = LocoStateManual;
	}

	void Loco::AutoMode()
	{
		Utils::Utils::SetMinThreadPriority();
		const string& name = GetName();
		Utils::Utils::SetThreadName(name);
		logger->Info(Languages::TextIsNowInAutoMode, name);

		while (true)
		{
			{ // sleep must be outside of locked block
				std::lock_guard<std::mutex> Guard(stateMutex);
				if (feedbackIdsReached.IsEmpty() == false)
				{
					FeedbackID feedbackId = feedbackIdsReached.Dequeue();
					if (feedbackId == feedbackIdFirst)
					{
						FeedbackIdFirstReached();
					}
					else if (feedbackId == feedbackIdStop)
					{
						FeedbackIdStopReached();
					}
					continue;
				}

				switch (state)
				{
					case LocoStateOff:
						// automode is turned off, terminate thread
						logger->Info(Languages::TextIsNowInManualMode, name);
						state = LocoStateTerminated;
						requestManualMode = false;
						return;

					case LocoStateSearchingFirst:
						if (requestManualMode)
						{
							state = LocoStateOff;
							break;
						}
						if (wait > 0)
						{
							--wait;
							break;
						}
						SearchDestinationFirst();
						break;

					case LocoStateSearchingSecond:
						if (requestManualMode)
						{
							logger->Info(Languages::TextIsRunningWaitingUntilDestination, name);
							state = LocoStateStopping;
							break;
						}
						if (manager->GetNrOfTracksToReserve() <= 1)
						{
							break;
						}
						if (wait > 0)
						{
							break;
						}
						SearchDestinationSecond();
						break;

					case LocoStateRunning:
						// loco is already running, waiting until destination reached
						if (requestManualMode)
						{
							logger->Info(Languages::TextIsRunningWaitingUntilDestination, name);
							state = LocoStateStopping;
						}
						break;

					case LocoStateStopping:
						logger->Info(Languages::TextHasNotReachedDestination, name);

						break;

					case LocoStateTerminated:
						logger->Error(Languages::TextIsInTerminatedState, name);
						state = LocoStateError;
						break;

					case LocoStateManual:
						logger->Error(Languages::TextIsInManualState, name);
						state = LocoStateError;
						#include "Fallthrough.h"

					case LocoStateError:
						logger->Error(Languages::TextIsInErrorState, name);
						manager->LocoSpeed(ControlTypeInternal, this, MinSpeed);
						if (requestManualMode)
						{
							state = LocoStateOff;
						}
						break;
				}
			}
			// FIXME: make configurable
			Utils::Utils::SleepForSeconds(1);
		}
	}

	void Loco::SearchDestinationFirst()
	{
		if (routeFirst != nullptr)
		{
			state = LocoStateError;
			logger->Error(Languages::TextHasAlreadyReservedRoute, GetName());
			return;
		}

		Route* usedRoute = SearchDestination(trackFrom, true);
		if (usedRoute == nullptr)
		{
			return;
		}

		const ObjectIdentifier& newTrackIdentifierFirst = usedRoute->GetToTrack();
		TrackBase* newTrack = manager->GetTrackBase(newTrackIdentifierFirst);
		if (newTrack == nullptr)
		{
			return;
		}

		bool isOrientationSet = newTrack->SetLocoOrientation(static_cast<Orientation>(usedRoute->GetToOrientation()));
		if (isOrientationSet == false)
		{
			return;
		}

		bool turnLoco = (trackFrom->GetLocoOrientation() != usedRoute->GetFromOrientation());
		Orientation newLocoOrientation = static_cast<Orientation>(orientation != turnLoco);
		if (turnLoco)
		{
			bool canTurnOrientation = trackFrom->SetLocoOrientation(usedRoute->GetFromOrientation());
			if (canTurnOrientation == false)
			{
				return;
			}
			manager->TrackBasePublishState(trackFrom);
		}
		manager->LocoOrientation(ControlTypeInternal, this, newLocoOrientation);
		logger->Info(Languages::TextHeadingToVia, newTrack->GetMyName(), usedRoute->GetName());

		trackFirst = newTrack;
		routeFirst = usedRoute;
		feedbackIdFirst = FeedbackNone;
		feedbackIdReduced = routeFirst->GetFeedbackIdReduced();
		feedbackIdCreep = routeFirst->GetFeedbackIdCreep();
		feedbackIdStop = routeFirst->GetFeedbackIdStop();
		feedbackIdOver = routeFirst->GetFeedbackIdOver();
		wait = routeFirst->GetWaitAfterRelease();

		// start loco
		manager->TrackBasePublishState(newTrack);
		Speed newSpeed;
		switch (routeFirst->GetSpeed())
		{
			case Route::SpeedTravel:
				newSpeed = travelSpeed;
				break;

			case Route::SpeedReduced:
				newSpeed = reducedSpeed;
				break;

			case Route::SpeedCreeping:
			default:
				newSpeed = creepingSpeed;
				break;
		}
		manager->LocoSpeed(ControlTypeInternal, this, newSpeed);
		state = LocoStateSearchingSecond;
	}

	void Loco::SearchDestinationSecond()
	{
		Route* usedRoute = SearchDestination(trackFirst, false);
		if (usedRoute == nullptr)
		{
			return;
		}

		const ObjectIdentifier& newTrackIdentifierSecond = usedRoute->GetToTrack();
		TrackBase* newTrack = manager->GetTrackBase(newTrackIdentifierSecond);
		if (newTrack == nullptr)
		{
			return;
		}

		const bool isOrientationSet = newTrack->SetLocoOrientation(static_cast<Orientation>(usedRoute->GetToOrientation()));
		if (isOrientationSet == false)
		{
			return;
		}
		logger->Info(Languages::TextHeadingToViaVia, newTrack->GetMyName(), routeFirst->GetName(), usedRoute->GetName());

		trackSecond = newTrack;
		routeSecond = usedRoute;
		feedbackIdFirst = feedbackIdStop;
		feedbackIdOver = routeSecond->GetFeedbackIdOver();
		feedbackIdStop = routeSecond->GetFeedbackIdStop();
		Route::Speed speedFirst = routeFirst->GetSpeed();
		Route::Speed speedSecond = routeSecond->GetSpeed();
		if (speedSecond == Route::SpeedTravel)
		{
			feedbackIdCreep = routeSecond->GetFeedbackIdCreep();
			feedbackIdReduced = routeSecond->GetFeedbackIdReduced();
			if (speedFirst == Route::SpeedTravel)
			{
				manager->LocoSpeed(ControlTypeInternal, this, travelSpeed);
			}
		}
		else if (speedSecond == Route::SpeedReduced)
		{
			feedbackIdCreep = routeSecond->GetFeedbackIdCreep();
			if (speedFirst == Route::SpeedReduced)
			{
				manager->LocoSpeed(ControlTypeInternal, this, reducedSpeed);
			}
		}

		wait = routeSecond->GetWaitAfterRelease();

		// start loco
		manager->TrackBasePublishState(newTrack);
		state = LocoStateRunning;
	}

	Route* Loco::SearchDestination(TrackBase* track, const bool allowLocoTurn)
	{
		if (manager->Booster() == BoosterStateStop)
		{
			return nullptr;
		}
		logger->Debug(Languages::TextLookingForDestination, track->GetMyName());
		if (routeSecond != nullptr)
		{
			state = LocoStateError;
			logger->Error(Languages::TextHasAlreadyReservedRoute, GetName());
			return nullptr;
		}

		if (track == nullptr)
		{
			state = LocoStateOff;
			logger->Info(Languages::TextIsNotOnTrack, GetName());
			return nullptr;
		}

		LocoID locoIdOfTrack = track->GetMyLoco();
		if (locoIdOfTrack != GetID())
		{
			state = LocoStateError;
			logger->Error(Languages::TextIsOnOcupiedTrack, GetName(), track->GetMyName(), manager->GetLocoName(locoIdOfTrack));
			return nullptr;
		}

		vector<Route*> validRoutes;
		track->GetValidRoutes(logger, this, allowLocoTurn, validRoutes);
		LocoID objectID = GetID();
		for (auto route : validRoutes)
		{
			logger->Debug(Languages::TextExecutingRoute, route->GetName());

			if (route->Reserve(logger, objectID) == false)
			{
				continue;
			}

			if (route->Lock(logger, objectID) == false)
			{
				route->Release(logger, objectID);
				continue;
			}

			const ObjectIdentifier& identifier = route->GetToTrack();
			TrackBase* newTrack = manager->GetTrackBase(identifier);

			if (newTrack == nullptr)
			{
				route->Release(logger, objectID);
				continue;
			}

			bool canSetOrientation = newTrack->CanSetLocoOrientation(route->GetToOrientation(), GetID());
			if (canSetOrientation == false)
			{
				route->Release(logger, objectID);
				newTrack->BaseRelease(logger, objectID);
				continue;
			}

			if (!allowLocoTurn && track->GetLocoOrientation() != route->GetFromOrientation())
			{
				route->Release(logger, objectID);
				newTrack->BaseRelease(logger, objectID);
				continue;
			}

			if (route->Execute(logger, objectID) == false)
			{
				route->Release(logger, objectID);
				newTrack->BaseRelease(logger, objectID);
				continue;
			}

			return route;
		}
		logger->Debug(Languages::TextNoValidRouteFound, GetName());
		return nullptr;
	}

	void Loco::LocationReached(const FeedbackID feedbackID)
	{
		if (feedbackID == feedbackIdOver)
		{
			manager->LocoSpeed(ControlTypeInternal, this, MinSpeed);
			manager->Booster(ControlTypeInternal, BoosterStateStop);
			logger->Error(Languages::TextHitOverrun, GetName(), manager->GetFeedbackName(feedbackID));
			return;
		}

		if (feedbackID == feedbackIdStop)
		{
			manager->LocoSpeed(ControlTypeInternal, this, MinSpeed);
			if (feedbackIdFirst != 0)
			{
				feedbackIdsReached.Enqueue(feedbackIdFirst);
			}
			feedbackIdsReached.Enqueue(feedbackIdStop);
			return;
		}

		if (feedbackID == feedbackIdCreep)
		{
			if (speed > creepingSpeed)
			{
				manager->LocoSpeed(ControlTypeInternal, this, creepingSpeed);
			}
			if (feedbackIdFirst != 0)
			{
				feedbackIdsReached.Enqueue(feedbackIdFirst);
			}
			return;
		}

		if (feedbackID == feedbackIdReduced)
		{
			if (speed > reducedSpeed)
			{
				manager->LocoSpeed(ControlTypeInternal, this, reducedSpeed);
			}
			if (feedbackIdFirst != 0)
			{
				feedbackIdsReached.Enqueue(feedbackIdFirst);
			}
			return;
		}

		if (feedbackID == feedbackIdFirst)
		{
			feedbackIdsReached.Enqueue(feedbackIdFirst);
			return;
		}
	}

	void Loco::SetSpeed(const Speed speed, const bool withSlaves)
	{
		this->speed = speed;
		if (!withSlaves)
		{
			return;
		}
		for (auto slave : slaves)
		{
			manager->LocoSpeed(ControlTypeInternal, slave->ObjectID2(), speed, false);
		}
	}

	void Loco::SetOrientation(const Orientation orientation)
	{
		this->orientation = orientation;
		for (auto slave : slaves)
		{
			manager->LocoOrientation(ControlTypeInternal, slave->ObjectID2(), orientation);
		}
	}

	void Loco::FeedbackIdFirstReached()
	{
		if (routeFirst == nullptr || trackFrom == nullptr)
		{
			manager->LocoSpeed(ControlTypeInternal, this, MinSpeed);
			state = LocoStateError;
			logger->Error(Languages::TextIsInAutomodeWithoutRouteTrack, GetName());
			return;
		}

		Speed newSpeed;
		switch (routeFirst->GetSpeed())
		{
			case Route::SpeedTravel:
				newSpeed = travelSpeed;
				break;

			case Route::SpeedReduced:
				newSpeed = reducedSpeed;
				break;

			case Route::SpeedCreeping:
			default:
				newSpeed = creepingSpeed;
				break;
		}

		if (speed > newSpeed)
		{
			manager->LocoSpeed(ControlTypeInternal, this, newSpeed);
		}


		routeFirst->Release(logger, GetID());
		routeFirst = routeSecond;
		routeSecond = nullptr;

		trackFrom->BaseRelease(logger, GetID());
		trackFrom = trackFirst;
		trackFirst = trackSecond;
		trackSecond = nullptr;

		// set state
		switch (state)
		{
			case LocoStateRunning:
				state = LocoStateSearchingSecond;
				break;

			case LocoStateStopping:
				// do nothing
				break;

			default:
				logger->Error(Languages::TextIsInInvalidAutomodeState, GetName(), state, manager->GetFeedbackName(feedbackIdFirst));
				state = LocoStateError;
				break;
		}

		feedbackIdFirst = FeedbackNone;
	}

	void Loco::FeedbackIdStopReached()
	{
		if (routeFirst == nullptr || trackFrom == nullptr)
		{
			manager->LocoSpeed(ControlTypeInternal, this, MinSpeed);
			state = LocoStateError;
			logger->Error(Languages::TextIsInAutomodeWithoutRouteTrack, GetName());
			return;
		}

		manager->LocoDestinationReached(this, routeFirst, trackFrom);
		routeFirst->Release(logger, GetID());
		routeFirst = nullptr;

		trackFrom->BaseRelease(logger, GetID());
		trackFrom = trackFirst;
		trackFirst = nullptr;
		logger->Info(Languages::TextReachedItsDestination, GetName());

		// set state
		switch (state)
		{
			case LocoStateSearchingSecond:
				state = LocoStateSearchingFirst;
				break;

			case LocoStateStopping:
				state = LocoStateOff;
				break;

			default:
				logger->Error(Languages::TextIsInInvalidAutomodeState, GetName(), state, manager->GetFeedbackName(feedbackIdStop));
				state = LocoStateError;
				break;
		}

		feedbackIdStop = FeedbackNone;
		feedbackIdCreep = FeedbackNone;
		feedbackIdReduced = FeedbackNone;
	}

	void Loco::DeleteSlaves()
	{
		while (slaves.size() > 0)
		{
			Relation* slave = slaves.back();
			slaves.pop_back();
			delete slave;
		}
	}

	bool Loco::AssignSlaves(const std::vector<DataModel::Relation*>& newslaves)
	{
		DeleteSlaves();
		slaves = newslaves;
		return true;
	}

	DataModel::LocoFunctionNr Loco::GetFunctionNumberFromFunctionIcon(DataModel::LocoFunctionIcon icon) const
	{
		for (DataModel::LocoFunctionNr nr = 0; nr < NumberOfLocoFunctions; ++nr)
		{
			if (icon == functions.GetFunctionIcon(nr))
			{
				return nr;
			}
		}
		return NumberOfLocoFunctions;
	}

	Loco& Loco::operator=(const Hardware::LocoCacheEntry& loco)
	{
		SetControlID(loco.GetControlID());
		SetAddress(loco.GetAddress());
		SetProtocol(loco.GetProtocol());
		SetName(loco.GetName());
		SetMatchKey(loco.GetMatchKey());
		ConfigureFunctions(loco.GetFunctionStates());
		return *this;
	}
} // namespace DataModel
