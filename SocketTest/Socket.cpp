#include "Socket.h"

#pragma region MySocket
MySocket::MySocket()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cerr << "Failed to initialize winsock" << std::endl;
		exit(EXIT_FAILURE);
	}

	m_socket = socket(AF_INET, SOCK_STREAM, 0);

	if (m_socket == INVALID_SOCKET) {
		std::cerr << "Failed to create socket" << std::endl;
		WSACleanup();
		exit(EXIT_FAILURE);
	}
}

MySocket::~MySocket()
{
	closesocket(m_socket);
	WSACleanup();
}

bool MySocket::connectTo(const char* IP, int PORT)
{
	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(PORT);
	serverAddress.sin_addr.s_addr = inet_addr(IP);

	if (connect(m_socket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
		std::cerr << "Cannot connect to server" << std::endl;
		return false;
	}
	return true;
}

void MySocket::disconnectServer()
{
	closesocket(m_socket);
}

void MySocket::sendCommand(std::string command)
{
	const char* CommandNeedToSend = command.c_str();
	send(m_socket, CommandNeedToSend, strlen(CommandNeedToSend) - 1, 0);
}

std::string MySocket::receiveServerResponse()
{
	char buffer[4096];
	int bytesRead = recv(m_socket, buffer, sizeof(buffer), 0);
	if (bytesRead == SOCKET_ERROR) {
		std::cout << "Cannot receive data" << std::endl;
	}
	else if (bytesRead == 0) {
		std::cout << "Connection closed" << std::endl;
	}
	else {
		buffer[bytesRead] = '\0';
	}
	return std::string(buffer);
}

#pragma endregion MySocket

#pragma region SMTP
std::string SMTP::to_base64(std::string inStr)
{
	const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	std::string ouStr;
	size_t i = 0;

	while (i < inStr.size()) {
		uint32_t octet_1 = 0;
		uint32_t octet_2 = 0;
		uint32_t octet_3 = 0;

		if (i < inStr.size()) {
			octet_1 = static_cast<uint8_t>(inStr[i++]);
		}

		if (i < inStr.size()) {
			octet_2 = static_cast<uint8_t>(inStr[i++]);
		}

		if (i < inStr.size()) {
			octet_3 = static_cast<uint8_t>(inStr[i++]);
		}

		uint32_t triple = (octet_1 << 16) + (octet_2 << 8) + octet_3;

		ouStr.push_back(base64_chars[(triple >> 18) & 0x3F]);
		ouStr.push_back(base64_chars[(triple >> 12) & 0x3F]);

		if (i > inStr.size() + 1) {
			ouStr.push_back('=');
		}
		else {
			ouStr.push_back(base64_chars[(triple >> 6) & 0x3F]);
		}

		if (i > inStr.size()) {
			ouStr.push_back('=');
		}
		else {
			ouStr.push_back(base64_chars[triple & 0x3F]);
		}
	}
	return ouStr;
}

bool SMTP::login(const char* IP, int PORT, std::string username, std::string password)
{
	if (!m_SMTP_SOCKET.connectTo(IP,PORT)) {
		return false;
	}
	std::string serverResponse;
	//Working with EHLO
	std::string domain = "one.com"; // one should be exchange to the testing domain
	std::string ehloCommand = "EHLO " + domain + "\r\n";
	m_SMTP_SOCKET.sendCommand(ehloCommand);

	serverResponse = m_SMTP_SOCKET.receiveServerResponse();
	if (serverResponse.substr(0, 3) != "250") {
		std::cerr << "EHLO failed." << std::endl;
		return false;
	}
	
	m_SMTP_SOCKET.sendCommand("AUTH LOGIN\r\n");
	serverResponse = m_SMTP_SOCKET.receiveServerResponse();
	if (serverResponse.substr(0, 3) != "334") {
		std::cerr << "Server does not support LOGIN authentication" << std::endl;
		return false;
	}

	m_SMTP_SOCKET.sendCommand(to_base64(username) + "\r\n");
	serverResponse = m_SMTP_SOCKET.receiveServerResponse();
	if (serverResponse.substr(0, 3) != "235") {
		std::cerr << "Wrong username" << std::endl;
		return false;
	}

	m_SMTP_SOCKET.sendCommand(to_base64(password) + "\r\n");
	serverResponse = m_SMTP_SOCKET.receiveServerResponse();
	if (serverResponse.substr(0, 3) != "235") {
		std::cerr << "Wrong password" << std::endl;
		return false;
	}


	return true;
}

void SMTP::sendMail(Mail mail)
{
	std::string ThingToSend;
	//edit the string to send
	m_SMTP_SOCKET.sendCommand(ThingToSend);
}

#pragma endregion SMTP

#pragma region POP3

bool POP3::isOk(std::string serverResponse)
{
	if (serverResponse.substr(0, 3) == "+OK") {
		return true;
	}
	return false;
}

bool POP3::login(const char* IP, int PORT, std::string username, std::string password)
{
	if (!m_POP3_SOCKET.connectTo(IP, PORT)) {
		return false;
	}
	std::string serverResponse;

	serverResponse = m_POP3_SOCKET.receiveServerResponse();
	if (isOk(serverResponse) == false) {
		std::cout << "Server is not ready yet";
		return false;
	}

	m_POP3_SOCKET.sendCommand("USER " + username + "\r\n");
	serverResponse = m_POP3_SOCKET.receiveServerResponse();
	if (isOk(serverResponse) == false) {
		std::cout << "Wrong username";
		return false;
	}

	m_POP3_SOCKET.sendCommand("USER " + password + "\r\n");
	serverResponse = m_POP3_SOCKET.receiveServerResponse();
	if (isOk(serverResponse) == false) {
		std::cout << "Wrong password";
		return false;
	}
	return true;
}

std::vector<Mail> POP3::reciveMail()
{
	std::vector<Mail> listMail;
	return listMail;
}

#pragma endregion POP3
