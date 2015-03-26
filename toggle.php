<?php
$rfPath = '/var/www/rfoutlet/codesend ';
$outletLight = $_POST['outletId'];
$outletStatus = $_POST['outletStatus'];

if ($outletLight == "1" && $outletStatus == "on") {
    $rfCodes = array(1070387);
} else if ($outletLight == "1" && $outletStatus == "off") {
    $rfCodes = array(1070396);
} else if ($outletLight == "2" && $outletStatus == "on") {
    $rfCodes = array(1070531);
} else if ($outletLight == "2" && $outletStatus == "off") {
    $rfCodes = array(1070540);
} else if ($outletLight == "3" && $outletStatus == "on") {
    $rfCodes = array(1070851);
} else if ($outletLight == "3" && $outletStatus == "off") {
    $rfCodes = array(1070860);
} else if ($outletLight == "4" && $outletStatus == "on") {
    $rfCodes = array(1072387);
} else if ($outletLight == "4" && $outletStatus == "off") {
    $rfCodes = array(1072396);
} else if ($outletLight == "5" && $outletStatus == "on") {
    $rfCodes = array(1078531);
} else if ($outletLight == "5" && $outletStatus == "off") {
    $rfCodes = array(1078540);
} else if ($outletLight == "6" && $outletStatus == "on") {
    $rfCodes = array(1070387, 1070531, 1070851, 1072387, 1078531);
} else if ($outletLight == "6" && $outletStatus == "off") {
        $rfCodes = array(1070396, 1070540, 1070860, 1072396, 1078540);
}


foreach ($rfCodes as $rfCode) {
        shell_exec($rfPath . $rfCode);
        sleep(.5);
}

echo json_encode(array('success' => true));
?>
