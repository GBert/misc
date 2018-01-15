<?php
foreach ($_FILES['icon']['error'] as $key => $error) {
    if ($error == UPLOAD_ERR_OK) {
        move_uploaded_file($_FILES['icon']['tmp_name'][$key], '/www/MaeCAN-Server/html/loco_icons/'.$_FILES['icon']['name'][$key]);
    }
}
?>