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

#ifndef AMALGAMATION
#include <dlfcn.h>              // dl*
#endif

#include "DataModel/Loco.h"
#include "DataTypes.h"
#include "Logger/Logger.h"
#include "Hardware/CS2.h"
#include "Hardware/CcSchnitte.h"
#include "Hardware/Ecos.h"
#include "Hardware/HardwareHandler.h"
#include "Hardware/Hsi88.h"
#include "Hardware/M6051.h"
#include "Hardware/OpenDcc.h"
#include "Hardware/RM485.h"
#include "Hardware/Virtual.h"
#include "Hardware/Z21.h"
#include "Utils/Utils.h"

using std::string;

namespace Hardware
{
	const std::string HardwareHandler::hardwareSymbols[] =
	{
		"none",
		"Virtual",
		"CS2",
		"M6051",
		"RM485",
		"OpenDcc",
		"Hsi88",
		"Z21",
		"CcSchnitte",
		"Ecos"
	};

	void HardwareHandler::Init(const HardwareParams* params)
	{
		this->params = params;
		hardwareType_t type = params->GetHardwareType();

#ifdef AMALGAMATION
		switch(type)
		{
			case HardwareTypeCS2:
				createHardware = (Hardware::HardwareInterface* (*)(const Hardware::HardwareParams*))(&create_CS2);
				destroyHardware = (void (*)(Hardware::HardwareInterface*))(&destroy_CS2);
				break;

			case HardwareTypeVirtual:
				createHardware = (Hardware::HardwareInterface* (*)(const Hardware::HardwareParams*))(&create_Virtual);
				destroyHardware = (void (*)(Hardware::HardwareInterface*))(&destroy_Virtual);
				break;


			case HardwareTypeM6051:
				createHardware = (Hardware::HardwareInterface* (*)(const Hardware::HardwareParams*))(&create_M6051);
				destroyHardware = (void (*)(Hardware::HardwareInterface*))(&destroy_M6051);
				break;

			case HardwareTypeRM485:
				createHardware = (Hardware::HardwareInterface* (*)(const Hardware::HardwareParams*))(&create_RM485);
				destroyHardware = (void (*)(Hardware::HardwareInterface*))(&destroy_RM485);
				break;

			case HardwareTypeOpenDcc:
				createHardware = (Hardware::HardwareInterface* (*)(const Hardware::HardwareParams*))(&create_OpenDcc);
				destroyHardware = (void (*)(Hardware::HardwareInterface*))(&destroy_OpenDcc);
				break;

			case HardwareTypeHsi88:
				createHardware = (Hardware::HardwareInterface* (*)(const Hardware::HardwareParams*))(&create_Hsi88);
				destroyHardware = (void (*)(Hardware::HardwareInterface*))(&destroy_Hsi88);
				break;

			case HardwareTypeZ21:
				createHardware = (Hardware::HardwareInterface* (*)(const Hardware::HardwareParams*))(&create_Z21);
				destroyHardware = (void (*)(Hardware::HardwareInterface*))(&destroy_Z21);
				break;

			case HardwareTypeCcSchnitte:
				createHardware = (Hardware::HardwareInterface* (*)(const Hardware::HardwareParams*))(&create_CcSchnitte);
				destroyHardware = (void (*)(Hardware::HardwareInterface*))(&destroy_CcSchnitte);
				break;

			case HardwareTypeEcos:
				createHardware = (Hardware::HardwareInterface* (*)(const Hardware::HardwareParams*))(&create_Ecos);
				destroyHardware = (void (*)(Hardware::HardwareInterface*))(&destroy_Ecos);
				break;

			default:
				createHardware = nullptr;
				destroyHardware = nullptr;
				break;
		}
#else
		// generate symbol and library names
		char* error;
		const string& symbol = hardwareSymbols[type];

		string moduleName = "Hardware/" + symbol + ".so";

		Logger::Logger* logger = Logger::Logger::GetLogger("HardwareHandler");
		void* dlhandle = manager.HardwareLibraryGet(type);
		if (dlhandle == nullptr)
		{
			// open dynamic library
			dlhandle = dlopen(moduleName.c_str(), RTLD_LAZY);
			if (dlhandle == nullptr)
			{
				logger->Error(Languages::TextCanNotOpenLibrary, moduleName, dlerror());
				return;
			}
			logger->Info(Languages::TextLibraryLoaded, symbol);
			if (!manager.HardwareLibraryAdd(type, dlhandle))
			{
				logger->Error(Languages::TextUnabelToStoreLibraryAddress, moduleName);
				return;
			}
		}

		// look for symbol create_*
		string createSymbol = "create_" + symbol;
		createHardware_t* new_create_hardware = (createHardware_t*)dlsym(dlhandle, createSymbol.c_str());
		error = dlerror();
		if (error)
		{
			logger->Error(Languages::TextUnableToFindSymbol, createSymbol, error);
			return;
		}

		// look for symbol destroy_*
		string destroySymbol = "destroy_" + symbol;
		destroyHardware_t* new_destroy_hardware = (destroyHardware_t*)dlsym(dlhandle, destroySymbol.c_str());
		error = dlerror();
		if (error)
		{
			logger->Error(Languages::TextUnableToFindSymbol, destroySymbol, error);
			return;
		}

		// register  valid symbols
		createHardware = new_create_hardware;
		destroyHardware = new_destroy_hardware;
#endif

		// start control
		if (createHardware != nullptr)
		{
			instance = createHardware(params);
		}
	}

