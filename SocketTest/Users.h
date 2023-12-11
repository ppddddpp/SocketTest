#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "FolderWorking.h"
#include <filesystem>

class User
{
private:
	std::vector<UserFolder> m_Folders;
	//Folder notes
	/* 
	* basic declaration for folder:
	1. Inbox
	2. Project
	3. Important
	4. Work
	5. Spam

	* can adding more
	*/
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
	void setServerIP(const char* serverIP);
	void setPortSMTP(int num);
	void setPortPOP3(int num);
	void setAutoLoad(int num);
public:
	std::string getUsername();
	std::string getUserMail();
	std::string getPassword();
	std::string getServerIP();
	int getPortSMTP();
	int getPortPOP3();
	int getAutoLoad();
	int getSizeOfFolder();
	UserFolder getFolder(int num);
public:
	User(std::string filename);
	UserFolder operator[](int num) {

	}
	void moveMailToFolder(MailFolder mail, UserFolder& toFolder);
	
};
