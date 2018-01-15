/*
MäCAN Node.js Server
By Maximilian Goldschmidt (cc)
Do what ever you want with this, but keep this header!

Last edited: 14.01.2018
*/

const use_local_config = true;

const naz = 19;

const ip = 'localhost';
const dest_port = 15734;
const frame_stop = new Buffer([0x00, 0x00, 0x03, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00]);
const frame_go   = new Buffer([0x00, 0x00, 0x03, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00]);

var power = false;

var count = 0;
var clients = {};

var config_data = [];

var gbox_uid;

//----------------------------------------------------------------------------------//
//VARIABLEN FÜR DEVICE INFOS:
var config_buffer = [];

//----------------------------------------------------------------------------------//

var http = require('http');
var server = http.createServer(function(request, response) {});
var dgram = require('dgram');
var udpServer = dgram.createSocket('udp4');
var udpClient = dgram.createSocket('udp4');

var exec = require('child_process').exec;

var fs = require('fs');

//----------------------------------------------------------------------------------//
//FUNKTIONEN:

function toUnsignedString(number){ // Zahl als Unsigned darstellen
  //Unsigned U32
  if (number < 0) {
    number = 0xffffffff + number  + 1;
  }

  return number.toString(16);
}

function hex2a(hexx) {
  //hex to string
    var hex = hexx.toString();//force conversion
    var str = '';
    for (var i = 0; i < hex.length; i += 2)
        str += String.fromCharCode(parseInt(hex.substr(i, 2), 16));
    return str;
}

function statusChanelColor(value){ // Farbe für einen Messwertkanal erreichnen ('rgb(r,g,b)')
  //generate RGB string from byte
  var color = 'rgb(255,255,255)';
  var red = 85 * (value >> 6);
  var green = 85 * ((value & 0b00110000) >> 4);
  var blue = 85 * ((value & 0b00001100) >> 2);
  var black = 85 * (value & 0b00000011);

  color = `rgb(${red},${green},${blue})`;

  return color;

}

function getDeviceInfo(uid, index) { // Grundlegende CAN-Geräteinformationen abrufen
  console.log(`Device info request for UID ${uid}, chanel ${index}`);
  udpClient.send(new Buffer([0, 0x3A, 3, 0, 5, (uid & 0xff000000)>> 24, (uid & 0x00ff0000)>> 16, (uid & 0x0000ff00) >> 8, uid & 0x000000ff, index, 0, 0, 0]), dest_port, ip)
}

function getStatus(uid, chanel){ // Messwert eines Kanals abrufen
  console.log(`Status request for UID ${uid}, chanel ${chanel}`);
  udpClient.send(new Buffer([0, 0, 3, 0, 6, (uid & 0xff000000)>> 24, (uid & 0x00ff0000)>> 16, (uid & 0x0000ff00) >> 8, uid & 0x000000ff, 0x0b, chanel, 0, 0]), dest_port, ip)
}

function getDeviceIndexFromUID(uid, devices){ // Index des Gerätelisteneintrages anhand der UID ermitteln
  for (let i = 0; i < devices.length; i++) {
    if (devices[i].uid == uid) {
      return i;
    }
  }
}

function buildDeviceInfo(buffer, device) { // Object für die Geräteliste erstellen
  device.status_chanels = buffer[1][0];
  device.config_chanels = buffer[1][1];

  device.serial_number = (buffer[1][4] << 24) + (buffer[1][5] << 16) + (buffer[1][6] << 8) + buffer[1][7];

  device.product_number = "";
  device.name = "";
  for (let i = 0; i < 8; i++) {
    device.product_number += (hex2a(buffer[2][i].toString(16))).replace('\u0000',''); 
  }
  for (let i = 3; i < buffer.length; i++) {
    let data = buffer[i];
        for (let j = 0; j < data.length; j++) {
      device.name += (hex2a(data[j].toString(16))).replace('\u0000','');
    }
  }

  return device;

}

