#pragma once
#include <iostream>
#include "User.h"
#include "clientTest.h"

class MyChat {
private:
	User m_user;
	std::string request = "";
public:
	MyChat() {}
	~MyChat() {}
	//variable for query переменная для запроса
	std::string m_request = "";
	std::string userRegistration();
	void interChat();
private:
	//очистка буфера (clear buffer)
	void cinClear() {
		std::cin.clear();
		std::cin.ignore(32767, '\n');
	}
	//ввод строки (line input)
	std::string getLineOfText()
	{
		std::string str = "";
		std::cin >> str;
		return str;
	}
	//ввод числа (Enter a number)
	int getint() {
		while (true) {
			int num = 0;
			std::cin >> num;
			if (std::cin.fail()) {
				cinClear();
				std::cout << "Веден не верный символ! Invalid character entered!";
			}
			else
				return num;
		}
	}
};