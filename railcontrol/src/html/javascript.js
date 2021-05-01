function activateFullScreen()
{
	var element = document.documentElement;
	if (element.requestFullscreen)
	{
		element.requestFullscreen();
	}
}

function closeFullScreen()
{
	if (document.exitFullscreen)
	{
		document.exitFullscreen();
	}
}

function fullScreen()
{
	if (document.fullscreen)
	{
		closeFullScreen();
	}
	else
	{
		activateFullScreen();
	}
}

function onChangeLocoFunctionType(nr)
{
	var locoFunctionType = document.getElementById('s_f' + nr + '_type');
	if (!locoFunctionType)
	{
		return false;
	}
	var locoFunctionIcon = document.getElementById('s_f' + nr + '_icon');
	if (!locoFunctionIcon)
	{
		return false;
	}
	var locoFunctionTimer = document.getElementById('d_f' + nr + '_timer');
	if (!locoFunctionTimer)
	{
		return false;
	}
	var type = locoFunctionType.value;
	if (type == 0) // LocoFunctionTypeNone
	{
		locoFunctionIcon.classList.add('hidden');
	}
	else
	{
		locoFunctionIcon.classList.remove('hidden');
	}
	if (type == 4) // LocoFunctionTypeTimer
	{
		locoFunctionTimer.classList.remove('hidden');
	}
	else
	{
		locoFunctionTimer.classList.add('hidden');
	}
	return false;
}

function onClickAddresses(signal)
{
	var addressElement = document.getElementById('address');
	if (!addressElement)
	{
		return false;
	}
	var address = addressElement.value;

	var typeElement = document.getElementById('s_signaltype');
	if (!typeElement)
	{
		return false;
	}
	var type = typeElement.value;

	var url = '?cmd=signaladdresses&address=' + address + '&type=' + type + '&signal=' + signal;

	requestUpdateItem('addresses', url);
}

function onClickProgramRead(cv)
{
	var controlElement = document.getElementById('s_controlraw');
	if (!controlElement)
	{
		return false;
	}
	var control = controlElement.value;

	var modeElement = document.getElementById('s_moderaw');
	if (!modeElement)
	{
		return false;
	}
	var mode = modeElement.value;

	var addressElement = document.getElementById('addressraw');
	if (!addressElement)
	{
		return false;
	}
	var address = addressElement.value;

	var indexElement = document.getElementById('indexraw');
	if (!indexElement)
	{
		return false;
	}
	var index = indexElement.value;

	var cvElement = document.getElementById('cvraw');
	if (!cvElement)
	{
		return false;
	}
	var cv = parseInt(cvElement.value) + (parseInt(index) * 1024);

	var url = '?cmd=programread&control=' + control + '&mode=' + mode + '&address=' + address + '&cv=' + cv;
	fireRequestAndForget(url);
	return false;
}

function onClickProgramWrite()
{
	var controlElement = document.getElementById('s_controlraw');
	if (!controlElement)
	{
		return false;
	}
	var control = controlElement.value;

	var modeElement = document.getElementById('s_moderaw');
	if (!modeElement)
	{
		return false;
	}
	var mode = modeElement.value;

	var addressElement = document.getElementById('addressraw');
	if (!addressElement)
	{
		return false;
	}
	var address = addressElement.value;

	var indexElement = document.getElementById('indexraw');
	if (!indexElement)
	{
		return false;
	}
	var index = indexElement.value;

	var cvElement = document.getElementById('cvraw');
	if (!cvElement)
	{
		return false;
	}
	var cv = parseInt(cvElement.value) + (parseInt(index) * 1024);

	var valueElement = document.getElementById('valueraw');
	if (!valueElement)
	{
		return false;
	}
	var value = valueElement.value;
	var url = '?cmd=programwrite&control=' + control + '&mode=' + mode + '&address=' + address + '&cv=' + cv + '&value=' + value;
	fireRequestAndForget(url);
	return false;
}

function loadProgramModeSelector()
{
	var selectControl = document.getElementById('s_controlraw');
	if (!selectControl)
	{
		return;
	}
	var control = selectControl.value;

	var modeElement = document.getElementById('s_moderaw');
	if (!modeElement)
	{
		return false;
	}
	var mode = modeElement.value;

	var elementName = 'program_mode_selector';
	var url = '/?cmd=programmodeselector';
	url += '&control=' + control;
	url += '&mode=' + mode;
	requestUpdateItem(elementName, url);
}

