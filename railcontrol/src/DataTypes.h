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

#include <string>

// common
typedef uint8_t ControlID;

// objects in db
typedef uint16_t ObjectID;

// loco
typedef ObjectID LocoID;
typedef uint16_t Address;
typedef uint16_t Speed;
typedef uint32_t Length;

// accessory
typedef ObjectID AccessoryID;

// feedback
typedef ObjectID FeedbackID;
typedef uint32_t FeedbackPin;

// track
typedef ObjectID TrackID;
typedef ObjectID ClusterID;

// switch
typedef AccessoryID SwitchID;

// signal
typedef AccessoryID SignalID;

// route
typedef ObjectID RouteID;
typedef uint16_t Delay;

// layer
typedef int16_t LayerID;

// relations
typedef uint16_t Priority;

typedef uint8_t Pause;


static const Address AddressNone = 0;
static const Address AddressDefault = 3;
static const LocoID LocoNone = 0;
static const ObjectID ObjectNone = 0;
static const AccessoryID AccessoryNone = 0;
static const FeedbackID FeedbackNone = 0;
static const FeedbackPin FeedbackPinNone = 0;
static const TrackID TrackNone = 0;
static const SwitchID SwitchNone = 0;
static const RouteID RouteNone = 0;
static const ControlID ControlNone = 0;
static const LayerID LayerNone = 0;
static const LayerID LayerUndeletable = 1;
static const SignalID SignalNone = 0;
static const ClusterID ClusterNone = 0;

static const Speed MaxSpeed = 1023;
static const Speed DefaultTravelSpeed = 700;
static const Speed DefaultReducedSpeed = 400;
static const Speed DefaultCreepingSpeed = 100;
static const Speed MinSpeed = 0;

enum ControlType : uint8_t
{
	ControlTypeHardware = 0,
	ControlTypeInternal,
	ControlTypeWebserver
};

enum ControlIDs : ControlID
{
	ControlIdNone = 0,
	ControlIdWebserver,
	ControlIdFirstHardware = 10
};

enum BoosterState : bool
{
	BoosterStateStop = false,
	BoosterStateGo = true
};

enum Protocol : uint8_t
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
	ProtocolFMZ = 13,
	ProtocolEnd = ProtocolFMZ
};

static const std::string ProtocolSymbols[] =
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
	"SX2",
	"FMZ"
};

enum AddressType : uint8_t
{
	AddressTypeLoco = 0,
	AddressTypeAccessory
};

enum ArgumentType : uint8_t
{
	ArgumentTypeIpAddress = 1,
	ArgumentTypeSerialPort = 2,
	ArgumentTypeS88Modules = 3
};

enum HardwareType : uint8_t
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
	HardwareTypeIntellibox = 11,
	HardwareTypeMasterControl = 12,
	HardwareTypeTwinCenter = 13
};

enum Automode : bool
{
	AutomodeNo = false,
	AutomodeYes = true
};

enum ObjectType : uint8_t
{
	ObjectTypeNone = 0,
	ObjectTypeLoco = 1,
	ObjectTypeTrack = 2,
	ObjectTypeFeedback = 3,
	ObjectTypeAccessory = 4,
	ObjectTypeSwitch = 5,
	ObjectTypeRoute = 6,
	ObjectTypeLayer = 7,
	ObjectTypeSignal = 8,
	ObjectTypeCluster = 9,
	ObjectTypeTimeTable = 10
};

enum Orientation : bool
{
	OrientationLeft = false,
	OrientationRight = true
};

enum ProgramMode : uint8_t
{
	ProgramModeNone,
	ProgramModeMm,
	ProgramModeMmPom,
	ProgramModeMfx,
	ProgramModeDccRegister,
	ProgramModeDccDirect,
	ProgramModeDccPomLoco,
	ProgramModeDccPomAccessory
};

typedef uint16_t CvNumber;
typedef uint8_t CvValue;
