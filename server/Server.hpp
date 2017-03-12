#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>

namespace protei_chat
{
class Server
{
public:
	static const int DEFAULT_PORT = 4567;

	Server (int portNumber=DEFAULT_PORT);

	void run();

private:
	int sockfd, new_fd, port;
	char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
	struct sigaction sa;

	static void sigchld_handler(int s);
};
}