function onChangeProgramModeSelector()
{
	var selectControl = document.getElementById('s_controlraw');
	if (!selectControl)
	{
		return;
	}
	var control = selectControl.value;

	var modeElement = document.getElementById('s_moderaw');
	if (!modeElement)
	{
		return false;
	}
	var mode = modeElement.value;

	var elementName = 'cv_fields';
	var url = '/?cmd=getcvfields';
	url += '&control=' + control;
	url += '&mode=' + mode;
	requestUpdateItem(elementName, url);
}

function updateName()
{
	var nameField = document.getElementById('name');
	if (!nameField)
	{
		return false;
	}
	var title = document.getElementById('popup_title');
	if (!title)
	{
		return false;
	}
	if (nameField.value.length > 0)
	{
		title.innerHTML = nameField.value;
	}
	else
	{
		title.innerHTML = 'NN';
	}
	return true;
}

function getArgumentsOfHardwareType()
{
	var hardwareType = document.getElementById('s_hardwaretype');
	if (!hardwareType)
	{
		return false;
	}
	var url = '?cmd=controlarguments&hardwaretype=' + hardwareType.value;
	requestUpdateItem('controlarguments', url);
	return false;
}

function updateFeedbacksOfTrack()
{
	var track = document.getElementById('s_totrack');
	if (!track)
	{
		return false;
	}
	var url = '?cmd=feedbacksoftrack&track=' + track.value;
	requestUpdateItem('feedbacks', url);
	return false;
}

function addSlave(prefix)
{
	var counter = document.getElementById(prefix + 'counter');
	if (!counter)
	{
		return false;
	}
	var div = document.getElementById(prefix + 's');
	if (!div)
	{
		return false;
	}

	counter.value++;
	var url = '/?cmd=slaveadd&priority=' + counter.value + '&prefix=' + prefix;
	requestAddItem(prefix + '_new_' + counter.value, url);
	return false;
}

function addRelation(type)
{
	var relationCounter = document.getElementById('relationcounter' + type);
	if (!relationCounter)
	{
		return false;
	}
	var relationDiv = document.getElementById('relation' + type);
	if (!relationDiv)
	{
		return false;
	}

	var url = '/?cmd=relationadd&priority=' + relationCounter.value + '&type=' + type;
	requestAddItem('new_' + type + '_priority_' + relationCounter.value, url);
	relationCounter.value++;
	return false;
}

function addFeedback()
{
	var feedbackCounter = document.getElementById('feedbackcounter');
	if (!feedbackCounter)
	{
		return false;
	}

	var identifier = '';
	var track = document.getElementById('track');
	if (track)
	{
		identifier = '&track=' + track.value;
	}
	else
	{
		var signal = document.getElementById('signal');
		if (signal)
		{
			identifier = '&signal=' + signal.value;
		}
	}

	if (identifier.length == 0)
	{
		return false;
	}

	feedbackCounter.value++;
	var url = '/?cmd=feedbackadd&counter=' + feedbackCounter.value + identifier;
	requestUpdateItem('div_feedback_' + feedbackCounter.value, url);
	return false;
}

function checkIntegerValue(name, min, max)
{
	if (min > max)
	{
		return;
	}
	var input = document.getElementById(name);
	if (!input)
	{
		return;
	}
	if (input.value < min)
	{
		input.value = min;
		return;
	}
	if (input.value > max)
	{
		input.value = max;
	}
}

function incrementIntegerValue(name, max)
{
	var input = document.getElementById(name);
	if (!input)
	{
		return;
	}
	var value = parseInt(input.value) + 1;
	if (value > max)
	{
		return;
	}
	input.value = value;
}

function decrementIntegerValue(name, min)
{
	var input = document.getElementById(name);
	if (!input)
	{
		return;
	}
	var value = parseInt(input.value) - 1;
	if (value < min)
	{
		return;
	}
	input.value = value;
}

function setToggleButton(elementName, on)
{
	var element = document.getElementById(elementName);
	if (element)
	{
		if (on == 'true')
		{
			element.classList.remove('button_off');
			element.classList.add('button_on');
		}
		else
		{
			element.classList.remove('button_on');
			element.classList.add('button_off');
		}
	}
}

function deleteElement(elementName)
{
	var element = document.getElementById(elementName);
	if (element)
	{
		element.parentNode.removeChild(element);
	}
}

function onClickAccessory(accessoryID)
{
	var element = document.getElementById('a_' + accessoryID);
	var url = '/?cmd=accessorystate';
	url += '&state=' + (element.classList.contains('accessory_off') ? 'on' : 'off');
	url += '&accessory=' + accessoryID;
	fireRequestAndForget(url);
	return false;
}

