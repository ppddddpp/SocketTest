#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "Mail.h"

class User
{
private:
	std::vector<Mail> m_ListOfMail;
	std::string m_username;
	std::string m_userMail;
	std::string m_password;
	std::string m_serverIP;
	int m_portSMTP;
	int m_portPOP3;
	int m_autoLoad;
public:
	void setUsername(std::string name);
	void setUserMail(std::string userMail);
	void setPassword(std::string pass);
	void setServerIP(std::string serverIP);
	void setPortSMTP(int num);
	void setPortPOP3(int num);
	void setAutoLoad(int num);
	void addMailToList(Mail& mail);
public:
	std::string getUsername();
	std::string getUserMail();
	std::string getPassword();
	std::string getServerIP();
	int getPortSMTP();
	int getPortPOP3();
	int getAutoLoad();
	int getSizeOfListMail();
	Mail getMail(int num);
public:
	void openMail();
	User(std::string filename);
	Mail operator[](int num) {
		return m_ListOfMail[num];
	}
	std::string getMailData(int num);
	void saveFileLocally(Mail mail, std::string& localFilePath, int fileWantToSave);
};
