<!--
Loksteuerung
By Maximilian Goldschmidt (cc)
Do what ever you want with this, but keep this header!

Last edited: 05.07.2017
-->
<link rel="stylesheet" type="text/css" href="styles/nexus7.slider.css?<?php echo time() ?>">
<div class="throttle" id="throttle_left">
	<div class="slider" style="margin-left: 10px;">
		<div class="button speed" id="speed_left">0 km/h</div>
		<input type="range" id="slider_left" data-orientation="vertical" min="0" max="1000" value="0" step="1">
		<a href=""><div class="button change_direction" id="change_direction_left">
			<div id="rev_left" style="display: none;">
				<div style="width: 0; height: 0; border-bottom: 2rem solid transparent; border-top: 2rem solid transparent; border-right: 2rem solid #2FA938; float: left;"></div>
				<div style="width: 0; height: 0; border-bottom: 2rem solid transparent; border-top: 2rem solid transparent; border-left: 2rem solid #adadad; float: right;"></div>
			</div>
			<div id="fwd_left" style="display: inline-block;">
				<div style="width: 0; height: 0; border-bottom: 2rem solid transparent; border-top: 2rem solid transparent; border-right: 2rem solid #adadad; float: left;"></div>
				<div style="width: 0; height: 0; border-bottom: 2rem solid transparent; border-top: 2rem solid transparent; border-left: 2rem solid #2fa938; float: right;"></div>
			</div>
		</div></a>
	</div>
	<div class="functions">
		<div class="f_top">
			<a href=""><div class="button" id="lokliste_toggle_left">Adresse</div></a>
			<div class="button lokliste" id="lokliste_left"><img id="icon_left" src="icons/.png">Lokauswahl</div>
			<div id="lokliste_container_left" class="dropdown">
				<!--LOKLISTE-->
			</div>
			<div class="address" id="address_left"></div>
		</div>
		<div class="f_left">
			<div class="button function_left">F0</div>
			<div class="button function_left">F1</div>
			<div class="button function_left">F2</div>
			<div class="button function_left">F3</div>
			<div class="button function_left">F4</div>
			<div class="button function_left">F5</div>
			<div class="button function_left">F6</div>
			<div class="button function_left">F7</div>
		</div>
		<div class="f_left">
			<div class="button function_left">F8</div>
			<div class="button function_left">F9</div>
			<div class="button function_left">F10</div>
			<div class="button function_left">F11</div>
			<div class="button function_left">F12</div>
			<div class="button function_left">F13</div>
			<div class="button function_left">F14</div>
			<div class="button function_left">F15</div>
		</div>
	</div>
</div>
<div class="throttle" id="throttle_right">
	<div class="functions">
		<div class="f_top">
			<a href=""><div class="button" id="lokliste_toggle_right">Adresse</div></a>
			<div class="button lokliste" id="lokliste_right"><img id="icon_right" src="icons/.png">Lokauswahl</div>
			<div id="lokliste_container_right" class="dropdown">
				<!--LOKLISTE-->
			</div>
			<div class="address" id="address_right"></div>
		</div>
		<div class="f_right">
			<a href=""><div class="button function_right">F0</div></a>
			<a href=""><div class="button function_right">F1</div></a>
			<a href=""><div class="button function_right">F2</div></a>
			<a href=""><div class="button function_right">F3</div></a>
			<a href=""><div class="button function_right">F4</div></a>
			<a href=""><div class="button function_right">F5</div></a>
			<a href=""><div class="button function_right">F6</div></a>
			<a href=""><div class="button function_right">F7</div></a>
		</div>
		<div class="f_right">
			<a href=""><div class="button function_right">F8</div></a>
			<a href=""><div class="button function_right">F9</div></a>
			<a href=""><div class="button function_right">F10</div></a>
			<a href=""><div class="button function_right">F11</div></a>
			<a href=""><div class="button function_right">F12</div></a>
			<a href=""><div class="button function_right">F13</div></a>
			<a href=""><div class="button function_right">F14</div></a>
			<a href=""><div class="button function_right">F15</div></a>
		</div>
	</div>
	<div class="slider" style="margin-right: 10px;">
		<div class=" button speed" id="speed_right">0 km/h</div>
		<input type="range" id="slider_right" data-orientation="vertical" min="0" max="1000" value="0" step="1">
		<a href=""><div class="button change_direction" id="change_direction_right">
			<div id="rev_right" style="display: none;">
				<div style="width: 0; height: 0; border-bottom: 2rem solid transparent; border-top: 2rem solid transparent; border-right: 2rem solid #2FA938; float: left;"></div>
				<div style="width: 0; height: 0; border-bottom: 2rem solid transparent; border-top: 2rem solid transparent; border-left: 2rem solid #adadad; float: right;"></div>
			</div>
			<div id="fwd_right" style="display: inline-block;">
				<div style="width: 0; height: 0; border-bottom: 2rem solid transparent; border-top: 2rem solid transparent; border-right: 2rem solid #adadad; float: left;"></div>
				<div style="width: 0; height: 0; border-bottom: 2rem solid transparent; border-top: 2rem solid transparent; border-left: 2rem solid #2fa938; float: right;"></div>
			</div>
		</div></a>
	</div>	
