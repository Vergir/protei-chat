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
	Client(char* hostName, int portNumber);

	void Launch();

	~Client();
private:
	void StartMsgLoop();
	void CheckForData();
	void SendMsg();
	void ReceiveMsg();

	static const int MAX_MSG_SIZE = 2048;

	int connectionSocket;  
	char msgBuffer[MAX_MSG_SIZE];
	struct hostent * hostEntity;
	struct sockaddr_in serverAddr;
	fd_set fileDescs;
};
}
