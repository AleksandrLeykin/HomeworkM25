#pragma once

#include <iostream>
#include <vector>


#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h> //printf

#pragma comment(lib, "Ws2_32.lib")

// receiving and transmitting a message ����� �������� ���������
std::string recAndTransMess(SOCKET client_sock, const std::string& str, char buff[BUFSIZ]);

//name verification ������ �����
bool nameVerification(SOCKET client_sock, char buff[BUFSIZ]);

//display received messages ����� �������� ���������
std::string receivedMessages(const std::string& name);

//write message to table ������ �oo������ � ������� 
std::string writingMessage(const std::string& name1, const std::string& name2, const std::string& strMes);
DWORD WINAPI SetToClient(LPVOID  client_socket);

//������ ��� ������ ���������� �������� �������������
#define PRINTUSERS if (nclients) std::cout << nclients << " - User on-line" << std::endl; \
						else std::cout << "No user on line\n";

//Key constants
const char IP_SERV[] = "127.0.0.1";
const int PORT_NUM = 7777;
const short BUFF_SIZE = 1024;

class m_server {
private:
	//�������� ������
	SOCKET server_socket{};
	SOCKET client_socket{};
	//������������� ���������� �������
	WSAData wsData = {0};	

	//��������_�������
	void server_loading();	
	void close_server();	

public:
	m_server();
	~m_server() { close_server(); }

 // DWORD WINAPI SetToClient(LPVOID  client_socket);
};