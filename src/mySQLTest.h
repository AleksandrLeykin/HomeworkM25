#pragma once
#include <iostream>
#include <mysql.h>
#include <string>
#include <memory>

struct mySQLTest {
	mySQLTest() { 
		if (mySQLDB()) 
			std::cout << "connection to server SQL is OK!" << std::endl;
		else
			std::cout << "Error: can`t connect to server " << mysql_error(&mysql) << std::endl;
	}
	~mySQLTest() {
		mysql_close(&mysql);
		std::cout << "exit" << std::endl;
	}	

	//создание таблиц
	bool mySQLDB();	

	//добавление пользователя
	std::string setUser(const std::string& name, const std::string& surname, const std::string& email, const std::string& pas);	
	
	//вход пользователя
	std::string userLogin(const std::string& name, const std::string& pass);

	//просмотр пользователей
	std::string getUser();	

private:
	//дескриптор
	MYSQL mysql;
	//ссылка на структуру MYSQL_RES результат
	MYSQL_RES* res;
	//Представляет одну строку из результата, содержащего строки.
	MYSQL_ROW row;

	//подключение или создание БД
	void connectDB();
	
};

