#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>

namespace protei_chat
{
class Client
{
public:
	static const int MAX_MSG_SIZE = 2048;

	Client (char* hostName, int portNumber);

	void launch();
private:
	int sockfd, numbytes;  
	char buf[MAX_MSG_SIZE];
	struct hostent *he;
	struct sockaddr_in their_addr;
};
}
