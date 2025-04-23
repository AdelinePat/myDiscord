#ifndef LISTENER_H
#define LISTENER_H

void *receive_messages(void *arg);
void send_message(int sock, int client_id, const char *text);

#endif