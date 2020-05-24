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
typedef unsigned char ControlID;

// objects in db
typedef unsigned short ObjectID;

// loco
typedef ObjectID LocoID;
typedef unsigned short Address;
typedef unsigned short Speed;
typedef unsigned char Function;
typedef unsigned int Length;

// accessory
typedef ObjectID AccessoryID;

// feedback
typedef ObjectID FeedbackID;
typedef unsigned int FeedbackPin;

// track
typedef ObjectID TrackID;

// switch
typedef AccessoryID SwitchID;

// signal
typedef AccessoryID SignalID;

// street
typedef ObjectID StreetID;
typedef unsigned short Delay;

// layer
typedef signed short LayerID;

// relations
typedef unsigned short Priority;

typedef unsigned char Pause;

static const Address AddressNone = 0;
static const LocoID LocoNone = 0;
static const ObjectID ObjectNone = 0;
static const AccessoryID AccessoryNone = 0;
static const FeedbackID FeedbackNone = 0;
static const FeedbackPin FeedbackPinNone = 0;
static const TrackID TrackNone = 0;
static const SwitchID SwitchNone = 0;
static const StreetID StreetNone = 0;
static const ControlID ControlNone = 0;
static const LayerID LayerNone = 0;
static const LayerID LayerUndeletable = 1;
static const SignalID SignalNone = 0;

static const Speed MaxSpeed = 1023;
static const Speed DefaultTravelSpeed = 700;
static const Speed DefaultReducedSpeed = 400;
static const Speed DefaultCreepingSpeed = 100;
static const Speed MinSpeed = 0;

enum ControlType : unsigned char
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

enum Protocol : unsigned char
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
	"SX2"
};

enum AddressType : unsigned char
{
	AddressTypeLoco = 0,
	AddressTypeAccessory
};

enum ArgumentType : unsigned char
{
	ArgumentTypeIpAddress = 1,
	ArgumentTypeSerialPort = 2,
	ArgumentTypeS88Modules = 3
};

enum HardwareType : unsigned char
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

enum Automode : bool
{
	AutomodeNo = false,
	AutomodeYes = true
};

enum ObjectType : unsigned char
{
	ObjectTypeNone = 0,
	ObjectTypeLoco = 1,
	ObjectTypeTrack = 2,
	ObjectTypeFeedback = 3,
	ObjectTypeAccessory = 4,
	ObjectTypeSwitch = 5,
	ObjectTypeStreet = 6,
	ObjectTypeLayer = 7,
	ObjectTypeSignal = 8
};

enum Direction : bool
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
