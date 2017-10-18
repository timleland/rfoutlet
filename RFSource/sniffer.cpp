#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#include "RFOutlet.h"
#include "RCSwitch.h"

#define DEFAULT_PIN 2

void printUsage(char *argv[]) {
    printf("Usage: %s [-p <PIN Number> (default: %i)].\n", argv[0], DEFAULT_PIN);
}

int main(int argc, char *argv[]) {
    char *argumentPIN = NULL;
    
    int c;
    while ((c = getopt(argc, argv, "p:")) != -1) {
        switch (c) {
            case 'p':
                argumentPIN = optarg;
                break;
                
            case '?':
            default:
                printUsage(argv);
                exit(EXIT_FAILURE);
                break;
        }
    }
    
    // This PIN is not the first PIN on the Raspberry Pi GPIO header!
    // Consult https://projects.drogon.net/raspberry-pi/wiringpi/pins/
    // for more information.
    int PIN = DEFAULT_PIN;
    
    if (argumentPIN != NULL) {
        if (!parseStringToInt(argumentPIN, &PIN, 10)) {
            perror("Invalid PIN");
            return EXIT_FAILURE;            
        }
        printf("Listening on PIN: %i\n", PIN);
    } else {
        printUsage(argv);
        return EXIT_FAILURE;
    }        
    
    if (wiringPiSetup() == -1) {
        return EXIT_FAILURE;
    }
    
    RCSwitch mySwitch = RCSwitch();
    mySwitch.enableReceive(PIN);  // Receiver on INTERRUPT 0 => that is pin #2
    
    while(true) {
        if (mySwitch.available()) {
            int value = mySwitch.getReceivedValue();
            
            if (value == 0) {
                printf("Unknown encoding");
            } else {
                printf("Received %i\n", mySwitch.getReceivedValue() );
                // Show pulse(Depends on your RF outlet device. You may need to change the pulse on codesend.cpp)
                printf("Received pulse %i\n", mySwitch.getReceivedDelay() );
            }
            
            mySwitch.resetAvailable();
        }
    }
    
    return EXIT_SUCCESS;
}