function buildStatusChanelInfo(buffer) { // Object für Messwertkanal erstellen
  var status_chanel = {};

  status_chanel.chanel = buffer[1][0];
  if (buffer[1][1] > 127) {
    status_chanel.power = buffer[1][1] - 0xff;
  } else { status_chanel.power = buffer[1][1];
  }
  status_chanel.color_1 = statusChanelColor(buffer[1][2]);
  status_chanel.color_2 = statusChanelColor(buffer[1][3]);
  status_chanel.color_3 = statusChanelColor(buffer[1][4]);
  status_chanel.color_4 = statusChanelColor(buffer[1][5]);
  status_chanel.origin = ((buffer[1][6] << 8 ) + buffer[1][7]);
  status_chanel.range_1 = ((buffer[2][0] << 8 ) + buffer[2][1]);
  status_chanel.range_2 = ((buffer[2][2] << 8 ) + buffer[2][3]);
  status_chanel.range_3 = ((buffer[2][4] << 8 ) + buffer[2][5]);
  status_chanel.range_4 = ((buffer[2][6] << 8 ) + buffer[2][7]);

  var buffer_string = "";
  for (let i = 3; i < buffer.length; i++) {
    for (let j = 0; j < buffer[i].length; j++) {
      buffer_string = buffer_string + hex2a(buffer[i][j].toString(16));
    }
  }

  buffer_string_array = buffer_string.split('\u0000');
  status_chanel.name = buffer_string_array[0];
  status_chanel.start = buffer_string_array[1];
  status_chanel.end = buffer_string_array[2];
  status_chanel.unit = buffer_string_array[3];

  return status_chanel;
}

function buildConfigChanelInfo(buffer) {

}

function setProtocol(protocol) { // Gleisprotokolle des GFP setzen (3 Bits: 1: MM, 2: MFX, 3: DCC)
  udpClient.send(new Buffer([0, 0, 3, 0, 6, (gbox_uid & 0xff000000)>> 24, (gbox_uid & 0x00ff0000)>> 16, (gbox_uid & 0x0000ff00) >> 8, gbox_uid & 0x000000ff, 0x08, protocol, 0, 0]), dest_port, ip);
  let config = require('./config.json');
  let MM = "";
  let DCC = "";
  let mfx = "";
  if (protocol & 0b001) MM = ' MM';
  if (protocol & 0b010) mfx = ' mfx';
  if (protocol & 0b100) DCC = ' DCC';
  config.protocol = protocol;
  console.log('Setting Protocol to' + MM + DCC + mfx + '...');
  //configUpdate(config);
}

function configUpdate(config) { // Config-Datei aktualisieren
  fs.writeFile('./config.json', JSON.stringify(config, null, 2), console.log('Updated config.'));
}

function sendLokNamen(entrys, index) { // Loknamen an Slave-Gerät senden
  let locolist = require('../html/config/locolist.json');
  let loco_num = locolist.length;

  let stream = "[lokomotive]\n"
  for (let i = index; i < entrys; i++) {
    stream += "lok\n .nr=" + i + "\n .name=" + locolist[i].name + "\n";
  }
  stream += "numloks\n .wert=" + locolist.length;

  //console.log(stream);

}

function mfxUidExists(mfxuid, locolist) { // Überprüft, ob eine MFX-UID bereits bekannt ist. Falls ja wird die SID zurückgegeben.
  let result = 0;

  for (let i = 0; i < locolist.length; i++) {
    if (locolist[i].typ == 'mfx') {
      if (locolist[i].mfxuid == mfxuid) {
        result = locolist[i].sid;
        break;
      }
    }
  }
  return result;
}

function bindMFX(mfxuid, sid) { // MFX-Lok anhand der MFX-UID und SID binden.
  console.log('Binding MFX-UID ' + toUnsignedString(mfxuid) + ' to SID ' + sid + '.');
  udpClient.send(new Buffer([0,0x04,3,0,6,(mfxuid & 0xff000000)>> 24, (mfxuid & 0x00ff0000)>> 16, (mfxuid & 0x0000ff00) >> 8, mfxuid & 0x000000ff, (sid & 0xff00) >> 8, sid & 0x00ff,0,0]), dest_port, ip);
}

function bindMfxSid(mfxuid) { // MFX-Lok anhand der MFX-UID binden und SID ermitteln.
  let sid_used = true;
  let locolist = require('../html/config/locolist.json');
  let sid = mfxUidExists(mfxuid, locolist)
  console.log('Checking for unused SID...');
  if (!sid) {
    sid++;
    while (sid_used) {
      console.log('   Chekcing for SID ' + sid);
      sid_used = false;
      for (let i = 0; i < locolist.length; i++) {
        if (locolist[i].typ == 'mfx' && locolist[i].sid == sid) {
          sid_used = true;
          sid ++;
          break;
        }
      }
    }
  }
  bindMFX(mfxuid, sid);
  return sid;
}

