#include <iostream>
#include <string>
#include <WinSock2.h>
#include "server.h"
#include "AI.h"

using std::string;
using std::cout;
using std::endl;
using std::cin;
using std::getline;
using std::to_string;
using std::stoi;

#pragma comment (lib, "ws2_32.lib")

void playGame(SOCKET connectSocket)
{
	string board = "";
	int temp;
	const string leadZero = "0";

	cout << "How many rock pile? (3-9 piles) ";
	cin >> temp;
	cout << endl;
	while (temp < 3 || temp > 9)
	{
		cout << "Wrong value. Please input a number between 3 and 9: ";
		cin >> temp;
		cout << endl;
	}
	board = to_string(temp);
	int x = 0;
	for (int i = 1; i <= temp; i++)
	{
		cout << "How many rocks in pile " << i << "? (1-20): ";
		cin >> x;
		while (x < 1 || x > 20)
		{
			cout << "Wrong value. Please input a number between 1 and 20: ";
			cin >> x;
			cout << endl;
		}
		if (x < 10)
		{
			board += leadZero;
			board += to_string(x);
		}
		else
		{
			board += to_string(x);
		}
	}
	send(connectSocket, (char*)board.c_str(), board.length() + 1, 0);
	play();
}


void server_main(int argc, char *argv[], string playerName)
{
	SOCKET s;
	char buf[MAX_RECV_BUF];
	string host;
	string port;
	char response_str[MAX_SEND_BUF];
	sockaddr_in	clientSocketInfo;
	SOCKET listenSocket;
	SOCKET connectSocket;
	int	clen = sizeof(clientSocketInfo);
	char ip_addr[MAXIPBUF];
	char buffer[MAXBUF];
	int	len;
	char choice;
	const string Refuse = "NO";
	const string Accept = "YES";


	cout << "Please enter your name: ";
	getline(cin, playerName);

	WORD wVersion = 0x0202;
	WSADATA wsaData;
	int error = WSAStartup(wVersion, &wsaData);
	if (error)
	{
		cout << "Unable to initialize Windows Socket library." << endl;
		return;
	}

	s = passivesock(UDPPORT_NIM, "udp");

	cout << endl << "Waiting for a challenge..." << endl;
	len = UDP_recv(s, buf, MAX_RECV_BUF, (char*)host.c_str(), (char*)port.c_str());

	bool flag = false;
	while (!flag)
	{
		if (strcmp(buf, Nim_QUERY) == 0)
		{
			UDP_send(s, (char*)playerName.c_str(), playerName.length() + 1,
				(char*)host.c_str(), (char*)port.c_str());

			len = UDP_recv(s, buf, MAX_RECV_BUF, (char*)host.c_str(), (char*)port.c_str());

			if (strncmp(buf, Nim_CHALLENGE, strlen(Nim_CHALLENGE)) == 0)
			{
				char *startOfName = strstr(buf, Nim_CHALLENGE);
				if (startOfName != NULL)
				{
					cout << endl << "You have been challenged by " << startOfName + strlen(Nim_CHALLENGE) << endl;
				}
				cout << "Accept challenge? Y/N: ";
				cin >> choice;
				if (choice == 'N' || choice == 'n')
				{
					UDP_send(s, (char*)Refuse.c_str(), Refuse.length() + 1,
						(char*)host.c_str(), (char*)port.c_str());
				}
				else if (choice == 'Y' || choice == 'y')
				{
					listenSocket = passivesock(TCPPORT_NIM, "TCP");
					UDP_send(s, (char*)Accept.c_str(), Accept.length() + 1,
						(char*)host.c_str(), (char*)port.c_str());
					if (wait(s, 5, 0))
					{
						int len = UDP_recv(s, buf, MAX_RECV_BUF, (char*)host.c_str(), (char*)port.c_str());
						if (strcmp(buf, Nim_ACK) == 0)
						{
							closesocket(s);
							connectSocket = accept(listenSocket, (LPSOCKADDR)&clientSocketInfo, &clen);
							strcpy_s(ip_addr, MAXIPBUF, inet_ntoa(clientSocketInfo.sin_addr));
							cout << "Connected!!" << endl;
							playGame(connectSocket);
						}
						else
						{
							closesocket(listenSocket);
						}
					}
				}
			}
		}
	}
}