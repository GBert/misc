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

#include <algorithm>
#include <cstring>		//memset
#include <netinet/in.h>
#include <signal.h>
#include <sstream>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

#include "DataTypes.h"
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
using DataModel::Street;
using DataModel::TrackBase;

namespace WebServer {

	WebServer::WebServer(Manager& manager, const unsigned short port)
	:	ControlInterface(ControlTypeWebserver),
		Network::TcpServer(port, "WebServer"),
		run(false),
		lastClientID(0),
		manager(manager),
		updateID(1)
	{
		Logger::Logger::GetLogger("Webserver")->Info(Languages::TextWebServerStarted);
		{
			std::lock_guard<std::mutex> lock(updateMutex);
			updates[updateID] = GetStatus(Languages::TextRailControlStarted);
		}
		run = true;
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
		Logger::Logger::GetLogger("Webserver")->Info(Languages::TextWebServerStopped);
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

	void WebServer::LocoDirection(__attribute__((unused)) const ControlType controlType, const DataModel::Loco* loco, const Direction direction)
	{
		stringstream command;
		command << "locodirection;loco=" << loco->GetID() << ";direction=" << (direction ? "true" : "false");
		AddUpdate(command.str(), direction ? Languages::TextLocoDirectionIsRight : Languages::TextLocoDirectionIsLeft, loco->GetName());
	}

	void WebServer::LocoFunction(__attribute__((unused)) const ControlType controlType, const DataModel::Loco* loco, const Function function, const DataModel::LocoFunctions::FunctionState state)
	{
		stringstream command;
		command << "locofunction;loco=" << loco->GetID() << ";function=" << static_cast<unsigned int>(function) << ";on=" << (state ? "true" : "false");
		AddUpdate(command.str(), state ? Languages::TextLocoFunctionIsOn : Languages::TextLocoFunctionIsOff, loco->GetName(), function);
	}

	void WebServer::AccessoryState(__attribute__((unused)) const ControlType controlType, const DataModel::Accessory* accessory, const DataModel::AccessoryState state)
	{
		stringstream command;
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

	void WebServer::StreetSettings(const StreetID streetID, const std::string& name)
	{
		stringstream command;
		command << "streetsettings;street=" << streetID;
		AddUpdate(command.str(), Languages::TextStreetUpdated, name);
	}

	void WebServer::StreetDelete(const StreetID streetID, const std::string& name)
	{
		stringstream command;
		command << "streetdelete;street=" << streetID;
		AddUpdate(command.str(), Languages::TextStreetDeleted, name);
	}

	void WebServer::SwitchState(__attribute__((unused)) const ControlType controlType, const DataModel::Switch* mySwitch, const DataModel::AccessoryState state)
	{
		stringstream command;
		command << "switch;switch=" << mySwitch->GetID() << ";state=" << (state ? "straight" : "turnout");
		AddUpdate(command.str(), state ? Languages::TextSwitchStateIsStraight : Languages::TextSwitchStateIsTurnout, mySwitch->GetName());
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
		stringstream command;
		command << "trackstate;track=" << track->GetID();
		TrackBaseState(command, dynamic_cast<const DataModel::TrackBase*>(track));
	}

	void WebServer::TrackBaseState(stringstream& command, const DataModel::TrackBase* track)
	{
		const DataModel::Loco* loco = manager.GetLoco(track->GetLocoDelayed());
		const bool reserved = loco != nullptr;
		const string& trackName = track->GetMyName();
		const string& locoName = reserved ? loco->GetName() : "";
		const bool occupied = track->GetFeedbackStateDelayed() == DataModel::Feedback::FeedbackStateOccupied;
		const bool blocked = track->GetBlocked();
		const Direction direction = track->GetLocoDirection();
		const string occupiedText = (occupied ? "true" : "false");
		const string blockedText = (blocked ? "true" : "false");
		const string reservedText = (reserved ? "true" : "false");
		const string directionText = (direction ? "true" : "false");
		command << ";occupied=" << occupiedText
			<< ";reserved=" << reservedText
			<< ";blocked=" << blockedText
			<< ";direction=" << directionText
			<< ";loconame=" << locoName;

		if (blocked)
		{
			if (reserved)
			{
				AddUpdate(command.str(), Languages::TextTrackStatusIsBlockedAndReserved, trackName, locoName);
			}
			else if (occupied)
			{
				AddUpdate(command.str(), Languages::TextTrackStatusIsBlockedAndOccupied, trackName);
			}
			else
			{
				AddUpdate(command.str(), Languages::TextTrackStatusIsBlocked, trackName);
			}
		}
		else
		{
			if (reserved)
			{
				AddUpdate(command.str(), Languages::TextTrackStatusIsReserved, trackName, locoName);;
			}
			else if (occupied)
			{
				AddUpdate(command.str(), Languages::TextTrackStatusIsOccupied, trackName);
			}
			else
			{
				AddUpdate(command.str(), Languages::TextTrackStatusIsFree, trackName);
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
		stringstream command;
		DataModel::AccessoryState state = signal->GetAccessoryState();
		command << "signal;signal=" << signal->GetID() << ";state=" << (state ? "green" : "red");
		AddUpdate(command.str(), state ? Languages::TextSignalStateIsGreen : Languages::TextSignalStateIsRed, signal->GetName());
		stringstream command2;
		command2 << "trackstate;signal=" << signal->GetID();
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

	void WebServer::LocoRelease(const LocoID locoID)
	{
		stringstream command;
		command << "locorelease;loco=" << locoID;
		AddUpdate(command.str(), Languages::TextLocoIsReleased, manager.GetLocoName(locoID));
	}

	void WebServer::StreetRelease(const StreetID streetID)
	{
		stringstream command;
		command << "streetRelease;street=" << streetID;
		AddUpdate(command.str(), Languages::TextStreetIsReleased, manager.GetStreetName(streetID));
	}

	void WebServer::LocoDestinationReached(const Loco* loco, const Street* street, const TrackBase* track)
	{
		string command("locoDestinationReached;loco=");
		command += to_string(loco->GetID());
		command += ";street=";
		command += to_string(street->GetID());
		command += ";";
		command += track->GetObjectIdentifier();
		AddUpdate(command, Languages::TextLocoHasReachedDestination, loco->GetName(), track->GetMyName(), street->GetName());
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

	void WebServer::LocoSettings(const LocoID locoID, const std::string& name)
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

}; // namespace WebServer
