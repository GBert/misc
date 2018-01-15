<?php
	$version = file_get_contents('./config/version.txt', true);
?>

<div class="subcontainer" id="settings_container">
	<div id="sidebar">
		<div class="button" id="sidebar_show_button" style="display: none;">
			<div></div>
			<div></div>
			<div></div>
		</div>
		<div id="sidebar_button_container">
			<div class="button_active">CAN-Geräte</div>
			<div class="button">Lokbilder</div>
			<!--<div class="button">CV-Programmierung</div>-->
			<div class="button">Server-Einstellungen</div>
		</div>
	</div>


	<!-- CAN-Devices -->
	<div class="frame_content frame_content_active" id="can_devices">
		<h1>CAN-Geräte</h1>
		<div class="button" id="can_dropdown_button">Gerät auswählen</div>
		<div id="can_dropdown_container" class="dropdown">
		</div>
		<div id="device_info">
			<h2>Informationen:</h2>
			<p class="ml30"></p>
			<p class="ml30"></p>
			<p class="ml30"></p>
			<p class="ml30"></p>	
			<p class="ml30"></p>	
			<h2 id="readings_h" style="display: none;">Messwerte:</h2>
			<div id="readings" style="display: none;">
			</div>
			<h2 id="settings_h" style="display: none;">Einstellungen:</h2>
			<div id="settings" style="display: none;">
			</div>
		</div>
	</div>


	<!-- Loco icons -->
	<div class="frame_content">
		<h1>Lokbilder</h1>
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
	</div>


	<!-- CV programming 
	<div class="frame_content">
		<h1>CV-Programmierung</h1>
	</div>-->


	<!-- Server settings -->
	<div class="frame_content" id="server_settings">
		<h1>Server-Einstellungen</h1>
		<div class="button" id="search_updates_button">Updates suchen ...</div>
		<div class="button power_button" id="restart_button">Server Neustarten</div>
		<div class="button power_button" id="shutdown_button">Server Herunterfahren</div>
		<h2>Informationen:</h2>
		<p class="ml30">Versions-ID: <?php echo $version; ?></p>
		<p class="ml30">Betriebssystem: <?php echo php_uname('s'); echo " "; echo php_uname('r'); echo " "; echo php_uname('m'); ?></p>
		<h2>Gleisprotokolle:</h2>
		<div id="protocol_buttons">
			<div class="button protocol_button">Motorola</div>
			<div class="button protocol_button">DCC</div>
			<div class="button protocol_button">mfx</div>
		</div>

	</div>
</div>

<script type="text/javascript" src="js/main.js"></script>
<script type="text/javascript" src="js/websocket.js"></script>
<script type="text/javascript">


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
	}

	function createDropdownPoint(name, serial_number){
		let obj_text = document.createTextNode(name + ' #' + serial_number);
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

	function updateFiller(uid, index, value) {
		if (uid == visible_device.uid) {
			visible_device.status_chanels_info.filter((chanels)=>{
				if (chanels.chanel == index) {
					let start = parseFloat(chanels.start);
					let end = parseFloat(chanels.end);
					let origin = chanels.origin;
					let max = chanels.range_4;
					let clear_value = ((((end - start) / (max - origin))*value)+start).toFixed(2);
					value = (100 / chanels.range_4) * value;
					document.getElementById('bar_filler_' + index).style.width = (100 - value) + "%";
					document.getElementById('reading_name_' + index).innerHTML = getReadingName(chanels.name) + ": " + clear_value + " " + chanels.unit;
					console.log('updating chanel ' + index + ' to value ' + value + "%." );
				}
			})
		}
	}

	can_dropdown_button.onclick = function(){
		let date = new Date();
		let device_request = new XMLHttpRequest();
		device_request.open('GET', './config/devices.json?' + date.getTime(), true);
		device_request.onload = function(){
			if (this.status == 200){
				devices = JSON.parse(this.responseText);
				can_dropdown_container.innerHTML = '';
				for (let i = 0; i < devices.length; i++) {
					createDropdownPoint(devices[i].name, devices[i].serial_number);
				}
				let can_dropdown_option = document.getElementsByClassName('can_dropdown_option');
				for (let i = 0; i < can_dropdown_option.length; i++) ((i)=>{
					can_dropdown_option[i].onclick = function(){
						dropdown_visible = false;
						can_dropdown_container.setAttribute('class', 'dropdown');
						can_dropdown_button.setAttribute('class', 'button');
						showDeviceInfo(devices[i]);
						visible_device = devices[i];
					};
				})(i);				
			}
		};
		device_request.send();
		if (dropdown_visible) {
			dropdown_visible = false;
			can_dropdown_container.setAttribute('class', 'dropdown');
			can_dropdown_button.setAttribute('class', 'button');
		} else {
			dropdown_visible = true;
			can_dropdown_container.setAttribute('class', 'dropdown dropdown_visible');
			can_dropdown_button.setAttribute('class', 'button button_dropdown');
		}
		can_dropdown_container.style.width = can_dropdown_button.clientWidth + "px";
	};

	let getReadings = setInterval(()=>{
		if (!(readings.offsetParent === null) && devices) {
			if (visible_device.status_chanels){
				for (let j = 0; j < visible_device.status_chanels; j++){
					send(`getStatus:${visible_device.uid}:${visible_device.status_chanels_info[j].chanel}`);
				}
			}
		}
	}, 1000); 


	// --- loco icons --- //

	function fileSelection(evt) {
		icon_list_upload.innerHTML = "";
		let files = evt.target.files;
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

	file_upload.addEventListener('change', fileSelection, false);

	icon_link.addEventListener('input', realTimeDownloadImage, false);

	upload_icon.onclick = function() {
		icon_list_upload.innerHTML = "";
	}

	download_icon.onclick = function() {
		ws.send('downloadIcon:' + icon_link.value);
		icon_list_download.innerHTML = "";
		icon_link.value = '';
	}


	// --- Server Settings --- //

	function getOnlineVersion(){
		let version = 0;
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
		version_request.send();

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

	search_updates_button.onclick = function(){
		local_version = <?php echo $version; ?>;
		getOnlineVersion();
	};

	restart_button.onclick = function(){
		parent.showAlertbox(parent.restart);
	};

	shutdown_button.onclick = function(){
		parent.showAlertbox(parent.shutdown);
	};

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
			ws.send(`setProtocol:${_protocol}`);
		};
	}

	setTimeout(function(){
		ws.send('getProtocol');
	}, 1000);


	// --- Misc --- //

	ws.onmessage = function(dgram){
		let msg = dgram.data.toString().split(':');
		let cmd = msg[0];
		if (cmd == 'updateReading') {
			updateFiller(msg[1], msg[2], msg[3]);
		} else if (cmd == 'updateProtocol') {
			updateProtocol(parseInt(msg[1]));
		}
	}


</script>
