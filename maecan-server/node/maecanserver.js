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


//----------------------------------------------------------------------------------//
// BEFEHLSKONSTANTEN
//----------------------------------------------------------------------------------//

const SYSTEM_CMD = 0x00
  // Subbefehle:
  const SYS_STOP = 0x00
  const SYS_GO = 0x01
  const SYS_HALT = 0x02
  const SYS_LOCO_EMERGENCY_STOP = 0x03
  const SYS_LOCO_CYCLE_STOP = 0x04
  const SYS_LOCO_PROTOCOL = 0x05
  const SYS_SWITCH_TIME = 0x06
  const SYS_FAST_READ = 0x07
  const SYS_TRACK_PROTOCOL = 0x08
  const SYS_MFX_NRC = 0x09
  const SYS_OVERLOAD = 0x0a
  const SYS_STATUS = 0x0b
  const SYS_ID = 0x0c
  const SYS_MFS_SEEK = 0x30
  const SYS_RESET = 0x80
const LOCO_DISCOVERY = 0x02
const MFX_BIND = 0x04
const MFX_VERIFY = 0x06
const LOCO_SPEED = 0x08
const LOCO_DIR = 0x0a
const LOCO_FN = 0x0c
const READ_CONFIG = 0x0e
const WRITE_CONFIG = 0x10
const ACC_SWITCH = 0x16
const ACC_CONFIG = 0x18
const S88_POLL = 0x20
const S88_EVENT = 0x22
const SX1_EVENT = 0x24
const PING = 0x30
const OFFER_UPDATE = 0x32
const READ_CONFIG_DATA = 0x34
const BOOTL_CAN = 0x36
const BOOTL_TRACK = 0x38
const STATUS_CONFIG = 0x3a
const DATA_QUERRY = 0x40
const CONFIG_DATA_STREAM = 0x42
const CONNECT_DATA_STREAM = 0x44


// CRC Lookup Table
const crcTable = [0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5,
  0x60c6, 0x70e7, 0x8108, 0x9129, 0xa14a, 0xb16b,
  0xc18c, 0xd1ad, 0xe1ce, 0xf1ef, 0x1231, 0x0210,
  0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
  0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c,
  0xf3ff, 0xe3de, 0x2462, 0x3443, 0x0420, 0x1401,
  0x64e6, 0x74c7, 0x44a4, 0x5485, 0xa56a, 0xb54b,
  0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
  0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6,
  0x5695, 0x46b4, 0xb75b, 0xa77a, 0x9719, 0x8738,
  0xf7df, 0xe7fe, 0xd79d, 0xc7bc, 0x48c4, 0x58e5,
  0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
  0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969,
  0xa90a, 0xb92b, 0x5af5, 0x4ad4, 0x7ab7, 0x6a96,
  0x1a71, 0x0a50, 0x3a33, 0x2a12, 0xdbfd, 0xcbdc,
  0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
  0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03,
  0x0c60, 0x1c41, 0xedae, 0xfd8f, 0xcdec, 0xddcd,
  0xad2a, 0xbd0b, 0x8d68, 0x9d49, 0x7e97, 0x6eb6,
  0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
  0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a,
  0x9f59, 0x8f78, 0x9188, 0x81a9, 0xb1ca, 0xa1eb,
  0xd10c, 0xc12d, 0xf14e, 0xe16f, 0x1080, 0x00a1,
  0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
  0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c,
  0xe37f, 0xf35e, 0x02b1, 0x1290, 0x22f3, 0x32d2,
  0x4235, 0x5214, 0x6277, 0x7256, 0xb5ea, 0xa5cb,
  0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
  0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447,
  0x5424, 0x4405, 0xa7db, 0xb7fa, 0x8799, 0x97b8,
  0xe75f, 0xf77e, 0xc71d, 0xd73c, 0x26d3, 0x36f2,
  0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
  0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9,
  0xb98a, 0xa9ab, 0x5844, 0x4865, 0x7806, 0x6827,
  0x18c0, 0x08e1, 0x3882, 0x28a3, 0xcb7d, 0xdb5c,
  0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
  0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0,
  0x2ab3, 0x3a92, 0xfd2e, 0xed0f, 0xdd6c, 0xcd4d,
  0xbdaa, 0xad8b, 0x9de8, 0x8dc9, 0x7c26, 0x6c07,
  0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
  0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba,
  0x8fd9, 0x9ff8, 0x6e17, 0x7e36, 0x4e55, 0x5e74,
  0x2e93, 0x3eb2, 0x0ed1, 0x1ef0];


const function_table = [0 ,0 ,50 ,1 ,2 , 3, 14, 5, 5, 5, 8, 102];

const devices_path = './devices.json';
const locolist_path = './locolist.json';
const accessories_path = './accessories.json';

//----------------------------------------------------------------------------------//
// Laden der Konfiguration

var local_config = require('./config.json');
var local_version = require('./version.json');

var devices = require(devices_path);
var locolist = require(locolist_path);
var accessories = require(accessories_path)

var naz = local_config.new_registration_counter;
var master = local_config.master;
var ip = local_config.ip;
var d_port = local_config.d_port;
var l_port = local_config.l_port;
var version = local_version.version;

var power = false;

var client_count = 0;
var clients = {};

var gbox_uid;


//----------------------------------------------------------------------------------//
// VARIABLEN FÜR DEVICE INFOS:
var config_buffer = [];
var temp_mfx_loco = {};

// Loknamen data query
var loknamen_request = false;

var lokinfo_request = 0;
var lokinfo_loconame = "";

var bussy_fetching = false;
var bussy_fetching_timeout;


//----------------------------------------------------------------------------------//

var http = require('http');
var server = http.createServer(function(request, response) {});
var dgram = require('dgram');
var udpServer = dgram.createSocket('udp4');
var udpClient = dgram.createSocket('udp4');

var exec = require('child_process').exec;

var fs = require('fs');


//----------------------------------------------------------------------------------//
// MFX-Puffer

var last_mfx_call = [];
var mfx_buffer = [];


//----------------------------------------------------------------------------------//
// FUNKTIONEN:
//----------------------------------------------------------------------------------//
// Allgemein:

function sendDatagram(data) {
  // Datagramm senden

  udpClient.send(new Buffer(data), 0, 13, d_port, ip);
}

function sendCanFrame(cmd, response, dlc, data) {
  // CAN-Nachricht senden
}

