<?php 
//  ini_set('display_errors',1); error_reporting(E_ALL);
$outletID = (!empty($_POST['outletId'])) ? $_POST['outletId'] : $_GET['outletId'];

// Check that outletID is not empty or all

$status_contents = file_get_contents('status.json');

if ($status_contents === FALSE) {
    die();
}

$statuses = json_decode($status_contents, true);

echo((string)$statuses[$outletID]);
