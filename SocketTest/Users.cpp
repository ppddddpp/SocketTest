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

Mail User::getMail(int num)
{
	return m_ListOfMail[num];
}

void User::openMail()
{
	//function to output all mail details
	int count = 1;
	for (int i = 0; i < getSizeOfListMail(); i++) {
		std::string basicMailData = m_ListOfMail[i].getBasicMailData();
		std::cout << count<< "." << basicMailData << std::endl;
		count++;
	}
}

User::User(std::string filename)
{
	std::ifstream in(filename);
	std::string buffer;

	// General
	std::getline(in, buffer);
	// ???

	// Username
	std::getline(in, buffer);
	setUsername(buffer.substr(buffer.find(':') + 1, buffer.find('<') - buffer.find(':') - 1));

	// UserMail
	setUserMail(buffer.substr(buffer.find('<'), buffer.find('>') - buffer.find('<')));

	// Password
	std::getline(in, buffer);
	setPassword(buffer.substr(buffer.find(':') + 1));

	// Server IP
	std::getline(in, buffer);
	setServerIP(buffer.substr(buffer.find(':') + 1));

	// SMTP
	std::getline(in, buffer);
	setPortSMTP(std::stoi(buffer.substr(buffer.find(':') + 1)));

	// POP3
	std::getline(in, buffer);
	setPortPOP3(std::stoi(buffer.substr(buffer.find(':') + 1)));

	// Autoload
	std::getline(in, buffer);
	setAutoLoad(std::stoi(buffer.substr(buffer.find(':') + 1)));
}

std::string User::getMailData(int num) 
{
	std::string mailData = m_ListOfMail[num].getAllMailData("open");
	return mailData;
}

void User::saveFileLocally(Mail mail, std::string& localFilePath, int fileWantToSave)
{
	std::vector<char> fileContent = mail.getAttachment(fileWantToSave);
	std::ofstream localFile(localFilePath, std::ios::binary);
	if (localFile.is_open()) {
		localFile.write(fileContent.data(), fileContent.size());
		localFile.close();
		std::cout << "File downloaded and saved locally at: " << localFilePath << std::endl;
	}
	else {
		std::cout << "Error: Cannot open the local file for writing" << std::endl;
	}
}
