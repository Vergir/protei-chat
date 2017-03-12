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
	FD_ZERO(&master);
	FD_ZERO(&read_fds);

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

	FD_SET(sockfd, &master);
	fdmax = sockfd;

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
	int i, j, nbytes, addrlen;
	std::cout << "Server launched on port " << port << std::endl;
	for(;;) {
		read_fds = master; 
		if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1)
			throw std::runtime_error("Could not poll for incoming connections (select)");
		for(i = 0; i <= fdmax; i++) {
			if (FD_ISSET(i, &read_fds)) { // we got one!!
				if (i == sockfd) {
					addrlen = sizeof(cli_addr);
					if ((new_fd = accept(sockfd, (struct sockaddr *)&cli_addr, (unsigned int*)&addrlen)) == -1) 
						throw std::runtime_error("Could not accept new connection");
							else 
							{
								FD_SET(new_fd, &master); // add to master set
								if (new_fd > fdmax)
									fdmax = new_fd;
								printf("selectserver: new connection from %s on " "socket %d\n", inet_ntoa(cli_addr.sin_addr), new_fd);
							}
				}
				if ((nbytes = recv(i, buffer, sizeof(buffer), 0)) <= 0) 
				{
					if (nbytes == 0)
						printf("selectserver: socket %d hung up\n", i);
					else
						throw std::runtime_error("Could not receive correctly");
					close(i);
					FD_CLR(i, &master);
				}
				else
					for(j = 0; j <= fdmax; j++)
						if (FD_ISSET(j, &master) && (j != sockfd && j != i) && (send(j, buffer, nbytes, 0) == -1))
									throw std::runtime_error("Could not send data to client");
			}
		}
	}
}
}
