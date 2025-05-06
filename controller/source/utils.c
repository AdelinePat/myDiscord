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

void serialize_client_data(cJSON * clientData, Client_data *client)
{
    // cJSON_AddNumberToObject(clientData, "sock_pointer", (int)client->sock_pointer);
    cJSON_AddStringToObject(clientData, "username", client->client_name);
    cJSON_AddNumberToObject(clientData, "user_id", client->user_id);
}

cJSON * serialize_channel_info(Channel_info channel_info)
{
    cJSON *channelInfo = cJSON_CreateObject();
    cJSON_AddNumberToObject(channelInfo, "channel_id", channel_info.channel_id);
    cJSON_AddStringToObject(channelInfo, "username", channel_info.channel_title);
    return channelInfo;
}
void serialize_channel_info_list(cJSON * channel_list, Client_package * client_package) {
    for (int i = 0; i < client_package->channel_list_size; i++)
    {
        cJSON *a_channel_info = serialize_channel_info(client_package->channels[i]);
        cJSON_AddItemToArray(channel_list, a_channel_info);
    }
}

cJSON * serialize_message(Message a_message)
{
    cJSON *message_json = cJSON_CreateObject();
    cJSON_AddNumberToObject(message_json, "channel_id", a_message.channel_id);
    cJSON_AddNumberToObject(message_json, "message_id", a_message.message_id);
    cJSON_AddNumberToObject(message_json, "user_id", a_message.user_id);
    cJSON_AddStringToObject(message_json, "username", a_message.username);
    cJSON_AddStringToObject(message_json, "timestamp", a_message.timestamp);
    cJSON_AddStringToObject(message_json, "message", a_message.message);
    return message_json;
}
void serialize_message_list(cJSON * message_list_json, Client_package * client_package) {
    for (int i = 0; i < client_package->number_of_messages; i++)
    {
        cJSON *a_message = serialize_message(client_package->messages_list[i]);
        cJSON_AddItemToArray(message_list_json, a_message);
    }
}

char * serialize_client_package(Client_package *client_package) {
    cJSON *root = cJSON_CreateObject();

    // Add every int into root json
    cJSON_AddNumberToObject(root, "send_type", client_package->send_type);
    cJSON_AddNumberToObject(root, "number_of_message", client_package->number_of_messages);
    cJSON_AddNumberToObject(root, "channel_list_size", client_package->channel_list_size);
    cJSON_AddNumberToObject(root, "current_channel", client_package->current_channel);

    // Call for serialize every nested json object
    
    cJSON *clientdata = cJSON_CreateObject();
    serialize_client_data(clientdata, client_package->client);
    cJSON_AddItemToObject(root, "client_data", clientdata);

    cJSON *message_list = cJSON_CreateArray();
    serialize_message_list(message_list, client_package);
    cJSON_AddItemToObject(root, "message_list", message_list);

    cJSON *login = cJSON_CreateObject();
    serialize_login_info(login, client_package->login_info);
    cJSON_AddItemToObject(root, "login_info", login);
    
    cJSON *channels_info_list = cJSON_CreateArray();
    serialize_channels_info(channels_info_list, client_package);
    cJSON_AddItemToObject(root, "channels", channels_info_list);
    
    char *json_string = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    return json_string; // Don't forget to free this after sending!
}

cJSON * parse_login_info(cJSON *root, Client_package * client_package) {
    cJSON *login = cJSON_GetObjectItem(root, "login_info");
    strcpy(client_package->login_info->username, cJSON_GetObjectItem(login, "username")->valuestring);
    strcpy(client_package->login_info->email, cJSON_GetObjectItem(login, "email")->valuestring);
    strcpy(client_package->login_info->password, cJSON_GetObjectItem(login, "password")->valuestring);
    return login;
}

cJSON * parse_client_data(cJSON *root, Client_package * client_package) {
    cJSON *client_data = cJSON_GetObjectItem(root, "client_data");
    strcpy(client_package->client->client_name, cJSON_GetObjectItem(client_data, "client_name")->valuestring);
    client_package->client->user_id = cJSON_GetObjectItem(client_data, "channel_id")->valueint;
    return client_data;
}

void parse_message_list(cJSON *root, Client_package * client_package) {
    cJSON *messages_list = cJSON_GetObjectItem(root, "messages_list");
    client_package->number_of_messages = cJSON_GetObjectItem(root, "number_of_messages")->valueint;

    for (int i = 0; i < client_package->number_of_messages; i++) {
        cJSON * a_message = cJSON_GetArrayItem(messages_list, i);
        parse_a_message(a_message, client_package);
    }
}

void parse_a_message(cJSON * a_message, Client_package * client_package) {
    client_package->messages_list[i].channel_id = cJSON_GetObjectItem(a_message, "channel_id")->valueint;
    client_package->messages_list[i].message_id = cJSON_GetObjectItem(a_message, "message_id")->valueint;
    client_package->messages_list[i].user_id = cJSON_GetObjectItem(a_message, "user_id")->valueint;

    strcpy(client_package->messages_list[i].username, cJSON_GetObjectItem(a_message, "username")->valuestring);
    strcpy(client_package->messages_list[i].timestamp, cJSON_GetObjectItem(a_message, "timestamp")->valuestring);
    strcpy(client_package->messages_list[i].message, cJSON_GetObjectItem(a_message, "message")->valuestring);

}

void parse_channels_info_list(cJSON *root, Client_package * client_package) {
    cJSON *channels_info_list = cJSON_GetObjectItem(root, "messages_list");
    client_package->channel_list_size = cJSON_GetObjectItem(root, "channel_list_size")->valueint;

    for (int i = 0; i < client_package->channel_list_size; i++) {
        cJSON * a_message = cJSON_GetArrayItem(channels_info_list, i);
        client_package->channels[i].channel_id = cJSON_GetObjectItem(a_message, "channel_id")->valueint;
        strcpy(client_package->channels[i].channel_title, cJSON_GetObjectItem(a_message, "channel_title")->valuestring);
    }
}

cJSON * parse_client_package_str(Client_package * client_package, char * client_package_str) {
    cJSON *root = cJSON_PARSE(client_package_str);
    client_package->send_type = cJSON_GetObjectItem(root, "send_type")->valueint;
    client_package->current_channel = cJSON_GetObjectItem(root, "current_channel")->valueint;
    cJSON *login = parse_login_info(root, client_package);
    parse_message_list(root, client_package);
    parse_channels_info_list(root, client_package);
    cJSON *client_data = parse_client_data(root, client_package);
}