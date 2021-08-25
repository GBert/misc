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

#include <map>
#include <string>
#include <vector>

#include "DataModel/DataModel.h"
#include "DataModel/ObjectIdentifier.h"
#include "WebServer/HtmlTagButton.h"
#include "WebServer/HtmlTagButtonCancel.h"
#include "WebServer/HtmlTagButtonCommandWide.h"
#include "WebServer/HtmlTagButtonOK.h"
#include "WebServer/HtmlTagButtonPopupWide.h"
#include "WebServer/HtmlTagInputCheckboxWithLabel.h"
#include "WebServer/HtmlTagInputHidden.h"
#include "WebServer/HtmlTagInputIntegerWithLabel.h"
#include "WebServer/HtmlTagInputTextWithLabel.h"
#include "WebServer/HtmlTagRoute.h"
#include "WebServer/HtmlTagSelect.h"
#include "WebServer/HtmlTagSelectOrientation.h"
#include "WebServer/HtmlTagSelectWithLabel.h"
#include "WebServer/WebClient.h"
#include "WebServer/WebClientStatic.h"

using namespace DataModel;
using LayoutPosition = DataModel::LayoutItem::LayoutPosition;
using LayoutItemSize = DataModel::LayoutItem::LayoutItemSize;
using LayoutRotation = DataModel::LayoutItem::LayoutRotation;
using Visible = DataModel::LayoutItem::Visible;
using std::map;
using std::string;
using std::to_string;
using std::vector;

namespace WebServer
{
	void WebClientRoute::HandleRouteGet(const map<string, string>& arguments)
	{
		RouteID routeID = Utils::Utils::GetIntegerMapEntry(arguments, "route");
		const DataModel::Route* route = manager.GetRoute(routeID);
		if (route == nullptr || route->GetVisible() == DataModel::LayoutItem::VisibleNo)
		{
			client.ReplyHtmlWithHeader(HtmlTag());
			return;
		}
		client.ReplyHtmlWithHeader(HtmlTagRoute(route));
	}