function onClickRoute(routeID)
{
	var element = document.getElementById('r_' + routeID);
	var url = '/?cmd=routeexecute';
	url += '&route=' + routeID;
	fireRequestAndForget(url);
	return false;
}

function onClickSwitch(switchID)
{
	var element = document.getElementById('sw_' + switchID);
	var url = '/?cmd=switchstate';
	url += '&state='
	if (element.classList.contains('switch_straight'))
	{
		url += 'third'
	}
	else if (element.classList.contains('switch_turnout'))
	{
		url += 'straight'
	}
	else
	{
		url += 'turnout'
	}
	url += '&switch=' + switchID;
	fireRequestAndForget(url);
	return false;
}

function onClickFeedback(feedbackID)
{
	var element = document.getElementById('f_' + feedbackID);
	var url = '/?cmd=feedbackstate';
	url += '&state=' + (element.classList.contains('feedback_free') ? 'occupied' : 'free');
	url += '&feedback=' + feedbackID;
	fireRequestAndForget(url);
	return false;
}

function onClickSignal(signalID)
{
	var element = document.getElementById('si_' + signalID);
	var url = '/?cmd=signalstate';
	url += '&state=' + (element.classList.contains('signal_clear') ? 'stop' : 'clear');
	url += '&signal=' + signalID;
	fireRequestAndForget(url);
	return false;
}

function showMenu(elementName) {
	var element = document.getElementById(elementName);
	if (!element)
	{
		return;
	}

	var mouseX = event.clientX;
	var mouseY = event.clientY;
	var windowX = window.innerWidth;
	var windowY = window.innerHeight;

	if (windowX > (mouseX * 2)) {
		element.style.left = mouseX + "px";
		element.style.right = "auto";
	}
	else {
		element.style.left = "auto";
		element.style.right = (windowX - mouseX) + "px";
	}

	if (windowY > (mouseY * 2)) {
		element.style.top = mouseY + "px";
		element.style.bottom = "auto";
	}
	else {
		element.style.top = "auto";
		element.style.bottom = (windowY - mouseY) + "px";
	}
	element.style.display = 'block';
}

function showOnClickMenu(event, ID)
{
	if (event.shiftKey)
	{
		return true;
	}
	event.stopPropagation();
	hideAllContextMenus();
	showMenu(ID + '_onclick');
	return false;
}

function showContextMenu(event, ID)
{
	if (event.shiftKey)
	{
		return true;
	}
	event.stopPropagation();
	hideAllContextMenus();
	showMenu(ID + '_context');
	return false;
}

function onChangeCheckboxShowHide(checkboxId, divId)
{
	var checkbox = document.getElementById(checkboxId);
	if (!checkbox)
	{
		return;
	}
	var div = document.getElementById(divId);
	if (!div)
	{
		return;
	}
	div.hidden = !checkbox.checked;
}

function onChangeTrackType()
{
	var trackType = document.getElementById('s_tracktype');
	if (!trackType)
	{
		return;
	}
	var trackTypeValue = trackType.value;
	var showname = document.getElementById('i_showname');
	if (!showname)
	{
		return;
	}
	showname.hidden = (trackTypeValue != 0)
	var length = document.getElementById('i_length');
	if (!length)
	{
		return;
	}
	var trackTypeValue = trackType.value;
	length.hidden = (trackTypeValue == 1 || trackTypeValue == 5 || trackTypeValue == 7 || trackTypeValue == 8 || trackTypeValue == 9)
}

function updateLayoutItem(elementName, data)
{
	var parentElement = document.getElementById('layout');
	if (parentElement)
	{
		deleteElement(elementName);
		var elementOnClickName = elementName + '_onclick';
		deleteElement(elementOnClickName);
		var elementContextName = elementName + '_context';
		deleteElement(elementContextName);
		parentElement.innerHTML += data;
		var i;
		var tags = document.getElementsByClassName('layout_item');
		for (i = 0; i < tags.length; i++)
		{
			var tag = tags[i];
			var clone = tag.cloneNode(true);
			tag.parentNode.replaceChild(clone, tag);
		}
	}
}

function requestUpdateLayoutItem(elementName, url)
{
	var xmlHttp = new XMLHttpRequest();
	xmlHttp.onreadystatechange = function() {
		if (xmlHttp.readyState == 4 && xmlHttp.status == 200)
		{
			updateLayoutItem(elementName, xmlHttp.responseText);
		}
	}
	xmlHttp.open('GET', url, true);
	xmlHttp.send(null);
}

function addItem(elementName, data)
{
	var element = document.getElementById(elementName);
	if (!element)
	{
		return;
	}
	element.innerHTML += data;
}

