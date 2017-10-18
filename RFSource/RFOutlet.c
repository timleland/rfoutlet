#include <errno.h>
#include <limits.h>
#include <stdlib.h>

bool parseStringToInt(const char *input, int *value, int base) {
    bool success = true;
    char *endptr;
    errno = 0;
    long longNumber = strtol(input, &endptr, base);

    if (longNumber < INT_MIN || INT_MAX < longNumber) {
        errno = ERANGE;
    }

    if (*endptr == longNumber || *endptr != '\0') {
        errno = EINVAL;
    }

    if (errno != 0) {
        success = false;
    } else {
        *value = longNumber;
    }

    return success;
}    
