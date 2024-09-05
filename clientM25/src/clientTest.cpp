#include "clientTest.h"

#if defined (__linux__)
#elif defined(_WIN64)

void myClient::client_loading()
{
	// For checking errors in sockets functions // Для проверки ошибок в функциях сокетов
	int erStat;

	// WinSock initialization // Инициализация WinSock
	WSADATA wsData;
	erStat = WSAStartup(MAKEWORD(2, 2), &wsData);
	if (erStat != 0) {
		std::cout << "Error WinSock version initializaion #";
		std::cout << WSAGetLastError();
		return;
	}
	else
		std::cout << "WinSock initialization is OK" << std::endl;

	// Socket initialization
	SOCKET clientSock = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSock == INVALID_SOCKET) {
		std::cout << "Error initialization socket # " << WSAGetLastError() << std::endl;
		closesocket(clientSock);
		WSACleanup();
	}
	else
		std::cout << "Client socket initialization is OK" << std::endl;

	// Establishing a connection to Server //Установление соединения с сервером
	sockaddr_in serverAddress;
	ZeroMemory(&serverAddress, sizeof(serverAddress));

	// Установим адрес сервера	
	serverAddress.sin_addr.s_addr = inet_addr(SERVER_IP);
	// Зададим номер порта
	serverAddress.sin_port = htons(SERVER_PORT_NUM);
	// Используем IPv4
	serverAddress.sin_family = AF_INET;

	// Установим соединение с сервером
	erStat = connect(clientSock, (sockaddr*)&serverAddress, sizeof(serverAddress));
	if (erStat != 0) {
		std::cout << "Connection to Server is FAILED. Error # " << WSAGetLastError() << std::endl;
		closesocket(clientSock);
		WSACleanup();
		return;
	}
	else
		std::cout << "Connection established SUCCESSFULLY. Ready to send a message to Server"
		<< "Соединение установлено УСПЕШНО.\nГотово к отправке сообщения на сервер" << std::endl;

	// Buffers for sending and receiving data // Буферы для отправки и получения данных
	std::vector<char> serverBuff(BUFF_SIZE), clientBuff(BUFF_SIZE);
	// The size of sending / receiving packet in bytes // Размер отправляемого/принимаемого пакета в байтах
	short packet_size = 0;

	while (true) {
		//Функция fgets считывает строку из входного аргумента stdin stream и сохраняет ее в clientBuff.data()
		//fgets(clientBuff.data(), clientBuff.size(), stdin);

		clientBuff = { cbegin(m_str), cend(m_str) };

		// Check whether client like to stop chatting 
		if (clientBuff[0] == 'e' && clientBuff[1] == 'x' && clientBuff[2] == 'i' && clientBuff[3] == 't') {
			shutdown(clientSock, SD_BOTH);
			closesocket(clientSock);
			WSACleanup();
			return;
		}
	
		//sending message to server отправка сообщения на сервер
		packet_size = send(clientSock, clientBuff.data(), clientBuff.size(), 0);
		if (packet_size == SOCKET_ERROR) {
			std::cout << "Can't send message to Server. Error # " << WSAGetLastError() << std::endl;
			closesocket(clientSock);
			WSACleanup();
			return;
		}
		
		//очистка вектора serverBuff
		auto it = serverBuff.cbegin();
		serverBuff.insert(it, serverBuff.size(), '\0');
		//receiving message from server прием сообщения с сервера
		packet_size = recv(clientSock, serverBuff.data(), serverBuff.size(), 0);
		if (packet_size == SOCKET_ERROR) {
			std::cout << "Can't receive message from Server. Error # " << WSAGetLastError() << std::endl;
			closesocket(clientSock);
			WSACleanup();
			return;
		}
		else
			std::cout << "Server message: " << serverBuff.data() << std::endl;
		//пробую перейти на сообщения
		if (serverBuff[0] == 'H')
		{
			//std::cout << "testProba!!!!" << std::endl;
			setMyString("u");
			clientBuff = { cbegin(m_str), cend(m_str) };
			//sending message to server отправка сообщения на сервер
			packet_size = send(clientSock, clientBuff.data(), clientBuff.size(), 0);
			if (packet_size == SOCKET_ERROR) {
				std::cout << "Can't send message to Server. Error # " << WSAGetLastError() << std::endl;
				closesocket(clientSock);
				WSACleanup();
				return;
			}
			//очистка вектора serverBuff
			auto it = serverBuff.cbegin();
			serverBuff.insert(it, serverBuff.size(), '\0');
			//receiving message from server прием сообщения с сервера			
			packet_size = recv(clientSock, serverBuff.data(), serverBuff.size(), 0);
			if (packet_size == SOCKET_ERROR) {
				std::cout << "Can't receive message from Server. Error # " << WSAGetLastError() << std::endl;
				closesocket(clientSock);
				WSACleanup();
				return;
			}
			else
				std::cout << "Registered users:\n" << serverBuff.data() << std::endl;
			userMessage(clientSock);
		}

		m_str = "";
		std::cin >> m_str;

		std::cin.clear();
		std::cin.ignore(32767, '\n');

		clientBuff.clear();

		//setMyString("");
		//serverBuff.clear();
	}
}

void myClient::userMessage(SOCKET client_sock) {	
	setMyString("n");
	//запрос имени name request
	std::string result = receptionTransmissionMes(client_sock, m_str);
	std::cout << result << std::endl;
	//отправка имени sending name
	std::string username = getLineOfText();
	result = receptionTransmissionMes(client_sock, username);
	std::cout << result << std::endl;

	//набор сообщения typing a message
	std::string userMessage = getLineOfText();
	result = receptionTransmissionMes(client_sock, userMessage);
	std::cout << result << std::endl;
}

std::string myClient::receptionTransmissionMes(SOCKET client_sock, const std::string& name)
{
	std::vector<char> serverBuff(BUFF_SIZE);

	//sending message to server отправка сообщения на сервер
	if (send(client_sock, name.c_str(), name.size(), 0) == SOCKET_ERROR) {		
		closesocket(client_sock);
		WSACleanup();
		return "Can't send message to Server. Error # ";
	}
	//receiving message from server прием сообщения с сервера	
	if (recv(client_sock, serverBuff.data(), serverBuff.size(), 0) == SOCKET_ERROR) {		
		closesocket(client_sock);
		WSACleanup();
		return "Can't receive message from Server. Error # ";
	}
	std::string str(serverBuff.begin(), serverBuff.end());
	return str;
}

#endif