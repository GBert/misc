<div class="subcontainer" id="settings_container">
	<div id="sidebar">
		<div class="button" id="sidebar_show_button" style="display: none;">
			<div></div>
			<div></div>
			<div></div>
		</div>
		<div id="sidebar_button_container">
			<div class="button_active">CAN-Geräte</div>
			<!--<div class="button">CAN-Monitor</div>-->
			<div class="button">Lokbilder</div>
			<!--<div class="button">CV-Programmierung</div>-->
			<div class="button">Server-Einstellungen</div>
		</div>
	</div>


	<!-- CAN-Devices -->
	<div class="frame_content frame_content_active" id="can_devices">
		<div>
			<h1>CAN-Geräte<div class="button" id="can_devices_help">?</div></h1>
		</div>
		<div class="button" id="clear_device_list" style="display: none;">Geräteliste zurücksetzen</div>
		<div class="button" id="can_dropdown_button">Gerät auswählen</div>
		<div id="can_dropdown_container" class="dropdown">
		</div>
		<p id="disconnect_warning" style="color: red; display: none">Gerät nicht verbunden!</p>
		<div id="device_info" style="display: none;">
		<div id="delete_device" class="button power_button">Aus Geräteliste löschen</div>
			<h2>Informationen:</h2>
			<p class="ml30"></p>
			<p class="ml30"></p>
			<p class="ml30"></p>
			<p class="ml30"></p>	
			<p class="ml30"></p>	
			<h2 id="readings_h" style="display: none;">Messwerte:</h2>
			<div id="readings" style="display: none;">
			</div>
			<h2 id="config_h" style="display: none;">Einstellungen:</h2>
			<div id="config_container">
				<div id="config" class="locolist_grid">
				</div>
			</div>
		</div>
	</div>


	<!-- CAN-Monitor

	<div class="frame_content">
		<h1>CAN-Monitor</h1>
		<div class="icon_list console" id="can_monitor">
		</div>
	</div>-->

	<!-- Loco icons -->
	<div class="frame_content">
		<div>
			<h1>Lokbilder<div class="button" id="loco_icons_help">?</div></h1>
		</div>
		<h2>Lokbilder Hochladen:</h2>
		<p>Lokbilder zum Server Hochladen. Es können mehrere Bilder gleichzeitig hochgeladen werden.</p>
		<form action="./php/upload.php" method="post", enctype="multipart/form-data" target="php_dummy">
			<label class="button">Lokbilder auswählen ...
				<input id="file_upload" name="icon[]" type="file" accept="image/*" style="display:none;" multiple> 
			</label>
			<div class="icon_list" id="icon_list_upload"></div>
			<button class="button" id="upload_icon" style="width: calc(100% - 20px);">Hochladen</button>
		</form>
		<iframe name="php_dummy" style="display:none;"></iframe>
		<h2>Oder Downloadink eingeben:</h2>
		<p>URL eines Lokbildes aus dem Internet eingeben. Mehrere Bilder können durch Leerzeichen getrennt eingegeben werden.</p>
		<input type="text" name="icon_download_link" id="icon_link" class="text_input" style="width: calc(100% - 30px); display: block;" placeholder="Downloadlink...">
		<div class="icon_list" id="icon_list_download"></div>
		<div class="button" id="download_icon">Herunterladen</div>
		<h2>Vorhandene Lokbilder:</h2>
		<div id="icon_list" class="locolist_grid">
			<?php
					$dirname = "loco_icons/";
					$images = scandir($dirname);