function addMfxLocoToList(mfxuid) { // Erkannte MFX-Lok zur Lokliste hinzufügen.
  console.log('Found MFX loco ' + mfxuid);
  let locolist = require('../html/config/locolist.json');
  if (!mfxUidExists(mfxuid, locolist)) {
    let index = locolist.length;
    let sid = bindMfxSid(mfxuid);

    let loco = {};
    loco.name = ""
    loco.mfxuid = mfxuid;
    loco.sid = sid;
    loco.adress = sid;
    loco.uid = 0x4000 + sid;
    loco.typ = 'mfx';

    locolist[index] = loco;

    fs.writeFile('../html/config/locolist.json', JSON.stringify(locolist, null, 2), function(){
      console.log("Adding MFX loco to locolist (UID: " + toUnsignedString(mfxuid) + ").");
    });

    readMfxConfig(loco.uid, 3, 1, 0x10);
  } else {
    console.log('Loco already exists!');
  }

}

function bindKnownMfx() { // In der Lokliste enthaltene MFX-Loks binden.
  let locolist = require('../html/config/locolist.json');

  for (let i = 0; i < locolist.length; i++) {
    const element = locolist[i];
    if (element.typ == 'mfx') {
      bindMFX(element.mfxuid, element.sid);
    }
  }
}

function stop() { // STOP an den GFP senden.
  udpClient.send(new Buffer([0x00, 0x00, 0x03, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00]), dest_port, ip);
  console.log('STOP')
}

function go() { // Go an den GFP senden.
  udpClient.send(new Buffer([0x00, 0x00, 0x03, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00]), dest_port, ip);
  console.log('GO');
  bindKnownMfx();
  setNAZ();
}

function setNAZ() {
  udpClient.send(new Buffer([0,0,3,0,7,0,0,0,0,0x09,0,naz,0]), dest_port, ip);
}

function ping() { // Ping aussenden.  
  udpClient.send(new Buffer([0,0x30,3,0,0,0,0,0,0,0,0,0,0]), dest_port, ip);
  console.log('Ping sent.');
}

function readMfxConfig(mfxuid, cv_n, cv_i, count) {
  let buffer = [0, 0x0e, 0x03, 0, 7, (mfxuid & 0xff000000)>> 24, (mfxuid & 0x00ff0000)>> 16, (mfxuid & 0x0000ff00) >> 8, mfxuid & 0x000000ff, 0, 0, count, 0];
  buffer[9] = (cv_i << 2) + (cv_n >> 6);
  buffer[10] = (cv_n & 0b00111111);
  udpClient.send(new Buffer(buffer), dest_port, ip);
}

function buildLocoName(uid, data) {
  if (data[6]){
    let locolist = require('../html/config/locolist');
    for (let i = 0; i < locolist.length; i++) {
      if (locolist[i].uid == uid){
        locolist[i].name += String.fromCharCode(data[6]);
        break;
      }
    }
    fs.writeFile('../html/config/locolist.json', JSON.stringify(locolist, null, 2), function(){
    });
  }
}

//----------------------------------------------------------------------------------//

fs.writeFile('../html/config/devices.json', JSON.stringify([], null, 2), function(){
  console.log("clearing devices");
});

udpServer.on('listening', () => {
  var address = udpServer.address();
  console.log(`UDP-Server listening on Port ${address.port}.`);
  stop();
});

udpServer.bind(15733);
ping();
//bindKnownMfx();

if (use_local_config) {
  let config = require('./config.json');
  setProtocol(config.protocol);
}

server.listen(8080, function() {
    console.log('WebSocet-Server is listening on port 8080');
});

var WebSocketServer = require('websocket').server;
wsServer = new WebSocketServer({
    httpServer: server
});

