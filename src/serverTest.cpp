#include "serverTest.h"
#include "mySQLTest.h"

//переменная - количество активных пользователей
int nclients = 0;
//имя получателя сообщения message recipient name
std::string messageRecipientName = "";

m_server::m_server() {
	server_loading();
}

void m_server::close_server() {
	closesocket(server_socket);
	WSACleanup();
	std::cout << "Server was stoped. You can close app" << std::endl;
}

void m_server::server_loading() {
	//переменная erStat для анализирования ошибок при подключении
	int erStat = WSAStartup(MAKEWORD(2, 2), &wsData);
	if (erStat != 0) {
		std::cout << "Error Winsock initialisation #" << WSAGetLastError();
		return;
	}
	else	std::cout << "WinSock initialisation is ok!" << std::endl;

	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == INVALID_SOCKET) {
		std::cout << "Error initialization socket # " << WSAGetLastError() << std::endl;
		close_server();
		return;
	}
	//связывание сокета с локальным адресом
	sockaddr_in server_addr;
	ZeroMemory(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT_NUM);
	//сервер принимает подключения на все свои IP-адреса
	server_addr.sin_addr.s_addr = INADDR_ANY;

	erStat = bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr));
	if (erStat != 0) {
		std::cout << "Error Socket binding to server info. Error #" << WSAGetLastError() << std::endl;
		close_server();
		return;
	}
	else
		std::cout << "Binding socket to Server info is OK" << std::endl;

	//ожидание подключений размер очереди - 0x100
	erStat = listen(server_socket, 0X100);
	if (erStat != 0) {
		std::cout << "Can`t start to listen to. Error #" << WSAGetLastError() << std::endl;
		close_server();
		return;
	}
	else {
		std::cout << "Listening...\n" << std::endl;
	}
	//извлекаем сообщение из очереди
//SOCKET clientSocket; // сокет для клиента
	sockaddr_in client_addr{};
	ZeroMemory(&client_socket, sizeof(client_socket));

	// функции accept необходимо передать размер структуры
	int client_addr_size = sizeof(client_addr);	
	
	// цикл извлечения запросов на подключение из очереди
	while ((client_socket = accept(server_socket, (sockaddr*)&client_addr, &client_addr_size))) {
		nclients++; // увеличиваем счетчик подключившихся клиентов 

		// пытаемся получить имя хоста. Структура hostent используется функциями для хранения 
		// информации о данном хосте, такой как имя хоста, адрес IPv4 и т. д.
		HOSTENT* hst;
		hst = gethostbyaddr((char*)&client_addr.sin_addr.s_addr, 4, AF_INET);
		// вывод сведений о клиенте
		//Функция inet_ntoa преобразует сетевой адрес Интернета (IPv4) в строку ASCII в стандартном формате 
		// Интернета с разделительными точками.
		std::cout << hst->h_name << " " << inet_ntoa(client_addr.sin_addr) << " new connect!\n";
		PRINTUSERS;
		// Вызов нового потока для обслужвания клиента.
		DWORD thid;
		CreateThread(NULL, NULL, SetToClient, &client_socket, NULL, &thid);
	}
	//(LPTHREAD_START_ROUTINE) ((LPVOID)client_socket)
	//connection_from_queue(client_addr, client_addr_size);
}

