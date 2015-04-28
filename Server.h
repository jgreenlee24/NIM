#include <WinSock2.h>
#include <string>

static char Nim_UDPPORT[] = "29333";

const int MAX_SEND_BUF  = 2048;
const int MAX_RECV_BUF  = 2048;
const int MAX_SERVERS   = 100;

const int MAXHOSTNAME = 80;
const int TIMEBUF_SIZE = 26;

#define MAXBUF  4096
#define MAXIPBUF  20
#define Nim_QUERY "Name?\0"
#define Nim_CHALLENGE "Play? Name=client_name\0"
#define Nim_ACK "Great!\0"

SOCKET passivesock (char*,char*);
int UDP_recv (SOCKET, char*, int, char*, char*);
int UDP_send (SOCKET, char*, int, char*, char*);
int wait(SOCKET, int, int);
char* GetHostIP();
