#pragma once
#include "Socket.h"
#include <chrono>
#include <thread>

class MailWorking
{
private:

protected:
	SMTP p_SMTP_Client;
	POP3 p_POP3_Client;
public:
	bool sendMail(const char* IP, int PORT, User person, Mail mail);
	bool receiveMail(const char* IP, int PORT,User& person);
};

class Menu
{
public:
	void display();
};