function toUnsignedString(number){ 
  // Lange Hex-Zahl als Unsigned darstellen

  if (number < 0) {
    number = 0xffffffff + number  + 1;
  }

  return number.toString(16);
}

function hex2a(hexx) {
  // hex to string

    var hex = hexx.toString();//force conversion
    var str = '';
    for (var i = 0; i < hex.length; i += 2)
        str += String.fromCharCode(parseInt(hex.substr(i, 2), 16));
    return str;
}

function configUpdate(config) { 
  // Config-Datei aktualisieren

  fs.writeFile('./config.json', JSON.stringify(config, null, 2), function(){
    console.log('Updated config.')  });
}

function crc16(s) {
  // Mithilfe des Lookup-Tables die Prüfsumme generieren

  var crc = 0xFFFF;
    var j, i;


    for (i = 0; i < s.length; i++) {

        let c = s.charCodeAt(i);
        if (c > 255) {
            throw new RangeError();
        }
        j = (c ^ (crc >> 8)) & 0xFF;
        crc = crcTable[j] ^ (crc << 8);
    }

    return ((crc ^ 0) & 0xFFFF);

}


//----------------------------------------------------------------------------------//
// CAN-Gerätekonfiguration:

function statusChanelColor(value){
  // Berechnet die Farbe für einen Statusbalken aus den jeweils zuständigen Bits.

  var color = 'rgb(255,255,255)';
  var red = 85 * (value >> 6);
  var green = 85 * ((value & 0b00110000) >> 4);
  var blue = 85 * ((value & 0b00001100) >> 2);
  var black = 85 * (value & 0b00000011);

  color = `rgb(${red},${green},${blue})`;

  return color;

}

function getDeviceInfo(uid, index) {
  // Grundlegende CAN-Geräteinformationen abrufen

  bussy_fetching = true;
  
  console.log(`Device info request for UID ${uid}, chanel ${index}`);
  sendDatagram([0, STATUS_CONFIG, 3, 0, 5, (uid & 0xff000000)>> 24, (uid & 0x00ff0000)>> 16, (uid & 0x0000ff00) >> 8, uid & 0x000000ff, index, 0, 0, 0]);

  setTimeout(() => {
    bussy_fetching = false;
  },3000);
}

function getStatus(uid, chanel){
  // Messwert eines Kanals abrufen

  console.log(`Status request for UID ${uid}, chanel ${chanel}`);
  sendDatagram([0, SYSTEM_CMD, 3, 0, 6, (uid & 0xff000000)>> 24, (uid & 0x00ff0000)>> 16, (uid & 0x0000ff00) >> 8, uid & 0x000000ff, SYS_STATUS, chanel, 0, 0]);
}

function getDeviceIndexFromUID(uid, devices){ 
  // Index des Gerätelisteneintrages anhand der UID ermitteln

  for (let i = 0; i < devices.length; i++) {
    if (devices[i].uid == uid) {
      return i;
    }
  }
}

function buildDeviceInfo(buffer, device) { 
  // Object für die Geräteliste erstellen

  if(buffer[2]) {
    device.status_chanels = buffer[1][0];
    device.config_chanels = buffer[1][1];

    device.serial_number = (buffer[1][4] << 24) + (buffer[1][5] << 16) + (buffer[1][6] << 8) + buffer[1][7];
   
    device.product_number = new Buffer(buffer[2]).toString().replace(/\u0000/g,'');

    let name_buffer = [];
    for (let i = 3; i < buffer.length; i++) {
      let data = buffer[i];
      for (let j = 0; j < data.length; j++) {
        if (data[j] == 0) {break}
        name_buffer[(i-3)*8+j] = data[j];
      }
    }
    device.name = new Buffer(name_buffer).toString();
  } else {
    console.log("buildDeviceInfo Error! New Attempt soon ...");
  }

    return device;

}