	void WebClientRoute::HandleRouteEdit(const map<string, string>& arguments)
	{
		HtmlTag content;
		RouteID routeID = Utils::Utils::GetIntegerMapEntry(arguments, "route", RouteNone);
		string name = Languages::GetText(Languages::TextNew);
		Delay delay = Route::DefaultDelay;
		Route::PushpullType pushpull = Route::PushpullTypeBoth;
		Length minTrainLength = 0;
		Length maxTrainLength = 0;
		vector<Relation*> relationsAtLock;
		vector<Relation*> relationsAtUnlock;
		LayoutPosition posx = Utils::Utils::GetIntegerMapEntry(arguments, "posx", 0);
		LayoutPosition posy = Utils::Utils::GetIntegerMapEntry(arguments, "posy", 0);
		LayoutPosition posz = Utils::Utils::GetIntegerMapEntry(arguments, "posz", LayerUndeletable);
		DataModel::LayoutItem::Visible visible = static_cast<Visible>(Utils::Utils::GetBoolMapEntry(arguments, "visible", routeID == RouteNone && ((posx || posy) && posz >= LayerUndeletable) ? DataModel::LayoutItem::VisibleYes : DataModel::LayoutItem::VisibleNo));
		Automode automode = static_cast<Automode>(Utils::Utils::GetBoolMapEntry(arguments, "automode", AutomodeNo));
		ObjectIdentifier fromTrack = Utils::Utils::GetStringMapEntry(arguments, "fromtrack");
		Orientation fromOrientation = static_cast<Orientation>(Utils::Utils::GetBoolMapEntry(arguments, "fromorientation", OrientationRight));
		ObjectIdentifier toTrack = Utils::Utils::GetStringMapEntry(arguments, "totrack");
		Orientation toOrientation = static_cast<Orientation>(Utils::Utils::GetBoolMapEntry(arguments, "toorientation", OrientationRight));
		Route::Speed speed = static_cast<Route::Speed>(Utils::Utils::GetIntegerMapEntry(arguments, "speed", Route::SpeedTravel));
		FeedbackID feedbackIdReduced = Utils::Utils::GetIntegerMapEntry(arguments, "feedbackreduced", FeedbackNone);
		FeedbackID feedbackIdCreep = Utils::Utils::GetIntegerMapEntry(arguments, "feedbackcreep", FeedbackNone);
		FeedbackID feedbackIdStop = Utils::Utils::GetIntegerMapEntry(arguments, "feedbackstop", FeedbackNone);
		FeedbackID feedbackIdOver = Utils::Utils::GetIntegerMapEntry(arguments, "feedbackover", FeedbackNone);
		Pause waitAfterRelease = Utils::Utils::GetIntegerMapEntry(arguments, "waitafterrelease", 0);
		if (routeID > RouteNone)
		{
			const DataModel::Route* route = manager.GetRoute(routeID);
			if (route != nullptr)
			{
				name = route->GetName();
				delay = route->GetDelay();
				pushpull = route->GetPushpull();
				minTrainLength = route->GetMinTrainLength();
				maxTrainLength = route->GetMaxTrainLength();
				relationsAtLock = route->GetRelationsAtLock();
				relationsAtUnlock = route->GetRelationsAtUnlock();
				visible = route->GetVisible();
				posx = route->GetPosX();
				posy = route->GetPosY();
				posz = route->GetPosZ();
				automode = route->GetAutomode();
				fromTrack = route->GetFromTrack();
				fromOrientation = route->GetFromOrientation();
				toTrack = route->GetToTrack();
				toOrientation = route->GetToOrientation();
				speed = route->GetSpeed();
				feedbackIdReduced = route->GetFeedbackIdReduced();
				feedbackIdCreep = route->GetFeedbackIdCreep();
				feedbackIdStop = route->GetFeedbackIdStop();
				feedbackIdOver = route->GetFeedbackIdOver();
				waitAfterRelease = route->GetWaitAfterRelease();
			}
		}

		content.AddChildTag(HtmlTag("h1").AddContent(name).AddId("popup_title"));
		HtmlTag tabMenu("div");
		tabMenu.AddChildTag(client.HtmlTagTabMenuItem("basic", Languages::TextBasic, true));
		tabMenu.AddChildTag(client.HtmlTagTabMenuItem("relationatlock", Languages::TextAtLock));
		tabMenu.AddChildTag(client.HtmlTagTabMenuItem("relationatunlock", Languages::TextAtUnlock));
		tabMenu.AddChildTag(client.HtmlTagTabMenuItem("position", Languages::TextPosition));
		tabMenu.AddChildTag(client.HtmlTagTabMenuItem("automode", Languages::TextAutomode));
		content.AddChildTag(tabMenu);

		HtmlTag formContent("form");
		formContent.AddId("editform");
		formContent.AddChildTag(HtmlTagInputHidden("cmd", "routesave"));
		formContent.AddChildTag(HtmlTagInputHidden("route", to_string(routeID)));

		HtmlTag basicContent("div");
		basicContent.AddId("tab_basic");
		basicContent.AddClass("tab_content");
		basicContent.AddChildTag(HtmlTagInputTextWithLabel("name", Languages::TextName, name).AddAttribute("onkeyup", "updateName();"));
		basicContent.AddChildTag(HtmlTagInputIntegerWithLabel("delay", Languages::TextWaitingTimeBetweenMembers, delay, 1, USHRT_MAX));
		formContent.AddChildTag(basicContent);

		HtmlTag relationDivAtLock("div");
		relationDivAtLock.AddId("relationatlock");
		Priority priorityAtLock = 1;
		for (auto relation : relationsAtLock)
		{
			relationDivAtLock.AddChildTag(HtmlTagRelation("atlock", to_string(relation->GetPriority()), relation->ObjectType2(), relation->ObjectID2(), relation->GetData()));
			priorityAtLock = relation->GetPriority() + 1;
		}
		relationDivAtLock.AddChildTag(HtmlTagInputHidden("relationcounteratlock", to_string(priorityAtLock)));
		relationDivAtLock.AddChildTag(HtmlTag("div").AddId("new_atlock_priority_" + to_string(priorityAtLock)));

		HtmlTag relationContentAtLock("div");
		relationContentAtLock.AddId("tab_relationatlock");
		relationContentAtLock.AddClass("tab_content");
		relationContentAtLock.AddClass("hidden");
		relationContentAtLock.AddChildTag(relationDivAtLock);
		HtmlTagButton newButtonAtLock(Languages::TextNew, "newrelationatlock");
		newButtonAtLock.AddAttribute("onclick", "addRelation('atlock');return false;");
		newButtonAtLock.AddClass("wide_button");
		relationContentAtLock.AddChildTag(newButtonAtLock);
		relationContentAtLock.AddChildTag(HtmlTag("br"));
		formContent.AddChildTag(relationContentAtLock);

		HtmlTag relationDivAtUnlock("div");
		relationDivAtUnlock.AddId("relationatunlock");
		Priority priorityAtUnlock = 1;
		for (auto relation : relationsAtUnlock)
		{
			relationDivAtUnlock.AddChildTag(HtmlTagRelation("atunlock", to_string(relation->GetPriority()), relation->ObjectType2(), relation->ObjectID2(), relation->GetData()));
			priorityAtUnlock = relation->GetPriority() + 1;
		}
		relationDivAtUnlock.AddChildTag(HtmlTagInputHidden("relationcounteratunlock", to_string(priorityAtUnlock)));
		relationDivAtUnlock.AddChildTag(HtmlTag("div").AddId("new_atunlock_priority_" + to_string(priorityAtUnlock)));

		HtmlTag relationContentAtUnlock("div");
		relationContentAtUnlock.AddId("tab_relationatunlock");
		relationContentAtUnlock.AddClass("tab_content");
		relationContentAtUnlock.AddClass("hidden");
		relationContentAtUnlock.AddChildTag(relationDivAtUnlock);
		HtmlTagButton newButtonAtUnlock(Languages::TextNew, "newrelationatunlock");
		newButtonAtUnlock.AddAttribute("onclick", "addRelation('atunlock');return false;");
		newButtonAtUnlock.AddClass("wide_button");
		relationContentAtUnlock.AddChildTag(newButtonAtUnlock);
		relationContentAtUnlock.AddChildTag(HtmlTag("br"));
		formContent.AddChildTag(relationContentAtUnlock);

		formContent.AddChildTag(client.HtmlTagTabPosition(posx, posy, posz, visible));

		HtmlTag automodeContent("div");
		automodeContent.AddId("tab_automode");
		automodeContent.AddClass("tab_content");
		automodeContent.AddClass("hidden");

		HtmlTagInputCheckboxWithLabel checkboxAutomode("automode", Languages::TextAutomode, "automode", static_cast<bool>(automode));
		checkboxAutomode.AddId("automode");
		checkboxAutomode.AddAttribute("onchange", "onChangeCheckboxShowHide('automode', 'tracks');");
		automodeContent.AddChildTag(checkboxAutomode);

		HtmlTag tracksDiv("div");
		tracksDiv.AddId("tracks");
		if (automode == AutomodeNo)
		{
			tracksDiv.AddAttribute("hidden");
		}
		tracksDiv.AddChildTag(HtmlTagSelectTrack("from", Languages::TextStartTrack, fromTrack, fromOrientation));
		tracksDiv.AddChildTag(HtmlTagSelectTrack("to", Languages::TextDestinationTrack, toTrack, toOrientation, "updateFeedbacksOfTrack(); return false;"));
		map<Route::Speed,Languages::TextSelector> speedOptions;
		speedOptions[Route::SpeedTravel] = Languages::TextTravelSpeed;
		speedOptions[Route::SpeedReduced] = Languages::TextReducedSpeed;
		speedOptions[Route::SpeedCreeping] = Languages::TextCreepingSpeed;
		tracksDiv.AddChildTag(HtmlTagSelectWithLabel("speed", Languages::TextSpeed, speedOptions, speed));
		HtmlTag feedbackDiv("div");
		feedbackDiv.AddId("feedbacks");
		feedbackDiv.AddChildTag(HtmlTagSelectFeedbacksOfTrack(toTrack, feedbackIdReduced, feedbackIdCreep, feedbackIdStop, feedbackIdOver));
		tracksDiv.AddChildTag(feedbackDiv);
		map<Route::PushpullType,Languages::TextSelector> pushpullOptions;
		pushpullOptions[Route::PushpullTypeNo] = Languages::TextNoPushPull;
		pushpullOptions[Route::PushpullTypeBoth] = Languages::TextAllTrains;
		pushpullOptions[Route::PushpullTypeOnly] = Languages::TextPushPullOnly;
		tracksDiv.AddChildTag(HtmlTagSelectWithLabel("pushpull", Languages::TextAllowedTrains, pushpullOptions, pushpull));
		tracksDiv.AddChildTag(HtmlTagInputIntegerWithLabel("mintrainlength", Languages::TextMinTrainLength, minTrainLength, 0, 99999));
		tracksDiv.AddChildTag(HtmlTagInputIntegerWithLabel("maxtrainlength", Languages::TextMaxTrainLength, maxTrainLength, 0, 99999));
		tracksDiv.AddChildTag(HtmlTagInputIntegerWithLabel("waitafterrelease", Languages::TextWaitAfterRelease, waitAfterRelease, 0, 300));
		automodeContent.AddChildTag(tracksDiv);
		formContent.AddChildTag(automodeContent);

		content.AddChildTag(HtmlTag("div").AddClass("popup_content").AddChildTag(formContent));
		content.AddChildTag(HtmlTagButtonCancel());
		content.AddChildTag(HtmlTagButtonOK());
		client.ReplyHtmlWithHeader(content);
	}

