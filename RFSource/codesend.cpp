#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "RCSwitch.h"

// For serializing access to hardware when called simultaneously
#include <signal.h>
#include <sys/stat.h>
#include "shared_mutex.h"

#define DEFAULT_PIN 0
#define DEFAULT_PULSE_LENGTH 189

shared_mutex_t mutex;

void printUsage(char *argv[]) {
    printf("Usage: %s <code> [-p <PIN Number> (default: %i)] [-l <Pulse Length> (default: %i)].\n", argv[0], DEFAULT_PIN, DEFAULT_PULSE_LENGTH);
}

void interruptHandler(int signal) {
    // None of the pthread functions are async signal safe
    printf("Please wait until proper shut down.\n");
}

int main(int argc, char *argv[]) {
    signal(SIGINT, interruptHandler);
    signal(SIGHUP, interruptHandler);
    signal(SIGQUIT, interruptHandler);
    signal(SIGABRT, interruptHandler);

    int i;

    char * argumentPIN = NULL;
    char * argumentPulseLength = NULL;

    int c;
    while ((c = getopt(argc, argv, "p:l:")) != -1) {
        switch (c) {
            case 'p':
                argumentPIN = optarg;
                break;

            case 'l':
                argumentPulseLength = optarg;
                break;

            case '?':
            default:
                printUsage(argv);
                break;
            }
    }


    // This PIN is not the first PIN on the Raspberry Pi GPIO header!
    // Consult https://projects.drogon.net/raspberry-pi/wiringpi/pins/
    // for more information.
    int PIN = DEFAULT_PIN;

    if (argumentPIN != NULL) {
        PIN = atoi(argumentPIN);
    }

    /* Now set the values of "argc" and "argv" to the values after the
       options have been processed, above. */
    argc -= optind;

    if (argc == 0) {
        printUsage(argv);
        return EXIT_FAILURE;
    }

    argv += optind;

    // Parse the first parameter to this command as an integer
    int code = atoi(argv[0]);

    // Acquire the shared mutex
    // Set a permissive mode since codesend may be executed by www-data and other terminal users. Make the shared memory wide open
    mutex = shared_mutex_init("codesend_mutex", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if (mutex.ptr == NULL) {
        return EXIT_FAILURE;
    }

    // if (!mutex.created) {
    //    printf("A previous codesend instance has already acquired a lock. Waiting!\n");
    // }

    pthread_mutex_lock(mutex.ptr);

    if (wiringPiSetup () == -1) {
        return EXIT_FAILURE;
    }

    // Pulse length depends on the RF outlets you are using. Use RFSniffer to see what pulse length your device uses.
    int pulseLength = DEFAULT_PULSE_LENGTH;

    if (argumentPulseLength != NULL) {
        pulseLength = atoi(argumentPulseLength);
    }

    printf("Sending Code: %i. PIN: %i. Pulse Length: %i\n", code, PIN, pulseLength);

    RCSwitch mySwitch = RCSwitch();
    mySwitch.setPulseLength(pulseLength);
    mySwitch.enableTransmit(PIN);
    mySwitch.send(code, 24);
    mySwitch.disableTransmit();

    pthread_mutex_unlock(mutex.ptr);

    if (shared_mutex_close(mutex)) {
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