function requestAddItem(elementName, url)
{
	var xmlHttp = new XMLHttpRequest();
	xmlHttp.onreadystatechange = function() {
		if (xmlHttp.readyState == 4 && xmlHttp.status == 200)
		{
			addItem(elementName, xmlHttp.responseText);
		}
	}
	xmlHttp.open('GET', url, true);
	xmlHttp.send(null);
}

function updateItem(elementName, data)
{
	var element = document.getElementById(elementName);
	if (!element)
	{
		return;
	}
	element.innerHTML = data;
}

function requestUpdateItem(elementName, url, followUpFunction)
{
	updateItem(elementName, "Loading...");
	var xmlHttp = new XMLHttpRequest();
	xmlHttp.onreadystatechange = function() {
		if (xmlHttp.readyState != 4 || xmlHttp.status != 200)
		{
			return;
		}
		updateItem(elementName, xmlHttp.responseText);
		if (followUpFunction === undefined)
		{
			return;
		}
		followUpFunction();
	}
	xmlHttp.open('GET', url, true);
	xmlHttp.send(null);
}

function updateTitle()
{
	var locoName = document.getElementById("loconame");
	if (!locoName)
	{
		return;
	}
	updateItem("title", locoName.innerHTML + " - RailControl");
}

function updateTrack(trackID)
{
	elementName = 't_' + trackID;
	var url = '/?cmd=trackget';
	url += '&track=' + trackID;
	requestUpdateLayoutItem(elementName, url);
}

function updateTrackState(argumentMap)
{
	var trackID = argumentMap.get('track');
	var signalID = argumentMap.get('signal');
	if (trackID > 0)
	{
		elementName = 't_' + trackID;
	}
	else if (signalID > 0)
	{
		elementName = 'si_' + signalID;
	}
	var element = document.getElementById(elementName);
	if (element)
	{
		var reserved = false;
		var occupied = false;
		var blocked = false;
		var error = false;
		var orientation = true;

		if (argumentMap.has('occupied'))
		{
			occupied = argumentMap.get('occupied') == 'true';
		}

		if (argumentMap.has('reserved'))
		{
			reserved = argumentMap.get('reserved') == 'true';
		}

		if (argumentMap.has('blocked'))
		{
			blocked = argumentMap.get('blocked') == 'true';
		}

		element.classList.remove('track_free');
		element.classList.remove('track_reserved');
		element.classList.remove('track_reserved_occupied');
		element.classList.remove('track_occupied');
		element.classList.remove('track_error');
		element.classList.remove('track_blocked');

		if (reserved && occupied)
		{
			element.classList.add('track_reserved_occupied');
		}
		else if (reserved)
		{
			element.classList.add('track_reserved');
		}
		else if (occupied)
		{
			element.classList.add('track_occupied');
		}
		else if (blocked)
		{
			element.classList.add('track_blocked');
		}
		else
		{
			element.classList.add('track_free');
		}

		if (error)
		{
			element.classList.add('track_error');
		}

		if (argumentMap.has('orientation'))
		{
			orientation = argumentMap.get('orientation') == 'true';
		}

		if (reserved)
		{
			element.classList.remove('loco_unknown');
			element.classList.add('loco_known');
		}
		else
		{
			element.classList.remove('loco_known');
			element.classList.add('loco_unknown');
		}
	}

	var onClickElement = document.getElementById(elementName + '_onclick');
	if (onClickElement)
	{
		if (reserved)
		{
			onClickElement.classList.remove('loco_unknown');
			onClickElement.classList.add('loco_known');
		}
		else
		{
			onClickElement.classList.remove('loco_known');
			onClickElement.classList.add('loco_unknown');
		}

		if (blocked)
		{
			onClickElement.classList.remove('track_unblocked');
			onClickElement.classList.add('track_blocked');
		}
		else
		{
			onClickElement.classList.remove('track_blocked');
			onClickElement.classList.add('track_unblocked');
		}

		if (orientation)
		{
			onClickElement.classList.remove('orientation_left');
			onClickElement.classList.add('orientation_right');
		}
		else
		{
			onClickElement.classList.remove('orientation_right');
			onClickElement.classList.add('orientation_left');
		}
	}

	var locoElement = document.getElementById(elementName + '_text_loconame');
	if (locoElement)
	{
		var orientationArrow = orientation ? '&rarr; ' : '&larr; ';
		var locoName = argumentMap.has('loconame') ? argumentMap.get('loconame') : '';
		locoElement.innerHTML = orientationArrow + locoName;
	}
}

