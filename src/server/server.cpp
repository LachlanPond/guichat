#include "server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <cstring>

struct sockaddr_in address;
std::condition_variable new_msg_cv; 
bool new_msg = false;
bool msg_sent = false;
std::mutex msg_mutex;
char msg_buffer[512];

int main(int argc, char* argv[]) {
	int client_sockets[MAX_CLIENTS];
	int client_count = 0;

	// Set all the client socket values to -1 (nothing assigned)
	for (int i = 0; i < MAX_CLIENTS; i++) {client_sockets[i] = -1;}

	int server_sock = socket(AF_INET, SOCK_STREAM, 0);
	address.sin_family = AF_INET;
	address.sin_port = htons(SERVER_PORT);
	address.sin_addr.s_addr = INADDR_ANY;	// Bind to any local address

	std::cout << "binding server socket" << std::endl;
	bind(server_sock, (struct sockaddr*)&address, sizeof(address));
	std::cout << "listening on server socket" << std::endl;
	listen(server_sock, 5);

	std::cout << "starting broadcast thread" << std::endl;
	std::thread broadcast_thread(handleBroadcast, client_sockets);
	broadcast_thread.detach();

	std::vector<std::thread> client_threads;

	while (1) {
		std::cout << "waiting for new connection from a client" << std::endl;
		int client_sock = accept(server_sock, NULL, NULL);
		client_sockets[client_count] = client_sock;
		std::cout << "new connection established\nstarting client thread" << std::endl;
		client_threads.emplace_back([&](){handleClient(client_sock, client_count);});
		client_threads.back().detach();
		client_count++;
	}

	return 0;
}

void handleClient(int client_sock, int client_count) {
	std::cout << "client thread started" << std::endl;
	char name_buffer[24];
	read(client_sock, name_buffer, sizeof(name_buffer));
	std::cout << "received: " << name_buffer << std::endl;

	while (1) {
		char msg_buffer_temp[512];
		read(client_sock, msg_buffer_temp, sizeof(msg_buffer_temp));

		std::unique_lock lock(msg_mutex);
		strcpy(msg_buffer, msg_buffer_temp);
		new_msg = true;
		std::cout << "client thread loaded message to broadcast" << std::endl;
		lock.unlock();
		
		new_msg_cv.notify_all();
	}

	close(client_sock);
	std::cout << "Socket closed :)" << std::endl;
	return;
}

void handleBroadcast(int* client_sockets) {
	while (1) {
		// Wait until one of the clients sends a new message
		std::unique_lock lock(msg_mutex);
		new_msg_cv.wait(lock, []{return new_msg;});

		std::cout << "Broadcast thread received new message" << std::endl;

		for (int i = 0; i < MAX_CLIENTS; i++) {
			if (client_sockets[i] != -1) {
				::send(client_sockets[i], msg_buffer, sizeof(msg_buffer), 0);
			}
		}

		new_msg = false;

		lock.unlock();
	}
}