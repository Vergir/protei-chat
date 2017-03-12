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
	static const int MAX_MSG_SIZE = 4096;

	Server (int portNumber=DEFAULT_PORT);

	void run();

private:
	int sockfd, new_fd, port;
	char buffer[4096];
    struct sockaddr_in serv_addr, cli_addr;
	struct sigaction sa;
	fd_set master;
	fd_set read_fds; 
	int fdmax;

	static void sigchld_handler(int s);
};
}
