#pragma once
#include <iostream>
#include <vector>

#define _WINSOCK_DEPRECATED_NO_WARNINGS

//заголовочный файл, содержащий актуальные реализации функций для работы с сокетами.
#include <WinSock2.h>
//заголовочный файл, который содержит различные программные интерфейсы, 
// связанные с работой протокола TCP/IP (переводы различных данных в формат, понимаемый протоколом и т.д.).
#include <WS2tcpip.h>
#include <stdio.h> //printf
//прилинковka к приложению динамическую библиотеку ядра ОС: ws2_32.dll. 
// Делаем это через директиву компилятору: #pragma comment
#pragma comment(lib, "Ws2_32.lib")

#include "mySQLTest.h"

// макрос для печати количества активных пользователей
#define PRINTUSERS if (nclients) std::cout << nclients << " - User on-line" << std::endl; \
						else std::cout << "No user on line\n";

//Key constants
const char IP_SERV[] = "127.0.0.1";
const int PORT_NUM = 7777;
const short BUFF_SIZE = 1024;
// глобальная переменная - количество активных пользователей
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
		// Шаг 1 - Инициализация Библиотеки Сокетов
		WSADATA wsData = {0};
		
		//переменная erStat для анализирования ошибок при подключении
		int erStat = WSAStartup(MAKEWORD(2, 2), &wsData);
		if (erStat != 0) {
			std::cout << "Error Winsock initialisation #" << WSAGetLastError();
			return;
		}
		//else	std::cout << "WinSock initialisation is ok!" << std::endl;
		
		// Шаг 2 - создание сокета
		SOCKET ServerSock = socket(AF_INET, SOCK_STREAM, 0);
		if (ServerSock == INVALID_SOCKET) {
			std::cout << "Error initialization socket # " << WSAGetLastError() << std::endl;
			closesocket(ServerSock);
			WSACleanup();
			return;
		}
		// Шаг 3 - связывание сокета с локальным адресом
		sockaddr_in server_addr;
		ZeroMemory(&server_addr, sizeof(server_addr));

		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(PORT_NUM);
		//сервер принимает подключения на все свои IP-адреса
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
		// Шаг 4 - ожидание подключений размер очереди - 0x100
		//listen() - функция для прослушки подключающихся "клиентов"
		erStat = listen(ServerSock, 0X100);
		if (erStat != 0) {
			std::cout << "Can`t start to listen to. Error #" << WSAGetLastError() << std::endl;
			closesocket(ServerSock);
			WSACleanup();
			return;
		}
		else
			std::cout << "Listening...\n" << std::endl;
		// Шаг 5 - извлекаем сообщение из очереди
		SOCKET clientSocket; // сокет для клиента
		sockaddr_in client_addr;
		ZeroMemory(&clientSocket, sizeof(clientSocket));

		// функции accept необходимо передать размер структуры
		int client_addr_size = sizeof(client_addr);
		// цикл извлечения запросов на подключение из очереди
		while ((clientSocket = accept(ServerSock, (sockaddr*)&client_addr, &client_addr_size))) {
			nclients++; // увеличиваем счетчик подключившихся клиентов 

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

			// пытаемся получить имя хоста
			//Структура hostent используется функциями для хранения информации о данном хосте, 
			// такой как имя хоста, адрес IPv4 и т. д.
			HOSTENT* hst;		
			
			hst = gethostbyaddr((char*)&client_addr.sin_addr.s_addr, 4, AF_INET);

			// вывод сведений о клиенте
			//Функция inet_ntoa преобразует сетевой адрес Интернета (IPv4) в строку ASCII в стандартном формате 
			// Интернета с разделительными точками.
			std::cout << hst->h_name << " " << inet_ntoa(client_addr.sin_addr) << " new connect!\n";
			PRINTUSERS;
			// Вызов нового потока для обслужвания клиента. Да, для этого рекомендуется использовать _beginthreadex
			// но, т.к. никаких вызовов функций стандартной Си библиотеки поток не делает, можно обойтись и CreateThread
			DWORD thid;
			CreateThread(NULL, NULL, SexToClient, &clientSocket, NULL, &thid);
		}
	}
};

DWORD WINAPI SexToClient(LPVOID client_socket) {
	SOCKET my_sock = ((SOCKET*)client_socket)[0];
	char buff[BUFSIZ]; //char buff[20 * 1024];

#define sHELLO "SOCKET CONNECTION!\r\n"
	// отправляем клиенту приветствие
	
	send(my_sock, sHELLO, sizeof(sHELLO), 0);

	// цикл эхо-сервера: прием строки от клиента и возвращение ее клиенту
	//std::vector<char> servBuff(BUFF_SIZE), clientBuff(BUFF_SIZE);

	int bytes_recv = 0;
	while ((bytes_recv = recv(my_sock, &buff[0], sizeof(buff), 0)) && bytes_recv != SOCKET_ERROR) {

		if (buff[0] == '1')
		{
			mySQLTest mysql;
			std::string result = mysql.getUser();
			send(my_sock, result.c_str(), result.length(), 0);
		}
		//возврат сообщения пользователю
		//send(my_sock, &buff[0], bytes_recv, 0);
	}
	// если мы здесь, то произошел выход из цикла по причине
   // возращения функцией recv ошибки - соединение с клиентом разорвано
	nclients--; // уменьшаем счетчик активных клиентов
	std::cout << "-disconnect\n"; PRINTUSERS

		// закрываем сокет
		closesocket(my_sock);
	return 0;
}