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
#include "DataModel/TrackBase.h"
#include "Manager.h"
#include "Utils/Utils.h"

using std::map;
using std::string;
using std::vector;

namespace DataModel
{
	std::string TrackBase::Serialize() const
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
		ss << "feedbacks=" << feedbackString
			<< ";selectstreetapproach=" << static_cast<int>(selectStreetApproach)
			<< ";trackstate=" << static_cast<int>(trackState)
			<< ";trackstatedelayed=" << static_cast<int>(trackStateDelayed)
			<< ";locoDirection=" << static_cast<int>(locoDirection)
			<< ";blocked=" << static_cast<int>(blocked)
			<< ";locodelayed=" << static_cast<int>(locoIdDelayed)
			<< ";releasewhenfree=" << static_cast<int>(releaseWhenFree);
		return ss.str();
	}

	bool TrackBase::Deserialize(const map<string, string> arguments)
	{
		string feedbackStrings = Utils::Utils::GetStringMapEntry(arguments, "feedbacks");
		vector<string> feedbackStringVector;
		Utils::Utils::SplitString(feedbackStrings, ",", feedbackStringVector);
		for (auto feedbackString : feedbackStringVector)
		{
			FeedbackID feedbackID = Utils::Utils::StringToInteger(feedbackString);
			if (!manager->FeedbackExists(feedbackID))
			{
				continue;
			}
			feedbacks.push_back(feedbackID);
		}
		selectStreetApproach = static_cast<SelectStreetApproach>(Utils::Utils::GetIntegerMapEntry(arguments, "selectstreetapproach", SelectStreetSystemDefault));
		trackState = static_cast<DataModel::Feedback::FeedbackState>(Utils::Utils::GetBoolMapEntry(arguments, "state", DataModel::Feedback::FeedbackStateFree)); // FIXME: remove later
		trackState = static_cast<DataModel::Feedback::FeedbackState>(Utils::Utils::GetBoolMapEntry(arguments, "trackstate", trackState));
		trackStateDelayed = static_cast<DataModel::Feedback::FeedbackState>(Utils::Utils::GetBoolMapEntry(arguments, "statedelayed", trackState)); // FIXME: remove later
		trackStateDelayed = static_cast<DataModel::Feedback::FeedbackState>(Utils::Utils::GetBoolMapEntry(arguments, "trackstatedelayed", trackStateDelayed));
		locoDirection = static_cast<Direction>(Utils::Utils::GetBoolMapEntry(arguments, "locoDirection", DirectionRight));
		blocked = Utils::Utils::GetBoolMapEntry(arguments, "blocked", false);
		locoIdDelayed = static_cast<LocoID>(Utils::Utils::GetIntegerMapEntry(arguments, "locodelayed", GetLockedLoco()));
		releaseWhenFree = Utils::Utils::GetBoolMapEntry(arguments, "releasewhenfree", false);
		return true;
	}

	bool TrackBase::BaseReserve(Logger::Logger* logger, const LocoID locoID)
	{
		std::lock_guard<std::mutex> Guard(updateMutex);
		if (this->locoIdDelayed != LocoNone && this->locoIdDelayed != locoID)
		{
			logger->Debug(Languages::TextTrackIsInUse, GetMyName());
			return false;
		}
		if (blocked == true)
		{
			logger->Debug(Languages::TextTrackStatusIsBlocked, GetMyName());
			return false;
		}
		if (trackState != DataModel::Feedback::FeedbackStateFree)
		{
			logger->Debug(Languages::TextIsNotFree, GetMyName());
			return false;
		}
		return BaseReserveForce(logger, locoID);
	}

	bool TrackBase::BaseReserveForce(Logger::Logger* logger, const LocoID locoID)
	{
		bool ret = ReserveInternal(logger, locoID);
		if (ret == false)
		{
			return false;
		}
		this->locoIdDelayed = locoID;
		return true;
	}

	bool TrackBase::BaseLock(Logger::Logger* logger, const LocoID locoID)
	{
		bool ret = LockInternal(logger, locoID);
		if (ret)
		{
			PublishState();
		}
		return ret;
	}

	bool TrackBase::BaseRelease(Logger::Logger* logger, const LocoID locoID)
	{
		{
			std::lock_guard<std::mutex> Guard(updateMutex);
			bool ret = ReleaseInternal(logger, locoID);
			if (ret == false)
			{
				return false;
			}
			if (trackState != DataModel::Feedback::FeedbackStateFree)
			{
				return true;
			}
			this->locoIdDelayed = LocoNone;
			this->trackStateDelayed = DataModel::Feedback::FeedbackStateFree;
		}
		PublishState();
		return true;
	}

	bool TrackBase::BaseReleaseForce(Logger::Logger* logger, const LocoID locoID)
	{
		bool ret;
		{
			std::lock_guard<std::mutex> Guard(updateMutex);
			ret = BaseReleaseForceUnlocked(logger, locoID);
		}
		PublishState();
		return ret;
	}

	bool TrackBase::BaseReleaseForceUnlocked(Logger::Logger* logger, const LocoID locoID)
	{
		bool ret = ReleaseInternal(logger, locoID);
		this->trackState = DataModel::Feedback::FeedbackStateFree;
		this->locoIdDelayed = LocoNone;
		this->trackStateDelayed = DataModel::Feedback::FeedbackStateFree;
		return ret;
	}

	bool TrackBase::SetFeedbackState(const FeedbackID feedbackID, const DataModel::Feedback::FeedbackState newTrackState)
	{
		{
			std::lock_guard<std::mutex> Guard(updateMutex);
			DataModel::Feedback::FeedbackState oldTrackState = this->trackState;
			bool oldBlocked = blocked;
			bool ret = FeedbackStateInternal(feedbackID, newTrackState);
			if (ret == false)
			{
				return false;
			}
			if (oldTrackState == newTrackState && oldBlocked == blocked)
			{
				return true;
			}
		}
		PublishState();
		return true;
	}

	bool TrackBase::FeedbackStateInternal(const FeedbackID feedbackID, const DataModel::Feedback::FeedbackState newTrackState)
	{
		if (newTrackState == DataModel::Feedback::FeedbackStateOccupied)
		{
			Loco* loco = manager->GetLoco(GetLocoDelayed());
			if (loco == nullptr)
			{
				if (blocked == false)
				{
					manager->Booster(ControlTypeInternal, BoosterStateStop);
					blocked = true;
				}
			}
			else
			{
				loco->LocationReached(feedbackID);
			}

			this->trackState = newTrackState;
			this->trackStateDelayed = newTrackState;
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
		this->trackState = DataModel::Feedback::FeedbackStateFree;

		if (releaseWhenFree)
		{
			LocoID locoID = GetLockedLoco();
			Loco* loco = manager->GetLoco(locoID);
			if (loco != nullptr && loco->IsRunningFromTrack(GetMyID()))
			{
				bool ret = BaseReleaseForceUnlocked(loco->GetLogger(), locoID);
				PublishState();
				return ret;
			}
		}

		if (this->GetLockedLoco() != LocoNone)
		{
			return true;
		}

		this->trackStateDelayed = DataModel::Feedback::FeedbackStateFree;
		this->locoIdDelayed = LocoNone;
		return true;
	}

	bool TrackBase::AddStreet(Street* street)
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

	bool TrackBase::RemoveStreet(Street* street)
	{
		std::lock_guard<std::mutex> Guard(updateMutex);
		size_t sizeBefore = streets.size();
		streets.erase(std::remove(streets.begin(), streets.end(), street), streets.end());
		size_t sizeAfter = streets.size();
		return sizeBefore > sizeAfter;
	}

	SelectStreetApproach TrackBase::GetSelectStreetApproachCalculated() const
	{
		if (selectStreetApproach == SelectStreetSystemDefault)
		{
			return manager->GetSelectStreetApproach();
		}
		return selectStreetApproach;
	}

	bool TrackBase::GetValidStreets(Logger::Logger* logger, const Loco* loco, const bool allowLocoTurn, std::vector<Street*>& validStreets) const
	{
		std::lock_guard<std::mutex> Guard(updateMutex);
		for (auto street : streets)
		{
			if (street->FromTrackDirection(logger, GetObjectIdentifier(), locoDirection, loco, allowLocoTurn))
			{
				validStreets.push_back(street);
			}
		}
		OrderValidStreets(validStreets);
		return true;
	}

	void TrackBase::OrderValidStreets(vector<Street*>& validStreets) const
	{
		switch (GetSelectStreetApproachCalculated())
		{

			case SelectStreetRandom:
				std::random_shuffle(validStreets.begin(), validStreets.end());
				break;

			case SelectStreetMinTrackLength:
				std::sort(validStreets.begin(), validStreets.end(), Street::CompareShortest);
				break;

			case SelectStreetLongestUnused:
				std::sort(validStreets.begin(), validStreets.end(), Street::CompareLastUsed);
				break;

			case SelectStreetDoNotCare:
			default:
				// do nothing
				break;
		}
	}
} // namespace DataModel
