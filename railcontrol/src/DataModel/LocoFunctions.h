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

#include <vector>

#include "DataModel/Serializable.h"

namespace DataModel
{
	typedef uint8_t LocoFunctionNr;

	static const LocoFunctionNr MaxLocoFunctions = 32;

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
		LocoFunctionTypeFlashing = 3,
		LocoFunctionTypeTimer = 4
	};

	enum LocoFunctionIcon : uint8_t
	{
		// Do not change numbers!
		// Only add numbers!
		// If you add numbers, add them also in ProtocolMaerklin.cpp
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
				if (nr >= MaxCount)
				{
					return;
				}
				entries[nr].state = state;
			}

			inline LocoFunctionState GetFunctionState(const LocoFunctionNr nr) const
			{
				if (nr >= MaxCount)
				{
					return LocoFunctionStateOff;
				}
				return entries[nr].state;
			}

			inline std::vector<LocoFunctionEntry> GetFunctionStates() const
			{
				std::vector<LocoFunctionEntry> out;
				for (LocoFunctionNr i = 0; i < MaxCount; ++i)
				{
					if (entries[i].type == LocoFunctionTypeNone)
					{
						continue;
					}
					out.push_back(entries[i]);
				}
				return out;
			}

			inline const LocoFunctionEntry* GetFunctions() const
			{
				return entries;
			}

			void ConfigureFunctions(const std::vector<LocoFunctionEntry>& newEntries)
			{
				for (LocoFunctionNr nr = 0; nr < MaxCount; ++nr)
				{
					entries[nr].type = LocoFunctionTypeNone;
				}
				for (const LocoFunctionEntry& newEntry : newEntries)
				{
					LocoFunctionNr nr = newEntry.nr;
					entries[nr] = newEntry;
				}
			}

			std::string Serialize() const override;

			bool Deserialize(const std::string& serialized) override;

			static std::string GetLocoFunctionIcon(const LocoFunctionNr nr, const LocoFunctionIcon icon);

		private:
			bool DeserializeNew(__attribute__((unused)) const std::string& serialized);

			// FIXME: remove later 2020-10-27
			bool DeserializeOld(const std::string& serialized);

			static const LocoFunctionNr MaxCount = MaxLocoFunctions + 1; // f0 - f32 = 33
			LocoFunctionEntry entries[MaxCount];
	};
} // namespace DataModel