//					shuffle($images);
					$ignore = Array(".", "..", "default.png");
					foreach($images as $curimg){
						if(!in_array($curimg, $ignore) && (strpos($curimg, 'jpg') || strpos($curimg, 'png'))) {
							$js_function = "setIcon(`".$curimg."`);";
							echo '
								<div class="icon_list text_input" id="'.$curimg.'_img">
									<img class="preview_icon" src="'.$dirname.$curimg.'"/>
								</div>
								<div id="'.$curimg.'_name">
									<div class="button" style="margin: 0; max-width: 150px;" onclick="deleteIcon(`'.$curimg.'`)" >Löschen</div>
									<p>'.$curimg.'</p>
								</div>
							';
						}
					}                 
				?>
		</div>
	</div>


	<!-- CV programming
	<div class="frame_content">
		<h1>CV-Programmierung</h1>
	</div>-->


	<!-- Server settings -->
	<div class="frame_content" id="server_settings">
		<div>
			<h1>Server-Einstellungen<div class="button" id="server_settings_help">?</div></h1>
		</div>
		<div class="button" id="search_updates_button" style="display: none;">Updates suchen ...</div>
		<div class="button_grid_2">
			<div class="button power_button" id="restart_button">Server Neustarten</div>
			<div class="button power_button" id="shutdown_button">Server Herunterfahren</div>
		</div>
		<h2>Informationen:</h2>
		<p class="ml30" id="version"></p>
		<p class="ml30">Betriebssystem: <?php echo php_uname('s'); echo " "; echo php_uname('r'); echo " "; echo php_uname('m'); ?></p>
		<h2>Gleisprotokolle:</h2>
		<div class="button_grid_3">
			<div class="button protocol_button">Motorola</div>
			<div class="button protocol_button">DCC</div>
			<div class="button protocol_button">mfx</div>
		</div>
		<h2>MFX/M4:</h2>
		<div class="button" id="set_naz">Neuanmeldezähler setzen</div>

	</div>
</div>

<script type="text/javascript" src="js/main.js"></script>
<script type="text/javascript">

