#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#include "RCSwitch.h"
#include "RFOutlet.h"
// For serializing access to hardware when called simultaneously
#include "shared_mutex.h"

#define DEFAULT_PIN 0
#define DEFAULT_PULSE_LENGTH 189
#define DEFAULT_HW_LOCK_TIMEOUT 2

shared_mutex_t mutex;

void printUsage(char *argv[]) {
    printf("Usage: %s <code> [-p <PIN Number> (default: %i)] [-l <Pulse Length> (default: %i)].\n", argv[0], DEFAULT_PIN, DEFAULT_PULSE_LENGTH);
}

void *interruptThread(void *signal) {
    printf("Shutting down...\n");
    // Give the locking mechanism some time and then exit
    sleep(DEFAULT_HW_LOCK_TIMEOUT + 1);
    exit(*(int *)signal);
}

void interruptHandler(int signal) {
    pthread_t thread;
    if (pthread_create(&thread, NULL, interruptThread, &signal) != 0) {
        exit(signal);
    }
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
        if (!parseStringToInt(argumentPIN, &PIN, 10)) {
            perror("Invalid PIN");
            return EXIT_FAILURE;            
        }
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
    int code = 0;

    if (!parseStringToInt(argv[0], &code, 10)) {
        perror("Invalid code");
        return EXIT_FAILURE;            
    }

    // Acquire the shared mutex and set a permissive mode since codesend may be executed by www-data and other terminal users
    mutex = shared_mutex_init("codesend_mutex", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    
    if (mutex.ptr == NULL) {
        return EXIT_FAILURE;
    }

    // Timeout for acquiring the lock, so we don't hang the process!
    struct timespec abs_time; 
    clock_gettime(CLOCK_REALTIME , &abs_time);
    abs_time.tv_sec += DEFAULT_HW_LOCK_TIMEOUT;

    if (pthread_mutex_timedlock(mutex.ptr, &abs_time) != 0) {
        const char * const fd_path = "/proc/self/fd";
        const char *shm_path = NULL;
        size_t needed = snprintf(NULL, 0, "%s/%d", fd_path, mutex.shm_fd) + 1;
        char *buffer = (char *)malloc(needed);
        if (snprintf(buffer, needed, "%s/%d", fd_path, mutex.shm_fd) >= 0) {
            shm_path = realpath(buffer, NULL);
        }
        free(buffer);

        if (shm_path == NULL) {
            shm_path = "/dev/shm";
        }

        fprintf(stderr, "Could not acquire an exclusive lock to activate the hardware, please try deleting the lock file at '%s' and try again\n", shm_path);
        return EXIT_FAILURE;
    }

    if (wiringPiSetup () == -1) {
        return EXIT_FAILURE;
    }

    // Pulse length depends on the RF outlets you are using. Use RFSniffer to see what pulse length your device uses.
    int pulseLength = DEFAULT_PULSE_LENGTH;

    if (argumentPulseLength != NULL) {
        if (!parseStringToInt(argumentPulseLength, &pulseLength, 10)) {
            perror("Invalid pulse length");
            return EXIT_FAILURE;            
        }
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
