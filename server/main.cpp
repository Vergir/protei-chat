#include <cstdlib>
#include <iostream>
#include "Server.hpp"

using namespace protei_chat;

int main(int argc, char* argv[])
{
	Server s;
	int port = 0;
	if (argc > 1)
		port = atoi(argv[1]);	

	if (port > 0)
		s = Server(port);
	else
		s = Server();

	s.run();
}