function updateSwitchState(argumentMap)
{
	if (!argumentMap.has('switch') || !argumentMap.has('state'))
	{
		return;
	}
	var elementName = 'sw_' + argumentMap.get('switch');
	var element = document.getElementById(elementName);
	if (!element)
	{
		return;
	}
	var state = argumentMap.get('state');
	updateSwitchStateDiv(element, state);
	elementName += '_onclick';
	element = document.getElementById(elementName);
	if (!element)
	{
		return;
	}
	updateSwitchStateDiv(element, state);
}

function updateSwitchStateDiv(element, state)
{
	element.classList.remove('switch_turnout');
	element.classList.remove('switch_straight');
	element.classList.remove('switch_third');
	switch (state)
	{
		case 'turnout':
			element.classList.add('switch_turnout');
			break;

		case 'straight':
			element.classList.add('switch_straight');
			break;

		case 'third':
			element.classList.add('switch_third');
			break;
	}
}

function updateSignalState(argumentMap)
{
	if (!argumentMap.has('signal') || !argumentMap.has('state'))
	{
		return;
	}
	var elementName = 'si_' + argumentMap.get('signal');
	var element = document.getElementById(elementName);
	if (!element)
	{
		return;
	}
	var state = argumentMap.get('state');
	updateSignalStateDiv(element, state);
	elementName += '_onclick';
	element = document.getElementById(elementName);
	if (!element)
	{
		return;
	}
	updateSignalStateDiv(element, state);
}

function updateSignalStateDiv(element, state)
{
	element.classList.remove('signal_stop');
	element.classList.remove('signal_clear');
	element.classList.remove('signal_aspect2');
	element.classList.remove('signal_aspect3');
	element.classList.remove('signal_aspect4');
	element.classList.remove('signal_aspect5');
	element.classList.remove('signal_aspect6');
	switch (state)
	{
		case 'stop':
			element.classList.add('signal_stop');
			break;

		case 'clear':
			element.classList.add('signal_clear');
			break;

		case 'aspect2':
			element.classList.add('signal_aspect2');
			break;

		case 'aspect3':
			element.classList.add('signal_aspect3');
			break;

		case 'aspect4':
			element.classList.add('signal_aspect4');
			break;

		case 'aspect5':
			element.classList.add('signal_aspect5');
			break;

		case 'aspect6':
			element.classList.add('signal_aspect6');
			break;
	}
}

function updateSignal(signalID)
{
	elementName = 'si_' + signalID;
	var url = '/?cmd=signalget';
	url += '&signal=' + signalID;
	requestUpdateLayoutItem(elementName, url);
}

