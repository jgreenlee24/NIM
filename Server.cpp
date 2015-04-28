#include <iostream>
#include <string>
#include <WinSock2.h>
#include "server.h"

using std::string;
using std::cout;
using std::endl;
using std::cin;
using std::getline;

#pragma comment (lib, "ws2_32.lib")

void server_main(int argc, char *argv[], string playerName)
{
	SOCKET s;
	char buf[MAX_RECV_BUF];
	string host;
	string port;
	char response_str[MAX_SEND_BUF];
	char service[MAXBUF] = "29334";
	sockaddr_in	clientSocketInfo;		
	SOCKET listenSocket;			
	SOCKET connectSocket;			
	int	clen = sizeof(clientSocketInfo);	
	char ip_addr[MAXIPBUF];		
	char buffer[MAXBUF];			
	int	len;
	char choice;
	const string Refuse = "NO";
	const string Accept = "Yes";


	cout << "Please enter your name: ";
	getline(cin, playerName);

	WORD wVersion = 0x0202;
	WSADATA wsaData;
	int error = WSAStartup( wVersion, &wsaData );
	if (error)
	{
		cout << "Unable to initialize Windows Socket library." << endl;
		return;
	}

	s = passivesock(Nim_UDPPORT,"udp");

	cout << endl << "Waiting for a challenge..." << endl;
	len = UDP_recv(s, buf, MAX_RECV_BUF, (char*)host.c_str() , (char*)port.c_str());
	cout << "Received: " << buf << endl; 

	bool flag = false;
	while (!flag)
	{
		if (strcmp(buf, Nim_QUERY) == 0)
		{
			UDP_send(s, (char*)playerName.c_str(), playerName.length()+1,
				(char*)host.c_str(), (char*)port.c_str());
		}
		else if (strncmp(buf, Nim_CHALLENGE, strlen(Nim_CHALLENGE)) == 0) 
		{
			char *startOfName = strstr(buf,Nim_CHALLENGE);
			if (startOfName != NULL) 
			{
				cout << endl << "You have been challenged by " << startOfName+strlen(Nim_CHALLENGE) << endl;
			}
			cout << "Accept challenge? Y/N: ";
			cin >> choice;
			if (choice == 'N' || choice == 'n')
			{
				UDP_send(s, (char*)Refuse.c_str(), Refuse.length()+1,
					(char*)host.c_str(), (char*)port.c_str());
			}
			else if (choice == 'Y' || choice == 'y')
			{
				listenSocket = passivesock(service,"TCP");
				UDP_send(s, (char*)Accept.c_str(), Accept.length()+1,
					(char*)host.c_str(), (char*)port.c_str());
				if (wait(s, 5, 0))
				{
					int len = UDP_recv(s, buf, MAX_RECV_BUF, (char*)host.c_str() , (char*)port.c_str());
					if (strcmp(buf, Nim_ACK) == 0)
					{
						closesocket(s);
						connectSocket = accept(listenSocket,(LPSOCKADDR) &clientSocketInfo, &clen);
						strcpy_s(ip_addr, MAXIPBUF, inet_ntoa(clientSocketInfo.sin_addr));
						cout << "Connected...";
						flag = true;
					}
				}
				else
				{
					closesocket(listenSocket);
				}
			}
		}
		cout << endl << "Waiting for a challenge..." << endl;
		len = UDP_recv(s, buf, MAX_RECV_BUF, (char*)host.c_str() , (char*)port.c_str());
		cout << "Received: " << buf << endl; 
	}
}
