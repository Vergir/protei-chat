#include <sstream>
#include <iostream>
#include <stdexcept>
#include <sys/wait.h>
#include "Server.hpp"

namespace protei_chat
{
Server::Server (int portNumber)
{
	port = portNumber;
	int yes = 1;

	if ((listeningSocket = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		throw std::runtime_error("Could not create socket");
	if (setsockopt(listeningSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
		throw std::runtime_error("Could not Make socket reuse address");

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	memset(&(serverAddr.sin_zero), '\0', 8);

	if (bind(listeningSocket, (struct sockaddr *)&serverAddr, sizeof(struct sockaddr)) == -1)
		std::runtime_error("Could not bind socket to port");
	if (listen(listeningSocket, MAX_CLIENTS) == -1)
		throw std::runtime_error("Could not start listening");

	FD_ZERO(&cleanFileDescs);
	FD_ZERO(&dirtyFileDescs);
	FD_SET(listeningSocket, &cleanFileDescs);
	maxFileDesc = listeningSocket;
}

void Server::Run()
{
	std::cout << "Server launched on port " << port << std::endl;
	StartMsgLoop();
}

void Server::StartMsgLoop()
{
	while(1)
   	{
		CheckSockets();

		for(int i = 0; i <= maxFileDesc; i++) 
			if (FD_ISSET(i, &dirtyFileDescs)) 
			{
				if (i == listeningSocket)
					AcceptNewClient();
				else
					HandleClientMsg(i);
			}
	}
}

void Server::CheckSockets()
{
	dirtyFileDescs = cleanFileDescs; 
	if (select(maxFileDesc+1, &dirtyFileDescs, NULL, NULL, NULL) == -1)
		throw std::runtime_error(strerror(errno));
}

void Server::AcceptNewClient()
{
	int addressLength = sizeof(clientAddr);
	if ((clientSocket = accept(listeningSocket, (struct sockaddr *)&clientAddr, (unsigned int*)&addressLength)) == -1) 
		throw std::runtime_error("Could not accept new connection");

	FD_SET(clientSocket, &cleanFileDescs);
	if (clientSocket > maxFileDesc)
		maxFileDesc = clientSocket;
	std::cout << "[META]: new connection. Address: " << inet_ntoa(clientAddr.sin_addr) << "; Socket #" << clientSocket << std::endl;
}

void Server::HandleClientMsg(int clientSocket)
{
	int receivedBytes = recv(clientSocket, msgBuffer, sizeof(msgBuffer), 0);

	if (receivedBytes < 0) 
		throw std::runtime_error(strerror(errno));

	if (receivedBytes > 0)
	{
		std::stringstream msg;
		msg << clientSocket << ": " << msgBuffer;
		std::cout << clientSocket << ": " << msgBuffer << std::endl;
		for(int i = 0; i <= maxFileDesc; ++i)
			if (FD_ISSET(i, &cleanFileDescs) && (i != listeningSocket && i != clientSocket) && (send(i, msg.str().c_str(), msg.str().length(), 0) == -1))
				throw std::runtime_error("Could not send data to client");
	}
	else
	{
		std::cout << "[META]: Client disconnect. Socket #" << clientSocket << std::endl;
		close(clientSocket);
		FD_CLR(clientSocket, &cleanFileDescs);
	}
}

Server::~Server()
{
	close(listeningSocket);
}
}
