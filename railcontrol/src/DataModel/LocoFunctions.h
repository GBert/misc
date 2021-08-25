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

#include <vector>

#include "DataModel/Serializable.h"

namespace DataModel
{
	typedef uint16_t LocoFunctionNr;
	// 16 bit because icon is added with 256 to select f number and icon in same select dropdown box in relation.

	static const LocoFunctionNr NumberOfLocoFunctions = 33; // f0 - f32 = 33

	enum LocoFunctionState : uint8_t
	{
		LocoFunctionStateOff = 0,
		LocoFunctionStateOn
	};

	enum LocoFunctionType : uint8_t
	{
		LocoFunctionTypeNone = 0,
		LocoFunctionTypePermanent = 1,
		LocoFunctionTypeMoment = 2,
		LocoFunctionTypeFlashing = 3, // actually not implemented
		LocoFunctionTypeTimer = 4 // actually not implemented
	};

	enum LocoFunctionIcon : uint16_t
	// 16 bit because icon is added with 256 to select f number and icon in same select dropdown box in relation.
	{
		// Do not change numbers!
		// Only add numbers!
		// If you add numbers, add them also in
		// - ProtocolMaerklin.cpp
		// - WebServer::WebClient::HtmlTagRelationObject
		// - WebServer::WebClient::HandleLocoEdit
		LocoFunctionIconNone = 0,
		LocoFunctionIconDefault = 1,
		// logical functions
		LocoFunctionIconShuntingMode = 2,
		LocoFunctionIconInertia,
		// light functions
		LocoFunctionIconLight = 32,
		LocoFunctionIconHeadlightLowBeamForward,
		LocoFunctionIconHeadlightLowBeamReverse,
		LocoFunctionIconHeadlightHighBeamForward,
		LocoFunctionIconHeadlightHighBeamReverse,
		LocoFunctionIconBacklightForward,
		LocoFunctionIconBacklightReverse,
		LocoFunctionIconShuntingLight,
		LocoFunctionIconBlinkingLight,
		LocoFunctionIconInteriorLight1,
		LocoFunctionIconInteriorLight2,
		LocoFunctionIconTableLight1,
		LocoFunctionIconTableLight2,
		LocoFunctionIconTableLight3,
		LocoFunctionIconCabLight1,
		LocoFunctionIconCabLight2,
		LocoFunctionIconCabLight12,
		LocoFunctionIconDriversDeskLight,
		LocoFunctionIconTrainDestinationIndicator,
		LocoFunctionIconLocomotiveNumberIndicator,
		LocoFunctionIconEngineLight,
		LocoFunctionIconFireBox,
		LocoFunctionIconStairsLight,
		// mechanical functions
		LocoFunctionIconSmokeGenerator = 64,
		LocoFunctionIconTelex1,
		LocoFunctionIconTelex2,
		LocoFunctionIconTelex12,
		LocoFunctionIconPanto1,
		LocoFunctionIconPanto2,
		LocoFunctionIconPanto12,
		LocoFunctionIconUp,
		LocoFunctionIconDown,
		LocoFunctionIconUpDown1,
		LocoFunctionIconUpDown2,
		LocoFunctionIconLeft,
		LocoFunctionIconRight,
		LocoFunctionIconLeftRight,
		LocoFunctionIconTurnLeft,
		LocoFunctionIconTurnRight,
		LocoFunctionIconTurn,
		LocoFunctionIconCrane,
		LocoFunctionIconMagnet,
		LocoFunctionIconCraneHook,
		LocoFunctionIconFan,
		LocoFunctionIconBreak,
		// sound functions
		LocoFunctionIconNoSound = 96,
		LocoFunctionIconSoundGeneral,
		LocoFunctionIconRunning1,
		LocoFunctionIconRunning2,
		LocoFunctionIconEngine1,
		LocoFunctionIconEngine2,
		LocoFunctionIconBreak1,
		LocoFunctionIconBreak2,
		LocoFunctionIconCurve,
		LocoFunctionIconHorn1,
		LocoFunctionIconHorn2,
		LocoFunctionIconWhistle1,
		LocoFunctionIconWhistle2,
		LocoFunctionIconBell,
		LocoFunctionIconStationAnnouncement1,
		LocoFunctionIconStationAnnouncement2,
		LocoFunctionIconStationAnnouncement3,
		LocoFunctionIconSpeak,
		LocoFunctionIconRadio,
		LocoFunctionIconMusic1,
		LocoFunctionIconMusic2,
		LocoFunctionIconOpenDoor,
		LocoFunctionIconCloseDoor,
		LocoFunctionIconFan1,
		LocoFunctionIconFan2,
		LocoFunctionIconFan3,
		LocoFunctionIconShovelCoal,
		LocoFunctionIconCompressedAir,
		LocoFunctionIconReliefValve,
		LocoFunctionIconSteamBlowOut,
		LocoFunctionIconSteamBlow,
		LocoFunctionIconDrainValve,
		LocoFunctionIconShakingRust,
		LocoFunctionIconAirPump,
		LocoFunctionIconWaterPump,
		LocoFunctionIconBufferPush,
		LocoFunctionIconGenerator,
		LocoFunctionIconGearBox,
		LocoFunctionIconGearUp,
		LocoFunctionIconGearDown,
		LocoFunctionIconFillWater,
		LocoFunctionIconFillDiesel,
		LocoFunctionIconFillGas,
		LocoFunctionIconSand,
		LocoFunctionIconRailJoint,
		LocoFunctionIconCoupler,
		LocoFunctionIconPanto,
		LocoFunctionIconMainSwitch,
		LocoFunctionIconSoundLouder,
		LocoFunctionIconSoundLower,
		LocoFunctionIconNoBreak,

		MaxLocoFunctionIcons
	};

