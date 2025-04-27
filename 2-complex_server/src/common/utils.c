#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "utils.h"

struct tm get_timestamp() {
    time_t now = time(NULL);
    struct tm time_now = *localtime(&now);
    return time_now;
}