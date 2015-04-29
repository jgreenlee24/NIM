// nim.h    Header file for Nim project

#include <winsock2.h>
#include <string>

static char BROADCAST_ADDR[]	= "10.1.255.255";
static char UDPPORT_NIM[]		= "29333";
static char TCPPORT_NIM[]		= "29334";

const int MAX_SEND_BUF     = 2048;
const int MAX_RECV_BUF     = 2048;
const int MAX_HOSTNAME     = 1024;
const int MAX_HOST_SIZE    = 16;
const int MAX_SERVICE_SIZE = 80;
const int MAX_LINE		   = 1024;
const int MAX_NAME	       = 1024;
const int MAX_HOST         = 100;

struct ServerStruct {
	std::string name;
	std::string host;
	std::string port;
};

void   chomp (char*);
char*  timestamp(void);
void   server_main(int,char*[]);
void   client_main(int,char*[]);
void   play_Nim(SOCKET, bool, char*);
void   getServers(SOCKET, char*	, char*, ServerStruct*, int&);
int    UDP_recv(SOCKET, char*, int, char*, char*);
int    UDP_send (SOCKET, char[], int, char[], char[]);
int    wait(SOCKET, int, int);

SOCKET passivesock (char*, char*);
SOCKET connectsock (char*, char*, char*);
