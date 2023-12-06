#include "Users.h"

void User::setUsername(std::string name)
{
	m_username = name;
}

void User::setUserMail(std::string userMail)
{
	m_userMail = userMail;
}

void User::setPassword(std::string pass)
{
	m_password = pass;
}

void User::setServerIP(std::string serverIP)
{
	m_serverIP = serverIP;
}

void User::setPortSMTP(int num)
{
	m_portSMTP = num;
}

void User::setPortPOP3(int num)
{
	m_portPOP3 = num;
}

void User::setAutoLoad(int num)
{
	m_autoLoad = num;
}

void User::addMailToList(Mail& mail)
{
	m_ListOfMail.push_back(mail);
}

std::string User::getUsername()
{
	return m_username;
}

std::string User::getUserMail()
{
	return m_userMail;
}

std::string User::getPassword()
{
	return m_password;
}

std::string User::getServerIP()
{
	return m_serverIP;
}

int User::getPortSMTP()
{
	return m_portSMTP;
}

int User::getPortPOP3()
{
	return m_portPOP3;
}

int User::getAutoLoad()
{
	return m_autoLoad;
}

int User::getSizeOfListMail()
{
	return m_ListOfMail.size();
}

void User::openMail(Mail mail)
{
	//function to output all mail details

	//set as read
	mail.setAsRead();
}

User::User(std::string filename)
{
	
}


