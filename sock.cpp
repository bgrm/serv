#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <err.h>
#include <cstring>
#include <iostream>
#include "sock.h"

/***********************************	SOCKET_T	**************************************/

Socket_t :: Socket_t()
{	memset(&addr, 0, sizeof(addr));	}

Socket_t :: Socket_t(const char* ip, short port) : Socket_t()
{
	init_addr(ip, port);
	init_id();
}

Socket_t :: Socket_t(string& ip, short port)
: Socket_t(ip.c_str(), port)
{}

void Socket_t :: init_addr(const char* ip, short port)
{
	addr = 
	{
		.sin_family = AF_INET,
		.sin_port = htons(port)
	};
	if (inet_pton(AF_INET, ip, &addr.sin_addr) <= 0)
		errx(EXIT_FAILURE, "invalid ip address: %s. errno: %s\n", ip, strerror(errno));
}

void Socket_t :: init_id()
{
	id = socket(AF_INET, SOCK_STREAM, 0);
	if (id < 0)
		errx(EXIT_FAILURE, "socket value: %d. errno: %s\n", id, strerror(errno));
}

int Socket_t :: rcv(int sock, char* msg)
{
	int len = recv(sock, msg, MAX_LEN, 0);
	if (len < 0)
		errx(EXIT_FAILURE, "recv error. errno: %s\n", strerror(errno));
	return len;
}

void Socket_t :: snd(int sock, char* msg, int len)
{
	if (send(sock, msg, len, 0) < 0)
		errx(EXIT_FAILURE, "send error. errno: %s\n", strerror(errno));			
}


/***********************************	SOCK_SERVER		**************************************/


Sock_server :: Sock_server(const char* ip, short port, int limit)
: Socket_t(ip, port)
{	
	//addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if (bind(id, (sockaddr*) &addr, sizeof(addr)) < 0)
		errx(EXIT_FAILURE, "binding error. errno: %s\n", strerror(errno));
		
	if (listen(id, limit))
		errx(EXIT_FAILURE, "unable to listen. errno: %s\n", strerror(errno));
}

Sock_server :: Sock_server(string& ip, short port, int limit)
: Sock_server(ip.c_str(), port, limit)
{}

int Sock_server :: acept()
{
	sockaddr client_addr;
	socklen_t len = sizeof(client_addr);
	
	int client_id = accept(id, &client_addr, &len);
	if (client_id < 0)
		errx(EXIT_FAILURE, "accept error: %d. errno: %s\n", client_id, strerror(errno));

	return client_id;
}

int Sock_server :: rcv(int client, char* msg)
{	return Socket_t :: rcv(client, msg);	}

void Sock_server :: snd(int client, char* msg, int len)
{	Socket_t :: snd(client, msg, len);	}


/***********************************	SOCK_CLIENT		**************************************/

void Sock_client :: conn()
{
	int er = connect(id, (sockaddr*) &addr, sizeof(addr));
	if (er < 0)
		std::cout << strerror(errno), errx(EXIT_FAILURE, "connect error: %d\n", er);
}

Sock_client :: Sock_client(const char* ip, short port)
: Socket_t(ip, port)
{}

Sock_client :: Sock_client(string& ip, short port)
: Sock_client(ip.c_str(), port)
{}

int Sock_client :: rcv(char* msg)
{	return Socket_t :: rcv(id, msg);	}

void Sock_client :: snd(char* msg, int len)
{	Socket_t :: snd(id, msg, len);	}
