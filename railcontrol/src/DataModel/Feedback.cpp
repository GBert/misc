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
#include <sstream>

#include "DataModel/Feedback.h"
#include "Manager.h"
#include "Utils/Utils.h"

using std::map;
using std::stringstream;
using std::string;

namespace DataModel
{
	std::string Feedback::Serialize() const
	{
		stringstream ss;
		ss << "objectType=Feedback;" << LayoutItem::Serialize()
			<< ";controlID=" << static_cast<int>(controlID)
			<< ";pin=" << static_cast<int>(pin)
			<< ";inverted=" << static_cast<int>(inverted)
			<< ";state=" << static_cast<int>(stateCounter > 0)
			<< ";track=" << static_cast<int>(trackID);
		return ss.str();
	}

	bool Feedback::Deserialize(const std::string& serialized)
	{
		map<string, string> arguments;
		ParseArguments(serialized, arguments);
		string objectType = Utils::Utils::GetStringMapEntry(arguments, "objectType");
		if (objectType.compare("Feedback") != 0)
		{
			return false;
		}
		LayoutItem::Deserialize(arguments);
		SetRotation(Rotation0);
		SetHeight(Height1);
		SetWidth(Width1);
		controlID = Utils::Utils::GetIntegerMapEntry(arguments, "controlID", ControlIdNone);
		pin = Utils::Utils::GetIntegerMapEntry(arguments, "pin");
		inverted = Utils::Utils::GetBoolMapEntry(arguments, "inverted", false);
		stateCounter = Utils::Utils::GetBoolMapEntry(arguments, "state", FeedbackStateFree) ? MaxStateCounter : 0;
		trackID = static_cast<trackID_t>(Utils::Utils::GetIntegerMapEntry(arguments, "track", TrackNone));
		return true;
	}

	void Feedback::SetState(const feedbackState_t newState)
	{
		feedbackState_t state = static_cast<feedbackState_t>(newState != inverted);
		{
			std::lock_guard<std::mutex> Guard(updateMutex);
			if (state == FeedbackStateFree)
			{
				if (stateCounter < MaxStateCounter)
				{
					return;
				}
				stateCounter = MaxStateCounter - 1;
				return;
			}

			unsigned char oldStateCounter = stateCounter;
			stateCounter = MaxStateCounter;

			if (oldStateCounter > 0)
			{
				return;
			}
		}

		manager->FeedbackPublishState(this);
		UpdateTrackState(FeedbackStateOccupied);
	}

	void Feedback::UpdateTrackState(const feedbackState_t state)
	{
		Track* track = manager->GetTrack(trackID);
		if (track == nullptr)
		{
			return;
		}
		track->SetFeedbackState(objectID, state);
		return;
	}

	void Feedback::Debounce()
	{
		{
			std::lock_guard<std::mutex> Guard(updateMutex);
			if (stateCounter == MaxStateCounter || stateCounter == 0)
			{
				return;
			}

			--stateCounter;
			if (stateCounter != 0)
			{
				return;
			}
		}
		manager->FeedbackPublishState(this);
		UpdateTrackState(FeedbackStateFree);
	}
} // namespace DataModel