	void WebClientRoute::HandleRouteSave(const map<string, string>& arguments)
	{
		RouteID routeID = Utils::Utils::GetIntegerMapEntry(arguments, "route", RouteNone);
		string name = Utils::Utils::GetStringMapEntry(arguments, "name");
		Delay delay = static_cast<Delay>(Utils::Utils::GetIntegerMapEntry(arguments, "delay"));
		Route::PushpullType pushpull = static_cast<Route::PushpullType>(Utils::Utils::GetIntegerMapEntry(arguments, "pushpull", Route::PushpullTypeBoth));
		Length mintrainlength = static_cast<Length>(Utils::Utils::GetIntegerMapEntry(arguments, "mintrainlength", 0));
		Length maxtrainlength = static_cast<Length>(Utils::Utils::GetIntegerMapEntry(arguments, "maxtrainlength", 0));
		Visible visible = static_cast<Visible>(Utils::Utils::GetBoolMapEntry(arguments, "visible"));
		LayoutPosition posx = Utils::Utils::GetIntegerMapEntry(arguments, "posx", 0);
		LayoutPosition posy = Utils::Utils::GetIntegerMapEntry(arguments, "posy", 0);
		LayoutPosition posz = Utils::Utils::GetIntegerMapEntry(arguments, "posz", 0);
		Automode automode = static_cast<Automode>(Utils::Utils::GetBoolMapEntry(arguments, "automode"));
		ObjectIdentifier fromTrack = Utils::Utils::GetStringMapEntry(arguments, "fromtrack");
		Orientation fromOrientation = static_cast<Orientation>(Utils::Utils::GetBoolMapEntry(arguments, "fromorientation", OrientationRight));
		ObjectIdentifier toTrack = Utils::Utils::GetStringMapEntry(arguments, "totrack");
		Orientation toOrientation = static_cast<Orientation>(Utils::Utils::GetBoolMapEntry(arguments, "toorientation", OrientationRight));
		Route::Speed speed = static_cast<Route::Speed>(Utils::Utils::GetIntegerMapEntry(arguments, "speed", Route::SpeedTravel));
		FeedbackID feedbackIdReduced = Utils::Utils::GetIntegerMapEntry(arguments, "feedbackreduced", FeedbackNone);
		FeedbackID feedbackIdCreep = Utils::Utils::GetIntegerMapEntry(arguments, "feedbackcreep", FeedbackNone);
		FeedbackID feedbackIdStop = Utils::Utils::GetIntegerMapEntry(arguments, "feedbackstop", FeedbackNone);
		FeedbackID feedbackIdOver = Utils::Utils::GetIntegerMapEntry(arguments, "feedbackover", FeedbackNone);
		Pause waitAfterRelease = Utils::Utils::GetIntegerMapEntry(arguments, "waitafterrelease", 0);

		Priority relationCountAtLock = Utils::Utils::GetIntegerMapEntry(arguments, "relationcounteratlock", 0);
		Priority relationCountAtUnlock = Utils::Utils::GetIntegerMapEntry(arguments, "relationcounteratunlock", 0);

		vector<Relation*> relationsAtLock;
		Priority priorityAtLock = 1;
		for (Priority relationId = 1; relationId <= relationCountAtLock; ++relationId)
		{
			string priorityString = to_string(relationId);
			ObjectType objectType = static_cast<ObjectType>(Utils::Utils::GetIntegerMapEntry(arguments, "relation_atlock_" + priorityString + "_type"));
			ObjectID objectId = Utils::Utils::GetIntegerMapEntry(arguments, "relation_atlock_" + priorityString + "_id", ObjectNone);
			if (objectId == 0 && objectType != ObjectTypeLoco && objectType != ObjectTypePause)
			{
				continue;
			}
			if (objectId == fromTrack.GetObjectID() && objectType == fromTrack.GetObjectType())
			{
				continue;
			}
			if (objectId == toTrack.GetObjectID() && objectType == toTrack.GetObjectType())
			{
				continue;
			}
			unsigned short state = Utils::Utils::GetIntegerMapEntry(arguments, "relation_atlock_" + priorityString + "_state");
			relationsAtLock.push_back(new Relation(&manager,
				ObjectIdentifier(ObjectTypeRoute, routeID),
				ObjectIdentifier(objectType, objectId),
				Relation::TypeRouteAtLock,
				priorityAtLock,
				state));
			++priorityAtLock;
		}

		vector<Relation*> relationsAtUnlock;
		Priority priorityAtUnlock = 1;
		for (Priority relationId = 1; relationId <= relationCountAtUnlock; ++relationId)
		{
			string priorityString = to_string(relationId);
			ObjectType objectType = static_cast<ObjectType>(Utils::Utils::GetIntegerMapEntry(arguments, "relation_atunlock_" + priorityString + "_type"));
			ObjectID objectId = Utils::Utils::GetIntegerMapEntry(arguments, "relation_atunlock_" + priorityString + "_id", ObjectNone);
			if (objectId == 0 && objectType != ObjectTypeLoco && objectType != ObjectTypePause)
			{
				continue;
			}
			if (objectId == fromTrack.GetObjectID() && objectType == fromTrack.GetObjectType())
			{
				continue;
			}
			if (objectId == toTrack.GetObjectID() && objectType == toTrack.GetObjectType())
			{
				continue;
			}
			unsigned char state = Utils::Utils::GetIntegerMapEntry(arguments, "relation_atunlock_" + priorityString + "_state");
			relationsAtUnlock.push_back(new Relation(&manager,
				ObjectIdentifier(ObjectTypeRoute, routeID),
				ObjectIdentifier(objectType, objectId),
				Relation::TypeRouteAtUnlock,
				priorityAtUnlock,
				state));
			++priorityAtUnlock;
		}

		string result;
		if (!manager.RouteSave(routeID,
			name,
			delay,
			pushpull,
			mintrainlength,
			maxtrainlength,
			relationsAtLock,
			relationsAtUnlock,
			visible,
			posx,
			posy,
			posz,
			automode,
			fromTrack,
			fromOrientation,
			toTrack,
			toOrientation,
			speed,
			feedbackIdReduced,
			feedbackIdCreep,
			feedbackIdStop,
			feedbackIdOver,
			waitAfterRelease,
			result))
		{
			client.ReplyResponse(client.ResponseError, result);
			return;
		}
		client.ReplyResponse(client.ResponseInfo, Languages::TextRouteSaved, name);
	}

