<!DOCTYPE html>
<html style="font-size: 10px; margin: 0;" id='html'>
	<head>
		<title>MäCAN-Webserver</title>
		<link rel="manifest" href="/manifest.json?2">
		<link rel="shortcut icon" type="image/x-icon" href="/favicon.png">
		<link rel="apple-touch-icon" sizes="180x180" href="/apple-touch-icon.png">
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
				<div id="fsicon"><div></div></div>
				<p id="time" style="float: right;"></p>
			</div>
			<div id="content">

				<iframe src="./integrator.php?page=lococontrol_grid" id="lococontrol" class="content_frame" style="display: block;"></iframe>
				<iframe src="./integrator.php?page=memory" id="memory" class="content_frame" style="display: none;"></iframe>
				<iframe src="./integrator.php?page=locolist" id="locolist" class="content_frame" style="display: none;"></iframe>
				<iframe src="./integrator.php?page=settings" id="settings" class="content_frame" style="display: none;"></iframe>

			</div>
			<div id="stopgo" class="go">GO</div>



		<!-- Alertbox Inhalte -->
			
		</div>

		<div id="alertbox" style="display: none;">
			<div class="alertbox" >
				<div class="sub_alertbox" id="connection_lost" style="display: none;">
					<p style="height: 130px;">Verbindung zum Socket-Server wurde unterbrochen! Verbindung erneut herstellen?</p>
					<div id="reconnect" class="button">Verbindungsversuch</div>
				</div>
				<div class="sub_alertbox" id="update_available" style="display: none;">
					<p id="update_available_p"></p>
					<div class="button" id="confirm_update">OK</div>
				</div>
				<div class="sub_alertbox" id="up_to_date" style="display: none;">
					<p id="up_to_date_p"></p>
				</div>
				<div class="sub_alertbox" id="restart" style="display: none;">
					<p>Server wirklich neu starten?</p>
					<div id="confirm_restart" class="button">OK</div>
				</div>
				<div class="sub_alertbox" id="shutdown" style="display: none;">
					<p>Server wirklich herunterfahren?</p>
					<div id="confirm_shutdown" class="button">OK</div>
				</div>
				<div class="button" id="close_alert">Abbrechen</div>
			</div>
		</div>




		<script type="text/javascript" src="./js/main.js"></script>
		<script type="text/javascript" src="js/websocket.js"></script>
		<script type="text/javascript">


			// --- Element-Arrays ---//


			const bookmark = document.getElementsByClassName('bookmark');
			const content_frames = document.getElementsByClassName('content_frame');
			const sub_alertboxes = document.getElementsByClassName('sub_alertbox');


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
			const close_alert = document.getElementById('close_alert');
			const restart = document.getElementById('restart');
			const shutdown = document.getElementById('shutdown');
			const up_to_date = document.getElementById('up_to_date');
			const up_to_date_p = document.getElementById('up_to_date_p');
			const update_available = document.getElementById('update_available');
			const update_available_p = document.getElementById('update_available_p');
		


			let power = false;


			// --- Funktionen --- //


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
				for (let i = 0; i < sub_alertboxes.length; i++) {
					hide(sub_alertboxes[i]);
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
				showAlertbox(connection_lost)
				hide(close_alert);
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

			close_alert.onclick = function(){
				hideAlertbox();
			};

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
