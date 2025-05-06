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
    if (login_info->username != NULL) {
        printf("username de login info n'est pas NULL\n");
        cJSON_AddStringToObject(login, "username", login_info->username);
    }
    if (login_info->email != NULL) {
        printf("email n'est pas NULL");
        cJSON_AddStringToObject(login, "email", login_info->email);
    }
    if (login_info->password != NULL) {
        printf("password n'est pas null");
        cJSON_AddStringToObject(login, "password", login_info->password);
    }    
    cJSON_AddNumberToObject(login, "user_id", login_info->user_id);
}

void serialize_client_data(cJSON * clientData, Client_data *client, int * size_string)
{
    char buffer[LARGE_PLUS_STRING];
    int int_length;
    
    // cJSON_AddNumberToObject(clientData, "sock_pointer", (int)client->sock_pointer);
    if (client->username != NULL) 
    {
        printf("client->username n'est pas null dans serialize_client_data\n");
        cJSON_AddStringToObject(clientData, "username", client->username);
    }

    printf("\nvaleur de client->user_id %d\n", client->user_id);
    cJSON_AddNumberToObject(clientData, "user_id", client->user_id);
    printf("serialize_client_data : send_type %d\n", client->user_id);

    printf("[client_data] valeur de size_string %d\n", *size_string);
    // char *json_client_data = cJSON_PrintUnformatted(clientData);
    // int client_data_len = strlen(json_client_data);
    // *size_string += strlen("client_data : { \"\",, }");
    sprintf(buffer, "%d", client->user_id);
    int_length = strlen(buffer);
    *size_string = *size_string + strlen("client_data : { \"\",, }") + strlen(client->username) + int_length;
    memset(buffer, 0, sizeof(buffer));
    printf("[client_data] valeur de size_string apres calcul %d\n", *size_string);
    
    // free(json_client_data);
}

cJSON * serialize_channel_info(Channel_info channel_info, int * size_string)
{
    cJSON *channelInfo = cJSON_CreateObject();
    cJSON_AddNumberToObject(channelInfo, "channel_id", channel_info.channel_id);
    printf("j'essaye d'imprimer le title du channel %s\n", channel_info.channel_title);
    if (channel_info.channel_title != NULL) {
        printf("channel_info.channel_title n'est pas null : %s\n", channel_info.channel_title);
        cJSON_AddStringToObject(channelInfo, "channel_title", channel_info.channel_title);
    }
    char buffer[LARGE_PLUS_STRING];
    int int_length;
    sprintf(buffer, "%d", channel_info.channel_id);
    int_length = strlen(buffer);
    *size_string = *size_string + strlen("channel_id : { \"\",, }") + strlen("channel_title : { \"\",, }") + strlen(channel_info.channel_title) + int_length;
    memset(buffer, 0, sizeof(buffer));
    printf("[serialize_channel_info] valeur de size_string apres calcul %d\n", *size_string);
    
    return channelInfo;
}
void serialize_channel_info_list(cJSON * channel_list, Client_package * client_package, int * size_string) {
    for (int i = 0; i < client_package->channel_list_size; i++)
    {
        printf("[serialize_channel_info_list] index %d\n", i);
        cJSON *a_channel_info = serialize_channel_info(client_package->channels[i], size_string);
        cJSON_AddItemToArray(channel_list, a_channel_info);
        printf("j'add l'item a l'array ?\n\n");
    }
    printf("\nd'après json la liste de message est de %d\n", cJSON_GetArraySize(channel_list));
    for (int index = 0; index < cJSON_GetArraySize(channel_list); index++)
    {
        cJSON * random_msg = cJSON_GetArrayItem(channel_list, index);
        if (!random_msg) {
            printf("cJSONgetarrayitm failed at index %d\n", index);
        } else if (!cJSON_IsObject(random_msg)) {
            printf("Item at index %d is not a valid JSON object\n", index);
        }
    }
    *size_string = *size_string + strlen("channels : { \"\",, }");
    printf("[serialize_channel_info_list] a la fin de la fonction valeur de size_string %d\n", *size_string);

    // char * array_str = cJSON_PrintUnformatted(channel_list);
    // printf("\n\n\n\n\tj'essaie de print l'array de channel_info %s\n\n\n\n", array_str);
}

