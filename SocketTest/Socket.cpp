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
	send(m_socket, CommandNeedToSend, strlen(CommandNeedToSend), 0);
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
std::string SMTP::to_base64(std::vector<uint8_t>& data)
{
	const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	std::string ouStr;
	size_t i = 0;

	while (i < data.size()) {
		uint32_t octet_1 = 0;
		uint32_t octet_2 = 0;
		uint32_t octet_3 = 0;

		if (i < data.size()) {
			octet_1 = static_cast<uint8_t>(data[i++]);
		}

		if (i < data.size()) {
			octet_2 = static_cast<uint8_t>(data[i++]);
		}

		if (i < data.size()) {
			octet_3 = static_cast<uint8_t>(data[i++]);
		}

		uint32_t triple = (octet_1 << 16) + (octet_2 << 8) + octet_3;

		ouStr.push_back(base64_chars[(triple >> 18) & 0x3F]);
		ouStr.push_back(base64_chars[(triple >> 12) & 0x3F]);

		if (i > data.size() + 1) {
			ouStr.push_back('=');
		}
		else {
			ouStr.push_back(base64_chars[(triple >> 6) & 0x3F]);
		}

		if (i > data.size()) {
			ouStr.push_back('=');
		}
		else {
			ouStr.push_back(base64_chars[triple & 0x3F]);
		}
	}
	return ouStr;
}

bool SMTP::login(const char* IP, int PORT, User person)
{
	if (!m_SMTP_SOCKET.connectTo(IP,PORT)) {
		return false;
	}
	std::string serverResponse;
	serverResponse = m_SMTP_SOCKET.receiveServerResponse();
	if (serverResponse.substr(0, 3) != "220") {
		std::cerr << "Server no response" << std::endl;
		return false;
	}
	//Working with EHLO
	std::string domain = "[127.0.0.1]"; // one should be exchange to the testing domain
	std::string ehloCommand = "EHLO " + domain + "\r\n";
	m_SMTP_SOCKET.sendCommand(ehloCommand);

	serverResponse = m_SMTP_SOCKET.receiveServerResponse();
	if (serverResponse.substr(0, 3) != "250") {
		std::cerr << "EHLO failed." << std::endl;
		return false;
	}
	
	bool LoginAUTHNeeding = true;
	m_SMTP_SOCKET.sendCommand("AUTH LOGIN\r\n");
	serverResponse = m_SMTP_SOCKET.receiveServerResponse();
	if (serverResponse.substr(0, 3) != "500") {
		std::cerr << "Server does not support LOGIN authentication" << std::endl;
		LoginAUTHNeeding = false;
	}
	
	if (!LoginAUTHNeeding) {
		m_SMTP_SOCKET.sendCommand(person.getUsername() + "\r\n");
		serverResponse = m_SMTP_SOCKET.receiveServerResponse();
		if (serverResponse.substr(0, 3) != "235") { //334
			std::cerr << "Wrong username" << std::endl;
		}
		m_SMTP_SOCKET.sendCommand(person.getPassword() + "\r\n");
		serverResponse = m_SMTP_SOCKET.receiveServerResponse();
		if (serverResponse.substr(0, 3) != "235") { //334
			std::cerr << "Wrong password" << std::endl;
		}
		serverResponse = m_SMTP_SOCKET.receiveServerResponse();
		if (serverResponse.substr(0, 3) != "250") { //220
			std::cerr << "Server response error" << std::endl;
		}
	}
	return true;
}

void SMTP::sendMail(Mail mail)
{	
	std::string serverResponse;
	std::string senderMail = "sender@test.com";
	m_SMTP_SOCKET.sendCommand("MAIL FROM: " + senderMail + "\r\n");
	serverResponse = m_SMTP_SOCKET.receiveServerResponse();
	if (serverResponse != "250") {

	}

	for (int i = 0; i < mail.sizeofTo(); i++) {
		std::string receiverMail = mail.getTo(i);
		m_SMTP_SOCKET.sendCommand("RCPT TO: " + receiverMail + "\r\n");
		serverResponse = m_SMTP_SOCKET.receiveServerResponse();
		if (serverResponse != "250") {

		}
	}
	
	for (int i = 0; i < mail.sizeofCC(); i++) {
		std::string receiverMail = mail.getCC(i);
		m_SMTP_SOCKET.sendCommand("RCPT TO: " + receiverMail + "\r\n");
		serverResponse = m_SMTP_SOCKET.receiveServerResponse();
		if (serverResponse != "250") {

		}
	}

	for (int i = 0; i < mail.sizeofBCC(); i++) {
		std::string receiverMail = mail.getBCC(i);
		m_SMTP_SOCKET.sendCommand("RCPT TO: " + receiverMail + "\r\n");
		serverResponse = m_SMTP_SOCKET.receiveServerResponse();
		if (serverResponse != "250") {

		}
	}

	m_SMTP_SOCKET.sendCommand("DATA\r\n");
	serverResponse = m_SMTP_SOCKET.receiveServerResponse();
	if (serverResponse.substr(0, 3) != "354") {
		std::cout << "Data send comment error ";
	}

	std::string ThingToSend = "";
	//edit the string to send: 
	ThingToSend += mail.getSubject();
	ThingToSend += '\n\n\n';
	ThingToSend += mail.getTextBody();

	m_SMTP_SOCKET.sendCommand(ThingToSend);
	//send attachment:
	//should adding method that make a flag for this function below run

	std::string attachmentFileName = "random.txt";
	mail.addAttachment(attachmentFileName);

	//send attachments
	for (int i = 0; i < mail.getSizeOfAttachments(); i++) {
		std::vector<uint8_t>tempAttachment = mail.getAttachment(i);
		std::string AttachmentAsStr = to_base64(tempAttachment);
		m_SMTP_SOCKET.sendCommand(AttachmentAsStr);
	}

	std::string EndMailDot = ".";
	m_SMTP_SOCKET.sendCommand(EndMailDot);
	serverResponse = m_SMTP_SOCKET.receiveServerResponse();
	if (serverResponse.substr(0, 3) != "250") {
		std::cout << "Send unsuccessful";
	}
	std::cout << "Send successful";

	m_SMTP_SOCKET.sendCommand("QUIT");
	serverResponse = m_SMTP_SOCKET.receiveServerResponse();
	if (serverResponse.substr(0, 3) == "221") {
		std::cout << "Quitted server";
	}
	return;
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

bool POP3::login(const char* IP, int PORT, User person)
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
	
	m_POP3_SOCKET.sendCommand("USER " + person.getUsername() + "\r\n");
	serverResponse = m_POP3_SOCKET.receiveServerResponse();
	if (isOk(serverResponse) == false) {
		std::cout << "Wrong username";
		return false;
	}

	m_POP3_SOCKET.sendCommand("PASS " + person.getPassword() + "\r\n");
	serverResponse = m_POP3_SOCKET.receiveServerResponse();
	if (isOk(serverResponse) == false) {
		std::cout << "Wrong password";
		return false;
	}
	return true;
}

std::vector<Mail> POP3::receiveMail()
{
	std::vector<Mail> listMail;
	return listMail;
}

#pragma endregion POP3
