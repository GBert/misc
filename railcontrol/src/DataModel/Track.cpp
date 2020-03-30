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
#include <string>

#include "DataModel/Feedback.h"
#include "DataModel/Track.h"
#include "Manager.h"
#include "Utils/Utils.h"

using std::map;
using std::string;
using std::vector;

namespace DataModel
{
	std::string Track::Serialize() const
	{
		std::string feedbackString;
		for (auto feedback : feedbacks)
		{
			if (feedbackString.size() > 0)
			{
				feedbackString += ",";
			}
			feedbackString += std::to_string(feedback);
		}
		std::stringstream ss;
		ss << "objectType=Track;"
			<< LayoutItem::Serialize()
			<< ";" << LockableItem::Serialize()
			<< ";type=" << static_cast<int>(type)
			<< ";feedbacks=" << feedbackString
			<< ";selectstreetapproach=" << static_cast<int>(selectStreetApproach)
			<< ";state=" << static_cast<int>(state)
			<< ";statedelayed=" << static_cast<int>(stateDelayed)
			<< ";locoDirection=" << static_cast<int>(locoDirection)
			<< ";blocked=" << static_cast<int>(blocked)
			<< ";locodelayed=" << static_cast<int>(locoIdDelayed)
			<< ";releasewhenfree=" << static_cast<int>(releaseWhenFree);
		return ss.str();
	}

	bool Track::Deserialize(const std::string& serialized)
	{
		map<string, string> arguments;
		ParseArguments(serialized, arguments);
		LayoutItem::Deserialize(arguments);
		LockableItem::Deserialize(arguments);
		SetWidth(Width1);
		SetVisible(VisibleYes);
		string objectType = Utils::Utils::GetStringMapEntry(arguments, "objectType");
		if (objectType.compare("Track") != 0)
		{
			return false;
		}
		type = static_cast<type_t>(Utils::Utils::GetIntegerMapEntry(arguments, "type", TrackTypeStraight));
		switch (type)
		{
			case TrackTypeTurn:
			case TrackTypeTunnelEnd:
				SetHeight(Height1);
				break;

			default:
				break;
		}
		string feedbackStrings = Utils::Utils::GetStringMapEntry(arguments, "feedbacks");
		vector<string> feedbackStringVector;
		Utils::Utils::SplitString(feedbackStrings, ",", feedbackStringVector);
		for (auto feedbackString : feedbackStringVector)
		{
			feedbackID_t feedbackID = Utils::Utils::StringToInteger(feedbackString);
			if (!manager->FeedbackExists(feedbackID))
			{
				continue;
			}
			feedbacks.push_back(feedbackID);
		}
		selectStreetApproach = static_cast<selectStreetApproach_t>(Utils::Utils::GetIntegerMapEntry(arguments, "selectstreetapproach", SelectStreetSystemDefault));
		state = static_cast<DataModel::Feedback::feedbackState_t>(Utils::Utils::GetBoolMapEntry(arguments, "state", DataModel::Feedback::FeedbackStateFree));
		stateDelayed = static_cast<DataModel::Feedback::feedbackState_t>(Utils::Utils::GetBoolMapEntry(arguments, "statedelayed", state));
		locoDirection = static_cast<direction_t>(Utils::Utils::GetBoolMapEntry(arguments, "locoDirection", DirectionRight));
		blocked = Utils::Utils::GetBoolMapEntry(arguments, "blocked", false);
		locoIdDelayed = static_cast<locoID_t>(Utils::Utils::GetIntegerMapEntry(arguments, "locodelayed", GetLoco()));
		releaseWhenFree = Utils::Utils::GetBoolMapEntry(arguments, "releasewhenfree", false);
		return true;
	}

	bool Track::Reserve(Logger::Logger* logger, const locoID_t locoID)
	{
		std::lock_guard<std::mutex> Guard(updateMutex);
		if (this->locoIdDelayed != LocoNone && this->locoIdDelayed != locoID)
		{
			logger->Debug(Languages::TextTrackIsInUse, GetName());
			return false;
		}
		if (blocked == true)
		{
			logger->Debug(Languages::TextTrackStatusIsBlocked, GetName());
			return false;
		}
		if (state != DataModel::Feedback::FeedbackStateFree)
		{
			logger->Debug(Languages::TextIsNotFree, GetName());
			return false;
		}
		return ReserveForce(logger, locoID);
	}

	bool Track::ReserveForce(Logger::Logger* logger, const locoID_t locoID)
	{
		bool ret = LockableItem::Reserve(logger, locoID);
		if (ret == false)
		{
			return false;
		}
		this->locoIdDelayed = locoID;
		return true;
	}

	bool Track::Lock(Logger::Logger* logger, const locoID_t locoID)
	{
		bool ret = LockableItem::Lock(logger, locoID);
		if (ret)
		{
			manager->TrackPublishState(this);
		}
		return ret;
	}

	bool Track::Release(Logger::Logger* logger, const locoID_t locoID)
	{
		{
			std::lock_guard<std::mutex> Guard(updateMutex);
			bool ret = LockableItem::Release(logger, locoID);
			if (ret == false)
			{
				return false;
			}
			if (state != DataModel::Feedback::FeedbackStateFree)
			{
				return true;
			}
			this->locoIdDelayed = LocoNone;
			this->stateDelayed = DataModel::Feedback::FeedbackStateFree;
		}
		manager->TrackPublishState(this);
		return true;
	}