cJSON * serialize_message(Message * a_message, int * size_string)
{
    char buffer[LARGE_PLUS_STRING];
    int int_length;

    printf("\n[serialize_message] valeur de size_string %d\n", *size_string);
    printf("\n\nvaluer dans serialize_message channel_id : %d, username %s\n\n", 
        a_message->channel_id,
        a_message->username
    );

    cJSON *message_json = cJSON_CreateObject();
    if (!message_json) {
        printf("la creation de message_json a échoué \n");
        return NULL;
    }

    if (a_message->username == NULL) printf("username is NULL\n");
    if (a_message->message == NULL) printf("message is NULL\n");
    if (a_message->timestamp == NULL) printf("timestamp is NULL\n");

    cJSON_AddNumberToObject(message_json, "channel_id", a_message->channel_id);
    sprintf(buffer, "%d", a_message->channel_id);
    int_length = strlen(buffer);
    *size_string += int_length;
    memset(buffer, 0, sizeof(buffer));

    cJSON_AddNumberToObject(message_json, "message_id", a_message->message_id);
    sprintf(buffer, "%d", a_message->message_id);
    int_length = strlen(buffer);
    *size_string += int_length;
    memset(buffer, 0, sizeof(buffer));

    cJSON_AddNumberToObject(message_json, "user_id", a_message->user_id);
    sprintf(buffer, "%d", a_message->user_id);
    int_length = strlen(buffer);
    *size_string += int_length;
    memset(buffer, 0, sizeof(buffer));

    // char *json_message_json = cJSON_PrintUnformatted(message_json);
    // int len_json_message_json = strlen(json_message_json);
    // *size_string += len_json_message_json;
    // free(json_message_json);


    if (a_message->username != NULL) {
        printf("username n'est pas null dans le msg\n");
        printf("strlen(username) = %zu\n", strlen(a_message->username));
        printf("username = %s\n", a_message->username);
        cJSON_AddStringToObject(message_json, "username", a_message->username);
        // *size_string += LARGE_PLUS_STRING;
    }
    if (a_message->timestamp != NULL) {
        printf("timestamp n'est pas null dans le msg\n");
        printf("strlen(timestamp) = %zu\n", strlen(a_message->timestamp));
        printf("timestamp = %s\n", a_message->timestamp);
        cJSON_AddStringToObject(message_json, "timestamp", a_message->timestamp);
        // *size_string += LARGE_PLUS_STRING;
    }
    if (a_message->message != NULL) {
        printf("message n'est pas null dans le msg\n");
        printf("strlen(message) = %zu\n", strlen(a_message->message));
        printf("message = %s\n", a_message->message);
        cJSON_AddStringToObject(message_json, "message", a_message->message);
        // *size_string += VERY_LARGE_STRING;
    }

    printf("[serialize_message] valuer de size_string avant ajout %d\n", *size_string);
    *size_string = *size_string + strlen("channel_id : { \"\", }") \
    + strlen("user_id : { \"\", }") + strlen("message_id : { \"\", }") \
    + + strlen("username : { \"\", }") + strlen("message : { \"\", }") 
    + strlen(a_message->username) + strlen(a_message->message) \
    + strlen(a_message->timestamp) + strlen("timestamp : { \"\", }") ;
    printf("[serialize_message] valuer de size_string après ajout %d\n", *size_string);
    // JE SUIS ICI MAINTENANT

    // char * message_json_str = cJSON_PrintUnformatted(message_json);
    // printf("\n\nj'essaie de print un objet message json string %s\n\n", message_json_str);
    printf("\n[serialize_message] valeur de size_string %d\n", *size_string);
    return message_json;
}
void serialize_message_list(cJSON * message_list_json, Client_package * client_package, int * size_string) {
    int inside_size_string = *size_string;
    for (int i = 0; i < client_package->number_of_messages; i++)
    {
        printf("\n[serialize_message_list] valeur de size_string %d\n", *size_string);
        printf("serialize message à index %d\n", i);
        printf("message[i] content \n%d : \t%s\t%s\n",
            client_package->messages_list[i].user_id,
            client_package->messages_list[i].username,
            client_package->messages_list[i].message);
            printf("\n[serialize_message_list] valeur de size_string %d\n", *size_string);
        cJSON *a_message = serialize_message(&client_package->messages_list[i], size_string);
        // adding the size of username, timestamp, message_content
        // *size_string = *size_string + SMALL_STRING + MEDIUM_STRING * 6 + VERY_LARGE_STRING;
        cJSON_AddItemToArray(message_list_json, a_message);
        // *size_string += inside_size_string;
        *size_string += MEDIUM_STRING;
    }
    // printf("\nj'essaie de print rapidement ma string json\n");
    // char * array_str = cJSON_PrintUnformatted(message_list_json);
    // printf("\n\nj'essaie de print l'array de message_list %s\n\n", array_str);
}

