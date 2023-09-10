#include "server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>
#include <iostream>
#include <thread>

struct sockaddr_in address;

int main(int argc, char* argv[]) {
	int server_sock = socket(AF_INET, SOCK_STREAM, 0);

	address.sin_family = AF_INET;
	address.sin_port = htons(8081);
	address.sin_addr.s_addr = INADDR_ANY;	// Bind to any local address

	std::cout << "binding server socket" << std::endl;
	bind(server_sock, (struct sockaddr*)&address, sizeof(address));
	std::cout << "listening on server socket" << std::endl;
	listen(server_sock, 5);

	std::vector<std::thread> threads;

	while (1) {
		std::cout << "waiting for new connection from a client" << std::endl;
		int client_sock = accept(server_sock, NULL, NULL);
		std::cout << "new connection established\nstarting client thread" << std::endl;
		threads.emplace_back([&](){handleClient(server_sock, client_sock);});
		threads.back().detach();
	}

	return 0;
}

void handleClient(int& server_sock, int client_sock) {
	std::cout << "client thread started" << std::endl;
	char name_buffer[24];
	read(client_sock, name_buffer, sizeof(name_buffer));
	std::cout << "received: " << name_buffer << std::endl;
	close(client_sock);
	std::cout << "Socket closed :)" << std::endl;
	return;
}