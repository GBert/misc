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

#include <map>
#include <string>
#include <thread>
#include <vector>

#include "DataModel/AccessoryBase.h"
#include "DataModel/ObjectIdentifier.h"
#include "Manager.h"
#include "Network/TcpConnection.h"
#include "WebServer/HtmlResponse.h"

namespace DataModel
{
	class ObjectIdentifier;
};

namespace WebServer
{
	class WebServer;

	class WebClient
	{
		public:
			WebClient() = delete;
			WebClient(const unsigned int id, Network::TcpConnection* connection, WebServer &webserver, Manager& m)
			:	logger(Logger::Logger::GetLogger("Webserver")),
				id(id),
				connection(connection),
				run(false),
				server(webserver),
				clientThread(&WebClient::Worker, this),
				manager(m),
				headOnly(false),
				buttonID(0)
			{}

			~WebClient();
			void Worker();
			int Stop();

		private:
			enum ResponseType : unsigned char
			{
				ResponseInfo = 'i',
				ResponseWarning = 'w',
				ResponseError = 'e'
			};

			void ReplyResponse(std::string& text)
			{
				connection->Send(HtmlResponse(HtmlTag().AddContent(text)));
			}

			void ReplyResponse(ResponseType type, std::string& text)
			{
				std::string s(1, static_cast<unsigned char>(type));
				s.append(text);
				ReplyResponse(s);
			}

			template<typename... Args>
			void ReplyResponse(ResponseType type, Languages::TextSelector text, Args... args)
			{
				std::string s(1, static_cast<unsigned char>(type));
				s.append(Logger::Logger::Format(Languages::GetText(text), args...));
				ReplyResponse(s);
			}

			void InterpretClientRequest(const std::vector<std::string>& lines, std::string& method, std::string& uri, std::string& protocol, std::map<std::string,std::string>& arguments, std::map<std::string,std::string>& headers);
			void PrintLoco(const std::map<std::string, std::string>& arguments);
			void PrintMainHTML();
			void ReplyHtmlWithHeader(const HtmlTag& tag);
			void ReplyHtmlWithHeaderAndParagraph(const std::string& content) { ReplyHtmlWithHeader(HtmlTag("p").AddContent(content)); }
			void ReplyHtmlWithHeaderAndParagraph(const char* content) { ReplyHtmlWithHeaderAndParagraph(std::string(content)); }

