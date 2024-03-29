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
	char* buffer = new char[4194304];
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	int bytesRead = recv(m_socket, buffer, 4194304, 0);
	if (bytesRead == SOCKET_ERROR) {
		std::cout << "Cannot receive data" << std::endl;
	}
	else if (bytesRead == 0) {
		std::cout << "Connection closed" << std::endl;
	}
	else {
		buffer[bytesRead] = '\0';
	}
	std::string temp(buffer);
	delete[] buffer;
	return temp;
}

#pragma endregion MySocket

#pragma region SMTP

bool SMTP::login(const char* IP, int PORT, User person)
{
	if (!m_SMTP_SOCKET.connectTo(IP,PORT)) {
		return false;
	}
	std::string serverResponse;
	serverResponse = m_SMTP_SOCKET.receiveServerResponse();
	if (serverResponse.substr(0, 3) != "220") {
		std::cerr << "Server didn't response" << std::endl;
		return false;
	}
	//Working with EHLO
	std::string domain = "[" + person.getServerIP() + "]";
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
		std::cout << "Server does not support LOGIN authentication" << std::endl;
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
	mail.setbaseUnread();
	std::string serverResponse;
	std::string senderMail = mail.getFrom();
	m_SMTP_SOCKET.sendCommand("MAIL FROM: " + senderMail + "\r\n");
	serverResponse = m_SMTP_SOCKET.receiveServerResponse();
	if (serverResponse.substr(0, 3) != "250") {
		std::cout << "From " + senderMail + " error! " << std::endl;
	}

	std::string BCC_ReceiverMail = "";
	std::string To_ReceiverMail = "";
	std::string CC_ReceiverMail = "";

	for (int j = 0; j < mail.getToSize(); j++) {
		To_ReceiverMail = mail.getTo(j);
		m_SMTP_SOCKET.sendCommand("RCPT TO: " + To_ReceiverMail + "\r\n");
		serverResponse = m_SMTP_SOCKET.receiveServerResponse();
		if (serverResponse.substr(0, 3) != "250") {
			std::cout << "TO " + To_ReceiverMail + " error!" << std::endl;
		}
	}

	for (int j = 0; j < mail.getCCSize(); j++) {
		CC_ReceiverMail = mail.getCC(j);
		m_SMTP_SOCKET.sendCommand("RCPT TO: " + CC_ReceiverMail + "\r\n");
		serverResponse = m_SMTP_SOCKET.receiveServerResponse();
		if (serverResponse.substr(0, 3) != "250") {
			std::cout << "CC" + CC_ReceiverMail + " error" << std::endl;
		}
	}
	for (int i = 0; i < mail.getBCCSize(); i++) {
		BCC_ReceiverMail = mail.getBCC(i);
		m_SMTP_SOCKET.sendCommand("RCPT TO: " + BCC_ReceiverMail + "\r\n");
		serverResponse = m_SMTP_SOCKET.receiveServerResponse();
		if (serverResponse.substr(0, 3) != "250") {
			std::cout << "BCC" + BCC_ReceiverMail + " error" << std::endl;
		}
	}
	
	m_SMTP_SOCKET.sendCommand("DATA\r\n");
	serverResponse = m_SMTP_SOCKET.receiveServerResponse();
	if (serverResponse.substr(0, 3) != "354") {
		std::cout << "Data command send comment error " << std::endl;
	}
	
	std::string ThingToSend = mail.getAllMailData("send");
	ThingToSend += "\r\n.\r\n";
	m_SMTP_SOCKET.sendCommand(ThingToSend);
	
	//m_SMTP_SOCKET.sendCommand("\r\n");
	//std::string EndMailDot = ".\r\n";
	//m_SMTP_SOCKET.sendCommand(EndMailDot);
	serverResponse = m_SMTP_SOCKET.receiveServerResponse();
	if (serverResponse.find("accepted") == serverResponse.npos) {
		std::cout << "Send unsuccessful" << std::endl;
	}
	else  std::cout << "Send successful" << std::endl;
	return;
}