	void HardwareHandler::Close()
	{
		Hardware::HardwareInterface* instanceTemp = instance;
		instance = nullptr;
		createHardware = nullptr;
		if (instanceTemp != nullptr)
		{
			destroyHardware(instanceTemp);
		}
		destroyHardware = nullptr;

#ifndef AMALGAMATION
		hardwareType_t type = params->GetHardwareType();
		params = nullptr;
		// close library
		if (manager.ControlsOfHardwareType(type) > 1)
		{
			return;
		}
		void* dlhandle = manager.HardwareLibraryGet(type);
		if (dlhandle == nullptr)
		{
			return;
		}
		if (manager.HardwareLibraryRemove(type) == false)
		{
			return;
		}
		dlclose(dlhandle);
#else
		params = nullptr;
#endif
	}

	const std::string HardwareHandler::GetName() const
	{
		if (instance == nullptr)
		{
			return "Unknown, not running";
		}
		return instance->GetName();
	}

	bool HardwareHandler::CanHandleLocos() const
	{
		if (instance == nullptr)
		{
			return false;
		}

		return instance->CanHandleLocos();
	}

	bool HardwareHandler::CanHandleAccessories() const
	{
		if (instance == nullptr)
		{
			return false;
		}

		return instance->CanHandleAccessories();
	}

	bool HardwareHandler::CanHandleFeedbacks() const
	{
		if (instance == nullptr)
		{
			return false;
		}

		return instance->CanHandleFeedback();
	}

	void HardwareHandler::LocoProtocols(std::vector<protocol_t>& protocols) const
	{
		if (instance == nullptr)
		{
			protocols.push_back(ProtocolNone);
			return;
		}

		instance->GetLocoProtocols(protocols);
	}

	bool HardwareHandler::LocoProtocolSupported(protocol_t protocol) const
	{
		if (instance == nullptr)
		{
			return false;
		}
		return instance->LocoProtocolSupported(protocol);
	}

	void HardwareHandler::AccessoryProtocols(std::vector<protocol_t>& protocols) const
	{
		if (instance == nullptr)
		{
			protocols.push_back(ProtocolNone);
			return;
		}

		instance->GetAccessoryProtocols(protocols);
	}

	bool HardwareHandler::AccessoryProtocolSupported(protocol_t protocol) const
	{
		if (instance == nullptr)
		{
			return false;
		}
		return instance->AccessoryProtocolSupported(protocol);
	}

	void HardwareHandler::Booster(const controlType_t controlType, const boosterState_t status)
	{
		if (controlType == ControlTypeHardware || instance == nullptr)
		{
			return;
		}
		instance->Booster(status);
	}

