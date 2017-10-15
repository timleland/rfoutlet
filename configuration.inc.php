<?php
// Edit these codes for each outlet
$codes = array(
    "1" => array(
        "on" => 349491,
        "off" => 349500
    ),
    "2" => array(
        "on" => 349635,
        "off" => 349644
    ),
    "3" => array(
        "on" => 349955,
        "off" => 349964
    ),
    "4" => array(
        "on" => 351491,
        "off" => 351500
    ),
    "5" => array(
        "on" => 357635,
        "off" => 357644
    ),
);

// Path to the codesend binary (current directory is the default)
$codeSendPath = './codesend';

// This PIN is not the first PIN on the Raspberry Pi GPIO header!
// Consult https://projects.drogon.net/raspberry-pi/wiringpi/pins/
// for more information.
$codeSendPIN = "0";

// Pulse length depends on the RF outlets you are using. Use RFSniffer to see what pulse length your device uses.
$codeSendPulseLength = "189";
