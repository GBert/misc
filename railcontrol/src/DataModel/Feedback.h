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

#include "DataTypes.h"
#include "DataModel/LayoutItem.h"
#include "Languages.h"

class Manager;

namespace DataModel
{
	class Feedback : public LayoutItem
	{
		public:
			enum feedbackState_t : bool
			{
				FeedbackStateFree = false,
				FeedbackStateOccupied = true
			};

			Feedback(Manager* manager,
				const feedbackID_t feedbackID)
			:	LayoutItem(feedbackID),
			 	controlID(ControlIdNone),
			 	pin(0),
			 	manager(manager),
			 	inverted(false),
			 	trackID(TrackNone),
				stateCounter(0)
			{
			}

			Feedback(Manager* manager, const std::string& serialized)
			:	manager(manager)
			{
				Deserialize(serialized);
			}

			objectType_t GetObjectType() const { return ObjectTypeFeedback; }

			std::string Serialize() const override;
			bool Deserialize(const std::string& serialized) override;
			std::string LayoutType() const override { return Languages::GetText(Languages::TextFeedback); };

			void SetInverted(const bool inverted) { this->inverted = inverted; }
			bool GetInverted() const { return inverted; }

			void SetState(const feedbackState_t state);
			feedbackState_t GetState() const { return static_cast<feedbackState_t>(stateCounter > 0); }
			void Debounce();
			void SetControlID(const controlID_t controlID) { this->controlID = controlID; }
			controlID_t GetControlID() const { return controlID; }
			void SetPin(const feedbackPin_t pin) { this->pin = pin; }
			feedbackPin_t GetPin() const { return pin; }
			void SetTrack(const trackID_t trackID) { this->trackID = trackID; }
			trackID_t GetTrack() const { return trackID; }

		private:
			controlID_t controlID;
			feedbackPin_t pin;

			void UpdateTrackState(const feedbackState_t state);

			Manager* manager;
			bool inverted;
			trackID_t trackID;
			unsigned char stateCounter;
			static const unsigned char MaxStateCounter = 10;
			mutable std::mutex updateMutex;
	};

} // namespace DataModel