function buildStatusChanelInfo(buffer) { 
  // Object für Messwertkanal erstellen

  var status_chanel = {};

  status_chanel.chanel = buffer[1][0];
  if (buffer[1][1] > 127) {
    status_chanel.power = buffer[1][1] - 0x100;
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

  var buffer_string = [];
  for (let i = 3; i < buffer.length; i++) {
    for (let j = 0; j < buffer[i].length; j++) {
      buffer_string[(i-3)*8+j] = buffer[i][j];
    }
  }

  let buffer_string_array = new Buffer(buffer_string).toString().split('\u0000');
  status_chanel.name = buffer_string_array[0];
  status_chanel.start = buffer_string_array[1];
  status_chanel.end = buffer_string_array[2];
  status_chanel.unit = buffer_string_array[3];

  return status_chanel;
}

function buildConfigChanelInfo(buffer) { 
  // Objekt für Einstellungskanal erstellen

  let config_chanel = {};

  config_chanel.chanel = buffer[1][0];
  config_chanel.type = buffer[1][1];

  if (config_chanel.type == 1) {
    // Auswahlliste

    config_chanel.num_options = buffer[1][2];
    config_chanel.def_option = buffer[1][3];

    let buffer_string = [];
    for (let i = 2; i < buffer.length; i++) {
      for (let j = 0; j < buffer[i].length; j++) {
        buffer_string[(i-2)*8+j] = buffer[i][j];
      }
    }
    
    let buffer_string_array = new Buffer(buffer_string).toString().split('\u0000');
    config_chanel.description = buffer_string_array[0];
    config_chanel.options = [];
    for (let i = 0; i < config_chanel.num_options; i++) {
      config_chanel.options[i] = buffer_string_array[i + 1];
    }
  }

  if (config_chanel.type == 2) {
    // Slider

    config_chanel.min_value = (buffer[1][2] << 8 ) + buffer[1][3];
    config_chanel.max_value = (buffer[1][4] << 8 ) + buffer[1][5];
    config_chanel.def_value = (buffer[1][6] << 8 ) + buffer[1][7];

    let buffer_string = [];
    for (let i = 2; i < buffer.length; i++) {
      for (let j = 0; j < buffer[i].length; j++) {
        buffer_string[(i-2)*8+j] = buffer[i][j];
      }
    }
    
    let buffer_string_array = new Buffer(buffer_string).toString().split('\u0000');
    config_chanel.description = buffer_string_array[0];
    config_chanel.start = buffer_string_array[1];
    config_chanel.end = buffer_string_array[2];
    config_chanel.unit = buffer_string_array[3];
  }


  return config_chanel;
}


//----------------------------------------------------------------------------------//
// GFP-Verwaltung:

function setProtocol(protocol) { 
  // Gleisprotokolle des GFP setzen (3 Bits: 1: MM, 2: MFX, 3: DCC)

  sendDatagram([0, SYSTEM_CMD, 3, 0, 6, (gbox_uid & 0xff000000)>> 24, (gbox_uid & 0x00ff0000)>> 16, (gbox_uid & 0x0000ff00) >> 8, gbox_uid & 0x000000ff, SYS_TRACK_PROTOCOL, protocol, 0, 0]);
  let MM = "";
  let DCC = "";
  let mfx = "";
  if (protocol & 0b001) MM = ' MM';
  if (protocol & 0b010) mfx = ' mfx';
  if (protocol & 0b100) DCC = ' DCC';
  local_config.protocol = protocol;
  console.log('Setting Protocol to' + MM + DCC + mfx + '...');
}

function setNAZ() { 
  // MFX-Neuanmeldezähler setzen

  sendDatagram([0, SYSTEM_CMD,3,0,7,0,0,0,0,SYS_MFX_NRC,0,naz,0])
}


//----------------------------------------------------------------------------------//
// Data Query-Verwaltung:

function sendDataQueryFrames(data_string, rec_hash) { 
  // Datenpakete versenden (z.B. Lokliste etc.)

  console.log(data_string);
  let length = data_string.length;
  let padding = 8 - (length % 8)

  if (padding != 8) {
    for(let i = 0; i < padding; i++){
      data_string += '\x00';
    }
  }

  let packets = length / 8;
  let packet_index = 0;
  let data = [0,0,0,0,0,0,0,0];
  let crc = crc16(data_string);

  // Paketlänge und CRC sendn
  sendDatagram([0x00, CONFIG_DATA_STREAM, rec_hash >> 8, rec_hash & 0xff, 0x06, 0x00, 0x00, length >> 8, length & 0xff, crc >> 8, crc & 0xff, 0x00, 0x00]);

  // Daten senden
  for (let i = 0; i < packets; i++){
    for (let j = 0; j < 8; j++) {
      if (data_string[(i*8)+j]){
        data[j] = data_string[(i*8)+j].charCodeAt(0);
      } else {
        data[j] = 0;
      }
      
    }
    sendDatagram([0, CONFIG_DATA_STREAM, rec_hash >> 8, rec_hash & 0xff, 8, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]]);
  }
}

function sendLocoNames(loco_index, loco_count, rec_hash){ 
  // Loknamen an Client senden

  loknamen_request = false;

  console.log("Loknamen-request for " + loco_count + " locos, starting at index " + loco_index + ".");

  // Antwort auf Anfrage
  sendDatagram([0x00, DATA_QUERRY + 1, 0x03, 0x00, 0x08, loco_index.toString().charCodeAt(0), 0x20, loco_count.toString().charCodeAt(0), 0, 0, 0, 0x00, 0x00]);
  
  // Sind genug Einträge vorhanden?
  if (loco_count > locolist.length) {
    loco_count = locolist.length;
  }

  // String zusammenbauen
  let loco_string = "[lokomotive]\x0a";

  for (let i = loco_index; i < loco_count + loco_index; i++) {
    loco_string += "lok\x0a .nr=" + i + "\x0a .name=" + locolist[i].name + "\x0a";
  }

  loco_string += "numloks\x0a .wert=" + locolist.length + "\x0a";

  sendDataQueryFrames(loco_string, rec_hash);
}

function buildLocoStringForCS2(loco, is_cs2_locolist){
  // Einen von einer CS2 o.Ä. lesbaren String erzeugen

  let loco_string = "lokomotive\x0a .uid=0x" + loco.uid.toString(16) + "\x0a .name=" + loco.name + "\x0a .adresse=0x" + loco.adress.toString(16) + "\x0a .typ=" + loco.typ;
    if (loco.typ == "mfx") {
      loco_string += ("\x0a .mfxuid=" + loco.mfxuid.toString(16));
    } else {
      loco_string += "\x0a .mfxuid=0xffffffff";
    }
    loco_string += ("\x0a .av=" + loco.av + "\x0a .bv=" + loco.bv + "\x0a .volume=" + loco.volume + "\x0a .vmax=" + loco.vmax + "\x0a .vmin=" + loco.vmin);

    if (!is_cs2_locolist) {
      let speed = "0";
      let dir = "0";
      if (loco.speed) speed = loco.speed.toString();
      if (loco.dir) dir = (loco.dir - 1).toString();
      loco_string += ("\x0a .velocity=" + speed + "\x0a .richtung=" + dir);
    }

    if (loco.functions && !is_cs2_locolist) {
      for (let i = 0; i < loco.functions.length; i++) {
        let loco_function = "0";
        let loco_function_value = "0";
        if (!(typeof loco.functions[i].toString() === "undefined")) loco_function = loco.functions[i].toString();
        if (loco.functions_values[i]) loco_function_value = loco.functions_values[i].toString();
        loco_string += "\x0a .fkt\x0a ..nr=" + i + "\x0a ..typ=" + loco_function + "\x0a ..wert=" + loco_function_value;
      }
    } else if(loco.functions && is_cs2_locolist) {
      for (let i = 0; i < loco.functions.length; i++) {
        let loco_function = "0";
        if (!(typeof loco.functions[i].toString() === "undefined")) loco_function = loco.functions[i].toString();
        loco_string += "\x0a .funktionen\x0a ..nr=" + i + "\x0a ..typ=" + loco_function;
      }
    }

    loco_string += "\x0a";

  return loco_string;
}

function exportCS2Locolist(locolist){
  // Lokliste für CS2-Anwendungen generieren

  let locolist_string = "[lokomotive]\x0aversion\x0a .minor=1\x0asession\x0a .id=" + naz + "\x0a";
  for (let i = 0; i < locolist.length; i++) {
    locolist_string += buildLocoStringForCS2(locolist[i], true);
  }
  
  fs.writeFile('/www/config/lokomotive.cs2', locolist_string, () => {
    console.log('Exported Lokomotive.cs2');
  });
}

