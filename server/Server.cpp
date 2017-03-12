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

	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		throw std::runtime_error("Could not create socket");
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
		throw std::runtime_error("Could not Make socket reuse address");

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	memset(&(serv_addr.sin_zero), '\0', 8);

	if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) == -1)
		std::runtime_error("Could not bind socket to port");
	if (listen(sockfd, 10) == -1)
		throw std::runtime_error("Could not start listening");
	
	sa.sa_handler = sigchld_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1)
		throw std::runtime_error("Could not estabilish child-process' killing");
}

void Server::sigchld_handler(int s)
{
	while (wait(NULL) > 0);
}

void Server::run()
{
	std::cout << "Server launched on port " << port << std::endl;
	while(1) 
	{ 
		int sin_size = sizeof(struct sockaddr_in);
        if ((new_fd = accept(sockfd, (struct sockaddr *)&cli_addr, (unsigned int*)&sin_size)) == -1)
			throw std::runtime_error("Could not accept connection");
		std::cout << "Connect caught" << std::endl;
        printf("server: got connection from %s\n", inet_ntoa(cli_addr.sin_addr));
        if (!fork()) 
		{
			std::cout << "Forked!" << std::endl;
            close(sockfd);
            if (send(new_fd, "Hello, world!\n", 14, 0) == -1)
				throw std::runtime_error("send");
            close(new_fd);
            exit(0);
        }
        close(new_fd);
    }
}
}