/*
* ----------------------------------------------------------------------------
* "THE BEER-WARE LICENSE" (Revision 42):
* <ixam97@ixam97.de> wrote this file. As long as you retain this notice you
* can do whatever you want with this stuff. If we meet some day, and you think
* this stuff is worth it, you can buy me a beer in return.
* Maximilian Goldschmidt
* ----------------------------------------------------------------------------
* MäCAN-Server
* https://github.com/Ixam97/MaeCAN-Server/
* ----------------------------------------------------------------------------
*/

	const buttons = document.getElementsByClassName('button');
	const containers = document.getElementsByClassName('frame_content');
	const protocol_button = document.getElementsByClassName('protocol_button')

	const can_dropdown_button = document.getElementById('can_dropdown_button');
	const can_dropdown_container = document.getElementById('can_dropdown_container');
	const search_updates_button = document.getElementById('search_updates_button');
	const restart_button = document.getElementById('restart_button');
	const shutdown_button = document.getElementById('shutdown_button');
	const settings_container = document.getElementById('settings_container');
	const sidebar = document.getElementById('sidebar');
	const sidebar_button_container = document.getElementById('sidebar_button_container');
	const sidebar_show_button = document.getElementById('sidebar_show_button');


	let dropdown_visible = false;
	
	let devices = [];
	let visible_device = {};
	let protocol = 0;

	let local_version;
	let online_version;

	var upload_icons;

	//--- Allgemein ---//

	function resizeSettings(){
		let container_width = parent.body.clientWidth - 10;
		if (container_width < 540) {
			hide(sidebar_button_container);
			show(sidebar_show_button);
			sidebar.style.width = "50px";
			for (let i = 0; i < containers.length; i++){
				containers[i].style.width = "calc(100% - 50px)";
			}
		} else if (container_width >= 540) {
			show(sidebar_button_container);
			hide(sidebar_show_button);
			sidebar.style.width = "200px";
			for (let i = 0; i < containers.length; i++){
				containers[i].style.width = "calc(100% - 200px)";
			}
		}

		can_dropdown_container.style.width = can_dropdown_button.clientWidth + "px";
	}

	for (let i = 0; i < sidebar_button_container.children.length; i++) ((i)=>{
		sidebar_button_container.children[i].onclick = function(){
			for (let j = 0; j < containers.length; j++) {
				containers[j].setAttribute('class', "frame_content");
				sidebar_button_container.children[j].setAttribute('class', "button");
				if (settings_container.clientWidth < 540) {
					hide(sidebar_button_container);
				}
			}
			sidebar_button_container.children[i].setAttribute('class', 'button_active');
			containers[i].setAttribute('class', 'frame_content frame_content_active');
			return false;
		}
	})(i);

	sidebar_show_button.onclick = function(){
		if (sidebar_button_container.style.display == 'block') {
			hide(sidebar_button_container);
		} else {
			show(sidebar_button_container);
		}
	};




	//--- CAN-Devices ---//

	function showDeviceInfo(device){
		
		show(device_info);
		can_dropdown_button.innerHTML = device.name + ' #' + device.serial_number;
		device_info.children[1].innerHTML = "Name: " + device.name;
		device_info.children[2].innerHTML = "Seriennummer: " + device.serial_number;
		device_info.children[3].innerHTML = "Artikelnummer: " + device.product_number;
		device_info.children[4].innerHTML = "Softwareversion: " + device.version;
		device_info.children[5].innerHTML = "UID: 0x" + device.uid.toString(16);

		if (device.status_chanels) {
			show(readings);
			show(readings_h);
			createReadings(device);
		} else {
			hide(readings);
			hide(readings_h);
		}

		if (device.config_chanels) {
			show(config_container);
			show(config_h);
			createConfig(device);
		} else {
			hide(config_container);
			hide(config_h);
		}
	}

	function createDropdownPoint(name){
		let obj_text = document.createTextNode(name);
		let obj = document.createElement('div');
		obj.setAttribute('class', 'can_dropdown_option');
		obj.appendChild(obj_text);
		can_dropdown_container.appendChild(obj);
	}

	function getReadingName(name) {
		if (name == 'TRACK') {
			return "Gleisstrom";
		} else if (name == "HGL") {
			return "Hauptgleis";
		} else if (name == "PGL") {
			return "Programmiergleis";
		} else if (name == "VOLT") {
			return "Versorgungsspannung";
		} else if (name == "TEMP") {
			return "Temperatur";
		} else if (name == "TEMPO") {
			return "Geschwindigkeit";
		} else if (name == "ACHSEN") {
			return "Achsen";
		} else {
			return name;
		}
	}

	function createReadings(device) {
		readings.innerHTML = "";
		fillers = [];
		for (let i = 0; i < device.status_chanels; i++){
			let chanel = device.status_chanels_info[i];

			let bar_container = document.createElement("DIV");
			bar_container.setAttribute('class', 'bar_container');

			let text = document.createElement("P");
			text.setAttribute('class', 'ml30');
			text.id = 'reading_name_' + chanel.chanel;
			let reading_name = document.createTextNode(getReadingName(device.status_chanels_info[i].name) + ":");
			text.appendChild(reading_name);
			bar_container.appendChild(text);

			let bar = document.createElement('DIV');
			bar.setAttribute('class', 'readings_bar ml30');
			bar_container.appendChild(bar);

			let range_1 = document.createElement('DIV');
			range_1.style.backgroundColor = chanel.color_1;
			range_1.style.width = (chanel.range_1 / chanel.range_4)*100 + "%";
			bar.appendChild(range_1);

			let range_2 = document.createElement('DIV');
			range_2.style.backgroundColor = chanel.color_2;
			range_2.style.width = ((chanel.range_2 / chanel.range_4) - (chanel.range_1 / chanel.range_4))*100 + "%";
			bar.appendChild(range_2);

			let range_3 = document.createElement('DIV');
			range_3.style.backgroundColor = chanel.color_3;
			range_3.style.width = ((chanel.range_3 / chanel.range_4) - (chanel.range_2 / chanel.range_4))*100 + "%";
			bar.appendChild(range_3);

			let range_4 = document.createElement('DIV');
			range_4.style.backgroundColor = chanel.color_4;
			range_4.style.width = (1 - (chanel.range_3 / chanel.range_4))*100 + "%";
			bar.appendChild(range_4);
			
			let bar_filler = document.createElement('DIV');
			bar_filler.setAttribute('class', 'bar_filler');
			bar_filler.id = 'bar_filler_' + chanel.chanel;
			bar.appendChild(bar_filler);

			readings.appendChild(bar_container);
		}
	}

	function setConfigValue(uid, chanel, value) {
		parent.ws.send(`setConfigValue:${uid}:${chanel}:${value}`);
		console.log(`setConfigValue:${uid}:${chanel}:${value}`);
	}

	function createConfig(device) {
		config.innerHTML = "";

		for (let i = 0; i < device.config_chanels; i++) {
			let chanel = device.config_chanels_info[i];

			let name = document.createElement('P');
			name.setAttribute('class', 'ml30');
			let text = document.createTextNode(chanel.description + ':');
			name.appendChild(text);

			config.appendChild(name);

			if (chanel.type == 1) {
				let config_dropdown = document.createElement('select');
				config_dropdown.setAttribute('id', "config_chanel_" + chanel.chanel);
				config_dropdown.setAttribute("class", "text_input");
				for (let j = 0; j < chanel.num_options; j++) {
					let option_name = document.createTextNode(chanel.options[j]);
					let option = document.createElement('option');
					option.setAttribute('value', j);
					option.appendChild(option_name);
					config_dropdown.appendChild(option);
				}
				config_dropdown.value = chanel.def_option;
				config_dropdown.setAttribute('onchange', `setConfigValue(${device.uid},${chanel.chanel},config_chanel_${chanel.chanel}.value)`);
				config.appendChild(config_dropdown);
			} else {
				let config_num = document.createElement('input');
				let config_num_container = document.createElement('div');
				config_num.setAttribute('type', 'number');
				config_num.setAttribute('id', 'config_chanel_' + chanel.chanel);
				config_num.setAttribute('class', 'text_input');
				config_num.setAttribute('min', chanel.start);
				config_num.setAttribute('max', chanel.end);
				config_num.setAttribute('value', chanel.def_value);
				config_num.setAttribute('style', "width: calc(100% - 60px); margin-right: 5px;");
				config_num.setAttribute('onchange', `setConfigValue(${device.uid},${chanel.chanel},config_chanel_${chanel.chanel}.value)`);

				let config_unit = document.createTextNode(chanel.unit);

				config_num_container.appendChild(config_num);
				config_num_container.appendChild(config_unit);

				config.appendChild(config_num_container);
			}
		}
	}

	function updateFiller(uid, index, value) {
		if (uid == visible_device.uid) {
			visible_device.status_chanels_info.filter((chanels)=>{
				if (chanels.chanel == index) {
					let start = parseFloat(chanels.start);
					let end = parseFloat(chanels.end);
					let origin = chanels.origin;
					let max = chanels.range_4;
					let divider = (end - start) / (max - origin);
					let clear_value = (start - (origin * divider) + (value * divider)).toFixed(3);
					value = (100 / chanels.range_4) * value;
					document.getElementById('bar_filler_' + index).style.width = (100 - value) + "%";
					document.getElementById('reading_name_' + index).innerHTML = getReadingName(chanels.name) + ": " + clear_value + " " + chanels.unit;
					console.log('updating chanel ' + index + ' to value ' + value + "%." );
				}
			})
		}
	}

	can_dropdown_button.onclick = function(){
		if (dropdown_visible) {
			dropdown_visible = false;
			can_dropdown_container.setAttribute('class', 'dropdown');
			can_dropdown_button.setAttribute('class', 'button');
		} else {
			parent.ws.send('getDevicelist');
			dropdown_visible = true;
			can_dropdown_container.setAttribute('class', 'dropdown dropdown_visible');
			can_dropdown_button.setAttribute('class', 'button button_dropdown');
		}
		can_dropdown_container.style.width = can_dropdown_button.clientWidth + "px";
	}

	function updateDevicelist(_devices) {
		devices = JSON.parse(_devices);
		console.log(devices);
		can_dropdown_container.innerHTML = '';
		if (devices.length == 0) {
			createDropdownPoint("Keine Geräte gefunden!");
		}
		for (let i = 0; i < devices.length; i++) {
			createDropdownPoint(devices[i].name + " #" + devices[i].serial_number);
		}
		let can_dropdown_option = document.getElementsByClassName('can_dropdown_option');
		
		for (let i = 0; i < can_dropdown_option.length; i++) ((i)=>{
			if (devices.length) {
				can_dropdown_option[i].onclick = function(){
					dropdown_visible = false;
					can_dropdown_container.setAttribute('class', 'dropdown');
					can_dropdown_button.setAttribute('class', 'button');
					showDeviceInfo(devices[i]);
					parent.ws.send('ping');
					visible_device = devices[i];
				}
			};
		})(i);
	};

	let getReadings = setInterval(()=>{
		if (!(readings.offsetParent === null) && devices) {
			if (visible_device.status_chanels){
				for (let j = 0; j < visible_device.status_chanels; j++){
					parent.ws.send(`getStatus:${visible_device.uid}:${visible_device.status_chanels_info[j].chanel}`);
				}
			}
		}
	}, 1000); 


	// --- loco icons --- //

	function fileSelection(evt) {
		icon_list_upload.innerHTML = "";
		let files = evt.target.files;
		upload_icons = files;
		for (let i = 0, f; f = files[i]; i++) {
			if (!f.type.match('image.*')) {
				continue;
			}
			let reader = new FileReader();
			reader.onload = (function(theFile) {
				return function(e){
					let preview = document.createElement('img');
					preview.className = 'preview_icon';
					preview.src = e.target.result;
					preview.title = theFile.name;
					icon_list_upload.insertBefore(preview, null);
				}
			})(f);
			reader.readAsDataURL(f);
		}
	}

	function realTimeDownloadImage(evt) {
		icon_list_download.innerHTML = "";
		let links = icon_link.value.split(' ');
		for (let i = 0; i < links.length; i++) {
			if(links[i] != '') {
				let preview = document.createElement('img');
				preview.className = 'preview_icon';
				preview.src = links[i];
				preview.alt = "Ungültiger Link!";
				icon_list_download.insertBefore(preview, null);
			}
		}
	}

	function deleteIcon(icon_name) {
		parent.send(`delIcon:${icon_name}`);
		document.getElementById(icon_name + '_img').remove();
		document.getElementById(icon_name + '_name').remove();
		parent.locolist.contentWindow.document.getElementById(icon_name).remove();
	}

	function addIconToLists(icon_name) {
		// Lokliste:
		let icon_img = document.createElement('img');
		icon_img.className = "preview_icon";
		icon_img.src = "loco_icons/" + icon_name;
		icon_img.id = icon_name;
		icon_img.setAttribute('onclick', 'setIcon("' + icon_name + '")');
		parent.locolist.contentWindow.icon_preview.appendChild(icon_img);

		// Lokbilder:
		let icon_img_div = document.createElement('div');
		icon_img_div.className = 'icon_list text_input';
		icon_img_div.id = icon_name + '_img';
		let icon_img_img = document.createElement('img');
		icon_img_img.src = 'loco_icons/' + icon_name;
		icon_img_img.className = 'preview_icon';

		icon_img_div.appendChild(icon_img_img);
		icon_list.appendChild(icon_img_div);

		let icon_name_div = document.createElement('div');
		icon_name_div.id = icon_name + '_name';
		let icon_name_button = document.createElement('div');
		icon_name_button.className = "button";
		icon_name_button.setAttribute('style', 'margin: 0; max-width: 150px;');
		icon_name_button.setAttribute('onclick', 'deleteIcon("' + icon_name + '")');
		icon_name_button.innerHTML = 'Löschen';
		let icon_name_p = document.createElement('p');
		icon_name_p.innerHTML = icon_name;

		icon_name_div.appendChild(icon_name_button);
		icon_name_div.appendChild(icon_name_p);
		icon_list.appendChild(icon_name_div);


	}

	file_upload.addEventListener('change', fileSelection, false);

	icon_link.addEventListener('input', realTimeDownloadImage, false);

	upload_icon.onclick = function() {
		icon_list_upload.innerHTML = "";
		setTimeout(() => {
			for (let i = 0; i < upload_icons.length; i++) {
				addIconToLists(upload_icons[i].name);
			}
		}, 500*upload_icons.length);
	}

	download_icon.onclick = function() {
		parent.ws.send('downloadIcon:' + icon_link.value);
		icon_list_download.innerHTML = "";
		let icons = icon_link.value.split(' ');
		icon_link.value = '';
		setTimeout(() => {
			for (let i = 0; i < icons.length; i++) {
				let icon_name = icons[i].split('/')[icons[i].split('/').length-1];
				addIconToLists(icon_name);
			}
		}, 500*icons.length);
	}

	set_naz.onclick = () => {
		parent.send('mfxDiscovery');
	}


	// --- Server Settings --- //

	function getOnlineVersion(){
		/*let version = 0;
		let version_request = new XMLHttpRequest();
		
		version_request.open('GET', "https://raw.githubusercontent.com/Ixam97/MaeCAN-Server/master/html/config/version.txt", true);
		version_request.onload = function(){
			if (this.status == 200){
				version = parseInt(this.responseText);
				if (version != false) {
					console.log("local: " + local_version + ", latest: " + version);

					if (parseInt(local_version) >= parseInt(version)) {
						parent.up_to_date_p.innerHTML = "Server ist auf dem aktuellen Stand.<br> Versions-ID " + version + ".";
						parent.showAlertbox(parent.up_to_date);
					} else {
						parent.update_available_p.innerHTML = 'Update auf Versions-ID ' + version + ' (aktuell ' + local_version + ') ist verfügbar! Jetzt updaten?';
						parent.showAlertbox(parent.update_available);
					}
				} else {
				parent.update_available_p.innerHTML = "Keine Internetverbindung vorhanden!";
				parent.showAlertbox(parent.update_available);
				}
			}
		}
		version_request.send();*/

	}

	function updateProtocol(new_prtocol) {
		if(new_prtocol & 0b001) {
			protocol_button[0].setAttribute('class', 'button button_active protocol_button');
		} else {
			protocol_button[0].setAttribute('class', 'button protocol_button');
		}
		if(new_prtocol & 0b010) {
			protocol_button[2].setAttribute('class', 'button button_active protocol_button');
		} else {
			protocol_button[2].setAttribute('class', 'button protocol_button');
		}
		if(new_prtocol & 0b100) {
			protocol_button[1].setAttribute('class', 'button button_active protocol_button');
		} else {
			protocol_button[1].setAttribute('class', 'button protocol_button');
		}
		protocol = new_prtocol;
	}

	function hexString(value) {
		let _value = "";

		if (value < 0x10) {
			_value = "0" + parseInt(value).toString(16);
		} else {
			_value = "" + parseInt(value).toString(16);
		}

		return _value;
	}

	function updateCanMonitor(msg) {
		let cmd = hexString(msg[1]);
		let hash = hexString(msg[2]);
		let dlc = msg[3];
		let data_array = msg[4].split(',');
		let data = "";
		let time = new Date();

		for (let i = 0; i < parseInt(dlc); i++) {
			data += (hexString(data_array[i]) + ' ');
		}

		let console_line = document.createElement('P');
		let console_line_text = document.createTextNode(time.getSeconds().toString() + time.getMilliseconds().toString() + ' CMD: ' + cmd + '   Hash: ' + hash + '   DLC: ' + dlc + '   Data: ' + data);
		console_line.appendChild(console_line_text);
		can_monitor.appendChild(console_line);

		document.getElementById('can_monitor').lastChild.scrollIntoView();
	}

	function deviceConnectionStatus(uid, status) {
		if (visible_device.uid == uid) {
			console.log("Status of " + uid + ": " + status);
			if (status == 'false') {
				show(disconnect_warning);
			} else {
				hide(disconnect_warning);
			}
		}
	}

	clear_device_list.onclick = () => {
		parent.ws.send('clearDeviceList');
		can_dropdown_button.innerHTML = "Gerät auswählen";
		hide(device_info);
	}

	search_updates_button.onclick = function(){
		//local_version = <?php echo $version; ?>;
		//getOnlineVersion();
	};

	restart_button.onclick = function(){
		parent.showAlertbox(parent.restart);
	};

	shutdown_button.onclick = function(){
		parent.showAlertbox(parent.shutdown);
	};

	can_devices_help.onclick = () =>{
		parent.showHelp("settings.html#can_devices");
	}

	delete_device.onclick = () => {
		parent.send(`delDevice:${visible_device.uid}`);
		can_dropdown_button.innerHTML = 'Gerät auswählen';
		hide(device_info);
		hide(disconnect_warning);
	}

	loco_icons_help.onclick = () => {
		parent.showHelp('settings.html#loco_icons');
	}

	server_settings_help.onclick = () => {
		parent.showHelp('settings.html#server_settings');
	}

	for (let i = 0; i < protocol_button.length; i++) {
		protocol_button[i].onclick = function(){
			let _protocol;
			if (i == 0){
				if (protocol & 0b001) {
					_protocol = protocol ^ 0b001;
				} else {
					_protocol = protocol | 0b001;
				}
			}
			if (i == 1) {
				if (protocol & 0b100) {
					_protocol = protocol ^ 0b100;
				} else {
					_protocol = protocol | 0b100;
				}
			}
			if (i == 2) {
				if (protocol & 0b010) {
					_protocol = protocol ^ 0b010;
				} else {
					_protocol = protocol | 0b010;
				}
			}
			console.log(`setProtocol:${_protocol}`)
			parent.ws.send(`setProtocol:${_protocol}`);
		};
	}

	setTimeout(function(){
		parent.ws.send('getProtocol');
		parent.ws.send('getVersion');
	}, 1000);

</script>
