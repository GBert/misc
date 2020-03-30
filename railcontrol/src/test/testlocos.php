#!/usr/bin/php
<?php

include('functions.php');

$socket = startrailcontrol();

// list locos
socket_write($socket, "lla\n");
$data = socket_read($socket, 128);
$numLocos = substr_count($data, "\n") - 1;
if ($numLocos != 0) {
	stoprailcontrol($socket, "Number of locos at start is not 0");
}

socket_write($socket, "al \"New Name\" 1 2 3\n");
$data = explode("\n", socket_read($socket, 128));
if ($data[0] != 'Loco "New Name" added') {
	stoprailcontrol($socket, "Unable to add loco: $data[0]");
}

stoprailcontrol($socket);

?>