DWORD __stdcall SetToClient(LPVOID client_socket) {

	SOCKET client_sock;
	client_sock = ((SOCKET*)client_socket)[0];

	//имя вошедшего в чат пользователя name of the user who entered the chat
	std::string clientName = "";
	
	char buff[BUFSIZ]{}; //char buff[20 * 1024];
	
	int bytes_recv = 0;

	// цикл эхо-сервера: прием строки от клиента и возвращение ее клиенту
	while ((bytes_recv = recv(client_sock, &buff[0], sizeof(buff), 0)) && bytes_recv != SOCKET_ERROR) {
		std::string result = "";
		bool completion = true;
		switch (*buff)
		{
		case 'u':
		{
			mySQLTest mysql;
			result = mysql.getUser();
			send(client_sock, result.c_str(), result.size(), 0);
			ZeroMemory(&buff, sizeof(buff));
		}
		break;
		case 'n':
			//сверка имени
			do
			{
				if (nameVerification(client_sock, buff)) {
					std::string strMessage = recAndTransMess(client_sock, "Enter message!", buff);
					//запись сooбщения в таблицу
					std::string msg = writingMessage(messageRecipientName, clientName, strMessage);

					result = msg + "\nprodolzim?? Enter - 'n' dly message, enter - 'exit' dly vixoda";
					send(client_sock, result.c_str(), result.length(), 0);
					completion = false;
				}
				else {
					result = "There is no such user! ";
					send(client_sock, result.c_str(), result.length(), 0);					
				}
			} while (completion);
			
			break;
		case 'r':
		{
			std::string str = "Enter you name:";
			send(client_sock, str.c_str(), str.length(), 0);
			ZeroMemory(&buff, sizeof(buff));
			recv(client_sock, &buff[0], sizeof(buff), 0);
			clientName = buff;
			str = "Enter you surname:";
			send(client_sock, str.c_str(), str.length(), 0);
			ZeroMemory(&buff, sizeof(buff));
			recv(client_sock, &buff[0], sizeof(buff), 0);
			std::string surname = buff;
				str = "Enter you email: ";
			send(client_sock, str.c_str(), str.length(), 0);
			ZeroMemory(&buff, sizeof(buff));
			recv(client_sock, &buff[0], sizeof(buff), 0);
			std::string email = buff;
				str = "Enter you pass: ";
			send(client_sock, str.c_str(), str.length(), 0);
			ZeroMemory(&buff, sizeof(buff));
			recv(client_sock, &buff[0], sizeof(buff), 0);
			std::string pass = buff;

			/*std::string name = recAndTransMess(client_sock,    "Enter you name:", buff);
			
			std::string surname = recAndTransMess(client_sock, "Enter you surname:", buff);
		
			std::string email = recAndTransMess(client_sock,   "Enter you email:  ", buff);
			
			std::string pass = recAndTransMess(client_sock,    "Enter you pass:   ", buff);
			*/
			mySQLTest mysql;
			result = mysql.setUser(clientName, surname, email, pass);
			send(client_sock, result.c_str(), result.length(), 0);		

			ZeroMemory(&buff, sizeof(buff));
			recv(client_sock, &buff[0], sizeof(buff), 0);
			result = buff;
			if (result == "y") {
				result = mysql.userLogin(clientName, pass);
				send(client_sock, result.c_str(), result.length(), 0);
			}
		}
		break;
		case 'v':
		{
			/*std::string str = "Enter you name:";
			send(client_sock, str.c_str(), str.length(), 0);
			ZeroMemory(&buff, sizeof(buff));
			recv(client_sock, &buff[0], sizeof(buff), 0);
			std::string name = buff;*/
			//str = "Enter you pass:";
			//send(client_sock, str.c_str(), str.length(), 0);
			//ZeroMemory(&buff, sizeof(buff));
			////memset(buff, 0, BUFSIZ);
			//recv(client_sock, &buff[0], sizeof(buff), 0);
			//std::string pass = buff;

			clientName = recAndTransMess(client_sock, "Enter you name:", buff);
			//ZeroMemory(&buff, sizeof(buff));
			std::string pass = recAndTransMess(client_sock, "Enter you pass:", buff);
			
			mySQLTest mysql;
			result = mysql.userLogin(clientName, pass);
			send(client_sock, result.c_str(), result.length(), 0);			
		}
		break;
		case 'e' + 'x' + 'i' + 't':
			closesocket(client_sock);
			break; 
		default:
			result = "Invalid character entered!";
			send(client_sock, result.c_str(), result.length(), 0);					
			//memset(buff, 0, BUFSIZ);
			break;
		}
	}
	// если мы здесь, то произошел выход из цикла по причине
    // возращения функцией recv ошибки - соединение с клиентом разорвано
	nclients--; // уменьшаем счетчик активных клиентов
	printf("-disconnect\n"); 
	PRINTUSERS
	// закрываем сокет closesocket(client_socket);
	if (client_sock != INVALID_SOCKET) {
		closesocket(client_sock);
	}
	return 0;
}

std::string recAndTransMess(SOCKET client_sock,const std::string& str, char buff[BUFSIZ])
{	
	memset(buff, '\0', BUFSIZ);
	send(client_sock, str.c_str(), str.length(), 0);
		
	recv(client_sock, &buff[0], sizeof(buff), 0);	
	return buff;
}

bool nameVerification(SOCKET client_sock, char buff[BUFSIZ])
{
	std::string nameRequest = "Who to send message to?"; // Кому отправить сообщение?			
	//запрос имени name request
	messageRecipientName = recAndTransMess(client_sock, nameRequest, buff);
	ZeroMemory(&buff, sizeof(buff));
	mySQLTest mysql;
	std::string result = mysql.getUser();
	//запись имен из таблицы
	std::vector<std::string> userName;
	//имя из списка name from the list
	std::string name = "";
	for (int i = 0; i < result.size(); i++) {
		name += result[i];
		if (result[i] == ' ' || result[i] == '\0') {
			userName.push_back(name);
			name = "";
		}
	}
	//сверка имени
	for (int i = 0; i < userName.size(); i++) {
		if (userName[i] == messageRecipientName + " ") {
			return true;
		}
		if (i == (userName.size() - 1) && userName[i] != (messageRecipientName + " ")) {
			return false;			
		}
	}
	return false;
}
//запись сooбщения в таблицу
std::string writingMessage(const std::string& name1, const std::string&  name2, const std::string& strMes)
{
	mySQLTest mysql;
	if (mysql.writingMessage(name1, name2, strMes)) {
		return "Message sent!";
	}
	else
	return "Message not sent!";
}