	void HardwareHandler::LocoSpeed(const controlType_t controlType, const DataModel::Loco* loco, const locoSpeed_t speed)
	{
		if (controlType == ControlTypeHardware || instance == nullptr || loco->GetControlID() != ControlID())
		{
			return;
		}
		instance->LocoSpeed(loco->GetProtocol(), loco->GetAddress(), speed);
	}

	void HardwareHandler::LocoDirection(const controlType_t controlType, const DataModel::Loco* loco, const direction_t direction)
	{
		if (controlType == ControlTypeHardware || instance == nullptr || loco->GetControlID() != ControlID())
		{
			return;
		}
		instance->LocoDirection(loco->GetProtocol(), loco->GetAddress(), direction);
	}

	void HardwareHandler::LocoFunction(const controlType_t controlType, const DataModel::Loco* loco, const function_t function, const bool on)
	{
		if (controlType == ControlTypeHardware || instance == nullptr || loco->GetControlID() != ControlID())
		{
			return;
		}
		instance->LocoFunction(loco->GetProtocol(), loco->GetAddress(), function, on);
	}

	void HardwareHandler::LocoSpeedDirectionFunctions(const DataModel::Loco* loco, const locoSpeed_t speed, const direction_t direction, std::vector<bool>& functions)
	{
		if (instance == nullptr || loco->GetControlID() != ControlID())
		{
			return;
		}
		instance->LocoSpeedDirectionFunctions(loco->GetProtocol(), loco->GetAddress(), speed, direction, functions);
	}

	void HardwareHandler::AccessoryState(const controlType_t controlType, const DataModel::Accessory* accessory, const accessoryState_t state)
	{
		if (controlType == ControlTypeHardware
			|| instance == nullptr
			|| accessory == nullptr
			|| accessory->GetControlID() != this->ControlID())
		{
			return;
		}
		instance->Accessory(accessory->GetProtocol(), accessory->GetAddress(), state, accessory->GetDuration());
	}

	void HardwareHandler::SwitchState(const controlType_t controlType, const DataModel::Switch* mySwitch, const switchState_t state)
	{
		if (controlType == ControlTypeHardware
			|| instance == nullptr
			|| mySwitch == nullptr
			|| mySwitch->GetControlID() != this->ControlID())
		{
			return;
		}
		instance->Accessory(mySwitch->GetProtocol(), mySwitch->GetAddress(), state, mySwitch->GetDuration());
	}

	void HardwareHandler::SignalState(const controlType_t controlType, const DataModel::Signal* signal, const signalState_t state)
	{
		if (controlType == ControlTypeHardware
			|| instance == nullptr
			|| signal == nullptr
			|| signal->GetControlID() != this->ControlID())
		{
			return;
		}
		instance->Accessory(signal->GetProtocol(), signal->GetAddress(), state, signal->GetDuration());
	}

	void HardwareHandler::ArgumentTypesOfHardwareType(const hardwareType_t hardwareType, std::map<unsigned char,argumentType_t>& arguments)
	{
		switch (hardwareType)
		{
			case HardwareTypeCS2:
				Hardware::CS2::GetArgumentTypes(arguments);
				return;

			case HardwareTypeM6051:
				Hardware::M6051::GetArgumentTypes(arguments);
				return;

			case HardwareTypeRM485:
				Hardware::RM485::GetArgumentTypes(arguments);
				return;

			case HardwareTypeOpenDcc:
				Hardware::OpenDcc::GetArgumentTypes(arguments);
				return;

			case HardwareTypeHsi88:
				Hardware::Hsi88::GetArgumentTypes(arguments);
				return;

			case HardwareTypeZ21:
				Hardware::Z21::GetArgumentTypes(arguments);
				return;

			case HardwareTypeCcSchnitte:
				Hardware::CcSchnitte::GetArgumentTypes(arguments);
				return;

			case HardwareTypeEcos:
				Hardware::Ecos::GetArgumentTypes(arguments);
				return;

			default:
				return;
		}
	}
} // namespace Hardware
