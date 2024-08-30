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

	//�������� ������
	bool mySQLDB();	

	//���������� ������������
	std::string setUser(const std::string& name, const std::string& surname, const std::string& email, const std::string& pas);	
	
	//���� ������������
	std::string userLogin(const std::string& name, const std::string& pass);

	//�������� �������������
	std::string getUser();	

private:
	//����������
	MYSQL mysql;
	//������ �� ��������� MYSQL_RES ���������
	MYSQL_RES* res;
	//������������ ���� ������ �� ����������, ����������� ������.
	MYSQL_ROW row;

	//����������� ��� �������� ��
	void connectDB();
	
};

