<!DOCTYPE html>
<html style="font-size: 10px; margin: 0;" id='html'>
	<head>
		<title>MäCAN-Webserver</title>
		<link rel="manifest" href="manifest.json?2">
		<link rel="shortcut icon" type="image/x-icon" href="favicon.png">
		<link rel="apple-touch-icon" sizes="180x180" href="apple-touch-icon.png">
		<link rel="stylesheet" type="text/css" href="styles/main.css?<?php echo time()?>">
		<link rel="stylesheet" type="text/css" href="styles/alertbox.css?<?php echo time()?>">
		<meta charset="utf-8">
		<meta name="mobile-web-app-capable" content="yes">
		<meta name="viewport" content="width=device-width, initial-scale=1">

		<!--<script type="text/javascript" src="js/jquery.min.js"></script>-->
		<noscript>Javascript ist erforderlich!</noscript>

	</head>
	<body id="body">
		<div id="wrapper">
			<div id="header">
				<a href="https://github.com/Ixam97/MaeCAN-Project"><img id="logo" src="./media/logo.png?1" alt="MäCAN-Server"></a>
				<div style="display: inline-block;">
					<div  id="left_arrow" class="arrow" style=" border-right: 7px solid white;"></div>
				</div>
				<div id="bookmarks">
					<div id="bookmarks_scroll">
						<div class="bookmark selected">Loksteuerung</div>
						<div class="bookmark">Magnetartikel</div>
						<div class="bookmark">Lokliste bearbeiten</div>
						<div class="bookmark">Einstellungen</div>
					</div>
				</div>
				<div style="display: inline-block;">
					<div  id="right_arrow" class="arrow" style=" border-left: 7px solid white;"></div>
				</div>
				<div id="help_button">?</div>
				<div id="fsicon"><div></div></div>
				<p id="time" style="float: right;"></p>
			</div>
			<div id="content">

				<iframe src="./integrator.php?page=lococontrol_grid" id="lococontrol" class="content_frame" style="display: block;"></iframe>
				<iframe src="./integrator.php?page=keyboard" id="keyboard" class="content_frame" style="display: none;"></iframe>
				<iframe src="./integrator.php?page=locolist" id="locolist" class="content_frame" style="display: none;"></iframe>
				<iframe src="./integrator.php?page=settings" id="settings" class="content_frame" style="display: none;"></iframe>

			</div>
			<div id="stopgo" class="go">GO</div>



		
			
		</div>

		<div id="alertbox" style="display: none;">

				<!-- Alertbox Inhalte -->

				<div class="alertbox" id="connection_lost" style="display: none;">
					<p style="height: 130px;">Verbindung zum Socket-Server wurde unterbrochen! Verbindung erneut herstellen?</p>
					<div id="reconnect" class="button">Verbindungsversuch</div>
				</div>

				<div class="alertbox" id="update_available" style="display: none;">
					<p id="update_available_p"></p>
					<div class="button" id="confirm_update">OK</div>
				<div class="button close_alert">Schließen</div>
				</div>

				<div class="alertbox" id="up_to_date" style="display: none;">
					<p id="up_to_date_p"></p>
				<div class="button close_alert">Schließen</div>
				</div>

				<div class="alertbox" id="restart" style="display: none;">
					<p>Server wirklich neu starten?</p>
					<div id="confirm_restart" class="button">OK</div>
				<div class="button close_alert">Schließen</div>
				</div>

				<div class="alertbox" id="shutdown" style="display: none;">
					<p>Server wirklich herunterfahren?</p>
					<div id="confirm_shutdown" class="button">OK</div>
				<div class="button close_alert">Schließen</div>
				</div>

				<div class="alertbox" id="mfx" style="display: none;">
					<p id="mfx_p"></p>
				<div class="button close_alert">Schließen</div>
				</div>

				<div class="alertbox" id="del_loco" style="display: none;">
					<p id="del_loco_p"></p>
				<div class="button" id="del_loco_confirm">Löschen</div>
				<div class="button close_alert">Abbrechen</div>
				</div>
		
				<!-- Dokumentation -->

				<div id="docs_container" style="display: none;">
					<h1>Hilfe (WIP)</h1>
					<iframe src="./docs/index.html" id="docs_frame"></iframe>
					<div class="button" id="close_docs">Schließen</div>
				</div>

			</div>
		</div>





		<script type="text/javascript" src="./js/main.js"></script>
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
* MäCAN-Server
* https://github.com/Ixam97/MaeCAN-Server/
* ----------------------------------------------------------------------------
*/

			// --- Element-Arrays ---//


			const bookmark = document.getElementsByClassName('bookmark');
			const content_frames = document.getElementsByClassName('content_frame');
			const alertboxes = document.getElementsByClassName('alertbox');
			const close_alert = document.getElementsByClassName('close_alert');


			// --- Elemente ---//

			const body = document.getElementById('body');
			const wrapper = document.getElementById('wrapper');
			const fullscreen = document.getElementById('fsicon');
			const time = document.getElementById('time');
			const content_frame = document.getElementById('content_frame');
			const stopgo = document.getElementById('stopgo');
			
			const bookmarks = document.getElementById('bookmarks');

			const alertbox = document.getElementById('alertbox');
			const connection_lost = document.getElementById('connection_lost');
			const reconnect = document.getElementById('reconnect');
			const confirm_update = document.getElementById('confirm_update');
			const confirm_restart = document.getElementById('confirm_restart');
			const confirm_shutdown = document.getElementById('confirm_shutdown');
			const restart = document.getElementById('restart');
			const shutdown = document.getElementById('shutdown');
			const up_to_date = document.getElementById('up_to_date');
			const up_to_date_p = document.getElementById('up_to_date_p');
			const update_available = document.getElementById('update_available');
			const update_available_p = document.getElementById('update_available_p');
		
			var loco_to_delete;

			let power = false;


			// --- Funktionen --- //

			(function() {
				function scrollHorizontally(e) {
					e = window.event || e;
					var delta = Math.max(-1, Math.min(1, (e.wheelDelta || -e.detail)));
					document.getElementById('bookmarks_scroll').scrollLeft -= (delta*40); // Multiplied by 40
					e.preventDefault();
				}
				if (document.getElementById('bookmarks_scroll').addEventListener) {
					// IE9, Chrome, Safari, Opera
					document.getElementById('bookmarks_scroll').addEventListener("mousewheel", scrollHorizontally, false);
					// Firefox
					document.getElementById('bookmarks_scroll').addEventListener("DOMMouseScroll", scrollHorizontally, false);
				} else {
					// IE 6/7/8
					document.getElementById('bookmarks_scroll').attachEvent("onmousewheel", scrollHorizontally);
				}
			})();


			function clearAlertBox(){
				for (let i = 0; i < sub_alertboxes.length; i++) {
					hide(sub_alertboxes[i]);
				}
			}

			function showAlertbox(element){
				show(element);
				show(alertbox);
				wrapper.style.filter = 'blur(2px)';
			}

			function hideAlertbox(){
				hide(alertbox);
				wrapper.style.filter = 'none';
				for (let i = 0; i < alertboxes.length; i++) {
					hide(alertboxes[i]);
				}
			}

			function showHideArrows(){
				if (bookmarks.clientWidth < 588) {
					show(right_arrow);
					show(left_arrow);
				} else {
					hide(right_arrow);
					hide(left_arrow);
				}
			}

			function resize(){
				showHideArrows();
				locolist.contentWindow.resizeSettings();
				settings.contentWindow.resizeSettings();

			}

			function disconnectAlert(){
				showAlertbox(connection_lost);
				hide(close_alert);
			}

			function mfxAlert(name){
				showAlertbox(mfx);
				mfx_p.innerHTML = 'mfx/M4-Lok "' + name + '" wurde der Lokliste hinzugefügt!';
			}

			function delLocoAlert(index, name){
				showAlertbox(del_loco);
				loco_to_delete = index;
				del_loco_p.innerHTML = 'Lok "' + name + '" wirklich löschen?';
			}

			function showHelp(page){
				show(alertbox);
				show(docs_container);
				docs_frame.src = ('docs/pages/' + page);
			}

			for (let i = 0; i < bookmark.length; i++) (function(i){
				bookmark[i].onclick = function(){
					for (let j = 0; j < bookmark.length; j++) {
						bookmark[j].setAttribute('class', 'bookmark');
						if (j != i ) {
							hide(content_frames[j]);
						}
					}
					bookmark[i].setAttribute('class', 'bookmark selected');
					show(content_frames[i]);
					resize();
					//locolist.loadLocolist();
					//locolist.loadIcons();
				};
			})(i);


			//--- Vollbild ---//

			fullscreen.onclick = function(){
				toggleFullscreen();
			};


			//--- STOP- GO-Taste ---//

			stopgo.onclick = function(){
				if (power) {
					send('stop');
				} else {
					send('go');
				}
			};


			//--- Alert-Fenster onClicks ---//


			for (let i = 0; i < close_alert.length; i++){
				close_alert[i].onclick = function(){
					hideAlertbox();
				};
			}
			
			reconnect.onclick = function(){
				location.reload();
			};

			confirm_update.onclick = function(){
				console.log("updating Server...");
				hideAlertbox();
			};

			confirm_restart.onclick = function(){
				console.log("restarting server...");
				hideAlertbox();
				send('restart');
			};

			confirm_shutdown.onclick = function(){
				console.log("shuttng down server ...");
				hideAlertbox();
				send('shutdown');
			};

			del_loco_confirm.onclick = () => {
				send(`deleteLoco:${loco_to_delete}`);
				hideAlertbox();
			}

			help_button.onclick = () => {
				show(alertbox);
				show(docs_container);
			}

			close_docs.onclick = () => {
				hide(alertbox);
				hide(docs_container);
				docs_frame.src = ('docs/index.html');
			}


			//--- Viewport-Änderung ---//

			body.onresize = function(){
				resize();
			};


			//--- Uhrzeit ---//

			setInterval(function(){
				let d = new Date()
				let hours = d.getHours();
				if (hours < 10) { hours = '0' + hours}
				let minutes = d.getMinutes();
				if (minutes < 10) { minutes = '0' + minutes}
				let seconds = d.getSeconds();
				if (seconds < 10) { seconds = '0' + seconds}
				time.innerHTML = hours + ':' + minutes + ':' + seconds;
			}, 1000);


			//--- Websocket Events ---//

			ws.onmessage = function(dgram){
				let msg = dgram.data.toString().split(':');
				let cmd = msg[0];
				if (cmd == 'go') {
					stopgo.setAttribute('class', 'stop');
					stopgo.textContent = 'STOP';
					document.getElementById('html').setAttribute('class', 'stop');
					power = true;
				} else if (cmd == 'stop') {
					stopgo.setAttribute('class', 'go');
					stopgo.textContent = 'GO';
					document.getElementById('html').setAttribute('class', 'go');
					power = false;
				} else if (cmd == 'foundMfx') {
					mfxAlert(msg[1]);
				} else if (msg[0] == 'updateLocolist') {
					locolist.contentWindow.loadLocolist();
				} else if (cmd == 'updateReading') {
					settings.contentWindow.updateFiller(msg[1], msg[2], msg[3]);
				} else if (cmd == 'updateProtocol') {
					settings.contentWindow.updateProtocol(parseInt(msg[1]));
				} else if (cmd == 'can') {
					settings.contentWindow.updateCanMonitor(msg);
				} else if (cmd == 'updateVersion') {
					settings.contentWindow.version.innerHTML = "Version: " + msg[1];
				} else {
					/*let data = [];
					for (let i = 0; i < dgram.data.length; i++) {
						data[i] = dgram.data[i].charCodeAt(0).toString(16);
					}*/
					console.log(dgram.data.toString());
				}
			}

			ws.onclose = function(){
				disconnectAlert();
			};

			ws.onerror = function(error){
				disconnectAlert();
			};

			//--- Misc ---/

			resize();


		</script>

	</body>
</html>
