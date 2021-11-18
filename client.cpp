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
	
	Sock_client client(ip, port);
	client.conn();
	char msg[MAX_LEN];
	
	while (true)
	{
		fgets(msg, MAX_LEN, stdin);
		int len = strlen(msg);
		msg[len++] = '\r', msg[len++] = '\n', msg[len] = '\0';
		client.snd(msg, len);
		len = client.rcv(msg);
		printf("%s", msg);
		
		if (len < 4)
			break;		
	}
	shutdown(client.id, 0);
	return 0;
}
