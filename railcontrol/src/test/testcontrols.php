#!/usr/bin/php
<?php

include('functions.php');

$socket = startrailcontrol();

// list controls
socket_write($socket, "cla\n");
$data = socket_read($socket, 128);
$numControls = substr_count($data, "\n") - 1;
if ($numControls != 0) {
	stoprailcontrol($socket, "Number of controls at start is not 0");
}

// add virtual control
socket_write($socket, "ac \"Virt\" virt \"\"\n");
$data = explode("\n", socket_read($socket, 128));
if ($data[0] != 'Control "Virt" added') {
	stoprailcontrol($socket, "Unable to add control: $data[0]");
}

// add cs2 control
socket_write($socket, "ac \"First CS2\" virt \"\"\n");
$data = explode("\n", socket_read($socket, 128));
if ($data[0] != 'Control "First CS2" added') {
	stoprailcontrol($socket, "Unable to add control: $data[0]");
}

// add second cs2
socket_write($socket, "ac \"Second CS2\" virt \"\"\n");
$data = explode("\n", socket_read($socket, 128));
if ($data[0] != 'Control "Second CS2" added') {
	stoprailcontrol($socket, "Unable to add control: $data[0]");
}

// list all controls
socket_write($socket, "cla\n");
$data = explode("\n", socket_read($socket, 128));
$numControls = substr_count($data, "\n") - 1;
if ($numControls != 3) {
	stoprailcontrol($socket, "Number of controls at start is not 0");
}

// delete second cs2
socket_write($socket, "dc 3\n");
$data = explode("\n", socket_read($socket, 128));
if ($data[0] != 'Control "Second CS2" deleted') {
	stoprailcontrol($socket, "Unable to add control: $data[0]");
}

// delete first cs2
socket_write($socket, "dc 2\n");
$data = explode("\n", socket_read($socket, 128));
if ($data[0] != 'Control "First CS2" deleted') {
	stoprailcontrol($socket, "Unable to add control: $data[0]");
}

// delete virt
socket_write($socket, "dc 1\n");
$data = explode("\n", socket_read($socket, 128));
if ($data[0] != 'Control "Virt" deleted') {
	stoprailcontrol($socket, "Unable to add control: $data[0]");
}



stoprailcontrol($socket);

?>
