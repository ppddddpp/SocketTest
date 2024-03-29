#pragma once
#include "Socket.h"
#include <thread>
#include <future>
#include <chrono>

class MailWorking
{
private:

protected:
	SMTP p_SMTP_Client;
	POP3 p_POP3_Client;
public:
	bool sendMail(const char* IP, int PORT, User person, Mail mail, bool connected);
	bool receiveMail(const char* IP, int PORT,User& person);
	POP3& getPOP3();
	void closeSMTPconnection();
};

class Menu
{
public:
	void start();
};
void display(MailWorking* test, User& person, bool& isDone, bool& connectedToPOP3);