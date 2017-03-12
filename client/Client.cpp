#include <iostream>
#include <stdexcept>
#include "Client.hpp"

namespace protei_chat
{
Client::Client (char* hostName, int portNumber)
{
	if ((he=gethostbyname(hostName)) == NULL)
		throw std::runtime_error("Could not translate host");
	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		throw std::runtime_error("Could not create socket");

	their_addr.sin_family = AF_INET;
	their_addr.sin_port = htons(portNumber);
	their_addr.sin_addr = *((struct in_addr *)he->h_addr);
	memset(&(their_addr.sin_zero), '\0', 8);
}

void Client::launch()
{
	if (connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1) 
		throw std::runtime_error("Could not connect");
	std::cout << "Successfully connected" << std::endl;

	if ((numbytes=recv(sockfd, buf, MAX_MSG_SIZE-1, 0)) == -1)
		throw std::runtime_error("Unknown error while receiving");

	buf[numbytes] = '\0';
	printf("Received: %s",buf);
	close(sockfd);
}
}
