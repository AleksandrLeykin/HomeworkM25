#include "myChat.h"
#include "clientTest.h"


std::string MyChat::userRegistration()
{
	std::cout << "¬ведите им€: Enter name: ";
	m_user.m_name = getLineOfText();
	cinClear();
	std::cout << "\n";
	std::cout << "¬ведите фамилие: Enter surname: ";
	m_user.m_surname = getLineOfText();
	cinClear();
	std::cout << "\n";
	std::cout << "Enter email: ";
	m_user.m_email = getLineOfText();
	cinClear();
	std::cout << "\n";
	std::cout << "¬ведите пароль: Enter password: ";
	m_user.m_pass = getLineOfText();
	cinClear();
	std::cout << "\n";

	std::string result = m_user.m_name + m_user.m_surname + m_user.m_email + m_user.m_pass;

	return result;
}

void MyChat::interChat()
{
	while (true) {
		std::cout << "ƒл€ входа нажмите - 1, дл€ регистрации - 2, дл€ выхода - 3 или exit:\n"
			<< "To enter, press - 1, to register - 2, to exit - 3 or exit: ";
		int number = getint();
				
		switch (number)
		{
		case 1:
		{
			myClient client;
			client.setMyString("v");			
			client.client_loading();			
		}
			break;
		case 2:
		{
			myClient client;
			client.setMyString("r");
			client.client_loading();
		}
 			break;
		case 3:
		{
			myClient client;
			client.setMyString("exit");
			client.client_loading();
		}
			return;
		default:
			std::cout << "¬веден не верный символ!! Invalid character entered!!" << "\n";
			break;
		}

	}
}