function dataUpdate(event)
{
	var status = document.getElementById('status');
	var arguments = event.data.split(';');
	var argumentMap = new Map();
	arguments.forEach(function(argument) {
		var parts = argument.split('=');
		if (parts[0] == 'status')
		{
			var statusData = status.innerHTML + parts[1] + '<br>';
			status.innerHTML = statusData.substring(statusData.length - 2500);
			status.scrollTop = status.scrollHeight - status.clientHeight;
		}
		else
		{
			argumentMap.set(parts[0], parts[1]);
		}
	});

	var elementName = "";
	var command = argumentMap.get('command');
	if (command == 'booster')
	{
		elementName = 'b_booster';
		var on = argumentMap.get('on');
		setToggleButton(elementName, on);
	}
	else if (command == 'locospeed')
	{
		elementName = 'locospeed_' + argumentMap.get('loco');
		var element = document.getElementById(elementName);
		if (element)
		{
			element.value = argumentMap.get('speed');
		}
	}
	else if (command == 'locofunction')
	{
		elementName = 'b_locofunction_' + argumentMap.get('loco') + '_' + argumentMap.get('function');
		var on = argumentMap.get('on');
		setToggleButton(elementName, on);
	}
	else if (command == 'locoorientation')
	{
		elementName = 'b_locoorientation_' + argumentMap.get('loco');
		var on = argumentMap.get('orientation');
		setToggleButton(elementName, on);
	}
	else if (command == 'accessory')
	{
		elementName = 'a_' + argumentMap.get('accessory');
		var element = document.getElementById(elementName);
		if (element && argumentMap.has('state'))
		{
			var state = argumentMap.get('state');
			if (state == 'green')
			{
				element.classList.remove('accessory_off');
				element.classList.add('accessory_on');
			}
			else
			{
				element.classList.remove('accessory_on');
				element.classList.add('accessory_off');
			}
		}
	}
	else if (command == 'accessorysettings')
	{
		var accessoryID = argumentMap.get('accessory');
		elementName = 'a_' + accessoryID;
		var url = '/?cmd=accessoryget';
		url += '&accessory=' + accessoryID;
		requestUpdateLayoutItem(elementName, url);
	}
	else if (command == 'accessorydelete')
	{
		elementName = 'a_' + argumentMap.get('accessory');
		var element = document.getElementById(elementName);
		if (element)
		{
			element.parentNode.removeChild(element);
		}
	}
	else if (command == 'switch')
	{
		updateSwitchState(argumentMap);
	}
	else if (command == 'switchsettings')
	{
		var switchID = argumentMap.get('switch');
		elementName = 'sw_' + switchID;
		var url = '/?cmd=switchget';
		url += '&switch=' + switchID;
		requestUpdateLayoutItem(elementName, url);
	}
	else if (command == 'switchdelete')
	{
		elementName = 'sw_' + argumentMap.get('switch');
		deleteElement(elementName);
		deleteElement(elementName + '_context');
	}
	else if (command == 'signal')
	{
		updateSignalState(argumentMap);
	}
	else if (command == 'signalsettings')
	{
		updateSignal(argumentMap.get('signal'));
	}
	else if (command == 'signaldelete')
	{
		elementName = 'si_' + argumentMap.get('signal');
		deleteElement(elementName);
		deleteElement(elementName + '_onclick');
		deleteElement(elementName + '_context');
	}
	else if (command == 'routesettings')
	{
		var routeID = argumentMap.get('route');
		elementName = 'r_' + routeID;
		var url = '/?cmd=routeget';
		url += '&route=' + routeID;
		requestUpdateLayoutItem(elementName, url);
	}
	else if (command == 'routedelete')
	{
		elementName = 'r_' + argumentMap.get('route');
		deleteElement(elementName);
		deleteElement(elementName + '_context');
	}
	else if (command == 'trackstate')
	{
		updateTrackState(argumentMap);
	}
	else if (command == 'tracksettings')
	{
		updateTrack(argumentMap.get('track'));
	}
	else if (command == 'trackdelete')
	{
		elementName = 't_' + argumentMap.get('track');
		deleteElement(elementName);
		deleteElement(elementName + '_context');
	}
	else if (command == 'feedback')
	{
		elementName = 'f_' + argumentMap.get('feedback');
		var element = document.getElementById(elementName);
		if (element && argumentMap.has('state'))
		{
			var state = argumentMap.get('state');
			if (state == 'on')
			{
				element.classList.remove('feedback_free');
				element.classList.add('feedback_occupied');
			}
			else
			{
				element.classList.remove('feedback_occupied');
				element.classList.add('feedback_free');
			}
		}
	}
	else if (command == 'feedbacksettings')
	{
		var feedbackID = argumentMap.get('feedback');
		var layerID = document.getElementById('s_layer').value;
		elementName = 'f_' + feedbackID;
		var url = '/?cmd=feedbackget';
		url += '&feedback=' + feedbackID;
		url += '&layer=' + layerID;
		requestUpdateLayoutItem(elementName, url);
	}
	else if (command == 'feedbackdelete')
	{
		elementName = 'f_' + argumentMap.get('feedback');
		deleteElement(elementName);
		deleteElement(elementName + '_context');
	}
	else if (command == 'locosettings' || command == 'locodelete')
	{
		loadLocoSelector();
	}
	else if (command == 'layersettings' || command == 'layerdelete')
	{
		loadLayerSelector();
	}
	else if (command == 'dcccvvalue')
	{
		var cv = argumentMap.get('cv');
		var cvElementName = 'cvraw';
		var cvElement = document.getElementById(cvElementName);
		if (cvElement &&  cvElement.value == cv)
		{
			var value = argumentMap.get('value');
			var valueElementName = 'valueraw';
			var valueElement = document.getElementById(valueElementName);
			if (valueElement)
			{
				valueElement.value = value;
			}
		}
	}
}

function loadPopup(url)
{
	url += '&posx=' + window.layoutPosX;
	url += '&posy=' + window.layoutPosY;
	url += '&posz=' + document.getElementById('s_layer').value;
	var xmlHttp = new XMLHttpRequest();
	xmlHttp.onreadystatechange = function() {
		if (xmlHttp.readyState !== 4 || xmlHttp.status !== 200)
		{
			return;
		}
		var popup = document.getElementById('popup');
		popup.innerHTML = xmlHttp.responseText;
		popup.style.display = 'block';
	}
	xmlHttp.open('GET', url, true);
	xmlHttp.send(null);
}

