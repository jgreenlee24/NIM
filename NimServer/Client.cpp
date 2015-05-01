// client_main.cpp
//   This function serves as the "main" function for the client-side
#include "NIM.h"
#include <iostream>
#include <string>
#include <WinSock2.h>

void client_main(int argc, char *argv[])
{
	std::string host;
	std::string port;
	ServerStruct server[MAX_HOST];
	std::string playerName;

	SOCKET s = connectsock("","","udp");
	
	std::cout << "What is your name?" << std::endl;
	std::getline(std::cin, playerName);
	std::cout << std::endl << "Looking for NIM servers ... " << std::endl;
	int numServers = 0;
	getServers(s, BROADCAST_ADDR, UDPPORT_NIM, server, numServers);

	if (numServers == 0) 
	{
		std::cout << std::endl << "Sorry.  No NIM servers were found.  Try again later." << std::endl << std::endl;
	} 
	else 
	{
		std::cout << std::endl << "Found NIM server";
		if (numServers == 1) 
		{
			std::cout << ":" << "  " << server[0].name << std::endl;
		} 
		else
		{
			std::cout << "s:" << std::endl;
			for (int i=0; i<numServers; i++) 
			{
				std::cout << "  " << i+1 << " - " << server[i].name << std::endl;
			}
			std::cout << std::endl << "  " << numServers+1 << " - QUIT" << std::endl;
		}
		std::cout << std::endl;

		int answer = 0;
		std::string answer_str;
		if (numServers == 1) 
		{
			std::cout << "Do you want to challenge " << server[0].name << "? ";
			std::getline(std::cin, answer_str);
			if (answer_str[0] == 'y' || answer_str[0] == 'Y')
			{
				answer = 1;
			}
		} 
		else if (numServers > 1)
		{
			std::cout << "Who would you like to challenge (1-" << numServers+1 << ")? ";
			std::getline(std::cin,answer_str);
			answer = atoi(answer_str.c_str());
			if (answer > numServers) 
			{
				answer = 0;
			}
		}
			
		if (answer >= 1 && answer <= numServers) 
		{
			// Extract the opponent's info from the server[] array
			std::string serverName;
			serverName = server[answer-1].name;		// Adjust for 0-based array
			host = server[answer-1].host;
			port = server[answer-1].port;

			// Append playerName to the TicTacToe_CHALLENGE string & send a challenge to host:port
			char buf[MAX_SEND_BUF];
			strcpy_s(buf,"Play? Name=");
			strcat_s(buf,playerName.c_str());
			strcat_s(buf,"\0");
			int len = UDP_send(s, buf, strlen(buf)+1, (char*)host.c_str(), (char*)port.c_str());

			int w = wait(s, 20, 0);

			if(w == 1)
			{
				len = UDP_recv(s, buf, MAX_RECV_BUF, (char*)host.c_str(), (char*)port.c_str());
				if(strcmp(buf, "YES") == 0 )
				{
					strcpy_s(buf,"GREAT!\0");
					len = UDP_send(s, buf, strlen(buf)+1, (char*)host.c_str(), (char*)port.c_str());
					closesocket(s);

					SOCKET s = connectsock((char*)host.c_str(),"29334","tcp");
					len = recv(s, buf, MAXBUF, 0);
					//play_Nim(s, buf);

				}
				else
				{
					std::cout << "NO RESPONSE" << std::endl;
				}
			}
			else
			{
				std::cout << "NO RESPONSE" << std::endl;
			}
		}
	}

	closesocket(s);
}

