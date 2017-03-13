#pragma once
#include <sys/types.h>
#include <sys/socket.h>
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
	static const int MAX_CLIENTS = 10;

	Server(int portNumber=DEFAULT_PORT);

	void Run();

	~Server();
private:
	static const int MAX_MSG_SIZE = 4096;

	void StartMsgLoop();
	void CheckSockets();
	void AcceptNewClient();
	void HandleClientMsg(int clientSocket);

	int listeningSocket, clientSocket, port, maxFileDesc;
	char msgBuffer[MAX_MSG_SIZE];
    struct sockaddr_in serverAddr, clientAddr;
	fd_set cleanFileDescs;
	fd_set dirtyFileDescs; 
};
}