	void WebClientRoute::HandleRouteAskDelete(const map<string, string>& arguments)
	{
		RouteID routeID = Utils::Utils::GetIntegerMapEntry(arguments, "route", RouteNone);

		if (routeID == RouteNone)
		{
			client.ReplyHtmlWithHeaderAndParagraph(Languages::TextRouteDoesNotExist);
			return;
		}

		const DataModel::Route* route = manager.GetRoute(routeID);
		if (route == nullptr)
		{
			client.ReplyHtmlWithHeaderAndParagraph(Languages::TextRouteDoesNotExist);
			return;
		}

		HtmlTag content;
		const string& routeName = route->GetName();
		content.AddContent(HtmlTag("h1").AddContent(Languages::TextDeleteRoute));
		content.AddContent(HtmlTag("p").AddContent(Languages::TextAreYouSureToDelete, routeName));
		content.AddContent(HtmlTag("form").AddId("editform")
			.AddContent(HtmlTagInputHidden("cmd", "routedelete"))
			.AddContent(HtmlTagInputHidden("route", to_string(routeID))
			));
		content.AddContent(HtmlTagButtonCancel());
		content.AddContent(HtmlTagButtonOK());
		client.ReplyHtmlWithHeader(content);
	}

	void WebClientRoute::HandleRouteDelete(const map<string, string>& arguments)
	{
		RouteID routeID = Utils::Utils::GetIntegerMapEntry(arguments, "route", RouteNone);
		const DataModel::Route* route = manager.GetRoute(routeID);
		if (route == nullptr)
		{
			client.ReplyResponse(client.ResponseError, Languages::TextRouteDoesNotExist);
			return;
		}

		string name = route->GetName();
		string result;
		if (!manager.RouteDelete(routeID, result))
		{
			client.ReplyResponse(client.ResponseError, result);
			return;
		}

		client.ReplyResponse(client.ResponseInfo, Languages::TextRouteDeleted, name);
	}

	void WebClientRoute::HandleRouteList()
	{
		HtmlTag content;
		content.AddChildTag(HtmlTag("h1").AddContent(Languages::TextRoutes));
		HtmlTag table("table");
		const map<string,DataModel::Route*> routeList = manager.RouteListByName();
		map<string,string> routeArgument;
		for (auto& route : routeList)
		{
			HtmlTag row("tr");
			row.AddChildTag(HtmlTag("td").AddContent(route.first));
			const string& routeIdString = to_string(route.second->GetID());
			routeArgument["route"] = routeIdString;
			row.AddChildTag(HtmlTag("td").AddChildTag(HtmlTagButtonPopupWide(Languages::TextEdit, "routeedit_list_" + routeIdString, routeArgument)));
			row.AddChildTag(HtmlTag("td").AddChildTag(HtmlTagButtonPopupWide(Languages::TextDelete, "routeaskdelete_" + routeIdString, routeArgument)));
			if (route.second->IsInUse())
			{
				row.AddChildTag(HtmlTag("td").AddChildTag(HtmlTagButtonCommandWide(Languages::TextRelease, "routerelease_" + routeIdString, routeArgument, "hideElement('b_routerelease_" + routeIdString + "');")));
			}
			table.AddChildTag(row);
		}
		content.AddChildTag(HtmlTag("div").AddClass("popup_content").AddChildTag(table));
		content.AddChildTag(HtmlTagButtonCancel());
		content.AddChildTag(HtmlTagButtonPopupWide(Languages::TextNew, "routeedit_0"));
		client.ReplyHtmlWithHeader(content);
	}

	void WebClientRoute::HandleRouteExecute(const map<string, string>& arguments)
	{
		RouteID routeID = Utils::Utils::GetIntegerMapEntry(arguments, "route", RouteNone);
		manager.RouteExecuteAsync(logger, routeID);
		client.ReplyHtmlWithHeaderAndParagraph("Route executed");
	}

