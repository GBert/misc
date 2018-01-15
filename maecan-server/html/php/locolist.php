

<div class="subcontainer" id="settings_container">
	<div id="sidebar">
		<div class="button" id="sidebar_show_button" style="display: none;">
			<div></div>
			<div></div>
			<div></div>
		</div>
		<div id="sidebar_button_container">
			<!--<div class="button button_active" id="add_loco">Neue Lok anlegen</div>-->
		</div>
	</div>
	<div class="frame_content frame_content_active" id="frame">
	<!--
		<h1>Neue Lok anlegen</h1>

		<p>Name: <input type="text" class="text_input" id="name" placeholder="Lokname..."></p>

		<p>Protokol:</p>
		<label>DCC
			<input class="radio" type="radio" id="dcc" name="protocol" checked="checked">
			<span class="radio_button"></span>
		</label>
		<label>Motorola
			<input class="radio" type="radio" name="protocol">
			<span class="radio_button"></span>
		</label>

		<p>Addresse: <input type="text" class="text_input" id="adress" placeholder="Lokadresse..."></p>

		<div class="button" id="save_loco">Lok Speichern</div>
	</div>-->
</div>

<script type="text/javascript" src="js/main.js"></script>
<script type="text/javascript" src="js/websocket.js"></script>
<script type="text/javascript">

	const containers = document.getElementsByClassName('frame_content');

	const settings_container = document.getElementById('settings_container');
	const sidebar = document.getElementById('sidebar');
	const sidebar_button_container = document.getElementById('sidebar_button_container');
	const sidebar_show_button = document.getElementById('sidebar_show_button');

	let loco = {};
	let locolist = [];

	//--- Allgemein ---//


	// Lokliste Laden

	function createLocoButton(loco){
		let loco_button = document.createElement('div');
		let loco_name = document.createTextNode(loco.name);
		loco_button.className = 'button loco_button';
		sidebar_button_container.appendChild(loco_button);
		loco_button.appendChild(loco_name);
		if (loco.icon) {
			let loco_icon = document.createElement('img');
			loco_icon.src = './loco_icons/' + loco.icon;
			loco_button.appendChild(document.createElement('br'));
			loco_button.appendChild(loco_icon);
		}

		return loco_button;
	}

	function setActiveLoco(index) {
		console.log(index);
	}


	function loadLocolist(){
		let locolist_request = new XMLHttpRequest();

		locolist_request.open('GET', './config/locolist.json', true);
		locolist_request.onload = function(){
			if (this.status == 200) {
				locolist = JSON.parse(this.responseText);
				for (let i = 0; i < locolist.length; i++) {
					createLocoButton(locolist[i]).onclick = function(){
						setActiveLoco(i);
					}
				}
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

	sidebar_show_button.onclick = function(){
		if (sidebar_button_container.style.display == 'block') {
			hide(sidebar_button_container);
		} else {
			show(sidebar_button_container);
		}
	};

	for (let i = 0; i < sidebar_button_container.children.length; i++) {
		sidebar_button_container.children[i].onclick = function(){
			if (parent.body.clientWidth < 540) {
				hide(sidebar_button_container);
			}
		};
	}

	// --- Eingabe --- //

	save_loco.onclick = function(){
		let loco_name = document.getElementById('name').value;
		let loco_protocol;
		if(dcc.checked) {
			loco_protocol = 'dcc';
		} else {
			loco_protocol = 'moto';
		}
		let loco_adress = adress.value;

		loco.name = loco_name;
		loco.protocol = loco_protocol;
		loco.adress = loco_adress;

		console.log(loco);

	};

	loadLocolist();

	// --- Bearbeiten einer vorhandenen Lok --- //
</script>
