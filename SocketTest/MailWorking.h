#pragma once
#include "Socket.h"

class MailWorking
{
private:
	std::vector<Mail> m_ListOfMail;
protected:
	SMTP p_SMTP_Client;
	POP3 p_POP3_Client;
public:
	Mail operator[](int num) {
		return m_ListOfMail[num];
	}
public:
	bool sendMail(const char* IP, int PORT, std::string username,
		std::string password, Mail mail);
	bool receiveMail(const char* IP, int PORT, std::string username,
		std::string password);

};