	void WebClientRoute::HandleRouteRelease(const map<string, string>& arguments)
	{
		RouteID routeID = Utils::Utils::GetIntegerMapEntry(arguments, "route");
		bool ret = manager.RouteRelease(routeID);
		client.ReplyHtmlWithHeaderAndParagraph(ret ? "Route released" : "Route not released");
	}

	void WebClientRoute::HandleRelationAdd(const map<string, string>& arguments)
	{
		string priorityString = Utils::Utils::GetStringMapEntry(arguments, "priority", "1");
		string type = Utils::Utils::GetStringMapEntry(arguments, "type", "atunlock");
		if (type.compare("atunlock") != 0)
		{
			type = "atlock";
		}
		Priority priority = Utils::Utils::StringToInteger(priorityString, 1);
		HtmlTag container;
		container.AddChildTag(HtmlTagRelation(type, priorityString));
		container.AddChildTag(HtmlTag("div").AddId("new_" + type + "_priority_" + to_string(priority + 1)));
		client.ReplyHtmlWithHeader(container);
	}

	void WebClientRoute::HandleRelationObject(const map<string, string>& arguments)
	{
		const string priority = Utils::Utils::GetStringMapEntry(arguments, "priority");
		const string type = Utils::Utils::GetStringMapEntry(arguments, "type");
		const string name = "relation_" + type + "_" + priority;
		const ObjectType objectType = static_cast<ObjectType>(Utils::Utils::GetIntegerMapEntry(arguments, "objecttype"));
		client.ReplyHtmlWithHeader(HtmlTagRelationObject(name, objectType));
	}

	void WebClientRoute::HandleRelationSwitchStates(const map<string, string>& arguments)
	{
		const string name = Utils::Utils::GetStringMapEntry(arguments, "name");
		const SwitchID switchId = static_cast<SwitchID>(Utils::Utils::GetIntegerMapEntry(arguments, "switch"));
		client.ReplyHtmlWithHeader(HtmlTagRelationSwitchState(name, switchId));
	}

	HtmlTag WebClientRoute::HtmlTagRelation(const string& type,
		const string& priority,
		const ObjectType objectType,
	    const ObjectID objectId,
	    const DataModel::Relation::Data state)
	{
		HtmlTag content("div");
		string name = "relation_" + type + "_" + priority;
		content.AddId(name);
		HtmlTagButton deleteButton(Languages::TextDelete, "delete_" + name);
		deleteButton.AddAttribute("onclick", "deleteElement('" + name + "');return false;");
		deleteButton.AddClass("wide_button");
		content.AddChildTag(deleteButton);

		map<ObjectType, Languages::TextSelector> objectTypeOptions;
		objectTypeOptions[ObjectTypeAccessory] = Languages::TextAccessory;
		objectTypeOptions[ObjectTypeSignal] = Languages::TextSignal;
		objectTypeOptions[ObjectTypeSwitch] = Languages::TextSwitch;
		objectTypeOptions[ObjectTypeTrack] = Languages::TextTrack;
		objectTypeOptions[ObjectTypeRoute] = Languages::TextRoute;
		objectTypeOptions[ObjectTypeLoco] = Languages::TextLoco;
		objectTypeOptions[ObjectTypePause] = Languages::TextPause;
		HtmlTagSelect select(name + "_type", objectTypeOptions, objectType);
		select.AddClass("select_relation_objecttype");
		select.AddAttribute("onchange", "loadRelationObject('" + type + "', " + priority + ");return false;");
		content.AddChildTag(select);
		HtmlTag contentObject("div");
		contentObject.AddId(name + "_object");
		contentObject.AddClass("inline-block");
		contentObject.AddChildTag(HtmlTagRelationObject(name, objectType, objectId, state));
		content.AddChildTag(contentObject);
		return content;
	}

	HtmlTag WebClientRoute::HtmlTagRelationSwitchState(const string& name,
		const SwitchID switchId,
		const DataModel::Relation::Data data)
	{
		map<DataModel::AccessoryState,Languages::TextSelector> stateOptions;
		Switch* mySwitch = manager.GetSwitch(switchId);
		if (mySwitch != nullptr)
		{
			stateOptions = mySwitch->GetStateOptions();
		}

		return HtmlTagSelect(name + "_state", stateOptions, static_cast<DataModel::AccessoryState>(data)).AddClass("select_relation_state");
	}

