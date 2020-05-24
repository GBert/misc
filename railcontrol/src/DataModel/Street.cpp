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

#include "DataModel/Loco.h"
#include "DataModel/Relation.h"
#include "DataModel/Street.h"
#include "Manager.h"
#include "Utils/Utils.h"

using std::map;
using std::string;
using std::to_string;

namespace DataModel
{
	Street::Street(Manager* manager, const std::string& serialized)
	:	LockableItem(),
	 	manager(manager),
	 	executeAtUnlock(false)
	{
		Deserialize(serialized);
		TrackBase* track = manager->GetTrackBase(fromTrack);
		if (track == nullptr)
		{
			return;
		}
		track->AddStreet(this);
	}

	std::string Street::Serialize() const
	{
		std::string str;
		str = "objectType=Street;";
		str += LayoutItem::Serialize();
		str += ";" + LockableItem::Serialize();
		str += ";delay=" + to_string(delay);
		str += ";lastused=" + to_string(lastUsed);
		str += ";counter=" + to_string(counter);
		str += ";automode=" + to_string(automode);
		if (automode == AutomodeNo)
		{
			return str;
		}
		str += ";fromTrack=";
		str += fromTrack;
		str += ";fromDirection=" + to_string(fromDirection);
		str += ";toTrack=";
		str += toTrack;
		str += ";toDirection=" + string(toDirection == DirectionLeft ? "left" : "right"); // FIXME: change later to int (like fromDirection)
		str += ";speed=" + to_string(speed);
		str += ";feedbackIdReduced=" + to_string(feedbackIdReduced);
		str += ";feedbackIdCreep=" + to_string(feedbackIdCreep);
		str += ";feedbackIdStop=" + to_string(feedbackIdStop);
		str += ";feedbackIdOver=" + to_string(feedbackIdOver);
		str += ";pushpull=" + to_string(pushpull);
		str += ";mintrainlength=" + to_string(minTrainLength);
		str += ";maxtrainlength=" + to_string(maxTrainLength);
		str += ";waitafterrelease=" + to_string(waitAfterRelease);
		return str;
	}

	bool Street::Deserialize(const std::string& serialized)
	{
		map<string,string> arguments;
		ParseArguments(serialized, arguments);
		string objectType = Utils::Utils::GetStringMapEntry(arguments, "objectType");
		if (objectType.compare("Street") != 0)
		{
			return false;
		}

		LayoutItem::Deserialize(arguments);
		LockableItem::Deserialize(arguments);

		delay = static_cast<Delay>(Utils::Utils::GetIntegerMapEntry(arguments, "delay", DefaultDelay));
		lastUsed = Utils::Utils::GetIntegerMapEntry(arguments, "lastused", 0);
		counter = Utils::Utils::GetIntegerMapEntry(arguments, "counter", 0);
		automode = static_cast<Automode>(Utils::Utils::GetBoolMapEntry(arguments, "automode", AutomodeNo));
		if (automode == AutomodeNo)
		{
			fromTrack = TrackNone;
			fromDirection = DirectionRight;
			toTrack = TrackNone;
			toDirection = DirectionLeft;
			speed = SpeedTravel;
			feedbackIdReduced = FeedbackNone;
			feedbackIdCreep = FeedbackNone;
			feedbackIdStop = FeedbackNone;
			feedbackIdOver = FeedbackNone;
			pushpull = PushpullTypeBoth;
			minTrainLength = 0;
			maxTrainLength = 0;
			waitAfterRelease = 0;
			return true;
		}
		fromTrack = Utils::Utils::GetStringMapEntry(arguments, "fromTrack");
		fromDirection = static_cast<Direction>(Utils::Utils::GetBoolMapEntry(arguments, "fromDirection", DirectionRight));
		toTrack = Utils::Utils::GetStringMapEntry(arguments, "toTrack");
		std::string directionString = Utils::Utils::GetStringMapEntry(arguments, "toDirection");
		if (directionString.compare("left") == 0)
		{
			toDirection = DirectionLeft;
		}
		else if (directionString.compare("right") == 0)
		{
			toDirection = DirectionRight;
		}
		else if (directionString.compare("0") == 0)
		{
			toDirection = DirectionRight; // FIXME: change later to left and serialize with int
		}
		else if (directionString.compare("1") == 0)
		{
			toDirection = DirectionLeft; // FIXME: change later to right and serialize with int
		}
		else
		{
			toDirection = DirectionRight;
		}
		speed = static_cast<Speed>(Utils::Utils::GetIntegerMapEntry(arguments, "speed", SpeedTravel));
		feedbackIdReduced = Utils::Utils::GetIntegerMapEntry(arguments, "feedbackIdReduced", FeedbackNone);
		feedbackIdCreep = Utils::Utils::GetIntegerMapEntry(arguments, "feedbackIdCreep", FeedbackNone);
		feedbackIdStop = Utils::Utils::GetIntegerMapEntry(arguments, "feedbackIdStop", FeedbackNone);
		feedbackIdOver = Utils::Utils::GetIntegerMapEntry(arguments, "feedbackIdOver", FeedbackNone);
		pushpull = static_cast<PushpullType>(Utils::Utils::GetIntegerMapEntry(arguments, "commuter", PushpullTypeBoth)); // FIXME: remove later
		pushpull = static_cast<PushpullType>(Utils::Utils::GetIntegerMapEntry(arguments, "pushpull", pushpull));
		minTrainLength = static_cast<Length>(Utils::Utils::GetIntegerMapEntry(arguments, "mintrainlength", 0));
		maxTrainLength = static_cast<Length>(Utils::Utils::GetIntegerMapEntry(arguments, "maxtrainlength", 0));
		waitAfterRelease = Utils::Utils::GetIntegerMapEntry(arguments, "waitafterrelease", 0);
		return true;
	}

