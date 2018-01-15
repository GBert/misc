var fs = require('fs');
var ini = require('ini');

var config = ini.parse(fs.readFileSync('./geraete.ini', 'utf-8'));

for (var key in config){
	console.log(key);
}