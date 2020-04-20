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

#include <string>

// common
typedef unsigned char controlID_t;

// objects in db
typedef unsigned short objectID_t;

// loco
typedef objectID_t locoID_t;
typedef unsigned short address_t;
typedef unsigned short locoSpeed_t;
typedef unsigned char function_t;
typedef unsigned int length_t;

// layoutItem
typedef unsigned char layoutItemSize_t;
typedef char layoutPosition_t;

// accessory
typedef objectID_t accessoryID_t;
typedef unsigned char accessoryType_t;
typedef bool accessoryState_t;
typedef unsigned short accessoryDuration_t;
typedef unsigned long waitTime_t;

// feedback
typedef objectID_t feedbackID_t;
typedef unsigned int feedbackPin_t;

// track
typedef objectID_t trackID_t;

// switch
typedef accessoryID_t switchID_t;
typedef accessoryDuration_t switchDuration_t;
typedef accessoryState_t switchState_t;
typedef accessoryType_t switchType_t;

// signal
typedef accessoryID_t signalID_t;
typedef accessoryDuration_t signalDuration_t;
typedef accessoryState_t signalState_t;
typedef accessoryType_t signalType_t;

// street
typedef objectID_t streetID_t;
typedef unsigned short delay_t;

// layer
typedef signed short layerID_t;

// relations
typedef unsigned short priority_t;

typedef unsigned char wait_t;

static const address_t AddressNone = 0;
static const locoID_t LocoNone = 0;
static const objectID_t ObjectNone = 0;
static const accessoryID_t AccessoryNone = 0;
static const feedbackID_t FeedbackNone = 0;
static const feedbackPin_t FeedbackPinNone = 0;
static const trackID_t TrackNone = 0;
static const switchID_t SwitchNone = 0;
static const streetID_t StreetNone = 0;
static const controlID_t ControlNone = 0;
static const layerID_t LayerNone = 0;
static const layerID_t LayerUndeletable = 1;
static const signalID_t SignalNone = 0;

static const locoSpeed_t MaxSpeed = 1023;
static const locoSpeed_t DefaultTravelSpeed = 700;
static const locoSpeed_t DefaultReducedSpeed = 400;
static const locoSpeed_t DefaultCreepingSpeed = 100;
static const locoSpeed_t MinSpeed = 0;

static const accessoryDuration_t DefaultAccessoryDuration = 100;

enum controlType_t : unsigned char
{
	ControlTypeHardware = 0,
	ControlTypeInternal,
	ControlTypeWebserver
};

enum controlIDs : controlID_t
{
	ControlIdNone = 0,
	ControlIdWebserver,
	ControlIdFirstHardware = 10
};

enum boosterState_t : bool
{
	BoosterStop = false,
	BoosterGo = true
};

enum protocol_t : unsigned char
{
	ProtocolNone = 0,
	ProtocolServer = 1,
	ProtocolMM1 = 2,
	ProtocolMM2 = 3,
	ProtocolMFX = 4,
	ProtocolDCC = 5,
	ProtocolDCC14 = 6,
	ProtocolDCC28 = 7,
	ProtocolDCC128 = 8,
	ProtocolMM = 9,
	ProtocolMM15 = 10,
	ProtocolSX1 = 11,
	ProtocolSX2 = 12,
	ProtocolEnd = ProtocolSX2
};

static const std::string protocolSymbols[] =
{
	"none",
	"all",
	"MM 1",
	"MM 2",
	"mfx",
	"DCC",
	"DCC 14",
	"DCC 28",
	"DCC 128",
	"MM",
	"MM 1.5",
	"SX1",
	"SX2"
};

enum addressType_t : unsigned char
{
	AddressTypeLoco = 0,
	AddressTypeAccessory
};

enum argumentType_t : unsigned char
{
	IpAddress = 1,
	SerialPort = 2,
	S88Modules = 3
};

enum hardwareType_t : unsigned char
{
	HardwareTypeNone = 0,
	HardwareTypeVirtual = 1,
	HardwareTypeCS2Udp = 2,
	HardwareTypeM6051 = 3,
	HardwareTypeRM485 = 4,
	HardwareTypeOpenDcc = 5,
	HardwareTypeHsi88 = 6,
	HardwareTypeZ21 = 7,
	HardwareTypeCcSchnitte = 8,
	HardwareTypeEcos = 9,
	HardwareTypeCS2Tcp = 10,
	HardwareTypeNumbers
};

enum visible_t : bool
{
	VisibleNo = false,
	VisibleYes = true
};

enum automode_t : bool
{
	AutomodeNo = false,
	AutomodeYes = true
};

enum objectType_t : unsigned char
{
	ObjectTypeLoco = 1,
	ObjectTypeTrack = 2,
	ObjectTypeFeedback = 3,
	ObjectTypeAccessory = 4,
	ObjectTypeSwitch = 5,
	ObjectTypeStreet = 6,
	ObjectTypeLayer = 7,
	ObjectTypeSignal = 8
};

enum direction_t : bool
{
	DirectionLeft = false,
	DirectionRight = true
};

enum ProgramMode : uint8_t
{
	ProgramModeMm,
	ProgramModeMmPom,
	ProgramModeMfx,
	ProgramModeDccDirect,
	ProgramModeDccPomLoco,
	ProgramModeDccPomAccessory
};

typedef uint16_t CvNumber;
typedef uint8_t CvValue;