char * serialize_client_package(Client_package *client_package) {
    // int * size_string_pointer = NULL;
    int size_string = 0;
    char buffer[VERY_LARGE_STRING];
    int int_length;
    // size_string_pointer = &size_string;

    printf("dans serializ_client_package\n");
    cJSON *root = cJSON_CreateObject();

    // Add every int into root json
    cJSON_AddNumberToObject(root, "send_type", client_package->send_type);
    printf("\ntest avant de compter la len\n");
    sprintf(buffer, "%d", client_package->send_type);
    int_length = strlen(buffer);
    size_string = size_string + strlen("{\"send_type \" : ,}") + int_length;
    memset(buffer, 0, sizeof(buffer));
    printf("\ntest apres avoir compté la len\n");
    // size_string = sizeof(root);
    // printf("valeur de la taille de root après send_type %d", size_string);
    // char *json_send_type = cJSON_PrintUnformatted(root);
    // int send_type_len = strlen(json_send_type);
    // size_string += send_type_len;
    // free(json_send_type);

    // printf("taille de size_string après sendtype %d", size_string);

    printf("\nserialize_client_package : send_type %d\n", client_package->send_type);
    cJSON_AddNumberToObject(root, "number_of_messages", client_package->number_of_messages);
    sprintf(buffer, "%d", client_package->number_of_messages);
    int_length = strlen(buffer);
    size_string = size_string + strlen("{\"number_of_messages \" : ,}") + int_length;
    memset(buffer, 0, sizeof(buffer));
    // char *json_nbr_of_message = cJSON_PrintUnformatted(root);
    // int nbr_of_message_len = strlen(json_nbr_of_message);
    // size_string += send_type_len;
    // free(json_nbr_of_message);

    printf("serialize_client_package : nbr_of_message %d\n", client_package->number_of_messages);

    
    // char *json_channel_list_size = cJSON_PrintUnformatted(root);
    // int channel_size_list_len = strlen(json_channel_list_size);
    // size_string += channel_size_list_len;
    // free(json_channel_list_size);
    cJSON_AddNumberToObject(root, "current_channel", client_package->current_channel);
    sprintf(buffer, "%d", client_package->current_channel);
    int_length = strlen(buffer);
    size_string = size_string + strlen("{\"current_channel \" : ,}") + int_length;
    memset(buffer, 0, sizeof(buffer));
    
    // char *json_current_channel = cJSON_PrintUnformatted(root);
    // int current_channel_len = strlen(json_current_channel);
    // size_string += current_channel_len;
    // free(json_current_channel);
    
    printf("serialize_client_package : current_channel %d\n", client_package->current_channel);

    cJSON_AddNumberToObject(root, "channel_list_size", client_package->channel_list_size);
    // size_string += LARGE_PLUS_STRING * 5; // 4 items + 1 en plus au cas où
    sprintf(buffer, "%d", client_package->channel_list_size);
    int_length = strlen(buffer);
    size_string = size_string + strlen("{\"channel_list_size \" : ,}") + int_length;
    memset(buffer, 0, sizeof(buffer));
    printf("serialize_client_package : channel_list_size %d\n", client_package->channel_list_size);


    printf("\n[apres current_channel] valeur de size_string %d\n", size_string);
    // Call for serialize every nested json object
    printf("\njuste avant client_data\n");
    cJSON *clientdata = cJSON_CreateObject();
    if (client_package->client->user_id != NULL && client_package->client->username != NULL)
    {
        serialize_client_data(clientdata, client_package->client, &size_string);
    } else {
        char *json_client_data = cJSON_PrintUnformatted(root);
        int client_data_len = strlen(json_client_data);
        size_string += client_data_len;
        free(json_client_data);
    }

    cJSON_AddItemToObject(root, "client_data", clientdata);
    size_string = size_string + strlen("{\"client_data \" : ,}");
    

    printf("\n[apres client data] valeur de size_string %d\n", size_string);
    size_string += LARGE_PLUS_STRING;

    printf("\njuste avant message_list\n");
    cJSON *messages_list = cJSON_CreateArray();
    printf("\njuste avant serialize_message_list\n");
    printf("valeur de nbr of message %d\n", client_package->number_of_messages);
    if (client_package->number_of_messages > 0) {
        serialize_message_list(messages_list, client_package, &size_string);
    }
    cJSON_AddItemToObject(root, "messages_list", messages_list);

    printf("juste avant login_info\n");
    cJSON *login = cJSON_CreateObject();
    serialize_login_info(login, client_package->login_info);
    cJSON_AddItemToObject(root, "login_info", login);
    
    printf("juste avant channels_info_list\n");
    cJSON *channels_info_list = cJSON_CreateArray();

    printf("taille de la liste des channels %d", client_package->channel_list_size);
    if (client_package->channel_list_size > 0) {
        serialize_channel_info_list(channels_info_list, client_package, &size_string);
    }
    cJSON_AddItemToObject(root, "channels", channels_info_list);
    
    printf("juste avant la transformation en string\n");

    if (!cJSON_IsObject(root)) {
        printf("Root JSON is invalid!\n");
    }

    // char *json_string = cJSON_PrintUnformatted(root);
    // if (!json_string) {
    //     printf("cJSON_PrintUnformatted failed\n");
    // }
    // printf("\nje vais essayer d'imprimer un message json\n");
    // printf("\nvaleur dans le json username par ex %s\n", cJSON_GetObjectItem(message_json, "username")->valuestring);
    
    // int size_of_json_string = sizeof(client_package) + 300;
    printf("\n\n\ntaille du json ? : %d\n\n\n", size_string);

    char json_string[size_string]; // Plenty of space
    int printed = cJSON_PrintPreallocated(root, json_string, sizeof(json_string), 0);
    if (printed) {
        printf("Preallocated print: %s\n", json_string);
    } else {
        printf("cJSON_PrintPreallocated failed!\n");
    }
    char *final_json_string = strdup(json_string);
    printf("mon json sans printunformatted ? FINAL_JSON_STRING %s\n\n", final_json_string);
    printf("print du json_string dans serialize_client_package JSON_STRING %s\n", json_string);
    cJSON_Delete(root);
    return final_json_string; // Don't forget to free this after sending!
}

