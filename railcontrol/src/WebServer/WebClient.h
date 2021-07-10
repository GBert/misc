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

#include <deque>
#include <map>
#include <string>
#include <thread>
#include <vector>

#include "DataModel/AccessoryBase.h"
#include "DataModel/ObjectIdentifier.h"
#include "Languages.h"
#include "Manager.h"
#include "Network/TcpConnection.h"
#include "ResponseHtml.h"
#include "WebServer/WebClientCluster.h"
#include "WebServer/WebClientRoute.h"
#include "WebServer/WebClientSignal.h"
#include "WebServer/WebClientStatic.h"
#include "WebServer/WebClientText.h"
#include "WebServer/WebClientTrack.h"

namespace DataModel
{
	class ObjectIdentifier;
	class Cluster;
}

namespace WebServer
{
	class WebServer;

	class WebClient
	{
		public:
			enum ResponseType : unsigned char
			{
				ResponseInfo = 'i',
				ResponseWarning = 'w',
				ResponseError = 'e'
			};

			WebClient() = delete;
			WebClient(const WebClient&) = delete;
			WebClient& operator=(const WebClient&) = delete;

			inline WebClient(const unsigned int id,
				Network::TcpConnection* connection, // connection must be deleted after using!
				WebServer &webserver,
				Manager& manager)
			:	logger(Logger::Logger::GetLogger("Webserver")),
				id(id),
				connection(connection),
				run(false),
				terminated(false),
				server(webserver),
				clientThread(&WebClient::Worker, this),
				manager(manager),
				cluster(manager, *this),
				track(manager, *this, logger),
				signal(manager, *this, logger),
				route(manager, *this, logger),
				text(manager, *this),
				headOnly(false),
				buttonID(0)
			{
			}

			~WebClient();

			void Worker();

			inline void Stop()
			{
				run = false;
			}

			inline bool IsTerminated()
			{
				return terminated;
			}

			inline void ReplyHtmlWithHeader(const HtmlTag& tag)
			{
				connection->Send(ResponseHtml(tag));
			}

			inline void ReplyResponse(const std::string& text)
			{
				ReplyHtmlWithHeader(HtmlTag().AddContent(text));
			}

			inline void ReplyResponse(ResponseType type, const std::string& text)
			{
				std::string s(1, static_cast<unsigned char>(type));
				s.append(text);
				ReplyResponse(s);
			}

			template<typename... Args>
			inline void ReplyResponse(ResponseType type, Languages::TextSelector text, Args... args)
			{
				std::string s(1, static_cast<unsigned char>(type));
				s.append(Logger::Logger::Format(Languages::GetText(text), args...));
				ReplyResponse(s);
			}

			inline void ReplyHtmlWithHeaderAndParagraph(const std::string& content)
			{
				ReplyHtmlWithHeader(HtmlTag("p").AddContent(content));
			}

			inline void ReplyHtmlWithHeaderAndParagraph(const char* content)
			{
				ReplyHtmlWithHeaderAndParagraph(std::string(content));
			}

			template<typename... Args>
			inline void ReplyHtmlWithHeaderAndParagraph(const Languages::TextSelector text, Args... args)
			{
				ReplyHtmlWithHeaderAndParagraph(Logger::Logger::Format(Languages::GetText(text), args...));
			}

			HtmlTag HtmlTagTabMenuItem(const std::string& tabName,
				const Languages::TextSelector buttonValue,
				const bool selected = false) const;

			HtmlTag HtmlTagSlaveSelect(const std::string& prefix,
				const std::vector<DataModel::Relation*>& relations,
				const std::map<std::string,ObjectID>& options,
				const bool allowNew = true) const;

			inline HtmlTag HtmlTagTabPosition(const DataModel::LayoutItem::LayoutPosition posx,
				const DataModel::LayoutItem::LayoutPosition posy,
				const DataModel::LayoutItem::LayoutPosition posz,
				const DataModel::LayoutItem::Visible visible) const
			{
				return HtmlTagTabPosition(posx, posy, posz, DataModel::LayoutItem::RotationNotRelevant, visible);
			}

