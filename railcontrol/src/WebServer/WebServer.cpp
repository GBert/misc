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

#include <algorithm>
#include <arpa/inet.h>
#include <cstring>		//memset
#include <ifaddrs.h>
#include <netinet/in.h>
#include <signal.h>
#include <sstream>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

#include "ControlInterface.h"
#include "DataTypes.h"
#include "DataModel/LocoFunctions.h"
#include "Languages.h"
#include "RailControl.h"
#include "Utils/Utils.h"
#include "WebServer/WebClient.h"
#include "WebServer/WebServer.h"

using std::map;
using std::thread;
using std::string;
using std::stringstream;
using std::to_string;
using std::vector;

using DataModel::Loco;
using DataModel::Route;
using DataModel::TrackBase;

namespace WebServer
{
	const std::string WebServer::UpdateStatus = "data: status=";
	const std::string WebServer::Webserver = "Webserver";

	WebServer::WebServer(Manager& manager, const unsigned short port)
	:	ControlInterface(ControlTypeWebserver),
		Network::TcpServer(port, "WebServer"),
		logger(Logger::Logger::GetLogger("WebServer")),
		run(false),
		lastClientID(0),
		manager(manager),
		updateID(1)
	{
		logger->Info(Languages::TextWebServerStarted);
		{
			std::lock_guard<std::mutex> lock(updateMutex);
			updates[updateID] = GetStatus(Languages::TextRailControlStarted);
		}
		run = true;
		LogBrowserInfo(port);
	}

	WebServer::~WebServer()
	{
		if (run == false)
		{
			return;
		}
		{
			std::lock_guard<std::mutex> lock(updateMutex);
			updates[++updateID] = GetStatus(Languages::TextStoppingRailControl);
		}
		TerminateTcpServer();
		Utils::Utils::SleepForSeconds(1);
		run = false;

		// stopping all clients
		for (auto client : clients)
		{
			client->Stop();
		}

		// delete all client memory
		while (clients.size())
		{
			WebClient* client = clients.back();
			clients.pop_back();
			delete client;
		}
		logger->Info(Languages::TextWebServerStopped);
	}

	void WebServer::LogBrowserInfo(const unsigned short port)
	{
		static const string Http("\n   http://");
		static const string Port(to_string(port));
		string localhostInfo(Http);
		localhostInfo += "localhost:" + Port + "/";
		string ipv4Info;
		string ipv6Info;

		struct ifaddrs* ifAddrStruct = nullptr;
		struct ifaddrs* ifa = nullptr;
		getifaddrs(&ifAddrStruct);

		for (ifa = ifAddrStruct; ifa != nullptr; ifa = ifa->ifa_next)
		{
			if (!ifa->ifa_addr)
			{
				continue;
			}
			if (ifa->ifa_addr->sa_family == AF_INET)
			{ // check it is IP4
				// is a valid IP4 Address
				void* tmpAddrPtr = &((struct sockaddr_in*) ifa->ifa_addr)->sin_addr;
				char addressBuffer[INET_ADDRSTRLEN];
				inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
				string address(addressBuffer);
				if (address.compare("0.0.0.0") == 0)
				{
					continue;
				}
				if (address.substr(0, 8).compare("169.254.") == 0)
				{
					continue;
				}
				ipv4Info += Http + addressBuffer + ":" + Port + "/";
			}
			else if (ifa->ifa_addr->sa_family == AF_INET6)
			{ // check it is IP6
				// is a valid IP6 Address
				void* tmpAddrPtr = &((struct sockaddr_in6*) ifa->ifa_addr)->sin6_addr;
				char addressBuffer[INET6_ADDRSTRLEN];
				inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
				ipv4Info += Http + "[" + addressBuffer + "]:" + Port + "/";
			}
		}
		if (ifAddrStruct != NULL)
		{
			freeifaddrs(ifAddrStruct);
		}
		logger->Info(Languages::TextBrowserInfo, localhostInfo, ipv4Info, ipv6Info);
	}

	void WebServer::Work(Network::TcpConnection* connection)
	{
		clients.push_back(new WebClient(++lastClientID, connection, *this, manager));
	}

	void WebServer::Booster(__attribute__((unused)) const ControlType controlType, const BoosterState status)
	{
		if (status)
		{
			AddUpdate("booster;on=true", Languages::TextTurningBoosterOn);
		}
		else
		{
			AddUpdate("booster;on=false", Languages::TextTurningBoosterOff);
		}
	}

