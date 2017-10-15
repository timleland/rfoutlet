<?php
header('Content-Type: application/json');
header('Cache-Control: no-cache, must-revalidate');

// Please edit this file with your outlet codes and pulse length
require_once('configuration.inc.php');

if (!file_exists($codeSendPath)) {
    error_log("$codeSendPath is missing, please edit the script", 0);
    die(json_encode(array('success' => false)));
}

$outletLight = (!empty($_POST['outletId'])) ? $_POST['outletId'] : $_GET['outletId'];
$outletStatus = (!empty($_POST['outletStatus'])) ? $_POST['outletStatus'] : $_GET['outletStatus']; 

if (empty($outletLight) || empty($outletStatus)) {
    error_log('Missing POST paremeters', 0);
    die(json_encode(array('success' => false)));
}

if ($outletLight == "6") {
    // 6 is all 5 outlets combined
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
    $codesToToggle = array($codes[$outletLight][$outletStatus]);
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

die(json_encode(array('success' => true)));