	typedef uint8_t LocoFunctionTimer;

	class LocoFunctionEntry
	{
		public:
			LocoFunctionEntry()
			{
				nr = 0;
				state = LocoFunctionStateOff;
				type = LocoFunctionTypeNone;
				icon = LocoFunctionIconNone;
				timer = 0;
			}

			LocoFunctionNr nr;
			LocoFunctionState state;
			LocoFunctionType type;
			LocoFunctionIcon icon;
			LocoFunctionTimer timer;
	};

	class LocoFunctions : private Serializable
	{
		public:
			LocoFunctions();

			inline LocoFunctions(const std::string& serialized)
			:	LocoFunctions()
			{
				Deserialize(serialized);
			}

			inline void SetFunctionState(const LocoFunctionNr nr, const LocoFunctionState state)
			{
				if (nr >= NumberOfLocoFunctions)
				{
					return;
				}
				entries[nr].state = state;
			}

			inline void SetFunction(const DataModel::LocoFunctionNr nr,
				const DataModel::LocoFunctionType type,
				const DataModel::LocoFunctionIcon icon,
				const DataModel::LocoFunctionTimer timer)
			{
				if (nr >= NumberOfLocoFunctions)
				{
					return;
				}
				LocoFunctionEntry& entry = entries[nr];
				entry.type = type;
				entry.icon = icon;
				entry.timer = timer;
			}

			inline void ClearFunction(const DataModel::LocoFunctionNr nr)
			{
				if (nr >= NumberOfLocoFunctions)
				{
					return;
				}
				LocoFunctionEntry& entry = entries[nr];
				entry.state = LocoFunctionStateOff;
				entry.type = LocoFunctionTypeNone;
				entry.icon = LocoFunctionIconNone;
				entry.timer = 0;
			}

			void ConfigureFunctions(const std::vector<LocoFunctionEntry>& newEntries);

			void SetFunctionStates(const std::vector<LocoFunctionEntry>& newEntries);

			inline LocoFunctionState GetFunctionState(const LocoFunctionNr nr) const
			{
				if (nr >= NumberOfLocoFunctions)
				{
					return LocoFunctionStateOff;
				}
				return entries[nr].state;
			}

			std::vector<LocoFunctionEntry> GetFunctionStates() const;

			inline void GetFunctions(LocoFunctionEntry* out) const
			{
				for (int nr = 0; nr < NumberOfLocoFunctions; ++nr)
				{
					out[nr] = entries[nr];
				}
			}

			std::string Serialize() const override;

			bool Deserialize(const std::string& serialized) override;

			inline LocoFunctionIcon GetFunctionIcon(const LocoFunctionNr nr) const
			{
				if (nr >= NumberOfLocoFunctions)
				{
					return LocoFunctionIconNone;
				}
				return entries[nr].icon;
			}

			static std::string GetLocoFunctionIcon(const LocoFunctionNr nr, const LocoFunctionIcon icon);

		private:
			bool DeserializeNew(const std::string& serialized);

			// FIXME: remove later 2020-10-27
			bool DeserializeOld(const std::string& serialized);

			LocoFunctionEntry entries[NumberOfLocoFunctions];
	};
} // namespace DataModel
