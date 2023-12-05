#include <winsock.h>
#include <iostream>

class Email {
private:
	std::string& _To;
	std::string& _CC;
	std::string& _BCC;
	std::string& _Subject;
	std::string& _TextBody;
public:
	std::string getTo() {
		return _To;
	}
	std::string getCC() {
		return _CC;
	}
	std::string getBCC() {
		return _BCC;
	}
	std::string getSubject() {
		return _Subject;
	}
	std::string getTextBody() {
		return _TextBody;
	}
	std::string AllOfEmailContent() {
		std::string MyMail = "ppdddd00123@gmail.com";
		std::string emailContent = "From: " + MyMail + "\n";
		emailContent += "To: " + _To + "\n";
		emailContent += "Cc: " + _CC + "\n";
		emailContent += "Bcc: " + _BCC + "\n";
		emailContent += "Subject: " + _Subject + "\n";
		emailContent += "\n\n";
		emailContent += _TextBody + "\n\n";
	}
public:
	Email();
};



void sendMessageHelper(SOCKET workingSocket, std::string message) {
	const char* sendMessage = message.c_str(); 
	send(workingSocket, sendMessage, strlen(sendMessage), 0);
}

void send(const char* IP, int PORT, Email& e) {
	SOCKET SMTP_SOCKET = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(PORT);
	serverAddress.sin_addr.s_addr = inet_addr(IP);

	connect(SMTP_SOCKET, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

	//Working with EHLO
	std::string domain = "one.com"; // one should be exchange to the testing domain
	std::string ehloCommand = "EHLO" ;
	sendMessageHelper(SMTP_SOCKET, ehloCommand);

	std::string MyMail = "ppdddd00123@gmail.com";
	std::string MailSending = "MAIL FROM: <" + MyMail + ">";
	sendMessageHelper(SMTP_SOCKET, MailSending);

	std::string MyTo = "RCPT TO: <" + e.getTo() + ">";
	sendMessageHelper(SMTP_SOCKET, MyTo);

	std::string MyCC = "RCPT TO: <" + e.getCC() + ">";
	sendMessageHelper(SMTP_SOCKET, "RCPT TO: <" + e.getCC() + ">");

	std::string MyBCC = "RCPT TO: <" + e.getBCC() + ">";
	sendMessageHelper(SMTP_SOCKET, "RCPT TO: <" + e.getBCC() + ">");

	std::string text = e.getTextBody();
	sendMessageHelper(SMTP_SOCKET, text);

	//Send the mail
	sendMessageHelper(SMTP_SOCKET, e.AllOfEmailContent());

	closesocket(SMTP_SOCKET);
}

bool setUpConnectionIPv4(const char* IP, int PORT) {
	SOCKET client_IPv4 = socket(AF_INET, SOCK_STREAM, 0);
	if (client_IPv4 == INVALID_SOCKET) {
		std::cout << "Socket connection error for IPv4";
		return false;
	}
	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(PORT);
	serverAddress.sin_addr.s_addr = inet_addr(IP);

	if (connect(client_IPv4, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
		std::cout << "Fail to connect";
		closesocket(client_IPv4);
		return false;
	}

	return true;
}

int main() {

	return 0;
}

//Some note:

/*
*calling WSADATA to store information about the winsock implementations:

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cerr << "Failed to initialize WinSock." << std::endl;
		return false;
	}

	remember to release data by WSACleanup();

*/

/*
#include <arpa/inet.h>

 if (inet_pton(AF_INET, "127.168.0.1", &serv_addr.sin_addr) <= 0) {
		std::cout << "Invalid address");
		return ;
	}

	checking ips
*/

/*
* sent message example :

	const char* greetingFromClient ="Hello I'm client! ";
	int SentBytes = send(client_IPv4, greetingFromClient, strlen(greetingFromClient), 0);

	if (SentBytes == SOCKET_ERROR) {
		int errorCode = WSAGetLastError();
		std::cout << "Fail to sent message";
	}
	else {
		std::cout << "Success";
	}

*/

/*
* receive message example :

	char receiveMessage[1024];
	int ReceviveBytes = recv(client_IPv4, receiveMessage, sizeof(receiveMessage), 0);

	if (ReceviveBytes > 0) {
		std::cout << receiveMessage << std::endl;
	}
	else if (ReceviveBytes == 0) {
		std::cout << "Connection close";
	}
	else {
		std::cout << "Fail to receive";
	}

*/