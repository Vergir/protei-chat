#include <cstdio>
#include "Client.hpp"
#include <iostream>

using namespace protei_chat;
using namespace std;

int main(int argc, char* argv[])
{
	if (argc != 3) 
	{
		printf("usage: %s <hostname> <port>\n", argv[0]);
		exit(1);
	}
	
	Client c(argv[1], atoi(argv[2]));

	try
	{
		c.Launch();
	}
	catch (exception& e)
	{
		cout << "Exception occured. Exception's Message: " << e.what() << endl;
		exit(1);
	}
}