</div>

<script type="text/javascript" src="js/jquery.min.js"></script>
<script type="text/javascript" src="js/rangeslider.min.js"></script>
<script type="text/javascript" src="js/websocket.js"></script>

<script type="text/javascript">
	//------------------------------LINKS-------------------------------------//

	var sides = ['left', 'right'];

	var lokliste_toggle = [];
	var lokliste_button = [];
	var lokliste_container = [];
	var lokliste_visible = [];

	var change_direction = [];

	var icon_img = [];
	var function_button = [];
	var speed = [];
	var slider = [];
	var slider_v = [];

	var sliding = [];
	var speedupdate = [];
	var old_speed = [];
	var direction = [];
	var fwd = [];
	var rev = [];

	var interval = [];
	var timer = [];

	//---LOKLISTE:
	var names = [];
	var uids = [];
	var icons = [];
	var vmaxes = [];

	var uid = [];
	var name_side = [];
	var tachomax = [];
	var icon = [];
	var fn_state = [];



	for (var i = 0; i < sides.length; i++) {
		var side = sides[i];

		lokliste_toggle[side] = document.getElementById(`lokliste_toggle_${side}`);
		lokliste_button[side] = document.getElementById(`lokliste_${side}`);
		lokliste_container[side] = document.getElementById(`lokliste_container_${side}`);
		lokliste_visible[side] = false;

		change_direction[side] = document.getElementById(`change_direction_${side}`);

		icon_img[side] = document.getElementById(`icon_${side}`);
		function_button[side] = document.getElementsByClassName(`function_${side}`);
		speed[side] = document.getElementById(`speed_${side}`);
		slider[side] = $(`#slider_${side}`);
		slider_v[side] = document.getElementById(`slider_${side}`);

		sliding[side] = false;
		speedupdate[side] = false;
		old_speed[side] = 0;
		direction[side] = 1;
		fwd[side] = document.getElementById(`fwd_${side}`);
		rev[side] = document.getElementById(`rev_${side}`);

		fn_state[side] = [];

		slider[side].rangeslider({polyfill: false});

		

		//uid[side] = 0;
	}

	slider['left'].on('input', function(){
		if (speedupdate['left']) {
			speedupdate['left'] = !speedupdate['left'];
		} else {
				if (!sliding['left']) {
					interval['left'] = setInterval(function(){
					setSpeed('left', slider_v['left'].value);
				}, 20),
				sliding['left'] = true;
			}
			clearTimeout(timer['left']);
			timer['left'] = setTimeout(function(){
			clearInterval(interval['left']);
				sliding['left'] = false;
			}, 500);
		}
	});

	slider['right'].on('input', function(){
		if (speedupdate['right']) {
			speedupdate['right'] = !speedupdate['right'];
		} else {
				if (!sliding['right']) {
					interval['right'] = setInterval(function(){
					setSpeed('right', slider_v['right'].value);
				}, 20),
				sliding['right'] = true;
			}
			clearTimeout(timer['right']);
			timer['right'] = setTimeout(function(){
			clearInterval(interval['right']);
				sliding['right'] = false;
			}, 500);
		}
	});

	//------------------FUNKTIONSTASTEN---------------------------//


	for (var j = 0; j < sides.length; j++) {
		var side = sides[j];
		for (var i = 0; i < function_button[side].length; i++) (function(i, side){
			function_button[side][i].onclick = function(){
				if (uid[side] >= 0x4000 || i <= 4) {
					setFn(uid[side], i, !fn_state[side][i]);
				}
				if (uid[side] < 0x4000 && 4 < i && i<= 8) {
					setFn(uid[side] + 1, i - 4, !fn_state[side][i]);
				}
				if (uid[side] < 0x4000 && 8 < i && i <= 12) {
					setFn(uid[side] + 2, i - 8, !fn_state[side][i]);
				}
				if (uid[side] < 0x4000 && 12 < i && i <= 16) {
					setFn(uid[side] + 3, i - 12, !fn_state[side][i]);
				}
				return false;
			};
		})(i, side);
	}

	for (var side in lokliste_toggle) (function(side){
		lokliste_toggle[side].onclick = function(){
			return false;
		};

		change_direction[side].onclick = function(){
			send(`toggleDir:${uid[side]}`);
			return false;
		}

		lokliste_button[side].onclick = function(){
			if (lokliste_visible[side]) {
				lokliste_container[side].setAttribute('class', 'dropdown');
				lokliste_button[side].setAttribute('class', 'button lokliste')
				lokliste_visible[side] = false;
			} else {
				for (var i = 0; i < uids.length; i++) {
					if (!document.getElementById(`lokliste_option_${side}_${i}`)) {
						createLoklistePoint(i, side);
					}
				}
				lokliste_container[side].setAttribute('class', 'dropdown dropdown_visible');
				lokliste_button[side].setAttribute('class', 'button button_dropdown lokliste')
				lokliste_visible[side] = true;
			}
		}
	})(side);
	

	//--------------------------LOKLISTEN------------------------//



	//---------------------------INITIAL-------------------------//

	ws.onopen = function(){
		getLokliste(true);
	};

	//---------------------------FUNKTIONEN-----------------------------------//

	function loadFn(side){
		for (var i = 0; i < function_button[side].length; i++) {
			if (uid[side] >= 0x4000 || i <= 4) {
				getFn(uid[side], i);
			}
			if (uid[side] < 0x4000 && 4 < i && i <= 8) {
				getFn(uid[side] + 1, i - 4);
			}
			if (uid[side] < 0x4000 && 8 < i && i <= 12) {
				getFn(uid[side] + 2, i - 8);
			}
			if (uid[side] < 0x4000 && 12 < i && i <= 16) {
				getFn(uid[side] + 3, i - 12);
			}
		}
	}

	function updateFn(value, side){
		var fn_num = value >> 8;
		var fn_value = value & 0x00ff;
		if(fn_value){
			function_button[side][fn_num].setAttribute('class', `button function_${side} button_active`);
		}else{
			function_button[side][fn_num].setAttribute('class', `button function_${side}`);
		}
		fn_state[side][fn_num] = fn_value;
		
	}

	function getFn(uid, value){
		send(`getFn:${uid}:${value}`);
	}

	function setFn(uid, fn_num, fn_value){
		var value = (fn_num << 8) + fn_value;
		send(`setFn:${uid}:${value}`);
	}

	function updateSpeed(side, value){
		speedupdate[side] = true;
		speed[side].textContent = `${Math.ceil((value / 10) * (tachomax[side] / 100))} km/h`;
      	slider[side].val(value).change();
      	old_speed[side] = value;
	}

	function getSpeed(uid){
		send(`getSpeed:${uid}`);
	}

	function setSpeed(side, value){
		if (value != old_speed[side]) {
			send(`setSpeed:${uid[side]}:${value}`);
		}
		old_speed[side] = value;
	}

	function updateDir(side, dir){
		if (dir == 1) {
			rev[side].style.display = 'none';
			fwd[side].style.display = 'inline-block';
			direction[side] = dir;
		} else if (dir == 2) {
			rev[side].style.display = 'inline-block';
			fwd[side].style.display = 'none';
			direction[side] = dir;
		} else {
			getDir(uid[side]);
		}
		getSpeed(uid[side]);
	}

	function getDir(uid){
		send(`getDir:${uid}`);
	}

	function setDir(uid, dir){
		send(`setDir:${uid}:${dir}`);
	}

	function setLok(side, id){
		uid[side] = parseInt(uids[id], 16);
		name_side[side] = names[id].substring(0, 12);
		icon[side] = icons[id];
		if(!vmaxes[id]){
			tachomax[side] = 100;
		} else {
			tachomax[side] = vmaxes[id];
		}
		icon_img[side].setAttribute('src', `icons/${icon[side]}.png`);
		lokliste_button[side].childNodes[1].textContent = name_side[side];
		getSpeed(uid[side]);
		getDir(uid[side]);
		loadFn(side);
	}

	function getLokliste(initial){
		$.get("php/lokliste.php",{}, function(data){
	    	var str_lokliste = data;
	    	var lokliste = str_lokliste.split("§");
	    	for (var i = lokliste.length - 1; i >= 0; i--) {
	      		names[i] = lokliste[i].split("$")[0];
	      		uids[i] = lokliste[i].split("$")[1];
	      		vmaxes[i] = lokliste[i].split("$")[2];
	      		icons[i] = lokliste[i].split('$')[3];
	    	}
	    	if (initial){
	    		setLok('left', 0);
	    		setLok('right', 1);
		    }
  		});
	}

	function createLoklistePoint(id, side){
		var obj = document.createElement('a');
		var obj_img = document.createElement('img');
		var obj_text = document.createTextNode(names[id].substring(0, 12));
		var obj_div = document.createElement('div');
		obj.setAttribute('href', '');
		obj_img.setAttribute('src', `icons/${icons[id]}.png`);
		obj_div.setAttribute('id', `lokliste_option_${side}_${id}`);
		obj_div.setAttribute('class', 'lokliste_option');
		if (id == 0) {
			obj_div.style.borderTop = 0;
		}
		obj_div.appendChild(obj_img);
		obj_div.appendChild(obj_text);
		obj.appendChild(obj_div);
		lokliste_container[side].appendChild(obj);
		obj_div.onclick = function(){
			setLok(side, id);
			lokliste_container[side].setAttribute('class', 'dropdown');
			lokliste_button[side].setAttribute('class', 'button lokliste')
			lokliste_visible[side] = false;
			return false;
		};
	}

	//------------------------------LISTENER---------------------------------//

	ws.onmessage = function(dgram){
		console.log(`Recieved: ${dgram.data.toString()}.`);
		var msg = dgram.data.toString().split(':');
		var cmd = msg[0];
		var value = parseInt(msg[2])

		for (var i = 0; i < sides.length; i++) (function(side){

			if (cmd == 'updateFn') {
				if ((msg[1] == uid[side] && uid[side] >= 0x4000) || (msg[1] == uid[side] && (value >> 8) <= 4 )) {
					updateFn(value, side);
				}
				if (uid[side] < 0x4000 && msg[1] == uid[side] + 1) {
					updateFn(value + 0x400, side);
				}
				if (uid[side] < 0x4000 && msg[1] == uid[side] + 2) {
					updateFn(value + 0x800, side);
				}
				if (uid[side] < 0x4000 && msg[1] == uid[side] + 3) {
					updateFn(value + 0xc00, side);
				}
			}

			if (cmd == 'updateSpeed' && uid[side] == msg[1]) {
				updateSpeed(side, value);
			}

			if (cmd == 'updateDir' && uid[side] == msg[1]) {
				updateDir(side, msg[2]);
			}
		})(sides[i]);
	};

</script>