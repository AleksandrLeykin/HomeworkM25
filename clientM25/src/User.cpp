#include "User.h"
#include <iostream>
#include <fstream>


User::User(const std::string& name, const std::string& surname, const std::string& email, const std::string& pass) :
	m_name(name), m_pass(pass), m_surname(surname), m_email(email)
{	
	userRecording(name, pass, surname, email);
}

std::string User::getValue()
{
	std::string str = "";
	std::cin >> str;
	return str;
}

void User::userRecording(const std::string& name, const std::string& surname, const std::string& email, const std::string& pass)
{
	//поток для записи(stream to write)
	std::ofstream out;
	//дозапись текста в файле (adding text to a file)
	out.open("persons.txt", std::ios::app);
	if (out.is_open())
		out << name << " " << surname << " " << email << pass << std::endl;
	out.close();
}