cJSON * parse_login_info(cJSON *root, Client_package * client_package) {
    cJSON *login = cJSON_GetObjectItem(root, "login_info");
    strcpy(client_package->login_info->username, cJSON_GetObjectItem(login, "username")->valuestring);
    strcpy(client_package->login_info->email, cJSON_GetObjectItem(login, "email")->valuestring);
    strcpy(client_package->login_info->password, cJSON_GetObjectItem(login, "password")->valuestring);
    return login;
}

cJSON * parse_client_data(cJSON *root, Client_package * client_package) {
    printf("dans parse_client_data\n");
    cJSON *client_data = cJSON_GetObjectItem(root, "client_data");

    printf("contenu username (ancien client_name) du json %s\n", cJSON_GetObjectItem(client_data, "username")->valuestring);
    printf("avant strcpy du username (client)\n");
    strcpy(client_package->client->username,
        cJSON_GetObjectItem(client_data, "username")->valuestring);

    client_package->client->user_id = cJSON_GetObjectItem(client_data, "user_id")->valueint;
    return client_data;
}

void parse_message_list(cJSON *root, Client_package * client_package) {
    printf("je rentre dans la fucking fonction \"parse_message_list\"\n");
    // client_package->number_of_messages = cJSON_GetObjectItem(root, "number_of_messages")->valueint;
    printf("dans parse_message list après number_of_messages\n");
    printf("\n\n\n\n DANS PARSE_MESSAGE_LIST , number_of_message = %d\n\n\n", client_package->number_of_messages);

    if (client_package->number_of_messages > 0) {
        Message * message_list_struct = malloc(sizeof(Message) * client_package->number_of_messages);
        if (!message_list_struct) {
            fprintf(stderr, "\nPARSE_MESSAGE_LIST :Memory allocation failed for messages_list\n");
            client_package->number_of_messages = 0;
            return;
        }
        printf("après le malloc de la message_list\n");
        

        client_package->messages_list = message_list_struct;
        if (!client_package->messages_list) {
            printf("Memory allocation failed for messages_list\n");
            return;
        }
        
        cJSON *messages_list = cJSON_GetObjectItem(root, "messages_list");
        if (!cJSON_IsArray(messages_list)) {
            printf("messages_list is not an array or is NULL\n");
            return;
        }

        for (int i = 0; i < client_package->number_of_messages; i++) {
            cJSON * a_message = cJSON_GetArrayItem(messages_list, i);
            parse_a_message(a_message, client_package, i);
            }
    }

}

