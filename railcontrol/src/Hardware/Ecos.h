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

#include "HardwareInterface.h"
#include "HardwareParams.h"
#include "Logger/Logger.h"
#include "Network/TcpClient.h"

namespace Hardware
{
	class Ecos : protected HardwareInterface
	{
		public:
			Ecos(const HardwareParams* params);
			~Ecos();

			bool CanHandleLocos() const override { return true; }
			bool CanHandleAccessories() const override { return true; }
			bool CanHandleFeedback() const override { return true; }

			void GetLocoProtocols(std::vector<Protocol>& protocols) const override
			{
				protocols.push_back(ProtocolServer);
			}

			bool LocoProtocolSupported(Protocol protocol) const override
			{
				return (protocol == ProtocolServer);
			}

			void GetAccessoryProtocols(std::vector<Protocol>& protocols) const override
			{
				protocols.push_back(ProtocolServer);
			}

			bool AccessoryProtocolSupported(Protocol protocol) const override
			{
				return (protocol == ProtocolServer);
			}

			static void GetArgumentTypesAndHint(std::map<unsigned char,ArgumentType>& argumentTypes, std::string& hint)
			{
				argumentTypes[1] = ArgumentTypeIpAddress;
				hint = Languages::GetText(Languages::TextHintEcos);
			}

			void Booster(const BoosterState status) override
			{
				Send(status == BoosterStateGo ? "set(1,go)\n" : "set(1,stop)\n");
			}

			void LocoSpeed(const Protocol protocol, const Address address, const Speed speed) override;
			void LocoDirection(const Protocol protocol, const Address address, const Direction direction) override;
			void LocoFunction(const Protocol protocol, const Address address, const Function function, const DataModel::LocoFunctions::FunctionState on) override;
			void AccessoryOnOrOff(const Protocol protocol, const Address address, const DataModel::AccessoryState state, const bool on) override;

			static const char* const CommandActivateBoosterUpdates;
			static const char* const CommandQueryLocos;
			static const char* const CommandQueryAccessories;
			static const char* const CommandQueryFeedbacks;

		private:
			static const unsigned short MaxMessageSize = 1024;
			static const unsigned short EcosPort = 15471;

			void Send(const char* data);
			void Receiver();
			void ReadLine();
			void Parser();
			void ParseReply();
			void ParseQueryLocos();
			void ParseQueryAccessories();
			void ParseQueryFeedbacks();
			void ParseLocoData();
			void ParseAccessoryData();
			void ParseFeedbackData();

			void ParseEvent();
			void ParseEventLine();
			void ParseBoosterEvent();
			void ParseLocoEvent(int loco);
			void ParseAccessoryEvent(int accessory);
			void ParseFeedbackEvent(int feedback);
			void CheckFeedbackDiff(unsigned int module, uint8_t data);

			void ParseOption(std::string& option, std::string& value);
			void ParseOptionInt(std::string& option, int& value);
			void ParseOptionHex(std::string& option, int& value);
			void ParseEndLine();
			std::string ReadUntilChar(const char c);
			std::string ReadUntilLineEnd();

			void SendActivateBoosterUpdates()
			{
				Send(CommandActivateBoosterUpdates);
			}

			void SendQueryLocos()
			{
				Send(CommandQueryLocos);
			}

			void SendQueryAccessories()
			{
				Send(CommandQueryAccessories);
			}

			void SendQueryFeedbacks()
			{
				Send(CommandQueryFeedbacks);
			}

			void SendActivateUpdates(const int id)
			{
				std::string command = "request(" + std::to_string(id) + ",view)\n";
				Send(command.c_str());
			}

			void SendGetHandle(const int id)
			{
				std::string command = "request(" + std::to_string(id) + ",control,force)\n";
				Send(command.c_str());
			}

			char GetChar(const size_t offset = 0) const
			{
				size_t position = readBufferPosition + offset;
				if (position >= MaxMessageSize)
				{
					return 0;
				}
				return readBuffer[position];
			}

			char ReadAndConsumeChar()
			{
				if (readBufferPosition >= MaxMessageSize)
				{
					return 0;
				}
				return readBuffer[readBufferPosition++];
			}

			bool CheckAndConsumeChar(const char charToCheck)
			{
				if (readBufferPosition >= MaxMessageSize)
				{
					return false;
				}
				return charToCheck == readBuffer[readBufferPosition++];
			}

			bool CheckChar(const char charToCheck)
			{
				if (readBufferPosition >= MaxMessageSize)
				{
					return false;
				}
				return charToCheck == readBuffer[readBufferPosition];
			}

			bool SkipOptionalChar(const char charToSkip);

			void SkipWhiteSpace()
			{
				while(SkipOptionalChar(' '));
			}

			bool Compare(const char* reference, const size_t size) const;
			bool CompareAndConsume(const char* reference, const size_t size);
			bool IsNumber() const;
			int ParseInt();

			bool CheckGraterThenAtLineEnd()
			{
				SkipWhiteSpace();
				return CompareAndConsume(">\n", 2);
			}

			Logger::Logger* logger;
			volatile bool run;
			std::thread receiverThread;

			Network::TcpConnection tcp;

			char readBuffer[MaxMessageSize];
			ssize_t readBufferLength;
			size_t readBufferPosition;

			static const unsigned int MaxFeedbackModules = 128;
			uint8_t feedbackMemory[MaxFeedbackModules];

			static const int OffsetLocoAddress = 999;
			static const int OffsetAccessoryAddress = 19999;
			static const int OffsetFeedbackModuleAddress = 100;
	};

	extern "C" Ecos* create_Ecos(const HardwareParams* params);
	extern "C" void destroy_Ecos(Ecos* ecos);
} // namespace
