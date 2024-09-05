#include "mySQLTest.h"

bool mySQLTest::mySQLDB()
{
	connectDB();
	//�������� ������	
	std::string createTable = "CREATE TABLE IF NOT EXISTS user("
		"id SERIAL PRIMARY KEY AUTO_INCREMENT, "
		"name VARCHAR(255) NOT NULL, "
		"surname VARCHAR(255) NOT NULL, "
		"email VARCHAR(255) NOT NULL, "
		"password VARCHAR(32) NOT NULL)";
	mysql_real_query(&mysql, createTable.c_str(), createTable.length());

	std::string createTableMessage = "CREATE TABLE IF NOT EXISTS messages("
		"id SERIAL PRIMARY KEY AUTO_INCREMENT, "
		"user1_id integer NOT NULL REFERENCES user(id), "
		"user2_id integer NOT NULL REFERENCES user(id), "
		"data_create DATETIME NOT NULL, "
		"messages TEXT NOT NULL)";
		
		mysql_real_query(&mysql, createTableMessage.c_str(), createTableMessage.length());

	// ��������� ���������� � ����� ������
	//mysql_close(&mysql);
	return true;
}

std::string mySQLTest::setUser(const std::string& name, const std::string& surname, const std::string& email, const std::string& pas)
{
	connectDB();
	int i = 0;
	//���������� �������
	std::string record_user = "INSERT INTO user(id, name, surname, email, password)"
		" VALUES(default, '" + name + "', '" + surname + "', '" + email + "', '" + pas + "')";
	
	//������
	std::string selectUser = "select name, surname from user";
	mysql_real_query(&mysql, selectUser.c_str(), selectUser.length());

	//������ � ����� ������
	std::string newName = name + surname;
	//�������� ����� � ������� ����� ����
	if (res = mysql_store_result(&mysql)) {
		while (row = mysql_fetch_row(res)) {
			std::string currentName = "";
			for (i = 0; i < mysql_num_fields(res); ++i) {
				currentName += row[i];
			}
			//�������� ������������������ ��������
			//std::cout << currentName << std::endl;
			if (newName == currentName) {
				mysql_close(&mysql);
				return "name taken!";				
			}
		}
	}
	else
		std::cout << "Error MySql # " << mysql_error(&mysql) << std::endl;
	//���� ��� �� ������ ���������� ���
	mysql_real_query(&mysql, record_user.c_str(), record_user.length());
	mysql_close(&mysql);
	return "You new user myChat!";
}

std::string mySQLTest::userLogin(const std::string& name, const std::string& pass)
{
	connectDB();
	int i = 0;
	
	//������
	std::string selectUser = "select name, password from user";
	mysql_real_query(&mysql, selectUser.c_str(), selectUser.length());

	//������ � ����� ������
	std::string newName = name + pass;
	//�������� ����� � ������� ����� ����
	if (res = mysql_store_result(&mysql)) {
		while (row = mysql_fetch_row(res)) {
			std::string currentName = "";
			for (i = 0; i < mysql_num_fields(res); ++i) {
				currentName += row[i];
			}
			//�������� ������������������ ��������
			//std::cout << currentName << std::endl;
			if (newName == currentName) {
				mysql_close(&mysql);
				newName = "";				
				newName = "Hello " + name + "   ";
				return newName;
			}
		}
	}
	else
		std::cout << "Error MySql # " << mysql_error(&mysql) << std::endl;
	return "name or password is incorrect!";
}

std::string mySQLTest::getUser()
{
	int i = 0;
	std::string result = "";
	//������
	std::string selectUser = "select name from user";
	mysql_real_query(&mysql, selectUser.c_str(), selectUser.length());
	if (res = mysql_store_result(&mysql)) {
		while (row = mysql_fetch_row(res)) {
			for (i = 0; i < mysql_num_fields(res); i++) {
				result += row[i];
			}
			result += " ";
		}
	}
	return result;
}

void mySQLTest::connectDB()
{
	// �������� ���������� ����������
	mysql_init(&mysql);
	if (&mysql == nullptr) {
		std::cout << "Error: can`t create MySQL - description" << std::endl;
		return;
	}
	// ������������ � �������
	if (!mysql_real_connect(&mysql, "localhost", "root", "root", NULL, NULL, NULL, 0)) {
		// ���� ��� ����������� ���������� ���������� � �� ������� ��������� �� ������
		std::cout << "Error: can`t connect to database " << mysql_error(&mysql) << std::endl;
		return;
	}
	//��������� ���� ������
	//������ ������
	std::string createDB = "CREATE DATABASE if not EXISTS chattestdb";
	std::string useDB = "use chattestdb";
	mysql_real_query(&mysql, createDB.c_str(), createDB.length());
	mysql_real_query(&mysql, useDB.c_str(), useDB.length());

	mysql_set_character_set(&mysql, "utf8");
	//������� ���������� �� ��������� �� ������, �� ��������� ��� latin1
	//std::cout << "connection characterset: " << mysql_character_set_name(&mysql) << std::endl;
}
