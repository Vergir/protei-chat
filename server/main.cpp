#include <iostream>
#include "Server.hpp"

using namespace protei_chat;
using namespace std;

int main(int argc, char* argv[])
{
	int port = 0;
	if (argc > 1)
		port = atoi(argv[1]);	

	Server *s;
	if (port > 0)
		s = new Server(port);
	else
		s = new Server();

	try
	{
		s->Run();
	}
	catch (exception& e)
	{
		cout << "Exception Occured. Exception's Message: " << e.what() << endl;
		exit(1);
	}
}
