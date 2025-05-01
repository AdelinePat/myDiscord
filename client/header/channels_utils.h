// channel_utils.h
#ifndef CHANNEL_UTILS_H
#define CHANNEL_UTILS_H

#include "../../controller/header/utils.h" /**/

// Channel_info *fetch_current_channel(Client_package *client_package);

Channel_info *find_channel_by_id(Channel_info *channels, int id);

#endif // CHANNEL_UTILS_H