	void Street::DeleteRelations(std::vector<DataModel::Relation*>& relations)
	{
		while (!relations.empty())
		{
			delete relations.back();
			relations.pop_back();
		}
	}

	bool Street::AssignRelations(std::vector<DataModel::Relation*>& relations, const std::vector<DataModel::Relation*>& newRelations)
	{
		std::lock_guard<std::mutex> Guard(updateMutex);
		if (GetLockState() != LockStateFree)
		{
			return false;
		}
		DeleteRelations(relations);
		relations = newRelations;
		for (auto relation : relations)
		{
			relation->ObjectID1(objectID); // FIXME: remove later. In older versions the objectID has always been stored with value 0
		}
		return true;
	}

	bool Street::FromTrackDirection(Logger::Logger* logger, const ObjectIdentifier& identifier, const Direction trackDirection, const Loco* loco, const bool allowLocoTurn)
	{
		if (automode == false)
		{
			return false;
		}

		if (fromTrack != identifier)
		{
			return false;
		}

		const Length locoLength = loco->GetLength();
		if (locoLength < minTrainLength)
		{
			logger->Debug(Languages::TextTrainIsToShort, GetName());
			return false;
		}
		if (maxTrainLength > 0 && locoLength > maxTrainLength)
		{
			logger->Debug(Languages::TextTrainIsToLong, GetName());
			return false;
		}

		const bool locoPushpull = loco->GetPushpull();
		if (pushpull != locoPushpull && pushpull != PushpullTypeBoth)
		{
			logger->Debug(Languages::TextDifferentPushpullTypes, GetName());
			return false;
		}

		if (allowLocoTurn == true && locoPushpull == true)
		{
			return true;
		}

		bool equalDirection = (fromDirection == trackDirection);
		if (equalDirection)
		{
			return true;
		}

		logger->Debug(Languages::TextDifferentDirections, GetName());
		return false;
	}