	HtmlTag WebClientRoute::HtmlTagRelationObject(const string& name,
		const ObjectType objectType,
		const ObjectID objectId,
		const DataModel::Relation::Data data)
	{
		HtmlTag content;
		switch (objectType)
		{
			case ObjectTypeSwitch:
			{
				std::map<string, AccessoryConfig> switches = manager.SwitchConfigByName();
				map<string, SwitchID> switchOptions;
				for (auto& mySwitch : switches)
				{
					switchOptions[mySwitch.first] = mySwitch.second.GetObjectIdentifier().GetObjectID();
				}
				SwitchID switchId = objectId;
				if (switchId == SwitchNone && switchOptions.size() > 0)
				{
					switchId = switchOptions.begin()->second;
				}
				HtmlTagSelect selectSwitch(name + "_id", switchOptions, switchId);
				selectSwitch.AddClass("select_relation_id");
				selectSwitch.AddAttribute("onchange", "loadRelationObjectStates('switch', '" + name + "', '" + to_string(switchId) + "');return false;");
				content.AddChildTag(selectSwitch);

				HtmlTag contentState("div");
				contentState.AddId(name + "_state");
				contentState.AddClass("inline-block");
				contentState.AddChildTag(HtmlTagRelationSwitchState(name, switchId, data));
				content.AddChildTag(contentState);
				return content;
			}

			case ObjectTypeSignal:
			{
				std::map<string, AccessoryConfig> signals = manager.SignalConfigByName();
				map<string, SignalID> signalOptions;
				for (auto& signal : signals)
				{
					signalOptions[signal.first] = signal.second.GetObjectIdentifier().GetObjectID();
				}
				SignalID signalId = objectId;
				if (signalId == SignalNone && signalOptions.size() > 0)
				{
					signalId = signalOptions.begin()->second;
				}
				HtmlTagSelect selectSignal(name + "_id", signalOptions, signalId);
				selectSignal.AddClass("select_relation_id");
				selectSignal.AddAttribute("onchange", "loadRelationObjectStates('signal', '" + name + "', '" + to_string(signalId) + "');return false;");
				content.AddChildTag(selectSignal);

				HtmlTag contentState("div");
				contentState.AddId(name + "_state");
				contentState.AddClass("inline-block");
				contentState.AddChildTag(client.HtmlTagRelationSignalState(name, signalId, data));
				content.AddChildTag(contentState);
				return content;
			}

			case ObjectTypeAccessory:
			{
				std::map<string, AccessoryConfig> accessories = manager.AccessoryConfigByName();
				map<string, AccessoryID> accessoryOptions;
				for (auto& accessory : accessories)
				{
					accessoryOptions[accessory.first] = accessory.second.GetObjectIdentifier().GetObjectID();
				}
				content.AddChildTag(HtmlTagSelect(name + "_id", accessoryOptions, objectId).AddClass("select_relation_id"));

				map<DataModel::AccessoryState,Languages::TextSelector> stateOptions;
				stateOptions[DataModel::AccessoryStateOn] = Languages::TextOn;
				stateOptions[DataModel::AccessoryStateOff] = Languages::TextOff;
				content.AddChildTag(HtmlTagSelect(name + "_state", stateOptions, static_cast<DataModel::AccessoryState>(data)).AddClass("select_relation_state"));
				return content;
			}

			case ObjectTypeTrack:
			{
				std::map<string, Track*> tracks = manager.TrackListByName();
				map<string, TrackID> trackOptions;
				for (auto& track : tracks)
				{
					trackOptions[track.first] = track.second->GetID();
				}
				content.AddChildTag(HtmlTagSelect(name + "_id", trackOptions, objectId).AddClass("select_relation_id"));

				content.AddChildTag(HtmlTagSelectOrientation(name + "_state", static_cast<Orientation>(data)).AddClass("select_relation_state"));
				return content;
			}

			case ObjectTypeRoute:
			{
				std::map<string, Route*> routes = manager.RouteListByName();
				map<string, RouteID> routeOptions;
				for (auto& track : routes)
				{
					routeOptions[track.first] = track.second->GetID();
				}
				content.AddChildTag(HtmlTagSelect(name + "_id", routeOptions, objectId).AddClass("select_relation_id"));
				return content;
			}

			case ObjectTypeLoco:
			{
				map<string,string> functionOptions;
				for (DataModel::LocoFunctionNr function = 0; function < NumberOfLocoFunctions; ++function)
				{
					functionOptions[Utils::Utils::ToStringWithLeadingZeros(function, 3)] = "F" + to_string(function);
				}

				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconShuntingMode, 3)] = Languages::GetText(Languages::TextLocoFunctionIconShuntingMode);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconInertia, 3)] = Languages::GetText(Languages::TextLocoFunctionIconInertia);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconLight, 3)] = Languages::GetText(Languages::TextLocoFunctionIconLight);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconHeadlightLowBeamForward, 3)] = Languages::GetText(Languages::TextLocoFunctionIconHeadlightLowBeamForward);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconHeadlightLowBeamReverse, 3)] = Languages::GetText(Languages::TextLocoFunctionIconHeadlightLowBeamReverse);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconHeadlightHighBeamForward, 3)] = Languages::GetText(Languages::TextLocoFunctionIconHeadlightHighBeamForward);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconHeadlightHighBeamReverse, 3)] = Languages::GetText(Languages::TextLocoFunctionIconHeadlightHighBeamReverse);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconBacklightForward, 3)] = Languages::GetText(Languages::TextLocoFunctionIconBacklightForward);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconBacklightReverse, 3)] = Languages::GetText(Languages::TextLocoFunctionIconBacklightReverse);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconShuntingLight, 3)] = Languages::GetText(Languages::TextLocoFunctionIconShuntingLight);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconBlinkingLight, 3)] = Languages::GetText(Languages::TextLocoFunctionIconBlinkingLight);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconInteriorLight1, 3)] = Languages::GetText(Languages::TextLocoFunctionIconInteriorLight1);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconInteriorLight2, 3)] = Languages::GetText(Languages::TextLocoFunctionIconInteriorLight2);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconTableLight1, 3)] = Languages::GetText(Languages::TextLocoFunctionIconTableLight1);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconTableLight2, 3)] = Languages::GetText(Languages::TextLocoFunctionIconTableLight2);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconTableLight3, 3)] = Languages::GetText(Languages::TextLocoFunctionIconTableLight3);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconCabLight1, 3)] = Languages::GetText(Languages::TextLocoFunctionIconCabLight1);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconCabLight2, 3)] = Languages::GetText(Languages::TextLocoFunctionIconCabLight2);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconCabLight12, 3)] = Languages::GetText(Languages::TextLocoFunctionIconCabLight12);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconDriversDeskLight, 3)] = Languages::GetText(Languages::TextLocoFunctionIconDriversDeskLight);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconTrainDestinationIndicator, 3)] = Languages::GetText(Languages::TextLocoFunctionIconTrainDestinationIndicator);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconLocomotiveNumberIndicator, 3)] = Languages::GetText(Languages::TextLocoFunctionIconLocomotiveNumberIndicator);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconEngineLight, 3)] = Languages::GetText(Languages::TextLocoFunctionIconEngineLight);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconFireBox, 3)] = Languages::GetText(Languages::TextLocoFunctionIconFireBox);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconStairsLight, 3)] = Languages::GetText(Languages::TextLocoFunctionIconStairsLight);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconSmokeGenerator, 3)] = Languages::GetText(Languages::TextLocoFunctionIconSmokeGenerator);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconTelex1, 3)] = Languages::GetText(Languages::TextLocoFunctionIconTelex1);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconTelex2, 3)] = Languages::GetText(Languages::TextLocoFunctionIconTelex2);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconTelex12, 3)] = Languages::GetText(Languages::TextLocoFunctionIconTelex12);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconPanto1, 3)] = Languages::GetText(Languages::TextLocoFunctionIconPanto1);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconPanto2, 3)] = Languages::GetText(Languages::TextLocoFunctionIconPanto2);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconPanto12, 3)] = Languages::GetText(Languages::TextLocoFunctionIconPanto12);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconUp, 3)] = Languages::GetText(Languages::TextLocoFunctionIconUp);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconDown, 3)] = Languages::GetText(Languages::TextLocoFunctionIconDown);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconUpDown1, 3)] = Languages::GetText(Languages::TextLocoFunctionIconUpDown1);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconUpDown2, 3)] = Languages::GetText(Languages::TextLocoFunctionIconUpDown2);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconLeft, 3)] = Languages::GetText(Languages::TextLocoFunctionIconLeft);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconRight, 3)] = Languages::GetText(Languages::TextLocoFunctionIconRight);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconLeftRight, 3)] = Languages::GetText(Languages::TextLocoFunctionIconLeftRight);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconTurnLeft, 3)] = Languages::GetText(Languages::TextLocoFunctionIconTurnLeft);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconTurnRight, 3)] = Languages::GetText(Languages::TextLocoFunctionIconTurnRight);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconTurn, 3)] = Languages::GetText(Languages::TextLocoFunctionIconTurn);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconCrane, 3)] = Languages::GetText(Languages::TextLocoFunctionIconCrane);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconMagnet, 3)] = Languages::GetText(Languages::TextLocoFunctionIconMagnet);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconCraneHook, 3)] = Languages::GetText(Languages::TextLocoFunctionIconCraneHook);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconFan, 3)] = Languages::GetText(Languages::TextLocoFunctionIconFan);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconBreak, 3)] = Languages::GetText(Languages::TextLocoFunctionIconBreak);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconNoSound, 3)] = Languages::GetText(Languages::TextLocoFunctionIconNoSound);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconSoundGeneral, 3)] = Languages::GetText(Languages::TextLocoFunctionIconSoundGeneral);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconRunning1, 3)] = Languages::GetText(Languages::TextLocoFunctionIconRunning1);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconRunning2, 3)] = Languages::GetText(Languages::TextLocoFunctionIconRunning2);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconEngine1, 3)] = Languages::GetText(Languages::TextLocoFunctionIconEngine1);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconEngine2, 3)] = Languages::GetText(Languages::TextLocoFunctionIconEngine2);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconBreak1, 3)] = Languages::GetText(Languages::TextLocoFunctionIconBreak1);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconBreak2, 3)] = Languages::GetText(Languages::TextLocoFunctionIconBreak2);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconCurve, 3)] = Languages::GetText(Languages::TextLocoFunctionIconCurve);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconHorn1, 3)] = Languages::GetText(Languages::TextLocoFunctionIconHorn1);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconHorn2, 3)] = Languages::GetText(Languages::TextLocoFunctionIconHorn2);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconWhistle1, 3)] = Languages::GetText(Languages::TextLocoFunctionIconWhistle1);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconWhistle2, 3)] = Languages::GetText(Languages::TextLocoFunctionIconWhistle2);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconBell, 3)] = Languages::GetText(Languages::TextLocoFunctionIconBell);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconStationAnnouncement1, 3)] = Languages::GetText(Languages::TextLocoFunctionIconStationAnnouncement1);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconStationAnnouncement2, 3)] = Languages::GetText(Languages::TextLocoFunctionIconStationAnnouncement2);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconStationAnnouncement3, 3)] = Languages::GetText(Languages::TextLocoFunctionIconStationAnnouncement3);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconSpeak, 3)] = Languages::GetText(Languages::TextLocoFunctionIconSpeak);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconRadio, 3)] = Languages::GetText(Languages::TextLocoFunctionIconRadio);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconMusic1, 3)] = Languages::GetText(Languages::TextLocoFunctionIconMusic1);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconMusic2, 3)] = Languages::GetText(Languages::TextLocoFunctionIconMusic2);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconOpenDoor, 3)] = Languages::GetText(Languages::TextLocoFunctionIconOpenDoor);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconCloseDoor, 3)] = Languages::GetText(Languages::TextLocoFunctionIconCloseDoor);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconFan1, 3)] = Languages::GetText(Languages::TextLocoFunctionIconFan1);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconFan2, 3)] = Languages::GetText(Languages::TextLocoFunctionIconFan2);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconFan3, 3)] = Languages::GetText(Languages::TextLocoFunctionIconFan3);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconShovelCoal, 3)] = Languages::GetText(Languages::TextLocoFunctionIconShovelCoal);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconCompressedAir, 3)] = Languages::GetText(Languages::TextLocoFunctionIconCompressedAir);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconReliefValve, 3)] = Languages::GetText(Languages::TextLocoFunctionIconReliefValve);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconSteamBlowOut, 3)] = Languages::GetText(Languages::TextLocoFunctionIconSteamBlowOut);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconSteamBlow, 3)] = Languages::GetText(Languages::TextLocoFunctionIconSteamBlow);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconDrainValve, 3)] = Languages::GetText(Languages::TextLocoFunctionIconDrainValve);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconShakingRust, 3)] = Languages::GetText(Languages::TextLocoFunctionIconShakingRust);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconAirPump, 3)] = Languages::GetText(Languages::TextLocoFunctionIconAirPump);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconWaterPump, 3)] = Languages::GetText(Languages::TextLocoFunctionIconWaterPump);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconBufferPush, 3)] = Languages::GetText(Languages::TextLocoFunctionIconBufferPush);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconGenerator, 3)] = Languages::GetText(Languages::TextLocoFunctionIconGenerator);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconGearBox, 3)] = Languages::GetText(Languages::TextLocoFunctionIconGearBox);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconGearUp, 3)] = Languages::GetText(Languages::TextLocoFunctionIconGearUp);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconGearDown, 3)] = Languages::GetText(Languages::TextLocoFunctionIconGearDown);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconFillWater, 3)] = Languages::GetText(Languages::TextLocoFunctionIconFillWater);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconFillDiesel, 3)] = Languages::GetText(Languages::TextLocoFunctionIconFillDiesel);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconFillGas, 3)] = Languages::GetText(Languages::TextLocoFunctionIconFillGas);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconSand, 3)] = Languages::GetText(Languages::TextLocoFunctionIconSand);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconRailJoint, 3)] = Languages::GetText(Languages::TextLocoFunctionIconRailJoint);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconCoupler, 3)] = Languages::GetText(Languages::TextLocoFunctionIconCoupler);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconPanto, 3)] = Languages::GetText(Languages::TextLocoFunctionIconPanto);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconMainSwitch, 3)] = Languages::GetText(Languages::TextLocoFunctionIconMainSwitch);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconSoundLouder, 3)] = Languages::GetText(Languages::TextLocoFunctionIconSoundLouder);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconSoundLower, 3)] = Languages::GetText(Languages::TextLocoFunctionIconSoundLower);
				functionOptions[Utils::Utils::ToStringWithLeadingZeros(256 + LocoFunctionIconNoBreak, 3)] = Languages::GetText(Languages::TextLocoFunctionIconNoBreak);

				content.AddChildTag(HtmlTagSelect(name + "_id", functionOptions, Utils::Utils::ToStringWithLeadingZeros(objectId, 3)).AddClass("select_relation_id"));

				// FIXME: load available functions of loco
				map<DataModel::LocoFunctionState,Languages::TextSelector> stateOptions;
				stateOptions[DataModel::LocoFunctionStateOff] = Languages::TextOff;
				stateOptions[DataModel::LocoFunctionStateOn] = Languages::TextOn;
				content.AddChildTag(HtmlTagSelect(name + "_state", stateOptions, static_cast<DataModel::LocoFunctionState>(data)).AddClass("select_relation_state"));
				return content;
			}

			case ObjectTypePause:
			{
				map<unsigned int,string> time;
				time[1u] = "0.1s";
				time[2u] = "0.2s";
				time[3u] = "0.3s";
				time[4u] = "0.4s";
				time[5u] = "0.5s";
				time[6u] = "0.6s";
				time[7u] = "0.7s";
				time[8u] = "0.8s";
				time[9u] = "0.9s";
				time[10u] = "1s";
				time[15u] = "1.5s";
				time[20u] = "2s";
				time[25u] = "2.5s";
				time[30u] = "3s";
				time[40u] = "4s";
				time[50u] = "5s";
				time[60u] = "6s";
				time[70u] = "7s";
				time[80u] = "8s";
				time[90u] = "9s";
				time[100u] = "10s";
				content.AddChildTag(HtmlTagSelect(name + "_state", time, static_cast<unsigned int>(data)).AddClass("select_relation_state"));
				return content;
			}

			default:
			{
				content.AddContent(Languages::TextUnknownObjectType);
				return content;
			}
		}
	}

	HtmlTag WebClientRoute::HtmlTagSelectTrack(const std::string& name,
		const Languages::TextSelector label,
		const ObjectIdentifier& identifier,
		const Orientation orientation,
		const string& onchange) const
	{
		HtmlTag tag;
		map<string,ObjectIdentifier> tracks = manager.TrackBaseListIdentifierByName();
		// FIXME: remove signal later and also remove Manager::TrackBaseListIdentifierByName() 2021-02-10
		if (identifier.GetObjectType() == ObjectTypeSignal)
		{
			Signal* signal = manager.GetSignal(identifier.GetObjectID());
			tracks[signal->GetName()] = ObjectIdentifier(ObjectTypeSignal, signal->GetID());
		}
		HtmlTagSelectWithLabel selectTrack(name + "track", label, tracks, identifier);
		selectTrack.AddClass("select_track");
		if (onchange.size() > 0)
		{
			selectTrack.AddAttribute("onchange", onchange);
		}
		tag.AddChildTag(selectTrack);
		tag.AddChildTag(HtmlTagSelectOrientation(name + "orientation", orientation).AddClass("select_orientation"));
		return tag;
	}

	HtmlTag WebClientRoute::HtmlTagSelectFeedbacksOfTrack(const ObjectIdentifier& identifier,
		const FeedbackID feedbackIdReduced,
		const FeedbackID feedbackIdCreep,
		const FeedbackID feedbackIdStop,
		const FeedbackID feedbackIdOver) const
	{
		HtmlTag tag;
		map<string,FeedbackID> feedbacks = manager.FeedbacksOfTrack(identifier);
		map<string,FeedbackID> feedbacksWithNone = feedbacks;
		feedbacksWithNone["-"] = FeedbackNone;
		tag.AddChildTag(HtmlTagSelectWithLabel("feedbackreduced", Languages::TextReducedSpeedAt, feedbacksWithNone, feedbackIdReduced).AddClass("select_feedback"));
		tag.AddChildTag(HtmlTagSelectWithLabel("feedbackcreep", Languages::TextCreepAt, feedbacksWithNone, feedbackIdCreep).AddClass("select_feedback"));
		tag.AddChildTag(HtmlTagSelectWithLabel("feedbackstop", Languages::TextStopAt, feedbacks, feedbackIdStop).AddClass("select_feedback"));
		tag.AddChildTag(HtmlTagSelectWithLabel("feedbackover", Languages::TextOverrunAt, feedbacksWithNone, feedbackIdOver).AddClass("select_feedback"));
		return tag;
	}

	void WebClientRoute::HandleFeedbacksOfTrack(const map<string, string>& arguments)
	{
		ObjectIdentifier identifier = Utils::Utils::GetStringMapEntry(arguments, "track");
		client.ReplyHtmlWithHeader(HtmlTagSelectFeedbacksOfTrack(identifier, FeedbackNone, FeedbackNone, FeedbackNone, FeedbackNone));
	}
} // namespace WebServer