function sendLocoInfo(loco_name, rec_hash) { 
  // Lokinformationen an Client senden

  loco_name = loco_name.replace(/\0/g, '');

  lokinfo_request = 0;

  console.log("Lokinfo-request for " + loco_name + ".");
  
  let found_loco = false;
  let loco;

  // Index für Loknamen ermitteln
  for (let i = 0; i < locolist.length; i++) {
    if (loco_name == locolist[i].name.substring(0, 16)) {
      loco = locolist[i];
      console.log("Found " + locolist[i].name + " at index " + i + ".");
      found_loco = true;
      break;
    }
  }

  if(found_loco) {
    // Antwort auf Anfrage
    sendDatagram([0x00, DATA_QUERRY + 1, 0x03, 0x00, 0x08, 'l', 'o', 'k', 'i', 'n', 'f', 'o', 0]);
    let data = [0,0,0,0,0,0,0,0];
    for (let i = 0; i < 8; i++) {
      if (i < loco_name.length){
        data[i] = loco_name[i].charCodeAt(0);
      } else {
        break;
      }
    }
    sendDatagram([0x00, DATA_QUERRY + 1, 0x03, 0x00, 0x08, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]]);

    data = [0,0,0,0,0,0,0,0];
    for (let i = 0; i < 8; i++) {
      if (i < (loco_name.length - 8)){
        data[i] = loco_name[i+8].charCodeAt(0);
      } else {
        break;
      }
    }
    sendDatagram([0x00, DATA_QUERRY + 1, 0x03, 0x00, 0x08, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]]);


    // String zusammenbauen
    let data_string = "[lokomotive]\x0a" + buildLocoStringForCS2(loco, false);

    sendDataQueryFrames(data_string, rec_hash);
  } else {
    console.log("No loco found!");
  }
}

function writeCV(uid, cv_n, cv_i, value) {
  // CV-Werte schreiben

  let cv = (cv_i << 10) + cv_n;
  let valid = false;
  if (uid <= 0x03ff) {
    // Motorola
    if (cv_n > 0 && cv_n <= 256) {
      
    } else {
      console.log('Exceeding MM2 CV space.')
    }
  } else if (uid >= 0xc000 && uid <= 0xffff) {
    // DCC
    if (cv_n > 0 && cv_n <= 1024) {
      if (value >= 0 && value <= 255) {
        valid = true;
      }
    } else {
      console.log('Exceeding DCC CV space.')
    }
  } else if (uid >= 0x4000) {
    //mfx
    valid = true;
  } else {
    console.log('Unknown Protokoll, not writing CV.')
  }

  if (valid) {
    sendDatagram([0, WRITE_CONFIG, 3, 0, 8, (uid & 0xff000000)>> 24, (uid & 0x00ff0000)>> 16, (uid & 0x0000ff00) >> 8, uid & 0x000000ff, (cv_i << 2) + (cv_n >> 8), cv_n & 0xff, value, 0])
  }
}


//----------------------------------------------------------------------------------//
// MFX-Behandlung:

