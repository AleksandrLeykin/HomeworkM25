#pragma once
#include <iostream>
#include <vector>
#include <string>

//#include "myChat.h"

//Key constants
// Enter IPv4 address of Server
const char SERVER_IP[] = "127.0.0.1";
// Enter Listening port on Server side
const short SERVER_PORT_NUM = 7777;
// Maximum size of buffer for exchange info between server and client
const short BUFF_SIZE = 1024;

#if defined (__linux__)

#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void client_loading() {
	// Создадание сокета
	int socket_fail_descriptor = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fail_descriptor == -1) {
		std::cout << "Creation of Socket failed!" << std::endl;
		return;
	}
	// Установим адрес сервера
	struct sockaddr_in serverAddress;
	serverAddress.sin_addr.s_addr = inet_addr(SERVER_IP);
	// Зададим номер порта
	serverAddress.sin_port = htons(SERVER_PORT_NUM);
	// Используем IPv4
	serverAddress.sin_family = AF_INET;

	// Установим соединение с сервером
	int server_connect = connect(socket_fail_descriptor, (sockaddr*)&serverAddress, sizeof(serverAddress));
	if (server_connect == -1) {
		std::cout << "Connection with the server failed!" << std::endl;
		return;
	}
	// Взаимодействие с сервером
	}

#elif defined(_WIN64)

#include <WinSock2.h>;
#include <WS2tcpip.h>
#include <stdio.h>
//#include <inaddr.h>

#pragma comment(lib, "Ws2_32.lib")


class myClient {
 public:
	//client_loading();
	myClient() { }
	~myClient() { WSACleanup(); }

	void setMyString(const std::string& str) {
		m_str = str;
	}
	void client_loading();
private:
	std::string m_str = "";
	void userMessage(SOCKET client_sock);

	std::string receptionTransmissionMes(SOCKET client_sock, const std::string& name);

	//ввод строки (line input)
	std::string getLineOfText() {
		std::string str = "";
		getline(std::cin, str);		
 		return str;
	}
};

#endif