void SMTP::disconnectServer()
{
	m_SMTP_SOCKET.disconnectServer();
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
		std::cout << "Server is not ready yet" << std::endl;
		return false;
	}

	m_POP3_SOCKET.sendCommand("CAPA\r\n");
	serverResponse = m_POP3_SOCKET.receiveServerResponse();
	if (isOk(serverResponse) == false) {
		std::cout << "Wrong username" << std::endl;
		return false;
	}
	
	m_POP3_SOCKET.sendCommand("USER " + person.getUserMail() + "\r\n");
	serverResponse = m_POP3_SOCKET.receiveServerResponse();
	if (isOk(serverResponse) == false) {
		std::cout << "Wrong username" << std::endl;
		return false;
	}

	m_POP3_SOCKET.sendCommand("PASS " + person.getPassword() + "\r\n");
	serverResponse = m_POP3_SOCKET.receiveServerResponse();
	if (isOk(serverResponse) == false) {
		std::cout << "Wrong password" << std::endl;
		return false;
	}
	return true;
}

bool POP3::IsExistedMail(std::string data, User person)
{
	for (int i = 0; i < 5; i++) {
		UserFolder localWorkingFolder = person.getFolder(i);
		for (int j = 0; j < localWorkingFolder.getSizeOfListMail(); j++) {
			if (data == localWorkingFolder[j].getMailAllData("check")) 
			{
				return true;
			}
		}
	}
	return false;
}

void POP3::receiveMail(User& person)
{
	std::vector<MailFolder> listMailReceive;
	std::string serverResponse;

	m_POP3_SOCKET.sendCommand("STAT\r\n");
	serverResponse = m_POP3_SOCKET.receiveServerResponse();
	if (isOk(serverResponse) == false) {
		std::cout << "Problem with STAT" << std::endl;
		return;
	}

	int numOfMails = stoi(serverResponse.substr(serverResponse.find_first_of("0123456789"), serverResponse.find_last_of(' ')));

	m_POP3_SOCKET.sendCommand("LIST\r\n");
	serverResponse = m_POP3_SOCKET.receiveServerResponse();
	if (isOk(serverResponse) == false) {
		std::cout << "List error" << std::endl;
		return;
	}

	m_POP3_SOCKET.sendCommand("UIDL\r\n");
	serverResponse = m_POP3_SOCKET.receiveServerResponse();
	if (isOk(serverResponse) == false) {
		std::cout << "UIDL error" << std::endl;
		return;
	}

	for (int i = 0; i < numOfMails; i++)
	{
		std::string returnMail = "RETR " + std::to_string(i + 1) + '\r' + '\n';
		if (i < numOfMails)
		{
			m_POP3_SOCKET.sendCommand(returnMail);
			serverResponse = m_POP3_SOCKET.receiveServerResponse();
			serverResponse = serverResponse.substr(serverResponse.find("OK"));
			Mail buffer(serverResponse);
			listMailReceive.push_back(MailFolder(buffer));
		}
	}

	for (int i = 0; i < listMailReceive.size(); i++) {
		if (IsExistedMail(listMailReceive[i].getMailAllData("save"), person) == false)
		{
			person.goThroughFilters(listMailReceive[i]).addMailToList(listMailReceive[i]);
		}
		for (int k = 0; k < 5; k++)
		{
			for (int j = 0; j < person[k].getSizeOfListMail(); j++)
			{
				person.moveMailToFolder(person[k].getMailFolder(j), person[k]);
			}
		}
	}

	for (int i = 0; i < 5; i++)
	{
		int numberOfMails = person.getSizeOfFolder(i);
		for (int j = 0; j < numberOfMails; j++)
		{
			std::string readStatus;
			std::string filePath = "./" + person.getUserMail() + "/" + person.getFolder(i).getFolderName() + "/"
				+ "msg" + std::to_string(j + 1) + "/" + "msg" + std::to_string(j + 1) + ".txt";
			std::ifstream inFile(filePath);
			if (inFile.is_open())
			{
				while (!inFile.eof())
				{
					std::getline(inFile, readStatus);
					if ("READ" == readStatus)
					{
						person.getFolder(i).getMailFolder(j).getMail().setAsRead();
					}
				}
			}
		}
	}
}

#pragma endregion POP3

