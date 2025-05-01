#include "../../header/channels_utils.h"
#include "../../header/chat_window.h"
#include "../../../controller/header/utils.h"
#include "../../../server/header/db_connection.h"

/*
Channel_info *fetch_current_channel(Client_package *client_package)
{
    int current_channel_id = client_package->login_info->current_channel; // Utilisation de client_package ici
    return find_channel_by_id(client_package->client->channels, current_channel_id);
}
*/

Channel_info *find_channel_by_id(Channel_info *channels, int num_channels, int channel_id)
{
    for (int i = 0; i < num_channels; i++)
    {
        if (channels[i].channel_id == id)
        {
            return &channels[i];
        }
    }
    return NULL;
}