	void WebServer::LocoSpeed(__attribute__((unused)) const ControlType controlType, const DataModel::Loco* loco, const Speed speed)
	{
		stringstream command;
		command << "locospeed;loco=" << loco->GetID() << ";speed=" << speed;
		AddUpdate(command.str(), Languages::TextLocoSpeedIs, loco->GetName(), speed);
	}

	void WebServer::LocoOrientation(__attribute__((unused)) const ControlType controlType, const DataModel::Loco* loco, const Orientation orientation)
	{
		stringstream command;
		command << "locoorientation;loco=" << loco->GetID() << ";orientation=" << (orientation ? "true" : "false");
		AddUpdate(command.str(), orientation ? Languages::TextLocoDirectionOfTravelIsRight : Languages::TextLocoDirectionOfTravelIsLeft, loco->GetName());
	}

	void WebServer::LocoFunction(__attribute__((unused)) const ControlType controlType,
		const DataModel::Loco* loco,
		const DataModel::LocoFunctionNr function,
		const DataModel::LocoFunctionState state)
	{
		stringstream command;
		command << "locofunction;loco=" << loco->GetID() << ";function=" << static_cast<unsigned int>(function) << ";on=" << (state ? "true" : "false");
		AddUpdate(command.str(), state ? Languages::TextLocoFunctionIsOn : Languages::TextLocoFunctionIsOff, loco->GetName(), function);
	}

	void WebServer::AccessoryState(__attribute__((unused)) const ControlType controlType, const DataModel::Accessory* accessory)
	{
		stringstream command;
		const DataModel::AccessoryState state = accessory->GetAccessoryState();
		command << "accessory;accessory=" << accessory->GetID() << ";state=" << (state == DataModel::AccessoryStateOn ? "green" : "red");
		AddUpdate(command.str(), state ? Languages::TextAccessoryStateIsGreen : Languages::TextAccessoryStateIsRed, accessory->GetName());
	}

	void WebServer::AccessorySettings(const AccessoryID accessoryID, const std::string& name)
	{
		stringstream command;
		command << "accessorysettings;accessory=" << accessoryID;
		AddUpdate(command.str(), Languages::TextAccessoryUpdated, name);
	}

	void WebServer::AccessoryDelete(const AccessoryID accessoryID, const std::string& name)
	{
		stringstream command;
		command << "accessorydelete;accessory=" << accessoryID;
		AddUpdate(command.str(), Languages::TextAccessoryDeleted, name);
	}

	void WebServer::FeedbackState(const std::string& name, const FeedbackID feedbackID, const DataModel::Feedback::FeedbackState state)
	{
		stringstream command;
		command << "feedback;feedback=" << feedbackID << ";state=" << (state ? "on" : "off");
		AddUpdate(command.str(), state ? Languages::TextFeedbackStateIsOn : Languages::TextFeedbackStateIsOff, name);
	}

	void WebServer::FeedbackSettings(const FeedbackID feedbackID, const std::string& name)
	{
		stringstream command;
		command << "feedbacksettings;feedback=" << feedbackID;
		AddUpdate(command.str(), Languages::TextFeedbackUpdated, name);
	}

	void WebServer::FeedbackDelete(const FeedbackID feedbackID, const std::string& name)
	{
		stringstream command;
		command << "feedbackdelete;feedback=" << feedbackID;
		AddUpdate(command.str(), Languages::TextFeedbackDeleted, name);
	}

	void WebServer::RouteSettings(const RouteID routeID, const std::string& name)
	{
		stringstream command;
		command << "routesettings;route=" << routeID;
		AddUpdate(command.str(), Languages::TextRouteUpdated, name);
	}

	void WebServer::RouteDelete(const RouteID routeID, const std::string& name)
	{
		stringstream command;
		command << "routedelete;route=" << routeID;
		AddUpdate(command.str(), Languages::TextRouteDeleted, name);
	}

	void WebServer::SwitchState(__attribute__((unused)) const ControlType controlType, const DataModel::Switch* mySwitch)
	{
		stringstream command;
		const DataModel::AccessoryState state = mySwitch->GetAccessoryState();
		command << "switch;switch=" << mySwitch->GetID() << ";state=";
		Languages::TextSelector text;
		switch (state)
		{
			case DataModel::AccessoryState::SwitchStateTurnout:
				command << "turnout";
				text = Languages::TextSwitchStateIsTurnout;
				break;

			case DataModel::AccessoryState::SwitchStateThird:
				command << "third";
				text = Languages::TextSwitchStateIsThird;
				break;

			case DataModel::AccessoryState::SwitchStateStraight:
			default:
				command << "straight";
				text = Languages::TextSwitchStateIsStraight;
				break;
		}
		AddUpdate(command.str(), text, mySwitch->GetName());
	}

