#include "myChat.h"
#include "clientTest.h"


std::string MyChat::userRegistration()
{
	std::cout << "������� ���: Enter name: ";
	m_user.m_name = getLineOfText();
	cinClear();
	std::cout << "\n";
	std::cout << "������� �������: Enter surname: ";
	m_user.m_surname = getLineOfText();
	cinClear();
	std::cout << "\n";
	std::cout << "Enter email: ";
	m_user.m_email = getLineOfText();
	cinClear();
	std::cout << "\n";
	std::cout << "������� ������: Enter password: ";
	m_user.m_pass = getLineOfText();
	cinClear();
	std::cout << "\n";

	std::string result = m_user.m_name + m_user.m_surname + m_user.m_email + m_user.m_pass;

	return result;
}

void MyChat::interChat()
{
	while (true) {
		std::cout << "��� ����� ������� - v, ��� ����������� - r, ��� ������ - 3 ��� exit:\n"
			<< "To enter, press - v, to register - r, to exit - 3 or exit: ";
		//int number = getint();
		char symbol = getChar();
				
		switch (symbol)
		{
		case 'v':
		{
			myClient client;
			client.setMyString("v");			
			client.client_loading();			
		}
			break;
		case 'r':
		{
			myClient client;
			client.setMyString("r");
			client.client_loading();
		}
 			break;
		case '3':
		{
			myClient client;
			client.setMyString("exit");
			client.client_loading();
		}
			return;
		default:
			std::cout << "������ �� ������ ������!! Invalid character entered!!" << "\n";
			break;
		}

	}
}