void parse_a_message(cJSON * a_message, Client_package * client_package, int index) {
    client_package->messages_list[index].channel_id = cJSON_GetObjectItem(a_message, "channel_id")->valueint;
    client_package->messages_list[index].message_id = cJSON_GetObjectItem(a_message, "message_id")->valueint;
    client_package->messages_list[index].user_id = cJSON_GetObjectItem(a_message, "user_id")->valueint;

    strcpy(client_package->messages_list[index].username, cJSON_GetObjectItem(a_message, "username")->valuestring);
    strcpy(client_package->messages_list[index].timestamp, cJSON_GetObjectItem(a_message, "timestamp")->valuestring);
    strcpy(client_package->messages_list[index].message, cJSON_GetObjectItem(a_message, "message")->valuestring);

}

void parse_channels_info_list(cJSON *root, Client_package * client_package) {

    printf("je rentre dans la fucking fonction \"parse_channels_info_list\"\n");
    client_package->channel_list_size = cJSON_GetObjectItem(root, "channel_list_size")->valueint;

    if (client_package->channel_list_size > 0) {
        Channel_info * channels = malloc(sizeof(Channel_info) * client_package->channel_list_size);
        client_package->channels = channels;
        printf("après le malloc dans parse_channels_info_list\n");
        cJSON *channels_info_list = cJSON_GetObjectItem(root, "channels");
        
        for (int i = 0; i < client_package->channel_list_size; i++) {
            cJSON * a_message = cJSON_GetArrayItem(channels_info_list, i);
            client_package->channels[i].channel_id = cJSON_GetObjectItem(a_message, "channel_id")->valueint;
            strcpy(client_package->channels[i].channel_title, cJSON_GetObjectItem(a_message, "channel_title")->valuestring);
        }
    }
}

void parse_client_package_str(Client_package * client_package, char * client_package_str) {
    cJSON *root = cJSON_Parse(client_package_str);
    printf("parse du json parent\n");
    client_package->send_type = cJSON_GetObjectItem(root, "send_type")->valueint;
    client_package->current_channel = cJSON_GetObjectItem(root, "current_channel")->valueint;
    client_package->number_of_messages = cJSON_GetObjectItem(root, "number_of_messages")->valueint;
    client_package->channel_list_size = cJSON_GetObjectItem(root, "channel_list_size")->valueint;
    printf("avant parse_login_info\n");
    cJSON *login = parse_login_info(root, client_package);
    printf("avant parse_message_list\n");
    parse_message_list(root, client_package);
    printf("avant channels_info_list\n");
    parse_channels_info_list(root, client_package);
    printf("avant parse_client_data\n");
    if (client_package->send_type != LOGIN && client_package->send_type != CREATE_ACCOUNT) {
        cJSON *client_data = parse_client_data(root, client_package);
    }
}