	void WebServer::SwitchSettings(const SwitchID switchID, const std::string& name)
	{
		stringstream command;
		command << "switchsettings;switch=" << switchID;
		AddUpdate(command.str(), Languages::TextSwitchUpdated, name);
	}

	void WebServer::SwitchDelete(const SwitchID switchID, const std::string& name)
	{
		stringstream command;
		command << "switchdelete;switch=" << switchID;
		AddUpdate(command.str(), Languages::TextSwitchDeleted, name);
	}

	void WebServer::TrackState(const DataModel::Track* track)
	{
		string command = "trackstate;track=" + to_string(track->GetID());
		TrackBaseState(command, dynamic_cast<const DataModel::TrackBase*>(track));
	}

	void WebServer::TrackBaseState(string& command, const DataModel::TrackBase* track)
	{
		const DataModel::Loco* loco = manager.GetLoco(track->GetLocoDelayed());
		const bool reserved = loco != nullptr;
		const string& trackName = track->GetMyName();
		const string& locoName = reserved ? loco->GetName() : "";
		const bool occupied = track->GetFeedbackStateDelayed() == DataModel::Feedback::FeedbackStateOccupied;
		const bool blocked = track->GetBlocked();
		const Orientation orientation = track->GetLocoOrientation();
		const string occupiedText = (occupied ? "true" : "false");
		const string blockedText = (blocked ? "true" : "false");
		const string reservedText = (reserved ? "true" : "false");
		const string orientationText = (orientation ? "true" : "false");
		command += ";occupied=" + occupiedText
			+ ";reserved=" + reservedText
			+ ";blocked=" + blockedText
			+ ";orientation=" + orientationText
			+ ";loconame=" + locoName;

		if (blocked)
		{
			if (reserved)
			{
				AddUpdate(command, Languages::TextTrackStatusIsBlockedAndReserved, trackName, locoName);
			}
			else if (occupied)
			{
				AddUpdate(command, Languages::TextTrackStatusIsBlockedAndOccupied, trackName);
			}
			else
			{
				AddUpdate(command, Languages::TextTrackStatusIsBlocked, trackName);
			}
		}
		else
		{
			if (reserved)
			{
				AddUpdate(command, Languages::TextTrackStatusIsReserved, trackName, locoName);;
			}
			else if (occupied)
			{
				AddUpdate(command, Languages::TextTrackStatusIsOccupied, trackName);
			}
			else
			{
				AddUpdate(command, Languages::TextTrackStatusIsFree, trackName);
			}
		}
	}

	void WebServer::TrackSettings(const TrackID trackID, const std::string& name)
	{
		stringstream command;
		command << "tracksettings;track=" << trackID;
		AddUpdate(command.str(), Languages::TextTrackUpdated, name);
	}

	void WebServer::TrackDelete(const TrackID trackID, const std::string& name)
	{
		stringstream command;
		command << "trackdelete;track=" << trackID;
		AddUpdate(command.str(), Languages::TextTrackDeleted, name);
	}

	void WebServer::SignalState(__attribute__((unused)) const ControlType controlType, const DataModel::Signal* signal)
	{
		const DataModel::AccessoryState state = signal->GetAccessoryState();
		string stateText;
		Languages::TextSelector text;
		switch(state)
		{
			case DataModel::SignalStateStop:
			default:
				stateText = "stop";
				text = Languages::TextSignalStateIsStop;
				break;

			case DataModel::SignalStateClear:
				stateText = "clear";
				text = Languages::TextSignalStateIsClear;
				break;

			case DataModel::SignalStateAspect2:
				stateText = "aspect2";
				text = Languages::TextSignalStateIsAspect2;
				break;

			case DataModel::SignalStateAspect3:
				stateText = "aspect3";
				text = Languages::TextSignalStateIsAspect3;
				break;

			case DataModel::SignalStateAspect4:
				stateText = "aspect4";
				text = Languages::TextSignalStateIsAspect4;
				break;

			case DataModel::SignalStateAspect5:
				stateText = "aspect5";
				text = Languages::TextSignalStateIsAspect5;
				break;

			case DataModel::SignalStateAspect6:
				stateText = "aspect6";
				text = Languages::TextSignalStateIsAspect6;
				break;
		}
		string signalIdText(to_string(signal->GetID()));
		string command = "signal;signal=" + signalIdText + ";state=" + stateText;
		AddUpdate(command, text, signal->GetName());
		string command2 = "trackstate;signal=" + signalIdText;
		TrackBaseState(command2, dynamic_cast<const DataModel::TrackBase*>(signal));
	}

