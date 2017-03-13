#include <iostream>
#include <stdexcept>
#include "Client.hpp"

namespace protei_chat
{
Client::Client (char* hostName, int portNumber)
{
	if ((hostEntity=gethostbyname(hostName)) == NULL)
		throw std::runtime_error("Could not translate host");
	if ((connectionSocket = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		throw std::runtime_error("Could not create socket");

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(portNumber);
	serverAddr.sin_addr = *((struct in_addr *)hostEntity->h_addr);
	memset(&(serverAddr.sin_zero), '\0', 8);
}

Client::~Client()
{
	close(connectionSocket);
}

void Client::Launch()
{
	if (connect(connectionSocket, (struct sockaddr *)&serverAddr, sizeof(struct sockaddr)) == -1) 
		throw std::runtime_error("Could not connect");

	std::cout << "Successfully connected to server" << std::endl;

	StartMsgLoop();
}
void Client::StartMsgLoop()
{
	while(1)
	{
		CheckForData();

		if (FD_ISSET(STDIN_FILENO, &fileDescs))
			SendMsg();

		if (FD_ISSET(connectionSocket, &fileDescs))
			ReceiveMsg();
	}
}

void Client::CheckForData()
{
	int maxFd = connectionSocket > STDIN_FILENO ? connectionSocket : STDIN_FILENO;
	int selectResult;

	FD_ZERO(&fileDescs);
	FD_SET(STDIN_FILENO, &fileDescs);
	FD_SET(connectionSocket, &fileDescs);

	selectResult = select(maxFd + 1, &fileDescs, NULL, NULL, NULL);

	if (selectResult == -1 && errno != EINTR)
		throw std::runtime_error("Could not poll for descriptors (select)");
	else if (selectResult == -1 && errno == EINTR)
		exit(1);
}

void Client::SendMsg()
{
	std::string userInput;
	std::getline(std::cin, userInput);
	if ((send(connectionSocket, userInput.c_str(), userInput.length(), 0)) == -1)
		throw std::runtime_error("Couldn't send message to server...");
}

void Client::ReceiveMsg()
{
	int bytesReceived;
	if ((bytesReceived = recv(connectionSocket, msgBuffer, MAX_MSG_SIZE-1, 0)) == -1)
		throw std::runtime_error("Couldn't receive message from server");
	msgBuffer[bytesReceived] = '\0';
	std::cout << msgBuffer << std::endl;
}

}
