#pragma once
#include <winsock.h>
#include <iostream>
#include "Users.h"

class MySocket
{
private:
	SOCKET m_socket;
public:
	MySocket();
	~MySocket();
public:
	bool connectTo(const char* IP, int PORT);
	void disconnectServer();
	void sendCommand(std::string comannd);
	std::string receiveServerResponse();
};

class SMTP
{
private:
	MySocket m_SMTP_SOCKET;
public:
	bool login(const char* IP, int PORT, User person);
	void sendMail(Mail mail);
};

class POP3
{
private:
	MySocket m_POP3_SOCKET;
public:
	bool isOk(std::string serverResponse);
	bool login(const char* IP, int PORT, User person);
	bool IsExistedMail(std::string data, User person);
	void receiveMail(User& person);
};