function mfxUidExists(mfxuid, locolist) { 
  // Überprüft, ob eine MFX-UID bereits bekannt ist. Falls ja wird die SID zurückgegeben.

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

function bindMFX(mfxuid, sid) { 
  // MFX-Lok anhand der MFX-UID und SID binden.

  console.log('Binding MFX-UID ' + toUnsignedString(mfxuid) + ' to SID ' + sid + '.');
  sendDatagram([0 ,MFX_BIND, 3, 0, 6, (mfxuid & 0xff000000)>> 24, (mfxuid & 0x00ff0000)>> 16, (mfxuid & 0x0000ff00) >> 8, mfxuid & 0x000000ff, (sid & 0xff00) >> 8, sid & 0x00ff,0,0]);
}

function bindMfxSid(mfxuid) { 
  // MFX-Lok anhand der MFX-UID binden und SID ermitteln.

  let sid_used = true;
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

function addMfxLocoToList(mfxuid) { 
  // Erkannte MFX-Lok zur Lokliste hinzufügen.

  console.log('Found MFX loco ' + mfxuid);
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

    locolist = locolist.sort((a, b) => {
      if (a.name < b.name) return -1;
      if (a.name > b.name) return 1;
      return 0;
    })
    
    fs.writeFile(locolist_path, JSON.stringify(locolist, null, 2), function(){
      console.log("Adding MFX loco to locolist (UID: " + toUnsignedString(mfxuid) + ").");
      //exportCS2Locolist(locolist);
      updateLocolist();
    });

    temp_mfx_loco = loco;

    readMfxConfig(loco.uid, 3, 1, 0x10); // Loknamen auslesen, weiter in processMfxBuffer() ...

  } else {
    console.log('Loco already exists!');
  }

}

function bindKnownMfx() { 
  // In der Lokliste enthaltene MFX-Loks binden.

  for (let i = 0; i < locolist.length; i++) {
    const element = locolist[i];
    if (element.typ == 'mfx') {
      bindMFX(element.mfxuid, element.sid);
    }
  }
}

function readMfxConfig(mfxuid, cv_n, cv_i, count) {
  // MFX-Decoder auslesen
  
  let buffer = [0, READ_CONFIG, 0x03, 0, 7, (mfxuid & 0xff000000)>> 24, (mfxuid & 0x00ff0000)>> 16, (mfxuid & 0x0000ff00) >> 8, mfxuid & 0x000000ff, 0, 0, count, 0];
  let cv = (cv_i << 10) + cv_n;
  buffer[9] = cv >> 8;
  buffer[10] = (cv & 0xff);
  sendDatagram(buffer);
  last_mfx_call[0] = cv_n;
  last_mfx_call[1] = count;
  last_mfx_call[2] = mfxuid;
}

function fillMfxBuffer(data) {
  // Empfangene Daten zwischenspeichern

  let cv = (data[4] << 8) + data[5];
  let cv_i = cv >> 10;
  let cv_n = cv & 0x3ff;

  mfx_buffer[0] = cv_n;
  mfx_buffer[cv_i] = data[6];
  if (cv_i == last_mfx_call[1]) {
    processMfxBuffer();
  }
}

function processMfxBuffer() {
  // Den Puffer eines mfx-Config-Reads auswerten

  let cv_n = mfx_buffer[0];
  
  if (cv_n == 3) {
    // Lokname

    let data = [];
    for (let i = 1; i < mfx_buffer.length; i++){
      if (mfx_buffer[i] == 0 ) break;
      data[i-1] = mfx_buffer[i];
    }

    temp_mfx_loco.name = new Buffer(data).toString();

    readMfxConfig(last_mfx_call[2], 4, 1, 9);

  } else if (cv_n == 4) {
    // Blockdefinitionen auslesen

    temp_mfx_loco.mfxadr = {};
    temp_mfx_loco.mfxadr.xcel = (mfx_buffer[4] * 4) + 2;
    temp_mfx_loco.mfxadr.speedtable = (mfx_buffer[4] * 4) +3
    temp_mfx_loco.mfxadr.volume = (mfx_buffer[7] * 4) + 1;
    temp_mfx_loco.mfxadr.func = (mfx_buffer[3] * 4) + 1;

    readMfxConfig(last_mfx_call[2], temp_mfx_loco.mfxadr.xcel, 1, 2); //Av Bv

  } else {
    if (cv_n == temp_mfx_loco.mfxadr.xcel) {
      temp_mfx_loco.av = mfx_buffer[1];
      temp_mfx_loco.bv = mfx_buffer[2];

      readMfxConfig(last_mfx_call[2], temp_mfx_loco.mfxadr.speedtable, 1, 2); // Vmin Vamx
    } else if (cv_n == temp_mfx_loco.mfxadr.speedtable) {
      temp_mfx_loco.vmin = mfx_buffer[1];
      temp_mfx_loco.vmax = mfx_buffer[2];

      readMfxConfig(last_mfx_call[2], temp_mfx_loco.mfxadr.volume, 1, 1); // Lautstärke
    } else if (cv_n == temp_mfx_loco.mfxadr.volume) {
      temp_mfx_loco.volume = mfx_buffer[1];
      temp_mfx_loco.functions = [];
      temp_mfx_loco.functions_values = [];

      readMfxConfig(last_mfx_call[2], temp_mfx_loco.mfxadr.func, 1, 2) // Lokfunktionen
    } else {
      for (let i = 0; i < 16; i++) {
        if (cv_n == temp_mfx_loco.mfxadr.func + (i * 3)) {
          if (mfx_buffer[2]) {
            if (i == 0) temp_mfx_loco.functions[i] = 1;
            else temp_mfx_loco.functions[i] = 50 + i;
          } 
          if (i < 15) {
            readMfxConfig(last_mfx_call[2], temp_mfx_loco.mfxadr.func + ((i + 1) * 3), 1, 2);
          } else {
            let index;
            for (let i = 0; i < locolist.length; i++ ){
              if (last_mfx_call[2] == locolist[i].uid) {
                index = i;
                break;
              }
            }
            locolist[index] = temp_mfx_loco;
            
            fs.writeFile(locolist_path, JSON.stringify(locolist, null, 2), function(){
              console.log('Updating loco ' + temp_mfx_loco.name);
              let _name = temp_mfx_loco.name;

              setTimeout(()=> {
                exportCS2Locolist(locolist);
                updateLocolist()
                for (var i in clients){
                  clients[i].sendUTF(`foundMfx:${_name}`);
                }
              }, 500)
                
              temp_mfx_loco = {};
            }); 
          }
        }
      }
    }
  }
    
  

  mfx_buffer = [];
} 

function mfxDiscovery() {
  // mfx-Neuanmeldezähler erhöhen

  if (naz < 0xff) {
    naz++
  } else {
    naz = 1;
  }

  configUpdate(local_config);
  setNAZ();
}

function mfxDelete(sid) {
  // Beim löschen einer mfx-Lok den NAZ erhöhen

  mfxDiscovery();
  sendDatagram([0, MFX_VERIFY, 3, 0, 6, 0, 0, 0, 0, (sid & 0xFF00) >> 8, (sid & 0x00FF), 0, 0]);
}


//----------------------------------------------------------------------------------//
// Standard-CAN-Befehle:

function stop() { 
  // STOP an den GFP senden.

  sendDatagram([0x00, SYSTEM_CMD, 0x03, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, SYS_STOP, 0x00, 0x00, 0x00]);
  console.log('STOP')
}

function go() { 
  // Go an den GFP senden.

  sendDatagram([0x00, SYSTEM_CMD, 0x03, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, SYS_GO, 0x00, 0x00, 0x00]);
  console.log('GO');
  if (master) {
    bindKnownMfx();
    setNAZ();
  }
}

function ping() { 
  // Ping aussenden.  

  sendDatagram([0, PING,3,0,0,0,0,0,0,0,0,0,0]);
  if (master) sendDatagram([0, PING + 1,3,0,8,0,0,0,0,0,0,0xff,0xff])
  console.log('Ping sent.');
}

/*
function setAcc(uid, state, time) {
  console.log('Switching 0x' + parseInt(uid).toString(16) + ' to ' + state + ' with ontime of ' + time + 'ms.');
  sendDatagram([0, ACC_SWITCH, 3, 0, 6, 0, 0, uid >> 8, uid & 0xff, state, 1, 0, 0]);
  setTimeout(() => {
    sendDatagram([0, ACC_SWITCH, 3, 0, 6, 0, 0, uid >> 8, uid & 0xff, state, 0, 0, 0]);
  }, time);
}
*/


//----------------------------------------------------------------------------------//
// Websocket-Pakete:


//----------------------------------------------------------------------------------//
// Lokliste bearbeiten:

function addLocoFromMsg(msg_string) {
  // Manuell erstellte Lok der Liste hinzufügen

  let cmd = msg_string.split(":")[0]
  let new_loco = JSON.parse(msg_string.substring(msg_string.indexOf("$")+1, msg_string.indexOf("§")));
  new_loco.functions_values = [];

  if (cmd == "addLoco") {
    locolist[locolist.length] = new_loco;
  } else if (cmd == "updateLoco") {
    locolist[msg_string.split(':')[1]] = new_loco;
  }

  fs.writeFile(locolist_path, JSON.stringify(locolist, null, 2), () => {
    console.log("updated loco: " + new_loco.name);
    exportCS2Locolist(locolist);
    updateLocolist()
  });
}

function updateLocolist(){
  let _locolist = JSON.stringify(locolist);
  for (let i in clients){
    clients[i].sendUTF(`updateLocolist:${_locolist}`);
  }
}

function deleteLoco(index) {
  // Eine Lok aus der Liste löschen

  let uid = parseInt(locolist[index].uid);
  let name = locolist[index].name;
  let type = locolist[index].typ;

  locolist.splice(index, 1);
  fs.writeFile(locolist_path, JSON.stringify(locolist, null, 2), () => {
    console.log("Deleted " + name);
    setTimeout(() => {
      exportCS2Locolist(locolist);
      updateLocolist();
    }, 500);
  });

  sendDatagram([0, SYSTEM_CMD, 3, 0, 5, 0, 0, (uid & 0xFF00) >> 8, (uid & 0x00FF), SYS_LOCO_CYCLE_STOP, 0, 0, 0])

  if (type == "mfx") mfxDelete(uid - 0x4000);

}

function clearDeviceList() {
  // CAN-Geräteliste vollständig leeren

  devices = [];

  fs.writeFile(devices_path, JSON.stringify(devices, null, 2), function(){
    console.log("clearing devices");
  });
}
/*
function updateKeyboard() {
  for (var i in clients){
    clients[i].sendUTF(`updateKeyboard:${JSON.stringify(accessories)}`);
  }
}

function updateAccessory(uid, state) {
  for (var i in clients){
    clients[i].sendUTF(`updateAccessory:${uid}:${state}`);
  }
}
*/
/*
function updateaccessoriestate(uid, state) {
  let protocol = '';
  let adrs;
  if (uid >= 0x3000 && uid <= 0x33ff) {
    //Motorola
    protocol = 'mm2';
    adrs = uid + 1 - 0x3000;
  } else if (uid >= 0x3800 && uid <= 0x3bff) {
    //DCC
    protocol = 'dcc';
    adrs = uid + 1 - 0x3800;
  }

  if (protocol != '') {
    for (let i = 0; i < accessories.length; i++) {
      if (accessories[i].protocol == protocol && accessories[i].adrs == adrs && accessories[i].state != state) {
        accessories[i].state = state;
        fs.writeFile(accessories_path, JSON.stringify(accessories, null, 2), () => {updateKeyboard();})
        break;
      }
    }
  } 
}

function addAccessory(accessory){
  accessories[accessories.length] = accessory;
  fs.writeFile(accessories_path, JSON.stringify(accessories, null, 2), () => {
    updateKeyboard();
    console.log("Adding new Accessory");
  });
}
*/


//----------------------------------------------------------------------------------//
// Magnetartikelliste bearbeiten:
/*
  Magnetartikelstruktur:
    {
      "name": "Name_String",
      "uid": 0x1234,
      "protocoll": 0, // 0: DCC, 1: MM, 3: MäCAN
      "type": 0 //WIP
    }
*/
function addArticle(msg_string){
  // Artikel hinzufügen:

  let accessorie = JSON.parse(msg_string)

  accessories[accessories.lenght] = accessorie;

  fs.writeFile(accessories_path, JSON.stringify(accessories, null, 2), () => {
      console.log("Added Accessorie " + accessorie.name);
    });
}

//----------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------//

//clearDeviceList();


udpServer.on('listening', () => {
  var address = udpServer.address();
  console.log(`UDP-Server listening on Port ${address.port}.`);
  stop();
});

udpServer.bind(l_port);
ping();

if (master) {
  setProtocol(local_config.protocol);
}

server.listen(8080, function() {
    console.log('WebSocet-Server is listening on port 8080');
});

var WebSocketServer = require('websocket').server;
var wsServer = new WebSocketServer({
    httpServer: server
});

//WebSocket-Clients aufnehmen:
wsServer.on('request', function(request){
	var connection = request.accept('maecan', request.origin);
	var id = client_count++;
	clients[id] = connection;
	console.log(connection.remoteAddress + ' connected.');

	connection.send(power ? 'go' : 'stop');

	connection.on('close', function(reasonCode, description){
		delete clients[id];
		console.log(connection.remoteAddress + ' disconnected.');
	});


	// Befehle vom WebSocet-Client verarbeiten:
	connection.on('message', function(dgram){

		var msg = dgram.utf8Data.split(':');
		var cmd = msg[0];
		var uid = parseInt(msg[1]);
		var value = parseInt(msg[2]);

		var uid_high = (uid & 0xFF00) >> 8;
		var uid_low = (uid & 0x00FF);
		var value_high = (value & 0xFF00) >> 8;
		var value_low = (value & 0x00FF);

		if (cmd == 'stop') {
      stop();
      
		} else if (cmd == 'go') {
      go();
      
		} else if (cmd == 'setSpeed') {
      sendDatagram([0, LOCO_SPEED, 3, 0, 6, 0, 0, uid_high, uid_low, value_high, value_low, 0, 0]);
      
		} else if (cmd == 'getSpeed') {
			sendDatagram([0, LOCO_SPEED, 3, 0, 4, 0, 0, uid_high, uid_low, 0, 0, 0, 0]);
    
    } else if (cmd == 'lokNothalt') {
			sendDatagram([0, SYSTEM_CMD, 3, 0, 5, 0, 0, uid_high, uid_low, SYS_LOCO_EMERGENCY_STOP, 0, 0, 0]);
    
    } else if (cmd == 'setFn') {
			sendDatagram([0, LOCO_FN, 3, 0, 6, 0, 0, uid_high, uid_low, value_high, value_low, 0, 0]);
    
    } else if (cmd == 'getFn') {
			sendDatagram([0, LOCO_FN, 3, 0, 5, 0, 0, uid_high, uid_low, value, 0, 0, 0]);
    
    } else if (cmd == 'toggleDir') {
			sendDatagram([0, LOCO_DIR, 3, 0, 5, 0, 0, uid_high, uid_low, 3, 0, 0, 0]);
			sendDatagram([0, LOCO_DIR, 3, 0, 4, 0, 0, uid_high, uid_low, 0, 0, 0, 0]);
    
    } else if (cmd == 'getDir') {
			sendDatagram([0, LOCO_DIR, 3, 0, 4, 0, 0, uid_high, uid_low, 0, 0, 0, 0]);
    
    } else if (cmd == 'getStatus') {
      getStatus(msg[1], msg[2]);
    
    } else if (cmd == 'setProtocol') {
      setProtocol(msg[1]);
    
    } else if (cmd == 'getProtocol') {
      for (let i in clients){
        clients[i].sendUTF(`updateProtocol:${local_config.protocol}`);
      }
    
    } else if (cmd == 'restart') {
      stop();
      wsServer.shutDown();
      exec('reboot -f -d 1');
    
    } else if (cmd == 'shutdown') {
      stop();
      wsServer.shutDown();
      exec('poweroff -f -d 1');
    
    } else if (cmd == 'downloadIcon') {
      let link = "";
      for (let i = 1; i < msg.length; i++){
        if(i > 1) link += ':';
        link += msg[i];
      }
      exec('wget -c -P ' + local_config.server_path + '/html/loco_icons/ ' + link, console.log('Downloading ' + link));

    } else if (cmd == "addLoco" || cmd == "updateLoco") {
      addLocoFromMsg(dgram.utf8Data);
    
    } else if (cmd == "mfxDiscovery") {
      let last_power = power;
      stop();
      mfxDiscovery();
      if (last_power){
        setTimeout(function(){
          go();
        }, 500)
      }
    
    } else if (cmd == 'deleteLoco') {
      deleteLoco(msg[1]);
      
    } else if (cmd == 'getVersion') {
      for (let i in clients){
        clients[i].sendUTF(`updateVersion:${version}`);
      }
    
    } else if (cmd == 'setConfigValue') {
      sendDatagram([0x00, SYSTEM_CMD, 0x03, 0x00, 0x08, (msg[1] & 0xff000000)>> 24, (msg[1] & 0x00ff0000)>> 16, (msg[1] & 0x0000ff00) >> 8, msg[1] & 0x000000ff, SYS_STATUS, msg[2], (msg[3] & 0xff00) >> 8, msg[3] & 0x00ff]);
      
      for (let i = 0; i < devices.length; i++) {
        if (msg[1] == devices[i].uid) {
          if (devices[i].config_chanels_info[msg[2]-1].type == 1) {
            devices[i].config_chanels_info[msg[2]-1].def_option = msg[3];
          } else {
            devices[i].config_chanels_info[msg[2]-1].def_value = msg[3];
          }
          break;
        }
      }
  
      fs.writeFile(devices_path, JSON.stringify(devices, null, 2), function(){
        console.log("updating devices entry.")  });
      
    
    } else if (cmd == 'progCV') {
      writeCV(uid, msg[2], msg[3], msg[4]);

    } else if (cmd  == 'delIcon') {
      exec('rm ../html/loco_icons/' + msg[1]);

    } else if (cmd == 'delDevice') {
      for (let i = 0; i < devices.length; i++) {
        if (devices[i].uid == msg[1]) {
          devices.splice(i, 1);
          break;
        }
      }
      
      fs.writeFile(devices_path, JSON.stringify(devices, null, 2), function(){
        console.log("Deleting device " + msg[1]);   });

      ping();
    
    } else if (cmd == 'ping') {
      ping();

    } else if (cmd == 'setAcc') {
      setAcc(msg[1], msg[2], msg[3]);

    } else if (cmd == 'getDevicelist') {
      let _devices = JSON.stringify(devices)
      for (let i in clients){
        clients[i].sendUTF(`updateDevicelist:${_devices}`);
      }

    } else if (cmd == 'getKeyboard') {
      //updateKeyboard();

    } else if (cmd == 'addAccessorie') {
      //addArticle(dgram.utf8Data.replace('addAccessory:', ''));

    } else if (cmd == 'getLocolist') {
      updateLocolist();
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
  var value = (parseInt(udp_msg[9]) << 8) + parseInt(udp_msg[10]);

  let can_msg = 'can:' + cmd + ':' + hash + ':' + dlc + ':' + data;

  if (cmd == (SYSTEM_CMD + 1)) {
    var sub_cmd = parseInt(udp_msg[9]);
    
	if ((sub_cmd == SYS_STOP) || (sub_cmd == SYS_OVERLOAD)) {
  		ws_msg = 'stop';
  		power = false;
    
    } else if (sub_cmd == SYS_GO) {
      if (master) {
        bindKnownMfx();
        setNAZ();
      }
  		ws_msg = 'go';
  		power = true;
    
    } else if (sub_cmd == SYS_LOCO_EMERGENCY_STOP){
      ws_msg = `updateSpeed:${uid}:${0}`;
    
    } else if (sub_cmd == SYS_TRACK_PROTOCOL) {
      ws_msg = `updateProtocol:${data[5]}`;
      local_config.protocol = data[5];
      configUpdate(local_config);
    
    } else if (sub_cmd == SYS_STATUS) {
      ws_msg = `updateReading:${uid}:${data[5]}:${(data[6] << 8) + data[7]}`;
    }
  
  } else if (cmd == (LOCO_DISCOVERY + 1) && dlc == 5) {
    if (master) {
      addMfxLocoToList(uid);
      //ws_msg = `foundMfx:${uid}`;
    }

  } else if (cmd == LOCO_SPEED + 1) {
    if (dlc == 4) value = 0;
    ws_msg = `updateSpeed:${uid}:${value}`;
    for (let i = 0; i < locolist.length; i++) {
      if (locolist[i].uid == uid) {
        locolist[i].speed = value;
        break;
      }
    }
  
  } else if (cmd == (LOCO_FN + 1)) {
    ws_msg = `updateFn:${uid}:${value}`;
    let fn_number = value >> 8;
    let fn_value = value & 0xff;
    for (let i = 0; i < locolist.length; i++) {
      if (locolist[i].uid == uid) {
        locolist[i].functions_values[fn_number] = fn_value;
        break;
      }
    }
  
  } else if (cmd == (LOCO_DIR + 1)) {
    ws_msg = `updateDir:${uid}:${parseInt(udp_msg[9])}`;
    for (let i = 0; i < locolist.length; i++) {
      if (locolist[i].uid == uid) {
        locolist[i].dir = parseInt(udp_msg[9]);
        break;
      }
    }
 
  } else if (cmd == (READ_CONFIG + 1)) {
    if (master) fillMfxBuffer(data);
  
  } else if (cmd == (BOOTL_CAN + 1)) {
    if (master) {
      sendDatagram([0,0x36,3,0,5,data[0],data[1],data[2],data[3],0x11,0,0,0]);
      setTimeout(() => sendDatagram([0,0x30,3,0,0,0,0,0,0,0,0,0,0]), 20);
      setTimeout(() => {
        setNAZ();
        bindKnownMfx();
      }, 40);
    }
  
  } else if (cmd == (STATUS_CONFIG + 1)) {
    // Statusdaten Konfiguration

    if (dlc == 8) {
      if (hash == 0x301) {
        config_buffer = [];
      }
      var packet_index = hash - 0x300;
      config_buffer[packet_index] = data;
    }

    if (dlc == 6 || dlc == 5) {
      let device = devices[getDeviceIndexFromUID(uid, devices)];
      config_buffer[0] = data;
      let chanel = config_buffer[0][4];
      console.log('Done getting chanel ' + chanel + ' from device ' + device.name);
      
      if (chanel > 0 && chanel <= device.status_chanels) {
        let status_chanel = buildStatusChanelInfo(config_buffer);
        device.status_chanels_info[chanel - 1] = status_chanel;
        fs.writeFile(devices_path, JSON.stringify(devices, null, 2), function(){
            console.log("updating devices entry."); });
      
      } else if (chanel == 0) {
        device = buildDeviceInfo(config_buffer, device);
        fs.writeFile(devices_path, JSON.stringify(devices, null, 2), function(){
            console.log("updating devices entry."); });
      
      } else if (chanel > 0 && chanel <= device.config_chanels && chanel >= device.status_chanels){
        let config_chanel = buildConfigChanelInfo(config_buffer);
        device.config_chanels_info[chanel - 1] = config_chanel;
        fs.writeFile(devices_path, JSON.stringify(devices, null, 2), function(){
            console.log("updating devices entry.")  });
      }
      bussy_fetching = false;
    }
  
  } else if (cmd == (PING + 1)) {
  	let str_uid = toUnsignedString(uid);
  	let str_ver = `${data[4]}.${data[5]}`
  	let str_typ = ((data[6] << 8) + data[7]).toString(16);
  	console.log(`Ping response from 0x${str_uid}, Version ${str_ver}, Type 0x${str_typ}`);

  	if (str_typ =="10" && !gbox_uid) {
  		console.log(`Found Gleisbox 0x${str_uid}`);
  		gbox_uid = uid;
    }
    
    var device_exists = false;
    let existing_index = 0;
  
    for (i = 0; i < devices.length; i++) {
      if (devices[i].uid == uid || uid == 0) {
        device_exists = true;
        devices[i].pingResponse = true;
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
      devices[index].pingResponse = true;

      fs.writeFile(devices_path, JSON.stringify(devices, null, 2), function(){
        console.log("writing new devices entry.");
      });
    }

    for (let i = 0; i < devices.length; i++) {
      for (var j in clients){
        clients[j].sendUTF(`deviceConnected:${devices[i].uid}:${devices[i].pingResponse}`);
      }
    }
    
    

  } else if (cmd == DATA_QUERRY){
    let data_string = "";

    for (let i = 0; i < 8; i++) {
      data_string += hex2a(data[i].toString(16));
    }

    // LOKNAMEN
    if (data_string == 'loknamen'){
      loknamen_request = true;
    } else if (loknamen_request == true && data[3] == 0) {
      sendLocoNames(parseInt(hex2a(data[0].toString(16))), parseInt(hex2a(data[2].toString(16))), hash);
    }

    // LOKINFO
    if (data_string == 'lokinfo\x00'){
      lokinfo_request = 1;
    } else if (lokinfo_request == 1) {
      lokinfo_loconame = data_string;
      lokinfo_request = 2;
    } else if (lokinfo_request == 2) {
      lokinfo_loconame += data_string;
      sendLocoInfo(lokinfo_loconame, hash);
    }

  } else if (cmd == S88_EVENT) {
    let device_ken = (data[0] << 8) + data[1];
    let contact_ken = (data[2] << 8) + data[3];
    let state_old = data[4];
    let state_new = data[5];

    console.log('Contact no. ' + contact_ken + ' changed state from ' + state_old + ' to ' + state_new);
    ws_msg = `s88Event:${contact_ken}:${state_old}:${state_new}`;

  } else if (cmd == ACC_SWITCH + 1) {
    let uid = (data[2] << 8) + data[3];
    let state = data[4];
    //updateaccessoriestate(uid, state);
    //ws_msg = `updateAcc:${uid}:${state}`;
  }

  if (ws_msg != '') {
	  for (var i in clients){
	  	clients[i].sendUTF(ws_msg);
	  }
  }
});

//----------------------------------------------------------------------------------//
// Periodischer Code:

setInterval(() => {
  for (let i = 0; i < devices.length; i++) {
    devices[i].pingResponse = false;
  }
  ping();
}, 10000);
  // Alle 10 Sekunden einen Ping senden

if (master) { setInterval(function(){
  let gbox_found = false;
  for (let i = 0; i < devices.length; i++) {
    if (devices[i].type == 10) {
      gbox_found = true;
      break;
    }
  }
  if(!gbox_found) {
    sendDatagram([0,BOOTL_CAN,3,0,0,0,0,0,0,0,0,0,0]);
  }
}, 1000);   }

var data_fetcher = setInterval(function(){
  // Configdaten aus CAN-Geräten auslesen

  for (var i = 0; i < devices.length; i++) {
    if (!bussy_fetching) {
      if (!devices[i].name){
        if (devices[i].request_count < 10 || !devices[i].request_count) {
        }  
      } else if (devices[i].status_chanels && (!devices[i].status_chanels_info || devices[i].status_chanels_info.length < devices[i].status_chanels)) {
        if (!devices[i].status_chanels_info) {
          devices[i].status_chanels_info = [];        
        }
        console.log('need to get ' + (devices[i].status_chanels - devices[i].status_chanels_info.length) + ' status chanels of ' + devices[i].name);
        getDeviceInfo(devices[i].uid, devices[i].status_chanels_info.length + 1);
        break;
      } else if(devices[i].config_chanels && (!devices[i].config_chanels_info || devices[i].config_chanels_info.length < devices[i].config_chanels)) {
        if (!devices[i].config_chanels_info) {
          devices[i].config_chanels_info = [];
        }
        console.log('need to get ' + (devices[i].config_chanels - devices[i].config_chanels_info.length) + ' config chanels of ' + devices[i].name);
        getDeviceInfo(devices[i].uid, devices[i].status_chanels + (devices[i].config_chanels_info.length + 1));
        break;
      }
      if (!devices[i].name && !devices[i].device_info_request_attepted) {
        getDeviceInfo(devices[i].uid, 0);
        devices[i].device_info_request_attepted = true;
      }
    }
  }
}, 100);