			template<typename... Args>
			void ReplyHtmlWithHeaderAndParagraph(const Languages::TextSelector text, Args... args)
			{
				ReplyHtmlWithHeaderAndParagraph(Logger::Logger::Format(Languages::GetText(text), args...));
			}
			void DeliverFile(const std::string& file);
			void DeliverFileInternal(FILE* f, const char* realFile, const std::string& file);
			HtmlTag HtmlTagLocoSelector() const;
			HtmlTag HtmlTagLayerSelector() const;
			static HtmlTag HtmlTagControlArgument(const unsigned char argNr, const ArgumentType type, const std::string& value);
			HtmlTag HtmlTagProtocol(const std::map<std::string,Protocol>& protocolMap, const Protocol selectedProtocol);
			HtmlTag HtmlTagProtocolLoco(const ControlID controlID, const Protocol selectedProtocol);
			HtmlTag HtmlTagProtocolAccessory(const ControlID controlID, const Protocol selectedProtocol);
			HtmlTag HtmlTagDuration(const DataModel::AccessoryPulseDuration duration, const Languages::TextSelector label) const;
			HtmlTag HtmlTagDuration(const DataModel::AccessoryPulseDuration duration) const { return HtmlTagDuration(duration, Languages::TextDuration); }
			HtmlTag HtmlTagPosition(const DataModel::LayoutItem::LayoutPosition posx, const DataModel::LayoutItem::LayoutPosition posy, const DataModel::LayoutItem::LayoutPosition posz);
			HtmlTag HtmlTagPosition(const DataModel::LayoutItem::LayoutPosition posx, const DataModel::LayoutItem::LayoutPosition posy, const DataModel::LayoutItem::LayoutPosition posz, const DataModel::LayoutItem::Visible visible);
			HtmlTag HtmlTagRotation(const DataModel::LayoutItem::LayoutRotation rotation) const;
			HtmlTag HtmlTagSelectTrack(const std::string& name, const Languages::TextSelector label, const DataModel::ObjectIdentifier& identifier, const Direction direction, const std::string& onchange = "") const;
			HtmlTag HtmlTagSelectFeedbacksOfTrack(const DataModel::ObjectIdentifier& identifier, const FeedbackID feedbackIdReduced, const FeedbackID feedbackIdCreep, const FeedbackID feedbackIdStop, const FeedbackID feedbackIdOver) const;
			HtmlTag HtmlTagRelation(const std::string& type, const std::string& priority, const ObjectType objectType = ObjectTypeSwitch, const ObjectID objectId = ObjectNone, const DataModel::Relation::Data = DataModel::Relation::DefaultData);
			HtmlTag HtmlTagSlave(const std::string& priority, const ObjectID objectId = ObjectNone);
			HtmlTag HtmlTagRelationObject(const std::string& name, const ObjectType objectType, const ObjectID objectId = ObjectNone, const DataModel::Relation::Data = DataModel::Relation::DefaultData);
			HtmlTag HtmlTagTabMenuItem(const std::string& tabName, const Languages::TextSelector buttonValue, const bool selected = false) const;
			HtmlTag HtmlTagSelectFeedbackForTrack(const unsigned int counter, const DataModel::ObjectIdentifier& identifier, const FeedbackID feedbackID = FeedbackNone);
			static HtmlTag HtmlTagSelectSelectStreetApproach(const DataModel::SelectStreetApproach selectStreetApproach, const bool addDefault = true);
			static HtmlTag HtmlTagNrOfTracksToReserve(const DataModel::Loco::NrOfTracksToReserve nrOfTracksToReserve);
			static HtmlTag HtmlTagLogLevel();
			static HtmlTag HtmlTagLanguage();
			static HtmlTag HtmlTagFeedbackOnControlLayer(const DataModel::Feedback* feedback);
			static HtmlTag HtmlTagControlArguments(const HardwareType hardwareType, const std::string& arg1 = "", const std::string& arg2 = "", const std::string& arg3 = "", const std::string& arg4 = "", const std::string& arg5 = "");
			static HtmlTag HtmlTagControl(const std::map<ControlID,std::string>& controls, const ControlID controlID, const std::string& objectType, const ObjectID objectID);
			static HtmlTag HtmlTagControl(const std::string& name, const std::map<ControlID,std::string>& controls);
			HtmlTag HtmlTagControlLoco(const ControlID controlID, const std::string& objectType, const ObjectID objectID);
			HtmlTag HtmlTagControlAccessory(const ControlID controlID, const std::string& objectType, const ObjectID objectID);
			HtmlTag HtmlTagControlFeedback(const ControlID controlID, const std::string& objectType, const ObjectID objectID);
			static HtmlTag HtmlTagTabTrackAutomode(DataModel::SelectStreetApproach selectStreetApproach, bool releaseWhenFree);
			HtmlTag HtmlTagTabTrackFeedback(const std::vector<FeedbackID>& feedbacks, const DataModel::ObjectIdentifier& objectIdentifier);
			HtmlTag HtmlTagTabPosition(const DataModel::LayoutItem::LayoutPosition posx,
				const DataModel::LayoutItem::LayoutPosition posy,
				const DataModel::LayoutItem::LayoutPosition posz,
				const DataModel::LayoutItem::LayoutRotation rotation = DataModel::LayoutItem::RotationNotRelevant,
				const DataModel::LayoutItem::Visible visible = DataModel::LayoutItem::VisibleNotRelevant);
			HtmlTag HtmlTagTabPosition(const DataModel::LayoutItem::LayoutPosition posx,
				const DataModel::LayoutItem::LayoutPosition posy,
				const DataModel::LayoutItem::LayoutPosition posz,
				const DataModel::LayoutItem::Visible visible)
			{
				return HtmlTagTabPosition(posx, posy, posz, DataModel::LayoutItem::RotationNotRelevant, visible);
			}
			void HandleSelectLoco(const std::map<std::string, std::string>& arguments);
			void HandleLayerEdit(const std::map<std::string, std::string>& arguments);
			void HandleLayerSave(const std::map<std::string, std::string>& arguments);
			void HandleLayerList();
			void HandleLayerAskDelete(const std::map<std::string, std::string>& arguments);
			void HandleLayerDelete(const std::map<std::string, std::string>& arguments);
			static const std::map<std::string,HardwareType> ListHardwareNames();
			void HandleControlEdit(const std::map<std::string, std::string>& arguments);
			void HandleControlSave(const std::map<std::string, std::string>& arguments);
			void HandleControlList();
			void HandleControlAskDelete(const std::map<std::string, std::string>& arguments);
			void HandleControlDelete(const std::map<std::string, std::string>& arguments);
			void HandleLocoSpeed(const std::map<std::string,std::string>& arguments);
			void HandleLocoDirection(const std::map<std::string,std::string>& arguments);
			void HandleLocoFunction(const std::map<std::string, std::string>& arguments);
			void HandleLocoEdit(const std::map<std::string, std::string>& arguments);
			void HandleLocoSave(const std::map<std::string, std::string>& arguments);
			void HandleLocoList();
			void HandleLocoAskDelete(const std::map<std::string, std::string>& arguments);
			void HandleLocoDelete(const std::map<std::string, std::string>& arguments);
			void HandleLocoRelease(const std::map<std::string, std::string>& arguments);
			void HandleProtocolLoco(const std::map<std::string, std::string>& arguments);
			void HandleProtocolAccessory(const std::map<std::string, std::string>& arguments);
			void HandleProtocolSwitch(const std::map<std::string, std::string>& arguments);
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
			void HandleSignalEdit(const std::map<std::string,std::string>& arguments);
			void HandleSignalSave(const std::map<std::string,std::string>& arguments);
			void HandleSignalState(const std::map<std::string,std::string>& arguments);
			void HandleSignalList();
			void HandleSignalAskDelete(const std::map<std::string,std::string>& arguments);
			void HandleSignalDelete(const std::map<std::string,std::string>& arguments);
			void HandleSignalGet(const std::map<std::string,std::string>& arguments);
			void HandleSignalRelease(const std::map<std::string,std::string>& arguments);
			void HandleStreetEdit(const std::map<std::string,std::string>& arguments);
			void HandleStreetSave(const std::map<std::string,std::string>& arguments);
			void HandleStreetList();
			void HandleStreetAskDelete(const std::map<std::string,std::string>& arguments);
			void HandleStreetDelete(const std::map<std::string,std::string>& arguments);
			void HandleStreetGet(const std::map<std::string,std::string>& arguments);
			void HandleStreetExecute(const std::map<std::string,std::string>& arguments);
			void HandleStreetRelease(const std::map<std::string,std::string>& arguments);
			void HandleTrackEdit(const std::map<std::string,std::string>& arguments);
			void HandleTrackSave(const std::map<std::string,std::string>& arguments);
			void HandleTrackList();
			void HandleTrackAskDelete(const std::map<std::string,std::string>& arguments);
			void HandleTrackDelete(const std::map<std::string,std::string>& arguments);
			void HandleTrackGet(const std::map<std::string, std::string>& arguments);
			void HandleTrackSetLoco(const std::map<std::string, std::string>& arguments);
			void HandleTrackRelease(const std::map<std::string, std::string>& arguments);
			void HandleTrackStartLoco(const std::map<std::string, std::string>& arguments);
			void HandleTrackStopLoco(const std::map<std::string, std::string>& arguments);
			void HandleTrackBlock(const std::map<std::string, std::string>& arguments);
			void HandleTrackDirection(const std::map<std::string, std::string>& arguments);
			void HandleFeedbackEdit(const std::map<std::string,std::string>& arguments);
			void HandleFeedbackSave(const std::map<std::string,std::string>& arguments);
			void HandleFeedbackState(const std::map<std::string,std::string>& arguments);
			void HandleFeedbackList();
			void HandleFeedbackAskDelete(const std::map<std::string,std::string>& arguments);
			void HandleFeedbackDelete(const std::map<std::string,std::string>& arguments);
			void HandleFeedbackGet(const std::map<std::string,std::string>& arguments);
			void HandleFeedbacksOfTrack(const std::map<std::string,std::string>& arguments);
			void HandleLocoSelector();
			void HandleLayerSelector();
			void HandleRelationAdd(const std::map<std::string,std::string>& arguments);
			void HandleFeedbackAdd(const std::map<std::string,std::string>& arguments);
			void HandleRelationObject(const std::map<std::string, std::string>& arguments);
			void HandleSettingsEdit();
			void HandleSettingsSave(const std::map<std::string, std::string>& arguments);
			void HandleSlaveAdd(const std::map<std::string,std::string>& arguments);
			void HandleTimestamp(const std::map<std::string,std::string>& arguments);
			void HandleControlArguments(const std::map<std::string,std::string>& arguments);
			void HandleProgram();
			void HandleProgramRead(const std::map<std::string,std::string>& arguments);
			void HandleProgramWrite(const std::map<std::string,std::string>& arguments);
			void HandleUpdater(const std::map<std::string,std::string>& headers);
			void UrlDecode(std::string& argumentValue);
			char ConvertHexToInt(char c);
			void WorkerImpl();

			Logger::Logger* logger;
			unsigned int id;
			Network::TcpConnection* connection;
			volatile unsigned char run;
			WebServer& server;
			std::thread clientThread;
			Manager& manager;
			bool headOnly;
			unsigned int buttonID;
	};

}; // namespace webserver

