#include "RCSwitch.h"
#include <stdlib.h>
#include <stdio.h>


RCSwitch mySwitch;



int main(int argc, char *argv[]) {

     // This pin is not the first pin on the RPi GPIO header!
     // Consult https://projects.drogon.net/raspberry-pi/wiringpi/pins/
     // for more information.
     int PIN = 2;

     if(wiringPiSetup() == -1)
       return 0;

     mySwitch = RCSwitch();
     mySwitch.enableReceive(PIN);  // Receiver on inerrupt 0 => that is pin #2


     while(1) {

      if (mySwitch.available()) {

        int value = mySwitch.getReceivedValue();

        if (value == 0) {
             printf("Unknown encoding");
        } else {
             printf("Received %i\n", mySwitch.getReceivedValue() );
             //Show pulse(Depends on your RF outlet device. You may need to change the pulse on codesend.cpp)
	       printf("Received pulse %i\n", mySwitch.getReceivedDelay() );
        }

        mySwitch.resetAvailable();

      }


  }

  exit(0);


}
