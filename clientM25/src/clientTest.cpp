#include "clientTest.h"

#if defined (__linux__)
#elif defined(_WIN64)

void myClient::client_loading()
{
	// For checking errors in sockets functions // ��� �������� ������ � �������� �������
	int erStat;

	// WinSock initialization // ������������� WinSock
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

	// Establishing a connection to Server //������������ ���������� � ��������
	sockaddr_in serverAddress;
	ZeroMemory(&serverAddress, sizeof(serverAddress));

	// ��������� ����� �������	
	serverAddress.sin_addr.s_addr = inet_addr(SERVER_IP);
	// ������� ����� �����
	serverAddress.sin_port = htons(SERVER_PORT_NUM);
	// ���������� IPv4
	serverAddress.sin_family = AF_INET;

	// ��������� ���������� � ��������
	erStat = connect(clientSock, (sockaddr*)&serverAddress, sizeof(serverAddress));
	if (erStat != 0) {
		std::cout << "Connection to Server is FAILED. Error # " << WSAGetLastError() << std::endl;
		closesocket(clientSock);
		WSACleanup();
		return;
	}
	else
		std::cout << "Connection established SUCCESSFULLY. Ready to send a message to Server"
		<< "���������� ����������� �������.\n������ � �������� ��������� �� ������" << std::endl;

	// Buffers for sending and receiving data // ������ ��� �������� � ��������� ������
	std::vector<char> serverBuff(BUFF_SIZE), clientBuff(BUFF_SIZE);
	// The size of sending / receiving packet in bytes // ������ �������������/������������ ������ � ������
	short packet_size = 0;


	while (true) {
		//������� fgets ��������� ������ �� �������� ��������� stdin stream � ��������� �� � clientBuff.data()
		//fgets(clientBuff.data(), clientBuff.size(), stdin);

		clientBuff = { cbegin(m_str), cend(m_str) };

		// Check whether client like to stop chatting 
		if (clientBuff[0] == 'e' && clientBuff[1] == 'x' && clientBuff[2] == 'i' && clientBuff[3] == 't') {
			shutdown(clientSock, SD_BOTH);
			closesocket(clientSock);
			WSACleanup();
			return;
		}
		//sending message to server �������� ��������� �� ������
		packet_size = send(clientSock, clientBuff.data(), clientBuff.size(), 0);
		if (packet_size == SOCKET_ERROR) {
			std::cout << "Can't send message to Server. Error # " << WSAGetLastError() << std::endl;
			closesocket(clientSock);
			WSACleanup();
			return;
		}
		//receiving message from server ����� ��������� � �������
		packet_size = recv(clientSock, serverBuff.data(), serverBuff.size(), 0);
		if (packet_size == SOCKET_ERROR) {
			std::cout << "Can't receive message from Server. Error # " << WSAGetLastError() << std::endl;
			closesocket(clientSock);
			WSACleanup();
			return;
		}
		else
			std::cout << "Server message: " << serverBuff.data() << std::endl;
		//������ ������� �� ���������
		if (serverBuff[0] == 'H')
		{
			//std::cout << "testProba!!!!" << std::endl;
			setMyString("u");
			clientBuff = { cbegin(m_str), cend(m_str) };
			//sending message to server �������� ��������� �� ������
			packet_size = send(clientSock, clientBuff.data(), clientBuff.size(), 0);
			if (packet_size == SOCKET_ERROR) {
				std::cout << "Can't send message to Server. Error # " << WSAGetLastError() << std::endl;
				closesocket(clientSock);
				WSACleanup();
				return;
			}
			//receiving message from server ����� ��������� � �������
			packet_size = recv(clientSock, serverBuff.data(), serverBuff.size(), 0);
			if (packet_size == SOCKET_ERROR) {
				std::cout << "Can't receive message from Server. Error # " << WSAGetLastError() << std::endl;
				closesocket(clientSock);
				WSACleanup();
				return;
			}
			else
				std::cout << "Registered users:\n" << serverBuff.data() << std::endl;
			userMessage();
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

void myClient::userMessage()
{
	std::cout << "Who should I send the message to? ���� ��������� ���������?" << std::endl;
	std::string username = getLineOfText();
	std::cout << "Type a message! for - �������� ���������! ��� -  " << username << std::endl;
	std::string userMessage = getLineOfText();

	std::cout << "message - " << userMessage << std::endl;
}

#endif