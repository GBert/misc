var is_fullscreen = false;

//Wichtige Elemente:
var html              = document.getElementById('root');
var content		        = document.getElementById('body');
var options           = document.getElementById('options');
var iframe_1          = document.getElementById('iframe_1');
var selector          = document.getElementById('selector');

//regelm��ig genutzte Farben:
var cs_red = '#ff0025';
var cs_green = '#2fa938';


/*function setRem(){
  var rem = parseInt((window.innerWidth / 360) * 10) + 'px';
  console.log(rem);

  //html.style.fontSize = rem;
}*/

function enterFullscreen(element) {
  if(element.requestFullscreen) {
    element.requestFullscreen();
  } else if(element.mozRequestFullScreen) {
    element.mozRequestFullScreen();
  } else if(element.msRequestFullscreen) {
    element.msRequestFullscreen();
  } else if(element.webkitRequestFullscreen) {
    element.webkitRequestFullscreen();
  }
  is_fullscreen = true;
}

function exitFullscreen() {
  if(document.exitFullscreen) {
    document.exitFullscreen();
  } else if(document.mozCancelFullScreen) {
    document.mozCancelFullScreen();
  } else if(document.webkitExitFullscreen) {
    document.webkitExitFullscreen();
  }
  is_fullscreen = false;
}

function toggleFullscreen(){
  if (is_fullscreen) {
    exitFullscreen();
  } else {
    enterFullscreen(document.documentElement);
  }
}

function hideIframe(id_iframe, id_selector){
  document.getElementById(id_iframe).style.display = 'none';
  document.getElementById(id_selector).style.display = 'block';
}

function showIframe(id_iframe, id_selector){
  document.getElementById(id_iframe).style.display = 'block';
  document.getElementById(id_selector).style.display = 'none';
}

function show(element) {
  element.style.display = 'block';
}

function hide(element) {
  element.style.display = 'none';
}

Element.prototype.remove = function() {
  this.parentElement.removeChild(this);
}