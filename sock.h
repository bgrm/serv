#include <string>
using std::string;

const int MAX_LEN = 1 << 8;

struct Socket_t
{
	sockaddr_in addr;
	int id;
	
	Socket_t();
	Socket_t(const char* ip, short port);
	Socket_t(string& ip, short port);
	
	void init_addr(const char* ip, short port);
	void init_id();
	
	int rcv(int sock, char* msg);
	
	void snd(int sock, char* msg, int len);
};

struct Sock_server : Socket_t
{
	Sock_server(const char* ip, short port, int limit = 10);
	Sock_server(string& ip, short port, int limit = 10);
	
	int acept();
	
	int rcv(int client, char* msg);
	
	void snd(int client, char* msg, int len);
};

struct Sock_client : Socket_t
{
	Sock_client(const char* ip, short port);
	Sock_client(string& ip, short port);
	
	void conn();
	
	int rcv(char* msg);
	
	void snd(char* msg, int len);
};
