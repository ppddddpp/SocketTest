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

void User::setServerIP(const char* serverIP)
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

int User::getSizeOfFolder()
{
	return m_Folders.size();
}

UserFolder User::getFolder(int num)
{
	return m_Folders[num];
}

UserFolder User::goThroughFilters(MailFolder mail)
{
	Mail toGetMail = mail.getMail();
	UserFolder temp = goThroughFrom(toGetMail);
	if ("" != temp.getFolderName())
		return temp;
	temp = goThroughSubject(toGetMail);
	if ("" != temp.getFolderName())
		return temp;
	temp = goThroughContent(toGetMail);
	if ("" != temp.getFolderName())
		return temp;
	return m_Folders[0];
}

UserFolder User::goThroughFrom(Mail mail)
{
	std::string from = mail.getFrom();
	for (int i = 0; i < 5; i++)
	{
		std::vector<std::string> fromFilters = m_Folders[i].getFromFilter();
		int size = fromFilters.size();
		if (0 == size)
			continue;
		else
		{
			for (int j = 0; j < size; j++)
			{
				if (from == fromFilters[j])
					return m_Folders[i];
			}
		}
	}
	return UserFolder("");
}

UserFolder User::goThroughSubject(Mail mail)
{
	std::string subject = mail.getSubject();
	for (int i = 0; i < 5; i++)
	{
		std::vector<std::string> subjectFilters = m_Folders[i].getSubjectFilter();
		int size = subjectFilters.size();
		if (0 == size)
			continue;
		else
		{
			for (int j = 0; j < size; j++)
			{
				if (subject.npos != subject.find(" " + subjectFilters[j] + " "))
					return m_Folders[i];
			}
		}
	}
	return UserFolder("");
}

UserFolder User::goThroughContent(Mail mail)
{
	std::string content = mail.getTextBody();
	for (int i = 0; i < 5; i++)
	{
		std::vector<std::string> contentFilters = m_Folders[i].getContentFilter();
		int size = contentFilters.size();
		if (0 == size)
			continue;
		else
		{
			for (int j = 0; j < size; j++)
			{
				if (content.npos != content.find(" " + contentFilters[j] + " "))
					return m_Folders[i];
			}
		}
	}
	return UserFolder("");
}

UserFolder User::operator[](int num)
{
	return m_Folders[num];
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
	setUserMail(buffer.substr(buffer.find('<') + 1, buffer.find('>') - buffer.find('<') - 1));

	// Password
	std::getline(in, buffer);
	setPassword(buffer.substr(buffer.find(':') + 1));

	// Server IP
	std::getline(in, buffer);
	const char* temp = new char[100];
	buffer = buffer.substr(buffer.find(':') + 2);
	temp = buffer.c_str();
	setServerIP(temp);

	// SMTP
	std::getline(in, buffer);
	setPortSMTP(std::stoi(buffer.substr(buffer.find(':') + 1)));

	// POP3
	std::getline(in, buffer);
	setPortPOP3(std::stoi(buffer.substr(buffer.find(':') + 1)));

	// Autoload
	std::getline(in, buffer);
	setAutoLoad(std::stoi(buffer.substr(buffer.find(':') + 1)));

	// Folders
	UserFolder Inbox("Inbox");
	m_Folders.push_back(Inbox);
	UserFolder Project("Project");
	m_Folders.push_back(Project);
	UserFolder Important("Important");
	m_Folders.push_back(Important);
	UserFolder Work("Work");
	m_Folders.push_back(Work);
	UserFolder Spam("Spam");
	m_Folders.push_back(Spam);

	// Filters
	while ("Filter:" != buffer)
	{
		getline(in, buffer);
	}

	while (!in.eof())
	{
		getline(in, buffer);
		if ("" == buffer)
			continue;
		getFolderFromFilter(buffer).addFilter(buffer);
	}
}

void User::moveMailToFolder(MailFolder mail, UserFolder& toFolder)
{
	mail.setMailName(toFolder.getSizeOfListMail());
	std::string folderPath = "./" + toFolder.getFolderName() + "./" + mail.getMailName();
	try {
		if (!std::filesystem::exists(folderPath)) {
			std::filesystem::create_directories(folderPath);
			std::cout << "Folder created successfully.\n";
		}
		else {
			std::cout << "Folder already exists.\n";
		}
		//remember to add mailPath
		std::string mailPath = "";

		std::filesystem::path mailFilePath = mailPath;
		std::string fileName = mailFilePath.filename().string();

		// Construct the destination path
		std::filesystem::path destinationPath = folderPath;
		destinationPath /= fileName;

		// Copy the file to the destination folder
		std::filesystem::copy(mailPath, destinationPath,
			std::filesystem::copy_options::overwrite_existing);
	}
	catch (const std::filesystem::filesystem_error& e) {
		std::cout << "Error creating folder: " << e.what() << "\n";
	}
}

UserFolder User::getFolderFromFilter(std::string filter)
{
	std::string folder = filter.substr(filter.find_last_of(' ') + 1);
	if ("Project" == folder)
		return m_Folders[1];
	else if ("Important" == folder)
		return m_Folders[2];
	else if ("Work" == folder)
		return m_Folders[3];
	else if ("Spam" == folder)
		return m_Folders[4];
	return m_Folders[0];
}
