
var ws;

function connectWebsocket(){
	var url = window.location.hostname;
	console.log('Current Domain: ' + url);
	ws = new WebSocket("ws://" + url + ':8080', 'echo-protocol');
	ws.onerror = function(error){
		console.log("WebSocket-Fehler");
	};

	ws.onclose = function(){
		console.log('WebSocket connection closed!')
	};
}

connectWebsocket();


function send(msg){
  ws.send(msg);
  //console.log('Sending: ' + msg + '.');
}