function loadLocoSelector()
{
	var selector = 1;
	var locoValue = document.getElementById('s_loco_' + selector).value;
	var url = '/?cmd=locoselector';
	var xmlHttp = new XMLHttpRequest();
	xmlHttp.onreadystatechange = function() {
		if (xmlHttp.readyState !== 4 || xmlHttp.status !== 200)
		{
			return;
		}
		var element = document.getElementById('loco_selector_' + selector);
		element.innerHTML = xmlHttp.responseText;
		var selectorElement = document.getElementById('s_loco_' + selector);
		if (selectHasValue('s_loco_' + selector, locoValue))
		{
			selectorElement.value = locoValue;
			loadLoco(selector);
			return;
		}
		for (var i = 1; i < 255; ++i)
		{
			if (selectHasValue('s_loco_' + selector, i))
			{
				selectorElement.value = i;
				loadLoco(selector);
				return;
			}
		}
	}
	xmlHttp.open('GET', url, true);
	xmlHttp.send(null);
}

function loadLayerSelector()
{
	var layer = document.getElementById('s_layer').value;
	var url = '/?cmd=layerselector';
	var xmlHttp = new XMLHttpRequest();
	xmlHttp.onreadystatechange = function() {
		if (xmlHttp.readyState !== 4 || xmlHttp.status !== 200)
		{
			return;
		}
		var element = document.getElementById('layer_selector');
		element.innerHTML = xmlHttp.responseText;
		var selector = document.getElementById('s_layer');
		if (selectHasValue('s_layer', layer))
		{
			selector.value = layer;
			loadLayout();
			return;
		}
		for (var i = 1; i < 255; ++i)
		{
			if (selectHasValue('s_layer', i))
			{
				selector.value = i;
				loadLayout();
				return;
			}
		}
	}
	xmlHttp.open('GET', url, true);
	xmlHttp.send(null);
}

function loadProtocol(type, ID)
{
	var selectControl = document.getElementById('s_control');
	if (!selectControl)
	{
		return;
	}
	var controlID = selectControl.value;
	var selectProtocol = document.getElementById('select_protocol');
	if (!selectProtocol)
	{
		return;
	}
	var elementName = 'select_protocol';
	var url = '/?cmd=protocol';
	url += '&control=' + controlID;
	url += '&' + type + '=' + ID;
	requestUpdateItem(elementName, url);
}

function loadRelationObject(type, priority)
{
	var elementName = 'relation_' + type + '_' + priority;
	var object = document.getElementById(elementName);
	if (!object)
	{
		return;
	}
	var typeSelector = document.getElementById('s_' + elementName + '_type');
	if (!typeSelector)
	{
		return;
	}
	var url = '/?cmd=relationobject';
	url += '&objecttype=' + typeSelector.value;
	url += '&priority=' + priority;
	url += '&type=' + type;
	requestUpdateItem(elementName + "_object", url);
}

function loadRelationObjectStates(type, name)
{
	var elementName = name + '_state';
	var object = document.getElementById(elementName);
	if (!object)
	{
		return;
	}
	var object = document.getElementById('s_' + name + '_id');
	if (!object)
	{
		return;
	}
	var objectId = object.value;
	var url = '/?cmd=' + type + 'states';
	url += '&' + type + '=' + objectId;
	url += '&name=' + name;
	requestUpdateItem(elementName, url);
}

function loadLayoutContext(event)
{
	if (event.shiftKey)
	{
		return true;
	}
	event.preventDefault();
	hideAllContextMenus();
	showMenu('layout_context');
	window.layoutPosX = Math.floor((event.pageX - 254) / 36);
	window.layoutPosY = Math.floor((event.pageY - 92) / 36);
	return true;
}

function isInLayout(position)
{
	layoutPosition = document.querySelector('.layout').getBoundingClientRect();
	return (position.pageX >= layoutPosition.left && position.pageX <= layoutPosition.right && position.pageY >= layoutPosition.top && position.pageY <= layoutPosition.bottom);
}

function hideAllContextMenus()
{
	var menus = document.getElementsByClassName('contextmenu');
	for (var i = 0; i < menus.length; ++i)
	{
		menus[i].style.display = 'none';
	}
}

function hideElement(name)
{
	var element = document.getElementById(name);
	if (!element)
	{
		return;
	}
	element.style.display = 'none';
}

function loadLoco(selector)
{
	var loco = document.getElementById('s_loco_' + selector);
	if (!loco)
	{
		return;
	}
	requestUpdateItem('loco_' + selector, '/?cmd=loco&loco=' + loco.value, updateTitle);
}