			HtmlTag HtmlTagTabPosition(const DataModel::LayoutItem::LayoutPosition posx,
				const DataModel::LayoutItem::LayoutPosition posy,
				const DataModel::LayoutItem::LayoutPosition posz,
				const DataModel::LayoutItem::LayoutRotation rotation = DataModel::LayoutItem::RotationNotRelevant,
				const DataModel::LayoutItem::Visible visible = DataModel::LayoutItem::VisibleNotRelevant) const ;

			HtmlTag HtmlTagControlAccessory(const ControlID controlID, const std::string& objectType, const ObjectID objectID);
			HtmlTag HtmlTagProtocolAccessory(const ControlID controlID, const Protocol selectedProtocol);

			HtmlTag HtmlTagSelectFeedbackForTrack(const unsigned int counter,
				const DataModel::ObjectIdentifier& identifier,
				const FeedbackID feedbackID = FeedbackNone) const;

			HtmlTag HtmlTagRelationSignalState(const std::string& name,
				const SignalID signalId,
				const DataModel::Relation::Data data = DataModel::SignalStateStop);

			static std::string ProtocolName(const Protocol protocol)
			{
				return ProtocolSymbols[protocol <= ProtocolEnd ? protocol : ProtocolNone];
			}

		private:
			void InterpretClientRequest(const std::deque<std::string>& lines, std::string& method, std::string& uri, std::string& protocol, std::map<std::string,std::string>& arguments, std::map<std::string,std::string>& headers);
			void HandleLoco(const std::map<std::string, std::string>& arguments);
			void PrintMainHTML();
			void DeliverFile(const std::string& file);
			void DeliverFileInternal(FILE* f, const char* realFile, const std::string& file);
			HtmlTag HtmlTagLocoSelector(const unsigned int selector) const;
			HtmlTag HtmlTagLayerSelector() const;
			HtmlTag HtmlTagMatchKeyProtocolLoco(const ControlID controlID,
				const std::string& selectedMatchKey,
				const Protocol selectedProtocol);

			HtmlTag HtmlTagMatchKeyProtocolAccessory(const ControlID controlID,
				const std::string& selectedMatchKey,
				const Protocol selectedProtocol);

			HtmlTag HtmlTagPosition(const DataModel::LayoutItem::LayoutPosition posx,
				const DataModel::LayoutItem::LayoutPosition posy,
				const DataModel::LayoutItem::LayoutPosition posz) const;

			HtmlTag HtmlTagPosition(const DataModel::LayoutItem::LayoutPosition posx,
				const DataModel::LayoutItem::LayoutPosition posy,
				const DataModel::LayoutItem::LayoutPosition posz,
				const DataModel::LayoutItem::Visible visible) const;

			HtmlTag HtmlTagControlLoco(const ControlID controlID, const std::string& objectType, const ObjectID objectID);
			HtmlTag HtmlTagControlFeedback(const ControlID controlID, const std::string& objectType, const ObjectID objectID);

			HtmlTag HtmlTagProgramModeSelector(const ControlID controlID, ProgramMode& mode) const;
			HtmlTag HtmlTagCvFields(const ControlID controlID, const ProgramMode programMode) const;

			std::map<std::string,ObjectID> GetLocoSlaveOptions(const LocoID locoID = LocoNone) const;

