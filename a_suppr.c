void recover_messages(Client_package_for_backend *package)
{
    printf("dans recover_message cote serveur :p juste avant get_full_chat_content\n");
    if (package->client_package->send_type != LOGIN && package->client_package->send_type != CREATE_ACCOUNT)
    {
        get_full_chat_content(package->client_package);
    }

    int size_of_list = package->client_package->number_of_messages;
    
    printf("message_list size (number of messages) : %d\n", size_of_list);

    char * client_pack_str = serialize_client_package(package->client_package);

    printf("Voici la string du json a envoyé dans recover_message : \n\n%s\n\n\n",
        client_pack_str);

    int len = strlen(client_pack_str);

    send(package->client_package->client->sock_pointer,
    (char *)&len,
    sizeof(int),
    0);

    send(package->client_package->client->sock_pointer,
    client_pack_str,
    len,
    0);

    int validation;
    recv(package->client_package->client->sock_pointer, (char *)validation, sizeof(int), 0);

    printf("sending messages");
    printf("messages number : %d\n", package->client_package->number_of_messages);

    printf("       messages sent\n");
}

client_package {
    send_type : 0,
    number_of_messages : 0,
    channel_list_size : 0,
    current_channel : 0,
    client_data : {
        username : "",
        user_id : 0

    },
    messages_list : [
        {
            channel_id : 1,
            message_id : 0,
            user_id : 0,
            username : "",
            timestamp : "",
            message : ""
        },
        {
            channel_id : 1,
            message_id : 1,
            user_id : 0,
            username : "",
            timestamp : "",
            message : ""
        }, ...
    ],
    login_info {
        username : "",
        email : "",
        password : "",
        user_id : 0

    },
    channels : [ 
        {
            channel_id : 0,
            channel_title : "title"
        },
        {
            channel_id : 1,
            channel_title : "title2"
        }, ...
    ]
}