function loadLayout()
{
	var layer = document.getElementById('s_layer');
	if (layer)
	{
		requestUpdateItem('layout', '/?cmd=layout&layer=' + layer.value);
		var context = document.getElementById('layout_context');
		if (layer.value > 0)
		{
			context.classList.remove('feedback_layer');
		}
		else
		{
			context.classList.add('feedback_layer');
		}
	}
}

function sendTimestamp()
{
	var url = '/?cmd=timestamp&timestamp=';
	var timestamp = Math.round(Date.now() / 1000);
	url += timestamp;
	fireRequestAndForget(url);
}

function startUp()
{
	var body = document.getElementById('body');
	if (body)
	{
		body.addEventListener('click', function(event) {
			if (event.button == 2)
			{
				return false;
			}
			hideAllContextMenus();
			return true;
		}, true);
	}
	loadLoco(1);
	loadLayout();
	sendTimestamp();
}

function ShowTab(tabName)
{
	var tabs = document.getElementsByClassName('tab_content');
	if (!tabs)
	{
		return;
	}

	for (var i = 0; i < tabs.length; ++i)
	{
		var tab = tabs[i];
		tab.classList.add('hidden');
	}

	var tab = document.getElementById('tab_' + tabName);
	if (!tab)
	{
		return;
	}
	tab.classList.remove('hidden');

	var tabButtons = document.getElementsByClassName('tab_button');
	if (!tabButtons)
	{
		return;
	}

	for (var i = 0; i < tabButtons.length; ++i)
	{
		var tabButton = tabButtons[i];
		tabButton.classList.remove('tab_button_selected');
	}

	var tabButton = document.getElementById('tab_button_' + tabName);
	if (!tabButton)
	{
		return;
	}
	tabButton.classList.add('tab_button_selected');
}

function hidePopup()
{
	var popup = document.getElementById('popup');
	popup.style.display = 'none';
}

var responseID = 0;

function addResponse(response)
{
	var id = 'res_' + ++responseID;

	var className;

	switch (response[21])
	{
		case 'i': // info
			hidePopup();
			className = 'responseinfo';
			setTimeout(function()
			{
				deleteElement(id);
			}, 2000);
			break;

		case 'w': // warning
			hidePopup();
			className = 'responsewarning';
			break;

		case 'e': // error
			className = 'responseerror';
			break;

		default:
			className  = 'responseunknown';
	}

	var preparedResponse = '<div id="' + id + '" class="' + className + '" onClick="deleteElement(\'' + id + '\');">' + response.substring(22, response.length - 7) + '</div>';
	var responses = document.getElementById('responses');
	responses.innerHTML += preparedResponse;
}

function submitEditForm()
{
	var url = '/?';
	var form = document.getElementById('editform');
	var i = 0;
	while (true)
	{
		var formElement = form[i];
		if (formElement == undefined)
		{
			break;
		}
		if (i > 0)
		{
			url += '&';
		}
		url += formElement.name;
		url += '=';
		if (formElement.type == 'checkbox')
		{
			url += formElement.checked;
		}
		else
		{
			url += formElement.value;
		}
		++i;
	}
	var xmlHttp = new XMLHttpRequest();
	xmlHttp.onreadystatechange = function() {
		if (xmlHttp.readyState !== 4 || xmlHttp.status !== 200)
		{
			return;
		}
		addResponse(xmlHttp.responseText);
	}
	xmlHttp.open('GET', url, true);
	xmlHttp.send(null);
	return false;
}

function locoSpeedSliderChange(locoId)
{
	var slider = document.getElementById('locospeed_' + locoId);
	if (!slider)
	{
		return false;
	}
	var url = '/?cmd=locospeed&loco=';
	url += locoId;
	url += '&speed=';
	url += slider.value;
	fireRequestAndForget(url);
	return false;
}

function fireRequestAndForget(url)
{
	var xmlHttp = new XMLHttpRequest();
	xmlHttp.open('GET', url, true);
	xmlHttp.send(null);
}

function selectHasValue(selectId, value) {
	obj = document.getElementById(selectId);
	if (!obj)
	{
		return false;
	}
	return (obj.innerHTML.indexOf('value="' + value + '"') >= 0);
}

var noSleep = new NoSleep();
document.addEventListener('click', function enableNoSleep() {
	document.removeEventListener('click', enableNoSleep, false);
	noSleep.enable();
}, false);

var updater = new EventSource('/?cmd=updater');
updater.onmessage = function(event) {
	dataUpdate(event);
};
updater.onerror = function(event) {
	setTimeout(function() {
		location.reload();
	}, 1000);
};

window.layoutPosX = 0;
window.layoutPosY = 0;