	bool Track::ReleaseForce(Logger::Logger* logger, const locoID_t locoID)
	{
		bool ret;
		{
			std::lock_guard<std::mutex> Guard(updateMutex);
			ret = ReleaseForceUnlocked(logger, locoID);
		}
		manager->TrackPublishState(this);
		return ret;
	}

	bool Track::ReleaseForceUnlocked(Logger::Logger* logger, const locoID_t locoID)
	{
		bool ret = LockableItem::Release(logger, locoID);
		this->state = DataModel::Feedback::FeedbackStateFree;
		this->locoIdDelayed = LocoNone;
		this->stateDelayed = DataModel::Feedback::FeedbackStateFree;
		return ret;
	}

	bool Track::SetFeedbackState(const feedbackID_t feedbackID, const DataModel::Feedback::feedbackState_t newState)
	{
		{
			std::lock_guard<std::mutex> Guard(updateMutex);
			DataModel::Feedback::feedbackState_t oldState = this->state;
			bool oldBlocked = blocked;
			bool ret = FeedbackStateInternal(feedbackID, newState);
			if (ret == false)
			{
				return false;
			}
			if (oldState == newState && oldBlocked == blocked)
			{
				return true;
			}
		}
		manager->TrackPublishState(this);
		return true;
	}

	bool Track::FeedbackStateInternal(const feedbackID_t feedbackID, const DataModel::Feedback::feedbackState_t newState)
	{
		if (newState == DataModel::Feedback::FeedbackStateOccupied)
		{
			Loco* loco = manager->GetLoco(GetLocoDelayed());
			if (loco == nullptr)
			{
				if (blocked == false)
				{
					manager->Booster(ControlTypeInternal, BoosterStop);
					blocked = true;
				}
			}
			else
			{
				loco->LocationReached(feedbackID);
			}

			this->state = newState;
			this->stateDelayed = newState;
			return true;
		}

		for (auto feedbackID : feedbacks)
		{
			DataModel::Feedback* feedback = manager->GetFeedbackUnlocked(feedbackID);
			if (feedback == nullptr)
			{
				continue;
			}
			if (feedback->GetState() != DataModel::Feedback::FeedbackStateFree)
			{
				return false;
			}
		}
		this->state = DataModel::Feedback::FeedbackStateFree;

		if (releaseWhenFree)
		{
			locoID_t locoID = GetLoco();
			Loco* loco = manager->GetLoco(locoID);
			if (loco != nullptr && loco->IsRunningFromTrack(GetID()))
			{
				bool ret = ReleaseForceUnlocked(loco->GetLogger(), locoID);
				manager->TrackPublishState(this);
				return ret;
			}
		}

		if (this->GetLoco() != LocoNone)
		{
			return true;
		}

		this->stateDelayed = DataModel::Feedback::FeedbackStateFree;
		this->locoIdDelayed = LocoNone;
		return true;
	}

	bool Track::AddStreet(Street* street)
	{
		std::lock_guard<std::mutex> Guard(updateMutex);
		for (auto s : streets)
		{
			if (s == street)
			{
				return false;
			}
		}
		streets.push_back(street);
		return true;
	}

	bool Track::RemoveStreet(Street* street)
	{
		std::lock_guard<std::mutex> Guard(updateMutex);
		size_t sizeBefore = streets.size();
		streets.erase(std::remove(streets.begin(), streets.end(), street), streets.end());
		size_t sizeAfter = streets.size();
		return sizeBefore > sizeAfter;
	}

	Track::selectStreetApproach_t Track::GetSelectStreetApproachCalculated() const
	{
		if (selectStreetApproach == SelectStreetSystemDefault)
		{
			return manager->GetSelectStreetApproach();
		}
		return selectStreetApproach;
	}

	bool Track::GetValidStreets(Logger::Logger* logger, const Loco* loco, const bool allowLocoTurn, std::vector<Street*>& validStreets) const
	{
		std::lock_guard<std::mutex> Guard(updateMutex);
		for (auto street : streets)
		{
			if (street->FromTrackDirection(logger, objectID, locoDirection, loco, allowLocoTurn))
			{
				validStreets.push_back(street);
			}
		}
		OrderValidStreets(validStreets);
		return true;
	}

	void Track::OrderValidStreets(vector<Street*>& validStreets) const
	{
		switch (GetSelectStreetApproachCalculated())
		{

			case Track::SelectStreetRandom:
				std::random_shuffle(validStreets.begin(), validStreets.end());
				break;

			case Track::SelectStreetMinTrackLength:
				std::sort(validStreets.begin(), validStreets.end(), Street::CompareShortest);
				break;

			case Track::SelectStreetLongestUnused:
				std::sort(validStreets.begin(), validStreets.end(), Street::CompareLastUsed);
				break;

			case Track::SelectStreetDoNotCare:
			default:
				// do nothing
				break;
		}
	}
} // namespace DataModel