	void WebServer::SignalSettings(const SignalID signalID, const std::string& name)
	{
		stringstream command;
		command << "signalsettings;signal=" << signalID;
		AddUpdate(command.str(), Languages::TextSignalUpdated, name);
	}

	void WebServer::SignalDelete(const SignalID signalID, const std::string& name)
	{
		stringstream command;
		command << "signaldelete;signal=" << signalID;
		AddUpdate(command.str(), Languages::TextSignalDeleted, name);
	}

	void WebServer::ClusterSettings(const ClusterID clusterID, const std::string& name)
	{
		stringstream command;
		command << "clustersettings;cluster=" << clusterID;
		AddUpdate(command.str(), Languages::TextClusterUpdated, name);
	}

	void WebServer::ClusterDelete(const ClusterID clusterID, const std::string& name)
	{
		stringstream command;
		command << "clusterdelete;cluster=" << clusterID;
		AddUpdate(command.str(), Languages::TextClusterDeleted, name);
	}

	void WebServer::LocoRelease(const LocoID locoID)
	{
		stringstream command;
		command << "locorelease;loco=" << locoID;
		AddUpdate(command.str(), Languages::TextLocoIsReleased, manager.GetLocoName(locoID));
	}

	void WebServer::RouteRelease(const RouteID routeID)
	{
		stringstream command;
		command << "routeRelease;route=" << routeID;
		AddUpdate(command.str(), Languages::TextRouteIsReleased, manager.GetRouteName(routeID));
	}

	void WebServer::LocoDestinationReached(const Loco* loco, const Route* route, const TrackBase* track)
	{
		string command("locoDestinationReached;loco=");
		command += to_string(loco->GetID());
		command += ";route=";
		command += to_string(route->GetID());
		command += ";";
		command += track->GetObjectIdentifier();
		AddUpdate(command, Languages::TextLocoHasReachedDestination, loco->GetName(), track->GetMyName(), route->GetName());
	}

	void WebServer::LocoStart(const LocoID locoID, const std::string& name)
	{
		stringstream command;
		command << "locoStart;loco=" << locoID;
		AddUpdate(command.str(), Languages::TextLocoIsInAutoMode, name);
	}

	void WebServer::LocoStop(const LocoID locoID, const std::string& name)
	{
		stringstream command;
		command << "locoStop;loco=" << locoID;
		AddUpdate(command.str(), Languages::TextLocoIsInManualMode, name);
	}

	void WebServer::LocoSettings(const LocoID locoID,
		const std::string& name,
		__attribute__((unused)) const std::string& matchKey)
	{
		stringstream command;
		command << "locosettings;loco=" << locoID;
		AddUpdate(command.str(), Languages::TextLocoUpdated, name);
	}

	void WebServer::LocoDelete(const LocoID locoID, const std::string& name)
	{
		stringstream command;
		command << "locodelete;loco=" << locoID;
		AddUpdate(command.str(), Languages::TextLocoDeleted, name);
	}

	void WebServer::LayerSettings(const LayerID layerID, const std::string& name)
	{
		stringstream command;
		command << "layersettings;layer=" << layerID;
		AddUpdate(command.str(), Languages::TextLayerUpdated, name);
	}

	void WebServer::LayerDelete(const LayerID layerID, const std::string& name)
	{
		stringstream command;
		command << "layerdelete;layer=" << layerID;
		AddUpdate(command.str(), Languages::TextLayerDeleted, name);
	}

	void WebServer::ProgramValue(const CvNumber cv, const CvValue value)
	{
		stringstream command;
		command << "dcccvvalue;cv=" << static_cast<int>(cv) << ";value=" << static_cast<int>(value);
		AddUpdate(command.str(), Languages::TextProgramReadValue , static_cast<int>(cv), static_cast<int>(value));
	}

	void WebServer::AddUpdate(const string& command, const string& status)
	{
		stringstream ss;
		ss << "data: command=" << command << ";status=" << status << "\r\n\r\n";
		std::lock_guard<std::mutex> lock(updateMutex);
		updates[++updateID] = ss.str();
		updates.erase(updateID - MaxUpdates);
	}

	bool WebServer::NextUpdate(unsigned int& updateIDClient, string& s)
	{
		std::lock_guard<std::mutex> lock(updateMutex);

		if (updateIDClient + MaxUpdates <= updateID)
		{
			updateIDClient = updateID - MaxUpdates + 1;
		}

		if (updates.count(updateIDClient) == 1)
		{
			s = updates.at(updateIDClient);
			return true;
		}

		return false;
	}

} // namespace WebServer