	bool Street::Execute(Logger::Logger* logger, const LocoID locoID)
	{
		bool isInUse = IsInUse();
		if (isInUse && locoID != GetLoco())
		{
			logger->Info(Languages::TextStreetIsLocked, GetName());
			return false;
		}

		if (manager->Booster() == BoosterStateStop)
		{
			logger->Debug(Languages::TextBoosterIsTurnedOff);
			return false;
		}

		std::lock_guard<std::mutex> Guard(updateMutex);
		for (auto relation : relationsAtLock)
		{
			bool retRelation = relation->Execute(logger, locoID, delay);
			if (retRelation == false)
			{
				return false;
			}
		}
		lastUsed = time(nullptr);
		++counter;
		if (isInUse)
		{
			executeAtUnlock = true;
		}

		if (fromTrack.GetObjectType() == ObjectTypeSignal)
		{
			Signal *signal = dynamic_cast<Signal*>(manager->GetTrackBase(fromTrack));
			if (signal != nullptr && signal->GetLocoDirection() == DirectionRight)
			{
				return manager->SignalState(ControlTypeInternal, signal, SignalStateGreen, true);
			}
		}

		return true;
	}

	bool Street::Reserve(Logger::Logger* logger, const LocoID locoID)
	{
		if (manager->Booster() == BoosterStateStop)
		{
			logger->Debug(Languages::TextBoosterIsTurnedOff);
			return false;
		}

		std::lock_guard<std::mutex> Guard(updateMutex);
		bool ret = LockableItem::Reserve(logger, locoID);
		if (ret == false)
		{
			return false;
		}

		if (automode == AutomodeYes)
		{
			TrackBase* track = manager->GetTrackBase(toTrack);
			if (track == nullptr)
			{
				ReleaseInternal(logger, locoID);
				return false;
			}
			if (track->BaseReserve(logger, locoID) == false)
			{
				ReleaseInternal(logger, locoID);
				return false;
			}
		}

		for (auto relation : relationsAtLock)
		{
			bool retRelation = relation->Reserve(logger, locoID);
			if (retRelation == false)
			{
				ReleaseInternalWithToTrack(logger, locoID);
				return false;
			}
		}
		return true;
	}

	bool Street::Lock(Logger::Logger* logger, const LocoID locoID)
	{
		if (manager->Booster() == BoosterStateStop)
		{
			logger->Debug(Languages::TextBoosterIsTurnedOff);
			return false;
		}

		std::lock_guard<std::mutex> Guard(updateMutex);
		bool ret = LockableItem::Lock(logger, locoID);
		if (ret == false)
		{
			return false;
		}

		if (automode == AutomodeYes)
		{
			TrackBase* track = manager->GetTrackBase(toTrack);
			if (track == nullptr)
			{
				ReleaseInternal(logger, locoID);
				return false;
			}
			if (track->BaseLock(logger, locoID) == false)
			{
				ReleaseInternal(logger, locoID);
				return false;
			}
		}

		for (auto relation : relationsAtLock)
		{
			bool retRelation = relation->Lock(logger, locoID);
			if (retRelation == false)
			{
				ReleaseInternalWithToTrack(logger, locoID);
				return false;
			}
		}
		return true;
	}

	bool Street::Release(Logger::Logger* logger, const LocoID locoID)
	{
		std::lock_guard<std::mutex> Guard(updateMutex);
		return ReleaseInternal(logger, locoID);
	}

	bool Street::ReleaseInternal(Logger::Logger* logger, const LocoID locoID)
	{
		if (executeAtUnlock)
		{
			for (auto relation : relationsAtUnlock)
			{
				relation->Execute(logger, locoID, delay);
			}
			executeAtUnlock = false;
		}

		for (auto relation : relationsAtLock)
		{
			relation->Release(logger, locoID);
		}

		if (fromTrack.GetObjectType() == ObjectTypeSignal)
		{
			Signal *signal = dynamic_cast<Signal*>(manager->GetTrackBase(fromTrack));
			if (signal != nullptr && signal->GetLocoDirection() == DirectionRight)
			{
				manager->SignalState(ControlTypeInternal, signal, SignalStateRed, true);
			}
		}

		return LockableItem::Release(logger, locoID);
	}

	void Street::ReleaseInternalWithToTrack(Logger::Logger* logger, const LocoID locoID)
	{
		TrackBase* track = manager->GetTrackBase(toTrack);
		if (track != nullptr)
		{
			track->BaseRelease(logger, locoID);
		}
		ReleaseInternal(logger, locoID);
	}
} // namespace DataModel

