#ifndef SERVER_H
#define SERVER_H

struct chat_message {
	char* username[16];
	char* msg[1024];
};

void handleClient(int& server_sock, int client_sock);

#endif // SERVER_H