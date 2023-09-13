#ifndef SERVER_H
#define SERVER_H
#include "../defines.h"

#define MAX_CLIENTS 16
#define SERVER_PORT 8083

void handleClient(int client_sock, int client_count);
void handleBroadcast(int* client_sockets);

#endif // SERVER_H