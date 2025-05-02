#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <cjson/cJSON.h>

#include "../header/utils.h"

char * get_str_timestamp() {
    time_t now = time(NULL);
    struct tm time_now = *localtime(&now);
    char *time_tr = timestamp_to_char(time_now);
    return time_tr;
}

char * timestamp_to_char(struct tm time) {
    char *time_str = malloc(20); 
    if (time_str == NULL) {
        fprintf(stderr, "Memory allocation failed in timestamp_to_char\n");
        return NULL;
    }
    strftime(time_str, 20, "%H:%M - %d/%m/%Y", &time);
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
    }
    result.tm_year -= 1900; // tm_year is years since 1900
    result.tm_mon -= 1;     // tm_mon is 0-based (0 = January)
    return result;
}

void serialize_login_info(cJSON * login, Login_infos *login_info) {
    
    cJSON_AddStringToObject(login, "username", login_info->username);
    cJSON_AddStringToObject(login, "password", login_info->password);
    cJSON_AddStringToObject(login, "email", login_info->email);
    cJSON_AddNumberToObject(login, "user_id", login_info->user_id);
}

// void serialize_message_list(cJSON * message_list_json, Message *messages[]) {
    

// }

char * serialize_client_package(Client_package *client_package) {
    cJSON *root = cJSON_CreateObject();

    // Add every int into root json
    cJSON_AddNumberToObject(root, "send_type", client_package->send_type);
    cJSON_AddNumberToObject(root, "number_of_message", client_package->number_of_messages);
    cJSON_AddNumberToObject(root, "channel_list_size", client_package->channel_list_size);
    cJSON_AddNumberToObject(root, "current_channel", client_package->current_channel);

    // Call for serialize every nested json object
    cJSON *login = cJSON_CreateObject();
    serialize_login_info(login, client_package->login_info);
    cJSON_AddItemToObject(root, "login_info", login);

    // cJSON *message_list = cJSON_CreateArray();
    // serialize_message_list(message_list, client_package->messages_list);
    char *json_string = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    return json_string; // Don't forget to free this after sending!
}