//WebSocket-Clients aufnehmen:
wsServer.on('request', function(request){
	var connection = request.accept('echo-protocol', request.origin);
	var id = count++;
	clients[id] = connection;
	console.log(connection.remoteAddress + ' connected.');

	connection.send(power ? 'go' : 'stop');

	connection.on('close', function(reasonCode, description){
		delete clients[id];
		console.log(connection.remoteAddress + ' disconnected.');
	});


	//Befehle vom WebSocet-Client verarbeiten:
	connection.on('message', function(dgram){

		var msg = dgram.utf8Data.split(':');
		var cmd = msg[0];
		var uid = parseInt(msg[1]);
		var value = parseInt(msg[2]);

		var uid_high = (uid & 0xFF00) >> 8;
		var uid_low = (uid & 0x00FF);
		var value_high = (value & 0xFF00) >> 8;
		var value_low = (value & 0x00FF);

		if (cmd == 'stop') {			// STOP-Taste
      stop();
      
		} else if (cmd == 'go') {		// GO-Taste
      go();
      
		} else if (cmd == 'setSpeed') {
      udpClient.send(new Buffer([0, 8, 3, 0, 6, 0, 0, uid_high, uid_low, value_high, value_low, 0, 0]), dest_port, ip);
      
		} else if (cmd == 'getSpeed') {
			udpClient.send(new Buffer([0, 8, 3, 0, 4, 0, 0, uid_high, uid_low, 0, 0, 0, 0]), dest_port, ip);
    
    } else if (cmd == 'lokNothalt') {
			udpClient.send(new Buffer([0, 0, 3, 0, 5, 0, 0, uid_high, uid_low, 3, 0, 0, 0]), dest_port, ip);
    
    } else if (cmd == 'setFn') {
			udpClient.send(new Buffer([0, 0x0c, 3, 0, 6, 0, 0, uid_high, uid_low, value_high, value_low, 0, 0]), dest_port, ip);
    
    } else if (cmd == 'getFn') {
			udpClient.send(new Buffer([0, 0x0c, 3, 0, 5, 0, 0, uid_high, uid_low, value, 0, 0, 0]), dest_port, ip);
    
    } else if (cmd == 'toggleDir') {
			udpClient.send(new Buffer([0, 0x0a, 3, 0, 5, 0, 0, uid_high, uid_low, 3, 0, 0, 0]), dest_port, ip);
			udpClient.send(new Buffer([0, 0x0a, 3, 0, 4, 0, 0, uid_high, uid_low, 0, 0, 0, 0]), dest_port, ip);
    
    } else if (cmd == 'getDir') {
			udpClient.send(new Buffer([0, 0x0a, 3, 0, 4, 0, 0, uid_high, uid_low, 0, 0, 0, 0]), dest_port, ip);
    
    } else if (cmd == 'getStatus') {
      getStatus(msg[1], msg[2]);
    
    } else if (cmd == 'setProtocol') {
      setProtocol(msg[1]);
    
    } else if (cmd == 'getProtocol') {
      let config = require('./config.json');
      for (let i in clients){
        clients[i].sendUTF(`updateProtocol:${config.protocol}`);
      }
    
    } else if (cmd == 'restart') {
      wsServer.shutDown();
      exec('shutdown -r now');
    
    } else if (cmd == 'shutdown') {
      wsServer.shutDown();
      exec('shutdown now');
    
    } else if (cmd == 'downloadIcon') {
      let link = "";
      for (let i = 1; i < msg.length; i++){
        if(i > 1) link += ':';
        link += msg[i];
      }
      exec('wget -c -P /www/MaeCAN-Server/html/loco_icons/ ' + link, console.log('Downloading ' + link));
    }
	});
});


udpServer.on('error', (err) => {
  console.log(`server error:\n${err.stack}`);
  udpServer.close();
});


