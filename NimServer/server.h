#include <winsock2.h>

static char BROADCAST_ADDR[] = "10.1.255.255";
static char UDPPORT_NIM[] = "29333";
static char TCPPORT_NIM[] = "29334";

const int MAX_SEND_BUF     = 2048;
const int MAX_RECV_BUF     = 2048;
const int MAX_HOSTNAME     = 1024;
const int MAX_HOST_SIZE    = 16;
const int MAX_SERVICE_SIZE = 80;
const int MAX_LINE      = 1024;
const int MAX_NAME      = 1024;
const int MAX_HOST      = 100;

#define MAXBUF  4096
#define MAXIPBUF  20
#define Nim_QUERY "Name?\0"
#define Nim_CHALLENGE "Play?\0"
#define Nim_ACK "GREAT!\0"

void   chomp (char*);
char*  timestamp(void);
void   server_main(int,char*[]);
void   client_main(int,char*[]);
void   play_Nim(SOCKET, bool, char*);
int    UDP_recv(SOCKET, char*, int, char*, char*);
int    UDP_send (SOCKET, char*, int, char*, char*);
int    wait(SOCKET, int, int);
SOCKET passivesock (char*, char*);
SOCKET connectsock (char*, char*, char*);
void play();



