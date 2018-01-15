var throttle_slider = $('input[type="range"]');
var throttle_slider_handle;

var throttle_sliding = false;
var speedupdate = false;
var direct_adress = true;
var list_visible = false;

var timer;
var throttle_watcher;
var throttle_manager;
var old_speed;

//Wichtige Elemente:
var stop_button_1     = document.getElementById('stop_button_1');
var go_button_1       = document.getElementById('go_button_1');
var function_buttons  = document.getElementsByClassName('function');
var change_direction  = document.getElementById('change_direction');
var confirm_adress    = document.getElementById('confirm_adress');
var adress_input      = document.getElementById('adress_input');
var motorola          = document.getElementById('radio01');
var dcc               = document.getElementById('radio02');
var rangeslider01     = document.getElementById('rangeslider01');
var locoselector_list = document.getElementById('locoselector_list');
var locoselector      = document.getElementById('locoselector');

//Standard LocID:
var locid = 0;
var vmax = 100;
var icon = '';
var names = [];
var uids = [];
var vmaxes = [];
var icons = [];
var lokliste = [];
var str_lokliste;
var fn_state = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0];

document.getElementsByClassName('toggle_lokliste')[0].onclick = function(){
   toggleLokliste();
   return false;
 }
document.getElementsByClassName('toggle_lokliste')[1].onclick = function(){
  toggleLokliste();
  return false;
}


function toggleLokliste(){
  if (direct_adress) {
    document.getElementById('directadress').style.display = 'none';
    document.getElementById('lokliste').style.display = 'block';
    direct_adress = false;
  }else{
    document.getElementById('directadress').style.display = 'block';
    document.getElementById('lokliste').style.display = 'none';
    direct_adress = true;
  }
};

throttle_slider.rangeslider({
  polyfill: false
});

throttle_slider.on('input', function(){

  // �berpr�ft, ob der Input vom Nutzer oder durch ein regelm��iges Geschwindikeitsupdate ausgel�st wurde
  if (speedupdate){ 
    speedupdate = false;    
  }else{
    // �berpr�ft, ob erster Input und startet entsprechend Throttlemanager
    if (!throttle_sliding) {
      throttle_manager = setInterval(function(){setSpeed(rangeslider01.value);}, 50);
    }

    throttle_sliding = true;
    throttle_slider_handle.textContent = Math.ceil((this.value / 10)*(vmax / 100)) + ' km/h';

    clearTimeout(timer);
    clearInterval(throttle_watcher);

    timer = setTimeout(function(){
      throttle_sliding = false;
      clearInterval(throttle_manager);
      getSpeed();
      console.log('restarting');
    },500);

  }
});

throttle_slider_handle = document.getElementById('js-rangeslider-0').childNodes[1];
throttle_slider_handle.textContent = Math.ceil((throttle_slider[0].value / 10)*(vmax / 100)) + ' km/h';


// allgemeine Funktionen:

// Status der Stop und Go Tasten abrufen.
function getButtonStatus(power){
    if (power == 'stop'){
      stop_button_1.style.backgroundColor = cs_red;
      go_button_1.style.backgroundColor = 'white';
      stop_button_1.style.color = 'white';
      go_button_1.style.color = cs_green;
    }else if (power == 'go'){
      stop_button_1.style.backgroundColor = 'white';
      go_button_1.style.backgroundColor = cs_green;
      stop_button_1.style.color = cs_red;
      go_button_1.style.color = 'white';
    }
}

// Geschwindigkeit der aktuellen LocID anfordern:
function updateSpeed(value){ 
    if (value == 'error') {
      console.log('speederror');
    }else if (value != rangeslider01.value && !throttle_sliding) {
      speedupdate = true;
      old_speed = value;
      throttle_slider.val(value).change();
      throttle_slider_handle.textContent = Math.ceil((value / 10)*(vmax / 100)) + ' km/h';
    }
}

function getSpeed(){
  var msg = `getSpeed:${locid}`;
  send(msg);
}

//Geschwindigkeit der aktuellen LocID setzen:
function setSpeed(value){
  if (value != old_speed) {
    var msg = `setSpeed:${locid}:${value}`;
    send(msg);
    old_speed = value;
  }
}

function lokNothalt(){
  var msg = `lokNothalt:${locid}`;
  send(msg);
}

function updateFn(value){
  var fn_num = parseInt(value) >> 8;
  var fn_value = parseInt(value) & 0x00ff;
  fn_state[fn_num] = fn_value;
  if (fn_value) {
    function_buttons[fn_num].style.borderColor = '#ff9800';
    function_buttons[fn_num].style.color = '#ff9800';
  }else{
    function_buttons[fn_num].style.borderColor = '#adadad';
    function_buttons[fn_num].style.color = 'black';
  }
}