// Befehle vom CAN verarbeiten:
udpServer.on('message', (udp_msg, rinfo) => {

  var ws_msg = '';
  var cmd = parseInt(udp_msg[1]);
  var hash = (parseInt(udp_msg[2]) << 8) + parseInt(udp_msg[3]);
  var dlc = parseInt(udp_msg[4]);
  var data = [parseInt(udp_msg[5]), parseInt(udp_msg[6]), parseInt(udp_msg[7]), parseInt(udp_msg[8]), parseInt(udp_msg[9]), parseInt(udp_msg[10]), parseInt(udp_msg[11]), parseInt(udp_msg[12])]
  var uid = ((data[0] << 24)>>>0) + ((data[1] << 16)>>>0) + ((data[2] << 8)>>>0) + data [3];
   value = (parseInt(udp_msg[9]) << 8) + parseInt(udp_msg[10]);

  if (cmd == 0x01) {
  	var sub_cmd = parseInt(udp_msg[9]);
  	if (sub_cmd == 0x00) {
  		ws_msg = 'stop';
  		power = false;
  	} else if (sub_cmd == 0x01) {
  		ws_msg = 'go';
  		power = true;
  	} else if (sub_cmd == 0x03){
      ws_msg = `updateSpeed:${uid}:${0}`;
    } else if (sub_cmd == 0x08) {
      ws_msg = `updateProtocol:${data[5]}`;
      let config = require('./config.json');
      config.protocol = data[5];
      configUpdate(config);
  	} else if (sub_cmd == 0x0B) {
      ws_msg = `updateReading:${uid}:${data[5]}:${(data[6] << 8) + data[7]}`;
    }
  } else if (cmd == 0x03 && dlc == 5) {
    addMfxLocoToList(uid);
  } else if (cmd == 0x09) {
    if (dlc == 4) value = 0;
  	ws_msg = `updateSpeed:${uid}:${value}`;
  } else if (cmd == 0x0d) {
  	ws_msg = `updateFn:${uid}:${value}`;
  } else if (cmd == 0x0b) {
    ws_msg = `updateDir:${uid}:${parseInt(udp_msg[9])}`;
  } else if (cmd == 0x0f) {
    buildLocoName(uid, data);
  } else if (cmd == 0x37) {
    udpClient.send(new Buffer([0,0x36,3,0,5,data[0],data[1],data[2],data[3],0x11,0,0,0]), dest_port, ip);
    setTimeout(() => udpClient.send(new Buffer([0,0x30,3,0,0,0,0,0,0,0,0,0,0]), dest_port, ip), 20);
    setTimeout(() => {
      setNAZ();
      //bindKnownMfx();
    }, 40);
  } else if (cmd == 0x3B) {

    // --- Statusdaten Konfiguration --- //
    if (dlc == 8) {
      if (hash == 0x301) {
        config_buffer = [];
      }
      var packet_index = hash - 0x300;
      config_buffer[packet_index] = data;
    }

    if (dlc == 6 || dlc == 5) {
      let devices = require('../html/config/devices.json');
      let device = devices[getDeviceIndexFromUID(uid, devices)];
      config_buffer[0] = data;
      let chanel = config_buffer[0][4];
      console.log('Done getting chanel ' + chanel + ' from device ' + device.name);
      if (chanel > 0 && chanel <= device.status_chanels) {
        let status_chanel = buildStatusChanelInfo(config_buffer);
        device.status_chanels_info[chanel - 1] = status_chanel;
        fs.writeFile('../html/config/devices.json', JSON.stringify(devices, null, 2), console.log("updating devices entry."));
      } else if (chanel == 0) {
        device = buildDeviceInfo(config_buffer, device);
        fs.writeFile('../html/config/devices.json', JSON.stringify(devices, null, 2), console.log("updating devices entry."));
      }
    }
  } else if (cmd == 0x30) {
    //udpClient.send(new Buffer([0,0x31,3,0,8,0,0,0,0,0,1,0xff,0xff]), dest_port, ip);
  } else if (cmd == 0x31) {
  	let str_uid = toUnsignedString(uid);
  	let str_ver = `${data[4]}.${data[5]}`
  	let str_typ = ((data[6] << 8) + data[7]).toString(16);
  	console.log(`Ping response from 0x${str_uid}, Version ${str_ver}, Type 0x${str_typ}`);

  	if (str_typ =="10" && !gbox_uid) {
  		console.log(`Found Gleisbox 0x${str_uid}`);
  		gbox_uid = uid;
    }
    
    var devices = require('../html/config/devices.json');
    var device_exists = false;
    let existing_index = 0;

    for (i = 0; i < devices.length; i++) {
      if (devices[i].uid == uid || uid == 0) {
        device_exists = true;
        existing_index = i;
        break;
      }
    }

    if (!device_exists) {
      var index = devices.length;
      devices[index] = {};
      devices[index].uid = uid;
      devices[index].version = str_ver;
      devices[index].type = str_typ;

      fs.writeFile('../html/config/devices.json', JSON.stringify(devices, null, 2), function(){
        console.log("writing new devices entry.");
      });
    }

    if (device_exists) {
      if (!devices[existing_index].name) {
        getDeviceInfo(uid, 0);
      }
    }
  }

  if (ws_msg != '') {
	  for (var i in clients){
	  	clients[i].sendUTF(ws_msg);
	  }
  }
});


setInterval(() => ping(), 10000);

setInterval(function(){
  let devices = require('../html/config/devices.json');
  if(!devices.length) {
    udpClient.send(new Buffer([0,0x36,3,0,0,0,0,0,0,0,0,0,0]), dest_port, ip);
    //udpClient.send(new Buffer([0,0x31,3,0,8,0,0,0,0,0,1,0xff,0xff]), dest_port, ip);
  }
}, 1000);

var data_fetcher = setInterval(function(){
  var devices = require('../html/config/devices.json');
  for (var i = 0; i < devices.length; i++) {
    if (!devices[i].name){
      if (devices[i].request_count < 10 || !devices[i].request_count) {
        /*if (!devices[i].request_count) {
          devices[i].request_count = 0
        }
        getDeviceInfo(devices[i].uid, 0);
        devices[i].request_count++;
        break;*/
      }  
    } else if (devices[i].status_chanels && (!devices[i].status_chanels_info || devices[i].status_chanels_info.length < devices[i].status_chanels)) {
      if (!devices[i].status_chanels_info) {
        devices[i].status_chanels_info = [];        
      }
      console.log('need to get ' + (devices[i].status_chanels - devices[i].status_chanels_info.length) + ' status chanels of ' + devices[i].name);
      getDeviceInfo(devices[i].uid, devices[i].status_chanels_info.length + 1);
    }
  }
}, 100);
