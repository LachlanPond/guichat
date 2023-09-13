#ifndef SERVER_H
#define SERVER_H

#define MAX_CLIENTS 16
#define SERVER_PORT 8083

struct chat_message {
	char* username[24];
	char* msg[1024];
	bool disconnect;
};

void handleClient(int client_sock, int client_count);
void handleBroadcast(int* client_sockets);

#endif // SERVER_H