<?php
header('Content-Type: application/json');
header('Cache-Control: no-cache, must-revalidate');

// Please edit this file with your outlet codes and pulse length
require_once('configuration.inc.php');

if (!file_exists($codeSendPath)) {
    error_log("$codeSendPath is missing, please edit the script", 0);
    die(json_encode(array('success' => false)));
}

$outletToToggle = (!empty($_POST['outletId'])) ? $_POST['outletId'] : $_GET['outletId'];
$outletStatus = (!empty($_POST['outletStatus'])) ? $_POST['outletStatus'] : $_GET['outletStatus']; 

if (empty($outletToToggle) || empty($outletStatus)) {
    error_log('Missing POST paremeters', 0);
    die(json_encode(array('success' => false)));
}

// Add check to see if outlet to toggle is in our codes or not

if ($outletToToggle == "all") {
    // all is every outlets combined
    if (function_exists('array_column')) {
        // PHP >= 5.5
        $codesToToggle = array_column($codes, $outletStatus);
    } else {
        $codesToToggle = array();
        foreach ($codes as $outletCodes) {
            array_push($codesToToggle, $outletCodes[$outletStatus]);
        }
    }
} else {
    // One
    $codesToToggle = array($codes[$outletToToggle][$outletStatus]);
}

$returnCode = 0;
$output = NULL;
foreach ($codesToToggle as $codeSendCode) {
    exec($codeSendPath . ' ' . $codeSendCode . ' -p ' . $codeSendPIN . ' -l ' . $codeSendPulseLength, $output, $returnCode);
    if ($returnCode != 0) {
        error_log("Failed to execute $codeSendPath. Output was: " . implode(", ", $output) . ". Code $returnCode");
        die(json_encode(array('success' => false, 'output' => $output)));
    }
}

$status_contents = file_get_contents('status.json');

$statuses = NULL;

if ($status_contents === FALSE || $outletToToggle == "all") {
    $defaultStatus = 0;
    if ($outletToToggle == "all") {
        $defaultStatus = $outletStatus == "on" ? 1 : 0;
    }

    $statuses = array();

    foreach ($codes as $outletNumber=>$information) {
        $statuses[$outletNumber] = $defaultStatus;
    }
} else {
    $statuses = json_decode($status_contents, true);
}

if ($outletToToggle != "all") {
    $statuses[$outletToToggle] = $outletStatus == "on" ? 1 : 0;
}

$JSON = json_encode($statuses, JSON_PRETTY_PRINT);

if (file_put_contents('status.json', $JSON) === FALSE) {
    die(json_encode(array('success' => false, 'output' => "file_put_contents")));
}

die(json_encode(array('success' => true)));