function setFn(fn_num, fn_value){
  var value = (fn_num << 8) + fn_value;
  var msg = `setFn:${locid}:${value}`;
  send(msg);
}

function getFn(fn_num){
  var msg = `getFn:${locid}:${fn_num}`;
  send(msg);
}

function updateDir(value){
  var left = document.getElementById('left');
  var right = document.getElementById('right');
  if (value == 1) {
    left.style.display = 'none';
    right.style.display = 'inline-block';
  } else if (value == 2) {
    left.style.display = 'inline-block';
    right.style.display = 'none';
  }
}

function toggleDir() {
  var msg = `toggleDir:${locid}`;
  send(msg);
}

function getDir() {
  var msg = `getDir:${locid}`;
  send(msg);
}

function updateLokInfo(){
  getSpeed();
  getDir();
  for (var i = 0; i < function_buttons.length; i++) {
    getFn(i);
  }
}

function getLokliste(){
  $.get("php/lokliste.php",{}, function(data){
    //console.log(data);
    str_lokliste = data;
    lokliste = str_lokliste.split("§");
    for (var i = lokliste.length - 1; i >= 0; i--) {
      names[i] = lokliste[i].split("$")[0];
      uids[i] = lokliste[i].split("$")[1];
      vmaxes[i] = lokliste[i].split("$")[2];
      icons[i] = lokliste[i].split('$')[3];
    }
  });
}

function createDropdown(){
  for ( var i = 0; i < lokliste.length; i++) (function(i){
    var obj = document.createElement('a');
    var obj_img = document.createElement('img');
    var obj_text = document.createTextNode(names[i]);
    var obj_div = document.createElement('div');
    obj.setAttribute('href','');
    obj_img.setAttribute('class', 'lokimg');
    obj_img.setAttribute('src', './icons/' + icons[i] + '.png');
    obj_div.setAttribute('class', 'dropdown_point');
    if (true) { obj_div.appendChild(obj_img); }
    obj_div.appendChild(obj_text);
    obj.appendChild(obj_div);
    locoselector_list.appendChild(obj);

    obj.onclick = function(){
      locid = parseInt(uids[i], 16);
      if (vmaxes[i] != ''){
        vmax = parseInt(vmaxes[i]);
      } else {
        vmax = 100;
      }
      icon = icons[i];
      updateLokInfo();
      locoselector.childNodes[0].setAttribute('src', './icons/' + icons[i] + '.png');
      locoselector.childNodes[1].textContent = names[i];
      hideDropdown();
      return false;
    };
  })(i);
}

function hideDropdown(){
  var listlength = locoselector_list.childNodes.length;
  for (var i = 0; i < listlength ; i++){
    locoselector_list.removeChild(locoselector_list.childNodes[0]);
  }
  list_visible = false;
}

//Intervallfuntionen:
/*
var button_watcher = setInterval(function(){
  getButtonStatus();
}, 1000);
console.log('button_watcher gestartet...');


throttle_watcher = setInterval(function() {
  getSpeed();
}, 1000);
console.log('throttle_watcher gestartet...');
*/

//onClick-Events f�r Funktionstasten:
for (var i = 0; i < function_buttons.length; i++) (function(i){
  function_buttons[i].onclick = function(){
    setFn(i, !fn_state[i]);
    return false;
  };
})(i);

confirm_adress.onclick = function(){
  if (motorola.checked) {
    locid = parseInt(adress_input.value, 10);
  }else if (dcc.checked) {
    locid = 0xc000 + parseInt(adress_input.value, 10);
  }
  vmax = 100;
  updateLokInfo();
  return false;
};

locoselector.onclick = function(){
  if (!list_visible) {
    getLokliste();
    var timer = setTimeout(function(){
      createDropdown();
    },100);
    list_visible = true
  }else{
    hideDropdown();
  }

  return false;

};

stop_button_1.onclick = function(){
  ws.send('stop');
  console.log('Sending: stop.');
  return false;
};

go_button_1.onclick = function(){
  ws.send('go');
  console.log('Sending: go.');
  return false;
};

change_direction.onclick = function(){
  lokNothalt();
  toggleDir();
  return false;
}


ws.onmessage = function(dgram){
  var msg = dgram.data.toString().split(':');
  var cmd = msg[0];
  var uid = parseInt(msg[1]);
  var value = parseInt(msg[2]);
  console.log(`Recieving: ${dgram.data.toString()}.`);

  if (cmd == 'stop' || cmd == 'go') {
    getButtonStatus(cmd);
  } else if (uid == locid) {
    if (cmd == 'updateSpeed') {
      updateSpeed(value);
    } else if (cmd == 'updateFn') {
      updateFn(value);
    } else if (cmd == 'updateDir') {
      updateDir(value);
    }
  }
};
