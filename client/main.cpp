#include "Client.hpp"

using namespace protei_chat;

int main(int argc, char* argv[])
{
	if (argc != 3) 
	{
		printf("usage: %s <hostname> <port>\n", argv[0]);
		exit(1);
	}
	
	Client c(argv[1], atoi(argv[2]));

	c.launch();
}
