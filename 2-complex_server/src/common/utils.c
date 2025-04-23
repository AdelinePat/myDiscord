#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <libpq-fe.h>

#include "utils.h"

char *get_timestamp() {
    char *buffer = malloc(16);
    if (!buffer) return NULL;

    time_t now = time(NULL);
    struct tm *time_now = localtime(&now);
    return time_now;
}