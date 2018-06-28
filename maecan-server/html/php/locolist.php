<div class="subcontainer" id="settings_container">
	<div id="sidebar">
		<div class="button" id="sidebar_show_button" style="display: none;">
			<div></div>
			<div></div>
			<div></div>
		</div>
		<div id="sidebar_button_container">
			<div class="button button_active" id="add_loco">Neue Lok anlegen</div>
			<div id="locolist_container">

			</div>
			<div class="button" id="reload_loco_list">Neu Laden</div>
		</div>
	</div>
	<div class="frame_content frame_content_active" id="frame">
	
		<h1>Neue Lok anlegen</h1>

		<style>
		</style>

		<div id="locolist_grid" class="locolist_grid">
			<p>Name:</p>
			<input id="name" type="text" class="text_input" placeholder="Lokname...">
			<!--
			<p>Lokbild:</p>
			<div id="current_icon" class="icon_list text_input">
				<img src="" id="current_icon_img">
			</div>
			<p></p>
			<div class="icon_list text_input">
				<?php
					$dirname = "loco_icons/";
					$images = scandir($dirname);
					shuffle($images);
					$ignore = Array(".", "..");
					foreach($images as $curimg){
						if(!in_array($curimg, $ignore) && (strpos($curimg, 'jpg') || strpos($curimg, 'png'))) {
							echo "<img class='preview_icon' src=".$dirname.$curimg." />";
						}
					}                 
				?>
			</div>
			-->
			<p>Protokoll:</p>
			<select name="Protokoll" id="protocol_dropdown" class="text_input">
				<option id="dcc" value="dcc">DCC</option>
				<option id="mm_prog" value="mm_prog">Motorola (Programmierbar)</option>
				<option id="mm_dil8" value="mm_dil8">Mototola (Kodierschalter)</option>
				<option id="mfx" value="mfx" disabled>MFX</option>
			</select>
			<!--
			<p>Fahrstufen:</p>
			<select name="Fahrstufen" id="ds_dropdown" class="text_input">
				<option id="ds14" value="ds14">14</option>
				<option id="ds28" value="ds28" selected>28</option>
				<option id="ds126" value="ds126">126</option>
			</select>-->
			<p>Adresse:</p>
			<input id="adress" type="number" class="text_input" min="1" max="2048" value="1">

			<p>Anfahrverz.:</p>
			<input id="av" type="number" class="text_input"  min="0" max="255" value="60" >

			<p>Bremsverz.:</p>
			<input id="bv" type="number" class="text_input"  min="0" max="255" value="40" >

			<p>Vmax:</p>
			<input id="vmax" type="number" class="text_input"  min="0" max="255" value="255" >

			<p>Vmin:</p>
			<input id="vmin" type="number" class="text_input"  min="0" max="255" value="3" >

			<p>Tacho:</p>
			<input id="tacho" type="number" class="text_input"  min="0" max="300" value="120" disabled>
			<!--
			<p>Funktionen:</p>
			<div class="function_div">
				<p>F0:</p>
				<select name="F0" id="f0" class="text_input">
					<option id="deactivated" value="0">Deaktiviert</option>
					<option id="generic" value="2">Generische Funktion</option>
					<option id="light" value="3">Lichtfunktion</option>
				</select>
				<p>F1:</p>
				<select name="F1" id="f1" class="text_input">
					<option id="deactivated" value="0">Deaktiviert</option>
					<option id="generic" value="2">Generische Funktion</option>
					<option id="light" value="3">Lichtfunktion</option>
				</select>
				<p>F2:</p>
				<select name="F2" id="f2" class="text_input">
					<option id="deactivated" value="0">Deaktiviert</option>
					<option id="generic" value="2">Generische Funktion</option>
					<option id="light" value="3">Lichtfunktion</option>
				</select>
				<p>F3:</p>
				<select name="F3" id="f3" class="text_input">
					<option id="deactivated" value="0">Deaktiviert</option>
					<option id="generic" value="2">Generische Funktion</option>
					<option id="light" value="3">Lichtfunktion</option>
				</select>
				<p>F4:</p>
				<select name="F4" id="f4" class="text_input">
					<option id="deactivated" value="0">Deaktiviert</option>
					<option id="generic" value="2">Generische Funktion</option>
					<option id="light" value="3">Lichtfunktion</option>
				</select>
			</div>
			-->
		</div>
		

		<div class="button_grid_3">
			<div class="button button_active" id="save_loco">Lok speichern</div>
			<div class="button" id="prog_loco">Lok programmieren</div>
			<div class="button power_button" id="delete_loco">Lok löschen</div>
		</div>
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
* MäCAN-Server, 2018-06-24
* https://github.com/Ixam97/MaeCAN-Server/
* ----------------------------------------------------------------------------
*/

	const containers = document.getElementsByClassName('frame_content');
	const loco_button = document.getElementsByClassName('loco_button');
	const preview_icon = document.getElementsByClassName('preview_icon');

	const settings_container = document.getElementById('settings_container');
	const sidebar = document.getElementById('sidebar');
	const sidebar_button_container = document.getElementById('sidebar_button_container');
	const sidebar_show_button = document.getElementById('sidebar_show_button');
	const name = document.getElementById('name');
	const adress = document.getElementById('adress');

	let current_loco = {};
	let current_loco_index = -1;
	let locolist = [];

	const loco_template = {
		uid: 0xc001,
		name: "Neue Lok",
		adress: 1,
		typ: "dcc",
		mfxuid: 0xffffffff,
		av: 60,
		bv: 40,
		volume: 100,
		vmax: 255,
		vmin: 13,
		functions: [1,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65]
	}

	//--- Allgemein ---//


	// Lokliste Laden

	function createLocoButton(loco){
		let loco_button = document.createElement('div');
		let loco_name = document.createTextNode(loco.name);
		loco_button.className = 'button loco_button';
		locolist_container.appendChild(loco_button);
		loco_button.appendChild(loco_name);
		if (loco.icon) {
			let loco_icon = document.createElement('img');
			loco_icon.src = './loco_icons/' + loco.icon;
			loco_button.appendChild(document.createElement('br'));
			loco_button.appendChild(loco_icon);
		}

		return loco_button;
	}

	function setNewLoco() {

		console.log("Setting to new loco.");

		// Aktive Schaltfläche
		for (let i = 0; i < loco_button.length; i++) {
			loco_button[i].className = "button loco_button";
		}
		add_loco.className = "button button_active"

		frame.children[0].innerHTML = "Neue Lok anlegen";

		//Lok zurücksetzen
		current_loco = loco_template;
		current_loco_index = -1;

		name.value = current_loco.name;
		protocol_dropdown.value = current_loco.typ;
		adress.value = current_loco.adress;
		av.value = current_loco.av;
		bv.value = current_loco.bv;
		vmax.value = current_loco.vmax;
		vmin.value = current_loco.vmin;
		protocol_dropdown.disabled = false;
		adress.disabled = false;

	}

	function setActiveLoco(index) {

		console.log("Setting active loco: " + index);

		//Aktive Schaltfläche
		add_loco.className = "button";
		for (let i = 0; i < loco_button.length; i++) {
			loco_button[i].className = "button loco_button";
		}
		loco_button[index].className = "button button_active loco_button";


		//Lok aus Lokliste laden
		current_loco = locolist[index];
		current_loco_index = index;

		name.value = current_loco.name;
		protocol_dropdown.value = current_loco.typ;
		adress.value = current_loco.adress;
		av.value = current_loco.av;
		bv.value = current_loco.bv;
		vmax.value = current_loco.vmax;
		vmin.value = current_loco.vmin;
		//tacho.value = current_loco.tacho;
		frame.children[0].innerHTML = (current_loco.name + " bearbeiten:");

		if (current_loco.typ == "mfx") {
			protocol_dropdown.disabled = true;
			adress.disabled = true;
		} else {
			protocol_dropdown.disabled = false;
			adress.disabled = false;
		}

	}

	function loadLocolist(){
		locolist_container.innerHTML = "";
		let locolist_request = new XMLHttpRequest();
		let date = new Date();

		locolist_request.open('GET', './config/locolist.json?' + date.getTime(), true);
		locolist_request.onload = function(){
			if (this.status == 200) {
				locolist = JSON.parse(this.responseText);
				for (let i = 0; i < locolist.length; i++) {
					createLocoButton(locolist[i]).onclick = function(){
						setActiveLoco(i);
					}
				}
				setNewLoco();
			}
		}

		locolist_request.send();
	}

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
	}

	function applyCurrentLoco() {
		current_loco.name = name.value;
		current_loco.typ = protocol_dropdown.value;
		current_loco.adress = adress.value;
		if (current_loco.typ == "dcc") {
			current_loco.uid = parseInt(current_loco.adress) + 0xc000;
		} else if (current_loco.typ == "mfx"){
			current_loco.uid = parseInt(current_loco.adress) + 0x4000;
		} else {
			current_loco.uid = current_loco.adress;
		}
		current_loco.av = av.value;
		current_loco.bv = bv.value;
		current_loco.vmax = vmax.value;
	}

	/*function setAvailableSettings() {
		let protocol = protocol_dropdown.value;
		if (protocol != "dcc") {
			ds_dropdown.disabled = true;
		} else {
			ds_dropdown.disabled = false;
		}
	}*/

	sidebar_show_button.onclick = function(){
		if (sidebar_button_container.style.display == 'block') {
			hide(sidebar_button_container);
		} else {
			show(sidebar_button_container);
		}
	};

	for (let i = 0; i < sidebar_button_container.children.length; i++) {
		sidebar_button_container.children[i].onclick = () => {
			resizeSettings();
		};
	}

	add_loco.onclick = () => { 
		setNewLoco();
		resizeSettings();	
	};
	
	reload_loco_list.onclick = () => { 
		loadLocolist();
		//resizeSettings();
		};

	//protocol_dropdown.onchange = () => { setAvailableSettings() };

	// --- Eingabe --- //

	save_loco.onclick = function(){

		if (name.value.indexOf('$') > -1 || name.value.indexOf('§') > -1) {
			// Ungültiger Lokname
		} else if ( av.value > 255 || bv.value > 255 || vmax.value > 255 || vmin.value > 255 || tacho.value > 255) {
			// Ungültige Config-Werte.
		} else {
			applyCurrentLoco();

			loco_string = JSON.stringify(current_loco);

			console.log(loco_string);
			
			if (current_loco_index == -1 ) {
				parent.send(`addLoco:$${loco_string}§`);
			} else {
				parent.send(`updateLoco:${current_loco_index}:$${loco_string}§`);
			}
		}
	};

	prog_loco.onclick = () => {
		document.pare
	}

	delete_loco.onclick = function(){
		if (current_loco_index >= 0) {
			parent.delLocoAlert(current_loco_index, current_loco.name);
		}
	};

	for (let i = 0; i < preview_icon.length; i++) {
		preview_icon[i].onclick = function(){
			let icon = this.src.split("/");
			icon = icon[icon.length - 1];
			console.log(icon);
		}
	}

	loadLocolist();
</script>
