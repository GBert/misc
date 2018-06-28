<link rel="stylesheet" href="styles/lococontrol_grid.css?<?php echo time() ?>">
<link rel="stylesheet" type="text/css" href="styles/nexus7.slider.css?<?php echo time() ?>">
<div id="lococontrol">
    <div id="throttle_left" class="throttle">
		<div class="slider_container">
			<div class=" button speed" id="speed_left">0 km/h</div>
			<div class="slider">
				<input type="range" id="slider_left" data-orientation="vertical" min="0" max="1000" value="0" step="1">
					
			</div>
			<div class="button change_direction" id="change_direction_left">
				<div id="rev_left" style="display: none;">
					<div style="width: 0; height: 0; border-bottom: 2rem solid transparent; border-top: 2rem solid transparent; border-right: 2rem solid #2FA938; float: left;"></div>
					<div style="width: 0; height: 0; border-bottom: 2rem solid transparent; border-top: 2rem solid transparent; border-left: 2rem solid #adadad; float: right;"></div>
				</div>
				<div id="fwd_left" style="display: inline-block;">
					<div style="width: 0; height: 0; border-bottom: 2rem solid transparent; border-top: 2rem solid transparent; border-right: 2rem solid #adadad; float: left;"></div>
					<div style="width: 0; height: 0; border-bottom: 2rem solid transparent; border-top: 2rem solid transparent; border-left: 2rem solid #2fa938; float: right;"></div>
				</div>
			</div>
		</div>
        <div class="functions">
            <div class="button lokliste_toggle" id="mfx_find"></div>
			<div class="button lokliste" id="lokliste_left"><img id="icon_left" src="" class="preview_icon">Lokauswahl
				<div id="lokliste_container_left" class="dropdown">
				</div>
            </div>
            <div class="address" id="address_left" style="display: none;"></div>
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

    <div id="throttle_right" class="throttle">
		<div class="slider_container">
			<div class=" button speed" id="speed_right">0 km/h</div>
			<div class="slider">
				<input type="range" id="slider_right" data-orientation="vertical" min="0" max="1000" value="0" step="1">
				
			</div>
			<div class="button change_direction" id="change_direction_right">
				<div id="rev_right" style="display: none;">
					<div style="width: 0; height: 0; border-bottom: 2rem solid transparent; border-top: 2rem solid transparent; border-right: 2rem solid #2FA938; float: left;"></div>
					<div style="width: 0; height: 0; border-bottom: 2rem solid transparent; border-top: 2rem solid transparent; border-left: 2rem solid #adadad; float: right;"></div>
				</div>
				<div id="fwd_right" style="display: inline-block;">
					<div style="width: 0; height: 0; border-bottom: 2rem solid transparent; border-top: 2rem solid transparent; border-right: 2rem solid #adadad; float: left;"></div>
					<div style="width: 0; height: 0; border-bottom: 2rem solid transparent; border-top: 2rem solid transparent; border-left: 2rem solid #2fa938; float: right;"></div>
				</div>
			</div>
    	</div>
        <div class="functions">
            <div class="button lokliste_toggle" id="lokliste_toggle_right"></div>
			<div class="button lokliste" id="lokliste_right"><img id="icon_right" src="" class="preview_icon">Lokauswahl
			   	<div id="lokliste_container_right" class="dropdown">
			</div>
                <!--LOKLISTE-->
            </div>
            <div class="address" id="address_right" style="display:none;"></div>
            <div class="f_right">
                <div class="button function_right">F0</div>
                <div class="button function_right">F1</div>
                <div class="button function_right">F2</div>
                <div class="button function_right">F3</div>
                <div class="button function_right">F4</div>
                <div class="button function_right">F5</div>
                <div class="button function_right">F6</div>
                <div class="button function_right">F7</div>
            </div>
            <div class="f_right">
                <div class="button function_right">F8</div>
                <div class="button function_right">F9</div>
                <div class="button function_right">F10</div>
                <div class="button function_right">F11</div>
                <div class="button function_right">F12</div>
                <div class="button function_right">F13</div>
                <div class="button function_right">F14</div>
                <div class="button function_right">F15</div>
            </div>
		</div>
	</div>
