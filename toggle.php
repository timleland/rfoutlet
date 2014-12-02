<?php

    $outletLight = $_POST['outletId'];
    $outletStatus = $_POST['outletStatus'];

    if($outletLight == "1" && $outletStatus == "on"){
        $rfCode = 349491;
    }else if($outletLight == "1" && $outletStatus == "off"){
        $rfCode = 349500;
    }else if($outletLight == "2" && $outletStatus == "on"){
        $rfCode = 349635;
    }else if($outletLight == "2" && $outletStatus == "off"){
        $rfCode = 349644;
    }else if($outletLight == "3" && $outletStatus == "on"){
        $rfCode = 349955;
    }else if($outletLight == "3" && $outletStatus == "off"){
        $rfCode = 349964;
    }else if($outletLight == "4" && $outletStatus == "on"){
        $rfCode = 351491;
    }else if($outletLight == "4" && $outletStatus == "off"){
        $rfCode = 351500;
    }else if($outletLight == "5" && $outletStatus == "on"){
        $rfCode = 357635;
    }else if($outletLight == "5" && $outletStatus == "off"){
        $rfCode = 357644;
    }

    $rfPath = '/home/pi/www/rfoutlet/codesend ' . $rfCode;

    shell_exec($rfPath);

    echo json_encode(['success' => true, 'rfPath' => $rfPath]);

?>
