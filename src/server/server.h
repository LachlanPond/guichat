#ifndef SERVER_H
#define SERVER_H

struct chat_message {
	char* username[24];
	char* msg[1024];
	bool disconnect;
};

void handleClient(int& server_sock, int client_sock);

#endif // SERVER_H