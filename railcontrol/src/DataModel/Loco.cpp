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

#include <algorithm>
#include <map>
#include <string>

#include "DataModel/Loco.h"
#include "DataModel/Track.h"
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
			logger->Info(Languages::TextWaitingUntilHasStopped, name);
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
		str += ";direction=";
		str += (direction == DirectionRight ? "right" : "left");
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
		direction = (Utils::Utils::GetStringMapEntry(arguments, "direction", "right").compare("right") == 0 ? DirectionRight : DirectionLeft);
		length = static_cast<Length>(Utils::Utils::GetIntegerMapEntry(arguments, "length", 0));
		pushpull = Utils::Utils::GetBoolMapEntry(arguments, "commuter", false);  // FIXME: remove later
		pushpull = Utils::Utils::GetBoolMapEntry(arguments, "pushpull", pushpull);
		maxSpeed = Utils::Utils::GetIntegerMapEntry(arguments, "maxspeed", MaxSpeed);
		travelSpeed = Utils::Utils::GetIntegerMapEntry(arguments, "travelspeed", DefaultTravelSpeed);
		reducedSpeed = Utils::Utils::GetIntegerMapEntry(arguments, "reducedspeed", DefaultReducedSpeed);
		creepingSpeed = Utils::Utils::GetIntegerMapEntry(arguments, "creepspeed", DefaultCreepingSpeed);
		creepingSpeed = Utils::Utils::GetIntegerMapEntry(arguments, "creepingspeed", creepingSpeed);
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

		if (streetFirst != nullptr)
		{
			streetFirst->Release(logger, objectID);
			streetFirst = nullptr;
		}
		if (streetSecond != nullptr)
		{
			streetSecond->Release(logger, objectID);
			streetSecond = nullptr;
		}
		if (trackFrom != nullptr)
		{
			trackFrom->BaseRelease(logger, objectID);
			trackFrom = nullptr;
		}
		if (trackFirst != nullptr)
		{
			trackFirst->BaseRelease(logger, objectID);
			trackFirst = nullptr;
		}
		if (trackSecond != nullptr)
		{
			trackSecond->BaseRelease(logger, objectID);
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
			logger->Warning(Languages::TextCanNotStartNotOnTrack, name);
			return false;
		}
		if (state == LocoStateError)
		{
			logger->Warning(Languages::TextCanNotStartInErrorState, name);
			return false;
		}
		if (state == LocoStateTerminated)
		{
			locoThread.join();
			state = LocoStateManual;
		}
		if (state != LocoStateManual)
		{
			logger->Info(Languages::TextCanNotStartAlreadyRunning, name);
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
					if (feedbackId == feedbackIdStop)
					{
						if (feedbackIdFirst != FeedbackNone)
						{
							FeedbackIdFirstReached();
						}
						FeedbackIdStopReached();
					}
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
		if (streetFirst != nullptr)
		{
			state = LocoStateError;
			logger->Error(Languages::TextHasAlreadyReservedStreet, name);
			return;
		}

		Street* usedStreet = SearchDestination(trackFrom, true);
		if (usedStreet == nullptr)
		{
			return;
		}

		const ObjectIdentifier& newTrackIdentifierFirst = usedStreet->GetToTrack();
		TrackBase* newTrack = manager->GetTrackBase(newTrackIdentifierFirst);
		if (newTrack == nullptr)
		{
			return;
		}

		trackFirst = newTrack;
		streetFirst = usedStreet;
		feedbackIdFirst = FeedbackNone;
		feedbackIdReduced = streetFirst->GetFeedbackIdReduced();
		feedbackIdCreep = streetFirst->GetFeedbackIdCreep();
		feedbackIdStop = streetFirst->GetFeedbackIdStop();
		feedbackIdOver = streetFirst->GetFeedbackIdOver();
		wait = streetFirst->GetWaitAfterRelease();
		bool turnLoco = (trackFrom->GetLocoDirection() != streetFirst->GetFromDirection());
		Direction newLocoDirection = static_cast<Direction>(direction != turnLoco);
		if (turnLoco)
		{
			trackFrom->SetLocoDirection(streetFirst->GetFromDirection());
			manager->TrackBasePublishState(trackFrom);
		}
		manager->LocoDirection(ControlTypeInternal, this, newLocoDirection);
		newTrack->SetLocoDirection(static_cast<Direction>(streetFirst->GetToDirection()));
		logger->Info(Languages::TextHeadingToVia, newTrack->GetMyName(), streetFirst->GetName());

		// start loco
		manager->TrackBasePublishState(newTrack);
		Speed newSpeed;
		switch (streetFirst->GetSpeed())
		{
			case Street::SpeedTravel:
				newSpeed = travelSpeed;
				break;

			case Street::SpeedReduced:
				newSpeed = reducedSpeed;
				break;

			case Street::SpeedCreeping:
			default:
				newSpeed = creepingSpeed;
				break;
		}
		manager->LocoSpeed(ControlTypeInternal, this, newSpeed);
		state = LocoStateSearchingSecond;
	}

	void Loco::SearchDestinationSecond()
	{
		Street* usedStreet = SearchDestination(trackFirst, false);
		if (usedStreet == nullptr)
		{
			return;
		}

		const ObjectIdentifier& newTrackIdentifierSecond = usedStreet->GetToTrack();
		TrackBase* newTrack = manager->GetTrackBase(newTrackIdentifierSecond);
		if (newTrack == nullptr)
		{
			return;
		}

		trackSecond = newTrack;
		streetSecond = usedStreet;
		feedbackIdFirst = feedbackIdStop;
		feedbackIdOver = streetSecond->GetFeedbackIdOver();
		feedbackIdStop = streetSecond->GetFeedbackIdStop();
		Street::Speed speedFirst = streetFirst->GetSpeed();
		Street::Speed speedSecond = streetSecond->GetSpeed();
		if (speedSecond == Street::SpeedTravel)
		{
			feedbackIdCreep = streetSecond->GetFeedbackIdCreep();
			feedbackIdReduced = streetSecond->GetFeedbackIdReduced();
			if (speedFirst == Street::SpeedTravel)
			{
				manager->LocoSpeed(ControlTypeInternal, this, travelSpeed);
			}
		}
		else if (speedSecond == Street::SpeedReduced)
		{
			feedbackIdCreep = streetSecond->GetFeedbackIdCreep();
			if (speedFirst == Street::SpeedReduced)
			{
				manager->LocoSpeed(ControlTypeInternal, this, reducedSpeed);
			}
		}

		wait = streetSecond->GetWaitAfterRelease();
		newTrack->SetLocoDirection(static_cast<Direction>(streetSecond->GetToDirection()));
		logger->Info(Languages::TextHeadingToViaVia, newTrack->GetMyName(), streetFirst->GetName(), streetSecond->GetName());

		// start loco
		manager->TrackBasePublishState(newTrack);
		state = LocoStateRunning;
	}

	Street* Loco::SearchDestination(TrackBase* track, const bool allowLocoTurn)
	{
		if (manager->Booster() == BoosterStateStop)
		{
			return nullptr;
		}
		logger->Debug(Languages::TextLookingForDestination, track->GetMyName());
		if (streetSecond != nullptr)
		{
			state = LocoStateError;
			logger->Error(Languages::TextHasAlreadyReservedStreet, name);
			return nullptr;
		}

		if (track == nullptr)
		{
			state = LocoStateOff;
			logger->Info(Languages::TextIsNotOnTrack, name);
			return nullptr;
		}

		LocoID locoIdOfTrack = track->GetMyLoco();
		if (locoIdOfTrack != GetID())
		{
			state = LocoStateError;
			logger->Error(Languages::TextIsOnOcupiedTrack, name, track->GetMyName(), manager->GetLocoName(locoIdOfTrack));
			return nullptr;
		}

		vector<Street*> validStreets;
		track->GetValidStreets(logger, this, allowLocoTurn, validStreets);
		for (auto street : validStreets)
		{
			logger->Debug(Languages::TextExecutingStreet, street->GetName());
			if (street->Reserve(logger, objectID) == false)
			{
				continue;
			}

			if (street->Lock(logger, objectID) == false)
			{
				street->Release(logger, objectID);
				continue;
			}

			if (street->Execute(logger, objectID) == false)
			{
				street->Release(logger, objectID);
				continue;
			}

			if (!allowLocoTurn && track->GetLocoDirection() != street->GetFromDirection())
			{
				continue;
			}

			return street;
		}
		logger->Debug(Languages::TextNoValidStreetFound, name);
		return nullptr;
	}

	void Loco::LocationReached(const FeedbackID feedbackID)
	{
		if (feedbackID == feedbackIdOver)
		{
			manager->LocoSpeed(ControlTypeInternal, this, MinSpeed);
			manager->Booster(ControlTypeInternal, BoosterStateStop);
			logger->Error(Languages::TextHitOverrun, name, manager->GetFeedbackName(feedbackID));
			return;
		}

		if (feedbackID == feedbackIdStop)
		{
			manager->LocoSpeed(ControlTypeInternal, this, MinSpeed);
			feedbackIdsReached.Enqueue(feedbackIdStop);
			return;
		}

		if (feedbackID == feedbackIdCreep)
		{
			if (speed > creepingSpeed)
			{
				manager->LocoSpeed(ControlTypeInternal, this, creepingSpeed);
			}
			return;
		}

		if (feedbackID == feedbackIdReduced)
		{
			if (speed > reducedSpeed)
			{
				manager->LocoSpeed(ControlTypeInternal, this, reducedSpeed);
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

	void Loco::SetDirection(const Direction direction)
	{
		this->direction = direction;
		for (auto slave : slaves)
		{
			manager->LocoDirection(ControlTypeInternal, slave->ObjectID2(), direction);
		}
	}

	void Loco::FeedbackIdFirstReached()
	{
		if (streetFirst == nullptr || trackFrom == nullptr)
		{
			manager->LocoSpeed(ControlTypeInternal, this, MinSpeed);
			state = LocoStateError;
			logger->Error(Languages::TextIsInAutomodeWithoutStreetTrack, name);
			return;
		}

		Speed newSpeed;
		switch (streetFirst->GetSpeed())
		{
			case Street::SpeedTravel:
				newSpeed = travelSpeed;
				break;

			case Street::SpeedReduced:
				newSpeed = reducedSpeed;
				break;

			case Street::SpeedCreeping:
			default:
				newSpeed = creepingSpeed;
				break;
		}

		if (speed > newSpeed)
		{
			manager->LocoSpeed(ControlTypeInternal, this, newSpeed);
		}


		streetFirst->Release(logger, objectID);
		streetFirst = streetSecond;
		streetSecond = nullptr;

		trackFrom->BaseRelease(logger, objectID);
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
				logger->Error(Languages::TextIsInInvalidAutomodeState, name, state, manager->GetFeedbackName(feedbackIdFirst));
				state = LocoStateError;
				break;
		}

		feedbackIdFirst = FeedbackNone;
	}

	void Loco::FeedbackIdStopReached()
	{
		if (streetFirst == nullptr || trackFrom == nullptr)
		{
			manager->LocoSpeed(ControlTypeInternal, this, MinSpeed);
			state = LocoStateError;
			logger->Error(Languages::TextIsInAutomodeWithoutStreetTrack, name);
			return;
		}

		manager->LocoDestinationReached(this, streetFirst, trackFrom);
		streetFirst->Release(logger, objectID);
		streetFirst = nullptr;

		trackFrom->BaseRelease(logger, objectID);
		trackFrom = trackFirst;
		trackFirst = nullptr;
		logger->Info(Languages::TextReachedItsDestination, name);

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
				logger->Error(Languages::TextIsInInvalidAutomodeState, name, state, manager->GetFeedbackName(feedbackIdStop));
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
} // namespace DataModel
