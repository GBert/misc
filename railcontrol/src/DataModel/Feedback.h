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

#pragma once

#include <mutex>
#include <string>

#include "DataTypes.h"
#include "DataModel/ObjectIdentifier.h"
#include "DataModel/LayoutItem.h"
#include "Languages.h"

class Manager;

namespace DataModel
{
	class TrackBase;

	class Feedback : public LayoutItem
	{
		public:
			enum FeedbackState : bool
			{
				FeedbackStateFree = false,
				FeedbackStateOccupied = true
			};

			inline Feedback(Manager* manager,
				const FeedbackID feedbackID)
			:	LayoutItem(feedbackID),
			 	controlID(ControlIdNone),
			 	pin(FeedbackPinNone),
			 	manager(manager),
			 	inverted(false),
			 	relatedObject(),
			 	track(nullptr),
				stateCounter(0)
			{
			}

			inline Feedback(Manager* manager, const std::string& serialized)
			:	manager(manager),
				track(nullptr)
			{
				Deserialize(serialized);
			}

			inline ObjectType GetObjectType() const override
			{
				return ObjectTypeFeedback;
			}

			std::string Serialize() const override;

			bool Deserialize(const std::string& serialized) override;

			inline std::string GetLayoutType() const override
			{
				return Languages::GetText(Languages::TextFeedback);
			}

			inline void SetInverted(const bool inverted)
			{
				this->inverted = inverted;
			}

			inline bool GetInverted() const
			{
				return inverted;
			}

			void SetState(const FeedbackState state);

			inline FeedbackState GetState() const
			{
				return static_cast<FeedbackState>(stateCounter > 0);
			}

			void Debounce();

			inline void SetControlID(const ControlID controlID)
			{
				this->controlID = controlID;
			}

			inline ControlID GetControlID() const
			{
				return controlID;
			}

			inline void SetPin(const FeedbackPin pin)
			{
				this->pin = pin;
			}

			inline FeedbackPin GetPin() const
			{
				return pin;
			}

			inline void ClearRelatedObject()
			{
				relatedObject.Clear();
				track = nullptr;
			}

			inline bool IsRelatedObjectSet() const
			{
				return relatedObject.IsSet();
			}

			inline void SetRelatedObject(const ObjectIdentifier& relatedObject)
			{
				this->relatedObject = relatedObject;
				track = nullptr;
			}

			inline ObjectIdentifier GetRelatedObject() const
			{
				return relatedObject;
			}

			inline bool CompareRelatedObject(const ObjectIdentifier& compare) const
			{
				return relatedObject == compare;
			}

			inline TrackBase* GetTrack()
			{
				UpdateTrack();
				return track;
			}

		private:
			void UpdateTrack();
			void UpdateTrackState(const FeedbackState state);

			ControlID controlID;
			FeedbackPin pin;

			Manager* manager;
			bool inverted;
			ObjectIdentifier relatedObject;
			TrackBase* track;
			unsigned char stateCounter;
			static const unsigned char MaxStateCounter = 10;
			mutable std::mutex updateMutex;
	};

} // namespace DataModel