			void HandleSelectLoco(const std::map<std::string, std::string>& arguments);
			void HandleLayerEdit(const std::map<std::string, std::string>& arguments);
			void HandleLayerSave(const std::map<std::string, std::string>& arguments);
			void HandleLayerList();
			void HandleLayerAskDelete(const std::map<std::string, std::string>& arguments);
			void HandleLayerDelete(const std::map<std::string, std::string>& arguments);
			void HandleControlEdit(const std::map<std::string, std::string>& arguments);
			void HandleControlSave(const std::map<std::string, std::string>& arguments);
			void HandleControlList();
			void HandleControlAskDelete(const std::map<std::string, std::string>& arguments);
			void HandleControlDelete(const std::map<std::string, std::string>& arguments);
			void HandleLocoSpeed(const std::map<std::string,std::string>& arguments);
			void HandleLocoOrientation(const std::map<std::string,std::string>& arguments);
			void HandleLocoFunction(const std::map<std::string, std::string>& arguments);
			void HandleLocoEdit(const std::map<std::string, std::string>& arguments);
			void HandleLocoSave(const std::map<std::string, std::string>& arguments);
			void HandleLocoList();
			void HandleLocoAskDelete(const std::map<std::string, std::string>& arguments);
			void HandleLocoDelete(const std::map<std::string, std::string>& arguments);
			void HandleLocoRelease(const std::map<std::string, std::string>& arguments);
			void HandleLocoAddTimeTable(const std::map<std::string, std::string>& arguments);
			void HandleProtocol(const std::map<std::string, std::string>& arguments);
			void HandleLayout(const std::map<std::string,std::string>& arguments);
			void HandleAccessoryEdit(const std::map<std::string,std::string>& arguments);
			void HandleAccessorySave(const std::map<std::string,std::string>& arguments);
			void HandleAccessoryState(const std::map<std::string,std::string>& arguments);
			void HandleAccessoryList();
			void HandleAccessoryAskDelete(const std::map<std::string,std::string>& arguments);
			void HandleAccessoryDelete(const std::map<std::string,std::string>& arguments);
			void HandleAccessoryGet(const std::map<std::string,std::string>& arguments);
			void HandleAccessoryRelease(const std::map<std::string,std::string>& arguments);
			void HandleSwitchEdit(const std::map<std::string,std::string>& arguments);
			void HandleSwitchSave(const std::map<std::string,std::string>& arguments);
			void HandleSwitchState(const std::map<std::string,std::string>& arguments);
			void HandleSwitchList();
			void HandleSwitchAskDelete(const std::map<std::string,std::string>& arguments);
			void HandleSwitchDelete(const std::map<std::string,std::string>& arguments);
			void HandleSwitchGet(const std::map<std::string,std::string>& arguments);
			void HandleSwitchRelease(const std::map<std::string,std::string>& arguments);
			void HandleFeedbackEdit(const std::map<std::string,std::string>& arguments);
			void HandleFeedbackSave(const std::map<std::string,std::string>& arguments);
			void HandleFeedbackState(const std::map<std::string,std::string>& arguments);
			void HandleFeedbackList();
			void HandleFeedbackAskDelete(const std::map<std::string,std::string>& arguments);
			void HandleFeedbackDelete(const std::map<std::string,std::string>& arguments);
			void HandleFeedbackGet(const std::map<std::string,std::string>& arguments);
			void HandleLocoSelector(const std::map<std::string,std::string>& arguments);
			void HandleLayerSelector();
			void HandleFeedbackAdd(const std::map<std::string,std::string>& arguments);
			void HandleSettingsEdit();
			void HandleSettingsSave(const std::map<std::string, std::string>& arguments);
			void HandleSlaveAdd(const std::map<std::string,std::string>& arguments);
			void HandleTimestamp(const std::map<std::string,std::string>& arguments);
			void HandleControlArguments(const std::map<std::string,std::string>& arguments);
			void HandleProgram();
			void HandleProgramModeSelector(const std::map<std::string,std::string>& arguments);
			void HandleProgramRead(const std::map<std::string,std::string>& arguments);
			void HandleProgramWrite(const std::map<std::string,std::string>& arguments);
			void HandleCvFields(const std::map<std::string,std::string>& arguments);
			void HandleNewPosition(const std::map<std::string,std::string>& arguments);
			void HandleNewPositionInternal(const std::map<std::string,std::string>& arguments, std::string& result);
			void HandleRotate(const std::map<std::string,std::string>& arguments);
			void HandleUpdater(const std::map<std::string,std::string>& headers);
			void WorkerImpl();

			Logger::Logger* logger;
			unsigned int id;
			Network::TcpConnection* connection;
			volatile bool run;
			bool terminated;
			WebServer& server;
			std::thread clientThread;
			Manager& manager;
			WebClientCluster cluster;
			WebClientTrack track;
			WebClientSignal signal;
			WebClientRoute route;
			WebClientText text;
			bool headOnly;
			unsigned int buttonID;
	};

} // namespace WebServer

