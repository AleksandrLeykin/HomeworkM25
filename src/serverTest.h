#pragma once
#include <iostream>
#include <vector>

#define _WINSOCK_DEPRECATED_NO_WARNINGS

//������������ ����, ���������� ���������� ���������� ������� ��� ������ � ��������.
#include <WinSock2.h>
//������������ ����, ������� �������� ��������� ����������� ����������, 
// ��������� � ������� ��������� TCP/IP (�������� ��������� ������ � ������, ���������� ���������� � �.�.).
#include <WS2tcpip.h>
#include <stdio.h> //printf
//���������ka � ���������� ������������ ���������� ���� ��: ws2_32.dll. 
// ������ ��� ����� ��������� �����������: #pragma comment
#pragma comment(lib, "Ws2_32.lib")

#include "mySQLTest.h"

// ������ ��� ������ ���������� �������� �������������
#define PRINTUSERS if (nclients) std::cout << nclients << " - User on-line" << std::endl; \
						else std::cout << "No user on line\n";

//Key constants
const char IP_SERV[] = "127.0.0.1";
const int PORT_NUM = 7777;
const short BUFF_SIZE = 1024;
// ���������� ���������� - ���������� �������� �������������
int nclients = 0;

DWORD WINAPI SexToClient(LPVOID client_socket);

class my_server {
private:
	mySQLTest mySql;
public:
	my_server() {
		
		server_loading();
	}
	~my_server() {
		if (WSACleanup() != 0)
		{
			std::cerr << "Error destructor!" << WSAGetLastError() << std::endl;
		}
		else std::cout << "OK destructor!" << std::endl;
	}
private:	
	void server_loading() {
		// ��� 1 - ������������� ���������� �������
		WSADATA wsData = {0};
		
		//���������� erStat ��� �������������� ������ ��� �����������
		int erStat = WSAStartup(MAKEWORD(2, 2), &wsData);
		if (erStat != 0) {
			std::cout << "Error Winsock initialisation #" << WSAGetLastError();
			return;
		}
		//else	std::cout << "WinSock initialisation is ok!" << std::endl;
		
		// ��� 2 - �������� ������
		SOCKET ServerSock = socket(AF_INET, SOCK_STREAM, 0);
		if (ServerSock == INVALID_SOCKET) {
			std::cout << "Error initialization socket # " << WSAGetLastError() << std::endl;
			closesocket(ServerSock);
			WSACleanup();
			return;
		}
		// ��� 3 - ���������� ������ � ��������� �������
		sockaddr_in server_addr;
		ZeroMemory(&server_addr, sizeof(server_addr));

		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(PORT_NUM);
		//������ ��������� ����������� �� ��� ���� IP-������
		server_addr.sin_addr.s_addr = 0;

		erStat = bind(ServerSock, (sockaddr*)&server_addr, sizeof(server_addr));
		if (erStat != 0) {
			std::cout << "Error Socket binding to server info. Error #" << WSAGetLastError() << std::endl;
			closesocket(ServerSock);
			WSACleanup();
			return;
		}
		else
			std::cout << "Binding socket to Server info is OK" << std::endl;
		// ��� 4 - �������� ����������� ������ ������� - 0x100
		//listen() - ������� ��� ��������� �������������� "��������"
		erStat = listen(ServerSock, 0X100);
		if (erStat != 0) {
			std::cout << "Can`t start to listen to. Error #" << WSAGetLastError() << std::endl;
			closesocket(ServerSock);
			WSACleanup();
			return;
		}
		else
			std::cout << "Listening...\n" << std::endl;
		// ��� 5 - ��������� ��������� �� �������
		SOCKET clientSocket; // ����� ��� �������
		sockaddr_in client_addr;
		ZeroMemory(&clientSocket, sizeof(clientSocket));

		// ������� accept ���������� �������� ������ ���������
		int client_addr_size = sizeof(client_addr);
		// ���� ���������� �������� �� ����������� �� �������
		while ((clientSocket = accept(ServerSock, (sockaddr*)&client_addr, &client_addr_size))) {
			nclients++; // ����������� ������� �������������� �������� 

		/*	char hostname[NI_MAXHOST];
			char servInfo[NI_MAXSERV];
			DWORD dwRetval;

			dwRetval = getnameinfo((struct sockaddr*)&client_addr, sizeof(struct sockaddr*),
									hostname, NI_MAXHOST, servInfo, NI_MAXSERV, NI_NUMERICSERV);
			if (dwRetval != 0) {
				std::cout << "getnameinfo failed with error # " << WSAGetLastError() << std::endl;
			}
			else {
				std::cout << hostname << servInfo << " new connect!\n " << std::endl;
				PRINTUSERS;
			}*/

			// �������� �������� ��� �����
			//��������� hostent ������������ ��������� ��� �������� ���������� � ������ �����, 
			// ����� ��� ��� �����, ����� IPv4 � �. �.
			HOSTENT* hst;		
			
			hst = gethostbyaddr((char*)&client_addr.sin_addr.s_addr, 4, AF_INET);

			// ����� �������� � �������
			//������� inet_ntoa ����������� ������� ����� ��������� (IPv4) � ������ ASCII � ����������� ������� 
			// ��������� � ��������������� �������.
			std::cout << hst->h_name << " " << inet_ntoa(client_addr.sin_addr) << " new connect!\n";
			PRINTUSERS;
			// ����� ������ ������ ��� ����������� �������. ��, ��� ����� ������������� ������������ _beginthreadex
			// ��, �.�. ������� ������� ������� ����������� �� ���������� ����� �� ������, ����� �������� � CreateThread
			DWORD thid;
			CreateThread(NULL, NULL, SexToClient, &clientSocket, NULL, &thid);
		}
	}
};

DWORD WINAPI SexToClient(LPVOID client_socket) {
	SOCKET my_sock = ((SOCKET*)client_socket)[0];
	char buff[BUFSIZ]; //char buff[20 * 1024];

#define sHELLO "SOCKET CONNECTION!\r\n"
	// ���������� ������� �����������
	
	send(my_sock, sHELLO, sizeof(sHELLO), 0);

	// ���� ���-�������: ����� ������ �� ������� � ����������� �� �������
	//std::vector<char> servBuff(BUFF_SIZE), clientBuff(BUFF_SIZE);

	int bytes_recv = 0;
	while ((bytes_recv = recv(my_sock, &buff[0], sizeof(buff), 0)) && bytes_recv != SOCKET_ERROR) {

		if (buff[0] == '1')
		{
			mySQLTest mysql;
			std::string result = mysql.getUser();
			send(my_sock, result.c_str(), result.length(), 0);
		}
		//������� ��������� ������������
		//send(my_sock, &buff[0], bytes_recv, 0);
	}
	// ���� �� �����, �� ��������� ����� �� ����� �� �������
   // ���������� �������� recv ������ - ���������� � �������� ���������
	nclients--; // ��������� ������� �������� ��������
	std::cout << "-disconnect\n"; PRINTUSERS

		// ��������� �����
		closesocket(my_sock);
	return 0;
}