</div>
<script type="text/javascript" src="js/jquery.min.js"></script>
<script type="text/javascript" src="js/rangeslider.min.js"></script>
<script type="text/javascript" src="js/main.js"></script>
<script type="text/javascript" src="js/websocket.js"></script>

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

	let locolist = [];
	let loco = [];

	for (var i = 0; i < sides.length; i++) {
		var side = sides[i];
		
		var mfx_find = document.getElementById('mfx_find');

		//lokliste_toggle[side] = document.getElementById(`lokliste_toggle_${side}`);
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

		

		//loco[side].uid = 0;
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
				if (loco[side].uid >= 0x4000 || i <= 4) {
					setFn(loco[side].uid, i, !fn_state[side][i]);
				}
				if (loco[side].uid < 0x4000 && 4 < i && i<= 8) {
					setFn(loco[side].uid + 1, i - 4, !fn_state[side][i]);
				}
				if (loco[side].uid < 0x4000 && 8 < i && i <= 12) {
					setFn(loco[side].uid + 2, i - 8, !fn_state[side][i]);
				}
				if (loco[side].uid < 0x4000 && 12 < i && i <= 16) {
					setFn(loco[side].uid + 3, i - 12, !fn_state[side][i]);
				}
				return false;
			};
		})(i, side);
	}

	function createDropdownPoint(name, icon, side) {
		let obj_text = document.createTextNode(name);
		let obj_icon = document.createElement('img');
		if (!icon) icon = "default.png";
		obj_icon.src = './loco_icons/' + icon;
		obj_icon.className = 'preview_icon';
		let obj = document.createElement('div');
		obj.clasName = 'locolist_dropdown_option';
		obj.appendChild(obj_icon);
		obj.appendChild(obj_text);
		lokliste_container[side].appendChild(obj);
		return obj;
	}

	function setLoco(side, _loco) {
		loco[side] = _loco;
		loadFn(side);
		getDir(loco[side].uid);
		getSpeed(loco[side].uid);
		if (!loco[side].icon) {
			icon_img[side].src = './loco_icons/default.png';
		} else {
			icon_img[side].src = './loco_icons/' + loco[side].icon;
		}
		lokliste_button[side].childNodes[1].textContent = loco[side].name;
	}

	for (var side in change_direction) (function(side){

		change_direction[side].onclick = function(){
			send(`toggleDir:${loco[side].uid}`);
			return false;
		}

		lokliste_button[side].onclick = function(){
			let locolist_request = new XMLHttpRequest();
			let time = new Date()

			locolist_request.open('GET', './config/locolist.json?' + time.getTime(), true);

			locolist_request.onload = function(){
				if (this.status == 200) {
					locolist = JSON.parse(this.responseText);
					lokliste_container[side].innerHTML = "";
					for (let i = 0; i < locolist.length; i++) {
						let _loco = locolist[i];
						createDropdownPoint(locolist[i].name, locolist[i].icon, side).onclick = () => {
							setLoco(side, _loco);
						};
					}
					if (lokliste_visible[side]) {
						lokliste_visible[side] = false;
						hide(lokliste_container[side]);
						lokliste_button[side].className = 'button lokliste';
					} else {
						lokliste_visible[side] = true;
						show(lokliste_container[side]);
						lokliste_button[side].className = 'button button_dropdown lokliste';
					}
				}
			};

			locolist_request.send();
		}
	})(side);
	

	//--------------------------LOKLISTEN------------------------//



	//---------------------------INITIAL-------------------------//

	ws.onopen = function(){
		//getLokliste(true);
		/*
		let locolist_request = new XMLHttpRequest();
		locolist_request.open('GET', './config/locolist.json', true);
		locolist_request.onload = function(){
			if (this.status == 200) {
				locolist = JSON.parse(this.responseText);
				setTimeout(() => setLoco('left', locolist[0]), 200);
				setTimeout(() => setLoco('right', locolist[1]), 200);
			}
		};
		locolist_request.send();
		*/
	};

	//---------------------------FUNKTIONEN-----------------------------------//

	function loadFn(side){
		for (var i = 0; i < function_button[side].length; i++) {
			if (loco[side].uid >= 0x4000 || i <= 4) {
				getFn(loco[side].uid, i);
			}
			if (loco[side].uid < 0x4000 && 4 < i && i <= 8) {
				getFn(loco[side].uid + 1, i - 4);
			}
			if (loco[side].uid < 0x4000 && 8 < i && i <= 12) {
				getFn(loco[side].uid + 2, i - 8);
			}
			if (loco[side].uid < 0x4000 && 12 < i && i <= 16) {
				getFn(loco[side].uid + 3, i - 12);
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
		let tachomax = 100;
		let unit = "%";
		if(loco[side].tachomax) {
			tachomax = loco[side].tachomax;
			unit = "km/h";
		}
		speed[side].textContent = Math.ceil((value / 10) * (tachomax / 100)) + unit;
      	slider[side].val(value).change();
      	old_speed[side] = value;
	}

	function getSpeed(uid){
		send(`getSpeed:${uid}`);
	}

	function setSpeed(side, value){
		if (value != old_speed[side]) {
			send(`setSpeed:${loco[side].uid}:${value}`);
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
			getDir(loco[side].uid);
		}
		getSpeed(loco[side].uid);
	}

	function getDir(uid){
		send(`getDir:${uid}`);
	}

	function setDir(uid, dir){
		send(`setDir:${uid}:${dir}`);
	}

	/*mfx_find.onclick = function() {
		send('mfxDiscovery');
		console.log('mfxDiscovery');
	}*/

	//------------------------------LISTENER---------------------------------//

	ws.onmessage = function(dgram){
		console.log(`Recieved: ${dgram.data.toString()}.`);
		var msg = dgram.data.toString().split(':');
		var cmd = msg[0];
		var value = parseInt(msg[2])

		for (var i = 0; i < sides.length; i++) (function(side){

			if (loco[side]) {	
				if (cmd == 'updateFn') {
					if ((msg[1] == loco[side].uid && loco[side].uid >= 0x4000) || (msg[1] == loco[side].uid && (value >> 8) <= 4 )) {
						updateFn(value, side);
					}
					if (loco[side].uid < 0x4000 && msg[1] == loco[side].uid + 1) {
						updateFn(value + 0x400, side);
					}
					if (loco[side].uid < 0x4000 && msg[1] == loco[side].uid + 2) {
						updateFn(value + 0x800, side);
					}
					if (loco[side].uid < 0x4000 && msg[1] == loco[side].uid + 3) {
						updateFn(value + 0xc00, side);
					}
				}

				if (cmd == 'updateSpeed' && loco[side].uid == msg[1]) {
					updateSpeed(side, value);
				}

				if (cmd == 'updateDir' && loco[side].uid == msg[1]) {
					updateDir(side, msg[2]);
				}
			}
		})(sides[i]);
	};

</script>