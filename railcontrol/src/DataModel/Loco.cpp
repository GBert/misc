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
#include <sstream>

#include "DataModel/Loco.h"
#include "DataModel/Track.h"
#include "Manager.h"
#include "Utils/Utils.h"

using std::map;
using std::stringstream;
using std::string;
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
					return;
				}
			}
			logger->Info(Languages::TextWaitingUntilHasStopped, name);
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		DeleteSlaves();
	}

	std::string Loco::Serialize() const
	{
		trackID_t trackIdFrom = TrackNone;
		if (trackFrom != nullptr)
		{
			trackIdFrom = trackFrom->GetID();
		}
		stringstream ss;
		ss << "objectType=Loco"
			<< ";" << Object::Serialize()
			<< ";" << HardwareHandle::Serialize()
			<< ";functions=" << functions.Serialize()
			<< ";direction=" << (direction == DirectionRight ? "right" : "left")
			<< ";trackID=" << static_cast<int>(trackIdFrom)
			<< ";length=" << length
			<< ";pushpull=" << static_cast<int>(pushpull)
			<< ";maxspeed=" << maxSpeed
			<< ";travelspeed=" << travelSpeed
			<< ";reducedspeed=" << reducedSpeed
			<< ";creepingspeed=" << creepingSpeed;
		return ss.str();
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
		trackID_t trackIdFrom = Utils::Utils::GetIntegerMapEntry(arguments, "trackID", TrackNone);
		trackFrom = manager->GetTrack(trackIdFrom);
		functions.Deserialize(Utils::Utils::GetStringMapEntry(arguments, "functions", "0"));
		direction = (Utils::Utils::GetStringMapEntry(arguments, "direction", "right").compare("right") == 0 ? DirectionRight : DirectionLeft);
		length = static_cast<length_t>(Utils::Utils::GetIntegerMapEntry(arguments, "length", 0));
		pushpull = Utils::Utils::GetBoolMapEntry(arguments, "commuter", false);  // FIXME: remove later
		pushpull = Utils::Utils::GetBoolMapEntry(arguments, "pushpull", pushpull);
		maxSpeed = Utils::Utils::GetIntegerMapEntry(arguments, "maxspeed", MaxSpeed);
		travelSpeed = Utils::Utils::GetIntegerMapEntry(arguments, "travelspeed", DefaultTravelSpeed);
		reducedSpeed = Utils::Utils::GetIntegerMapEntry(arguments, "reducedspeed", DefaultReducedSpeed);
		creepingSpeed = Utils::Utils::GetIntegerMapEntry(arguments, "creepspeed", DefaultCreepingSpeed);
		creepingSpeed = Utils::Utils::GetIntegerMapEntry(arguments, "creepingspeed", creepingSpeed);
		return true;
	}

	bool Loco::ToTrack(const trackID_t trackID)
	{
		std::lock_guard<std::mutex> Guard(stateMutex);
		// there must not be set a track
		if (this->trackFrom != nullptr)
		{
			return false;
		}
		this->trackFrom = manager->GetTrack(trackID);
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
			trackFrom->Release(logger, objectID);
			trackFrom = nullptr;
		}
		if (trackFirst != nullptr)
		{
			trackFirst->Release(logger, objectID);
			trackFirst = nullptr;
		}
		if (trackSecond != nullptr)
		{
			trackSecond->Release(logger, objectID);
			trackSecond = nullptr;
		}
		feedbackIdOver = FeedbackNone;
		feedbackIdStop = FeedbackNone;
		feedbackIdCreep = FeedbackNone;
		feedbackIdReduced = FeedbackNone;
		feedbackIdFirst = FeedbackNone;
		return true;
	}

	bool Loco::IsRunningFromTrack(const trackID_t trackID) const
	{
		std::lock_guard<std::mutex> Guard(stateMutex);
		return trackFirst != nullptr && trackFrom != nullptr && trackFrom->GetID() == trackID;
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
					feedbackID_t feedbackId = feedbackIdsReached.Dequeue();
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
			std::this_thread::sleep_for(std::chrono::seconds(1));
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

		trackID_t newTrackIdFirst = usedStreet->GetToTrack();
		Track* newTrack = manager->GetTrack(newTrackIdFirst);
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
		direction_t newLocoDirection = static_cast<direction_t>(direction != turnLoco);
		if (turnLoco)
		{
			trackFrom->SetLocoDirection(streetFirst->GetFromDirection());
			manager->TrackPublishState(trackFrom);
		}
		manager->LocoDirection(ControlTypeInternal, this, newLocoDirection);
		newTrack->SetLocoDirection(static_cast<direction_t>(!streetFirst->GetToDirection()));
		logger->Info(Languages::TextHeadingToVia, newTrack->GetName(), streetFirst->GetName());

		// start loco
		manager->TrackPublishState(newTrack);
		locoSpeed_t newSpeed;
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

		trackID_t newTrackIdSecond = usedStreet->GetToTrack();
		Track* newTrack = manager->GetTrack(newTrackIdSecond);
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
		newTrack->SetLocoDirection(static_cast<direction_t>(!streetSecond->GetToDirection()));
		logger->Info(Languages::TextHeadingToViaVia, newTrack->GetName(), streetFirst->GetName(), streetSecond->GetName());

		// start loco
		manager->TrackPublishState(newTrack);
		state = LocoStateRunning;
	}

	Street* Loco::SearchDestination(Track* track, const bool allowLocoTurn)
	{
		if (manager->Booster() == BoosterStop)
		{
			return nullptr;
		}
		logger->Debug(Languages::TextLookingForDestination, track->GetName());
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

		if (track->GetLoco() != objectID)
		{
			state = LocoStateError;
			logger->Error(Languages::TextIsOnOcupiedTrack, name, track->GetName(), manager->GetLocoName(track->GetLoco()));
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

	void Loco::LocationReached(const feedbackID_t feedbackID)
	{
		if (feedbackID == feedbackIdOver)
		{
			manager->LocoSpeed(ControlTypeInternal, this, MinSpeed);
			manager->Booster(ControlTypeInternal, BoosterStop);
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

	void Loco::Speed(const locoSpeed_t speed, const bool withSlaves)
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

	void Loco::SetDirection(const direction_t direction)
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

		locoSpeed_t newSpeed;
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

		trackFrom->Release(logger, objectID);
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

		manager->LocoDestinationReached(objectID, streetFirst->GetID(), trackFrom->GetID());
		streetFirst->Release(logger, objectID);
		streetFirst = nullptr;

		trackFrom->Release(logger, objectID);
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
		while (!slaves.empty())
		{
			delete slaves.back();
			slaves.pop_back();
		}
	}

	bool Loco::AssignSlaves(const std::vector<DataModel::Relation*>& newslaves)
	{
		DeleteSlaves();
		slaves = newslaves;
		return true;
	}
} // namespace DataModel
