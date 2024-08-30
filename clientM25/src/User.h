#pragma once
#include <string>

class User {
public:	
	User() {};
	User(const std::string& name, const std::string& surname, const std::string& email, const std::string& pass);
	~User() = default;
	
	std::string m_name;
	std::string m_pass;
	std::string m_surname;
	std::string m_email;

	void userRecording(const std::string& name, const std::string& surname, const std::string& email, const std::string& pass);
private:
	std::string getValue();	
};