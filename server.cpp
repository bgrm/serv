#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <err.h>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <algorithm>
#include "sock.h"

const string localhost = "127.0.0.1";
short port = 55555;

int main(int argc, char* argv[])
{
	string ip = localhost;
	if (argc > 1)
		ip = string(argv[1]);
	if (argc > 2)
		port = atoi(argv[2]);
	
	Sock_server serv(ip, port);	
	char msg[MAX_LEN];
	
	while (true)
	{
		int client_id = serv.acept();
		if (!fork()) while (true)
		{
			int len = serv.rcv(client_id, msg);
			std::reverse(msg, msg + len-2);
			serv.snd(client_id, msg, len);
			if (len < 3)
			{
				shutdown(client_id, 0);
				errx(0, "Client served.\n");
			}
		}
	}
		
	shutdown(serv.id, 0);
	return 0;
}
