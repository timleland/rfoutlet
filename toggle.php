<?php

    $lightId = Input::get('lightId');
    $lightStatus = Input::get('lightStatus');

    if($lightId == "1" && $lightStatus == "on"){
        $rfCode = 349491;
    }else if($lightId == "1" && $lightStatus == "off"){
        $rfCode = 349500;
    }else if($lightId == "2" && $lightStatus == "on"){
        $rfCode = 349635;
    }else if($lightId == "2" && $lightStatus == "off"){
        $rfCode = 349644;
    }else if($lightId == "3" && $lightStatus == "on"){
        $rfCode = 349955;
    }else if($lightId == "3" && $lightStatus == "off"){
        $rfCode = 349964;
    }else if($lightId == "4" && $lightStatus == "on"){
        $rfCode = 351491;
    }else if($lightId == "4" && $lightStatus == "off"){
        $rfCode = 351500;
    }else if($lightId == "5" && $lightStatus == "on"){
        $rfCode = 357635;
    }else if($lightId == "5" && $lightStatus == "off"){
        $rfCode = 357644;
    }

    $rfPath = '/home/pi/www/outlet/codesend ' . $rfCode;

    shell_exec($rfPath);

    echo json_encode(['success' => true, 'rfPath' => $rfPath]);
