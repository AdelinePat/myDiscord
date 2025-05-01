#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "../header/utils.h"

struct tm get_timestamp() {
    time_t now = time(NULL);
    struct tm time_now = *localtime(&now);
    return time_now;
}

// struct tm parse_db_query_time(char *time_str) {
    // struct tm result;
    // memset(&result, 0, sizeof(struct tm)); // Initialize to zero
//     // // Parse time in format "HH:MM:SS"
//     // if (strptime(time_str, "%H:%M:%S", &result) == NULL) {
//     //     fprintf(stderr, "Failed to parse time string: %s\n", time_str);
//     //     // handle error or set default
//     // }

//     // // strftime(time_str, sizeof(time_str), "[%H:%M:%S]", &client_message->timestamp);
//     // return result;
//     // ##########################################""
//     struct tm result;
//     memset(&result, 0, sizeof(struct tm));

//     int hour, min, sec;
//     if (sscanf(time_str, "%d:%d:%d", &hour, &min, &sec) != 3) {
//         fprintf(stderr, "Failed to parse time string: %s\n", time_str);
//     } else {
//         result.tm_hour = hour;
//         result.tm_min = min;
//         result.tm_sec = sec;
//     }
//     return result;
// }

char * timestamp_to_char(struct tm time) {
    // char time_str[100];

    // strftime(time_str, sizeof(time_str), "%x - %H:%M - %d/%b%Y", &time);
    // return time_str;
    char *time_str = malloc(100); // Dynamically allocated, caller must free
    if (time_str == NULL) {
        fprintf(stderr, "Memory allocation failed in timestamp_to_char\n");
        return NULL;
    }

    strftime(time_str, 100, "%H:%M - %d/%m/%Y", &time);
    return time_str;
}

struct tm parse_db_query_time(char *time_str) {
    struct tm result;
    memset(&result, 0, sizeof(struct tm)); // zero the struct

    int matched = sscanf(
        time_str,
        "%d-%d-%d %d:%d:%d",
        &result.tm_year,
        &result.tm_mon,
        &result.tm_mday,
        &result.tm_hour,
        &result.tm_min,
        &result.tm_sec
    );

    if (matched != 6) {
        fprintf(stderr, "Failed to parse timestamp: %s\n", time_str);
        // Optional: set defaults or exit
    }

    result.tm_year -= 1900; // tm_year is years since 1900
    result.tm_mon -= 1;     // tm_mon is 0-based (0 = January)

    return result;
}
    // time( &t );
     
    //localtime() uses the time pointed by t ,
    // to fill a tm structure with the 
    // values that represent the 
    // corresponding local time.
     
    // tmp = localtime( &t );
     
    // using strftime to display time

    
    // struct tm *tmp ;
        // char *time_str = strptime(time, "%H:%M - %d/%b%Y", &time_str);
    // using strftime()
    // %H = Hour in 24h format (00-23) 
    // %M = Minutes in decimal ranging from 00 to 59.
    // %d = Day of the month (01-31)
    // %b = Abbreviated month name