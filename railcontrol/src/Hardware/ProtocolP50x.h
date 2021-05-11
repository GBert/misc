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

#include "Hardware/HardwareInterface.h"
#include "Hardware/HardwareParams.h"
#include "Hardware/ProtocolP50xCache.h"
#include "Logger/Logger.h"

namespace Hardware
{
	class ProtocolP50x : protected HardwareInterface
	{
		public:
			ProtocolP50x() = delete;
			ProtocolP50x(const ProtocolP50x&) = delete;
			ProtocolP50x& operator=(const ProtocolP50x&) = delete;

			ProtocolP50x(const HardwareParams* const params,
				const std::string& controlName);

			virtual ~ProtocolP50x();

			inline Hardware::Capabilities GetCapabilities() const override
			{
				return Hardware::CapabilityLoco
					| Hardware::CapabilityAccessory
					| Hardware::CapabilityFeedback;
			}

			inline void GetLocoProtocols(std::vector<Protocol>& protocols) const override
			{
				protocols.push_back(ProtocolDCC);
			}

			inline bool LocoProtocolSupported(Protocol protocol) const override
			{
				return (protocol == ProtocolDCC);
			}

			inline void GetAccessoryProtocols(std::vector<Protocol>& protocols) const override
			{
				protocols.push_back(ProtocolDCC);
			}

			inline bool AccessoryProtocolSupported(Protocol protocol) const override
			{
				return (protocol == ProtocolDCC);
			}

			void Booster(const BoosterState status) override;
			void LocoSpeed(const Protocol protocol, const Address address, const Speed speed) override;
			void LocoOrientation(const Protocol protocol, const Address address, const Orientation orientation) override;

			void LocoFunction(const Protocol protocol,
				const Address address,
				const DataModel::LocoFunctionNr function,
				const DataModel::LocoFunctionState on) override;

			void LocoSpeedOrientationFunctions(const Protocol protocol,
				const Address address,
				const Speed speed,
				const Orientation orientation,
				std::vector<DataModel::LocoFunctionEntry>& functions) override;

			void AccessoryOnOrOff(const Protocol protocol, const Address address, const DataModel::AccessoryState state, const bool on) override;

			void CheckEventsWorker();

		protected:
			virtual int Send(const unsigned char* buffer, const size_t bufferLength) const = 0;
			virtual ssize_t Receive(unsigned char* data, const size_t length) const = 0;
			virtual ssize_t ReceiveExact(unsigned char* data, const size_t length) const = 0;

			bool SendP50XOnly() const;
			bool SendRestart() const;
			unsigned char SendXP88Get(unsigned char param) const;
			bool SendXP88Set(unsigned char param, unsigned char value) const;

			inline bool SendNop() const
			{
				return SendOneByteCommand(XNop);
			}

			inline ssize_t SendInternal(const unsigned char data) const
			{
				return SendInternal(&data, 1);
			}

			inline ssize_t SendInternal(const unsigned char* data, const size_t dataLength) const
			{
				logger->Hex(data, dataLength);
				return Send(data, dataLength);
			}

			inline ssize_t ReceiveInternal(unsigned char* data, const size_t length) const
			{
				const ssize_t ret = Receive(data, length);
				if (ret > 0)
				{
					logger->Hex(data, ret);
				}
				return ret;
			}

			inline ssize_t ReceiveExactInternal(unsigned char* data, const size_t length) const
			{
				const ssize_t ret = ReceiveExact(data, length);
				if (ret > 0)
				{
					logger->Hex(data, ret);
				}
				return ret;
			}

			static const unsigned char MaxS88Modules = 128;

			unsigned short s88Modules;
			std::thread checkEventsThread;

		private:
			enum Commands : unsigned char
			{
				XNop = 0xC4,
				XPwrOn = 0xA7,
				XPwrOff = 0xA6,
				XLok = 0x80,
				XFunc = 0x88,
				XFunc2 = 0x89,
				XFunc34 = 0x8A,
				XTrnt = 0x90,
				XP88Get = 0x9C,
				XP88Set = 0x9D,
				XEvent = 0xC8,
				XEvtLok = 0xC9,
				XEvtTrnt = 0xCA,
				XEvtSen = 0xCB
			};

			enum Answers : unsigned char
			{
				OK = 0x00,
				XBADPRM = 0x02,
				XPWOFF = 0x06,
				XNODATA = 0x0A,
				XNOSLOT = 0x0B,
				XLOWTSP = 0x40,
				XLKHALT = 0x41,
				XLKPOFF = 0x42
			};

			static const unsigned char MaxLocoFunctions = 28;
			static const unsigned short MaxLocoAddress = 10239;
			static const unsigned short MaxAccessoryAddress = 2043;

			static inline bool CheckLocoAddress(const Address address)
			{
				return 0 < address && address <= MaxLocoAddress;
			}

			static inline bool CheckAccessoryAddress(const Address address)
			{
				return 0 < address && address <= MaxAccessoryAddress;
			}

			bool SendOneByteCommand(const unsigned char data) const;

			inline bool SendPowerOn() const
			{
				return SendOneByteCommand(XPwrOn);
			}

			inline bool SendPowerOff() const
			{
				return SendOneByteCommand(XPwrOff);
			}

			bool SendXLok(const Address address) const;
			bool SendXFunc(const Address address) const;
			bool SendXFunc2(const Address address) const;
			bool SendXFunc34(const Address address) const;
			bool ReceiveFunctionCommandAnswer() const;
			void CheckSensorData(const unsigned char module, const unsigned char data) const;
			void SendXEvtSen() const;
			void SendXEvent() const;

			volatile bool run;

			mutable unsigned char s88Memory[MaxS88Modules];

			Hardware::ProtocolP50xCache cache;

			mutable std::mutex communicationLock;
	};
} // namespace

