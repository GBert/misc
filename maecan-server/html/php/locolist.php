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
			
			<p>Lokbild:</p>
			<div class="locolist_grid" style="margin: 0;">
				<div id="current_icon" class="icon_list text_input">
					<img src="./loco_icons/default.png" id="current_icon_img" class="preview_icon">
				</div>
				<div>
					<div class="button"	onclick="setIcon('default.png')" style="margin: 0; max-width: 150px;">Entfernen</div>
					<p id="current_icon_name">default.png</p>
				</div>
			</div>
			<p></p>
			<div class="icon_list text_input" id="icon_preview">
				<?php
					$dirname = "loco_icons/";
					$images = scandir($dirname);
//					shuffle($images);
					$ignore = Array(".", "..", "default.png");
					foreach($images as $curimg){
						if(!in_array($curimg, $ignore) && (strpos($curimg, 'jpg') || strpos($curimg, 'png'))) {
							$js_function = "setIcon(`".$curimg."`);";
							echo "<img id='$curimg' class='preview_icon' src='$dirname$curimg' onclick='setIcon(`".$curimg."`);'/>";
						}
					}                 
				?>
			</div>
		
			<p>Protokoll:</p>
			<select name="Protokoll" id="protocol_dropdown" class="text_input">
				<option id="dcc" value="dcc">DCC</option>
				<option id="mm_prog" value="mm_prog">Motorola (Programmierbar)</option>
				<option id="mm_dil8" value="mm_dil8">Mototola (Kodierschalter)</option>
				<option id="mfx" value="mfx" disabled>mfx/M4</option>
			</select>
			<!--
			<p>Fahrstufen:</p>
			<select name="Fahrstufen" id="ds_dropdown" class="text_input">
				<option id="ds14" value="ds14">14</option>
				<option id="ds28" value="ds28" selected>28</option>
				<option id="ds126" value="ds126">126</option>
			</select>-->
			<p>Adresse:</p>
			<div class="locolist_option_grid">
				<input id="adress" type="number" class="text_input" min="1" max="2048" value="1">
				<div class="button prog" id="prog_adr" onclick="progLoco('adress')">prog.</div>
			</div>

			<p>Anfahrverz.:</p>
			<div class="locolist_option_grid">
				<input id="av" type="number" class="text_input"  min="0" max="255" value="60" >
				<div class="button prog" id="prog_av" onclick="progLoco('av')">prog.</div>
			</div>

			<p>Bremsverz.:</p>
			<div class="locolist_option_grid">
				<input id="bv" type="number" class="text_input"  min="0" max="255" value="40" >
				<div class="button prog" id="prog_bv" onclick="progLoco('bv')">prog.</div>
			</div>

			<p>Vmax:</p>
			<div class="locolist_option_grid">
				<input id="vmax" type="number" class="text_input"  min="0" max="255" value="255" >
				<div class="button prog" id="prog_vmax" onclick="progLoco('vmax')">prog.</div>
			</div>

			<p>Vmin:</p>
			<div class="locolist_option_grid">
				<input id="vmin" type="number" class="text_input"  min="0" max="255" value="3" >
				<div class="button prog" id="prog_vmin" onclick="progLoco('vmin')">prog.</div>
			</div>

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
		

		<div class="button_grid_2">
			<div class="button button_active" id="save_loco">Lok speichern</div>
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
* MäCAN-Server
* https://github.com/Ixam97/MaeCAN-Server/
* ----------------------------------------------------------------------------
*/

	const containers = document.getElementsByClassName('frame_content');
	const loco_button = document.getElementsByClassName('loco_button');
	const preview_icon = document.getElementsByClassName('preview_icon');
	const prog_buttons = document.getElementsByClassName('prog');

	const settings_container = document.getElementById('settings_container');
	const sidebar = document.getElementById('sidebar');
	const sidebar_button_container = document.getElementById('sidebar_button_container');
	const sidebar_show_button = document.getElementById('sidebar_show_button');
	const name = document.getElementById('name');
	const adress = document.getElementById('adress');
	const icon_preview = document.getElementById('icon_preview');

	//var dir_list = <?//=$images?>;

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
		if (!loco.icon) loco.icon = "default.png";
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

		for (let i = 0; i < prog_buttons.length; i++) {
			hide(prog_buttons[i]);
		}
		setIcon('default.png');

	}

	function setActiveLoco(index) {

		console.log("Setting active loco: " + index);

		//Aktive Schaltfläche
		add_loco.className = "button";
		for (let i = 0; i < loco_button.length; i++) {
			loco_button[i].className = "button loco_button";
		}
		loco_button[index].className = "button loco_button button_active";


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
		
		protocol_dropdown.disabled = true;
		setIcon(current_loco.icon);

		if (current_loco.typ == "mfx") {
			adress.disabled = true;
		} else {
			adress.disabled = false;
		}

		if (current_loco.typ == "mm_dil8") {
			for (let i = 0; i < prog_buttons.length; i++) {
				hide(prog_buttons[i]);
			}
			vmin.disabled = true;
			vmax.disabled = true;
		} else if (current_loco.typ == 'mfx' || current_loco.typ == 'dcc') {
			for (let i = 0; i < prog_buttons.length; i++) {
				show(prog_buttons[i]);
			}
			vmin.disabled = false;
			vmax.disabled = false;
		} else if(current_loco.typ == 'mm_prog') {
			for (let i = 0; i < prog_buttons.length; i++) {
				show(prog_buttons[i]);
			}
			hide(prog_av);
			hide(prog_bv);
			vmin.disabled = false;
			vmax.disabled = false;
		}

	}

	/*function loadLocolist(){
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
	}*/

	function updateLocolist(_locolist){
		locolist_container.innerHTML = "";
		locolist = JSON.parse(_locolist);
		for (let i = 0; i < locolist.length; i++) {
			createLocoButton(locolist[i]).onclick = function(){
				setActiveLoco(i);
			}
		}
		setNewLoco();
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
		current_loco.adress = parseInt(adress.value);
		if (current_loco.typ == "dcc") {
			current_loco.uid = parseInt(current_loco.adress) + 0xc000;
		} else if (current_loco.typ == "mfx"){
			current_loco.uid = parseInt(current_loco.adress) + 0x4000;
		} else {
			current_loco.uid = current_loco.adress;
		}
		current_loco.av = parseInt(av.value);
		current_loco.bv = parseInt(bv.value);
		current_loco.vmax = parseInt(vmax.value);
		current_loco.vmin = parseInt(vmin.value);
		current_loco.tacho = parseInt(tacho.value);
	}

	function progLoco(option){
		if (option == "adress") {
			if (current_loco.typ == "dcc") {
				if (current_loco.adress <= 127) {
					console.log("Programming short DCC-adress: " + current_loco.adress);
				} else {
					console.log("Programming long DCC-adress: " + current_loco.adress);
				}
			} else if (current_loco.typ == "mm_prog") {
				console.log("Programming MM-adress: " + current_loco.adress);
			}
		} else if (option == "av") {
			if (current_loco.typ == "mfx") {
				parent.send(`progCV:${current_loco.uid}:${current_loco.mfxadr.xcel}:1:${current_loco.av}`);
			} else if (current_loco.typ = "dcc") {

			}
		} else if (option == "bv") {
			if (current_loco.typ == "mfx") {
				parent.send(`progCV:${current_loco.uid}:${current_loco.mfxadr.xcel}:1:${current_loco.bv}`);
			} else if (current_loco.typ = "dcc") {

			}
		} else if (option == "vmax") {
			if (current_loco.typ == "mfx") {
				parent.send(`progCV:${current_loco.uid}:${current_loco.mfxadr.speedtable}:2:${current_loco.vmax}`);
			} else if (current_loco.typ = "dcc") {

			}
		} else if (option == "vmin") {
			if (current_loco.typ == "mfx") {
				parent.send(`progCV:${current_loco.uid}:${current_loco.mfxadr.speedtable}:1:${current_loco.vmin}`);
			} else if (current_loco.typ = "dcc") {

			}
		}
	}

	function setIcon(icon_name) {
		if (!icon_name) {
			icon_name = "default.png";
		}
		current_loco.icon = icon_name;
		current_icon_img.src = "./loco_icons/" + icon_name;
		current_icon_name.innerHTML = icon_name;
	}

	name.onchange = () => {
		applyCurrentLoco();
	}

	protocol_dropdown.onchange = () => {
		applyCurrentLoco();
	}

	adress.onchange = () => {
		applyCurrentLoco();
	}

	av.onchange = () => {
		applyCurrentLoco();
	}

	bv.onchange = () => {
		applyCurrentLoco();
	}

	vmax.onchange = () => {
		applyCurrentLoco();
	}

	vmin.onchange = () => {
		applyCurrentLoco();
	}

	tacho.onchange = () => {
		applyCurrentLoco();
	}

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
		//loadLocolist();
		parent.ws.send('getLocolist');
		//resizeSettings();
		};

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

	delete_loco.onclick = function(){
		if (current_loco_index >= 0) {
			parent.delLocoAlert(current_loco_index, current_loco.name);
		}
	};

	//loadLocolist();
</script>
