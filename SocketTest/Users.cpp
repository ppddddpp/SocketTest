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

void User::openFolder(int num)
{
	int numberOfMails = m_Folders[num].getSizeOfListMail();
	for (int i = 0; i < numberOfMails; i++)
	{
		Mail temp = m_Folders[num].getMailFolder(i).getMail();
		if ("Unread" == temp.getReadStatus())
			std::cout << i + 1 << ". " << "(Unread) " << temp.getFrom() << ", " << temp.getSubject() << '\n';
		else std::cout << i + 1 << ". " << temp.getFrom() << ", " << temp.getSubject() << '\n';
	}
}

int User::getSizeOfFolder(int i)
{
	return m_Folders[i].getSizeOfListMail();
}

UserFolder& User::getFolder(int num)
{
	return m_Folders[num];
}

UserFolder& User::goThroughFilters(MailFolder mail)
{
	Mail toGetMail = mail.getMail();
	if (m_Folders[0].getFolderName() != goThroughFrom(toGetMail).getFolderName())
		return goThroughFrom(toGetMail);
	else if (m_Folders[0].getFolderName() != goThroughSubject(toGetMail).getFolderName())
		return goThroughSubject(toGetMail);
	else if (m_Folders[0].getFolderName() != goThroughContent(toGetMail).getFolderName())
		return goThroughContent(toGetMail);
	else if (m_Folders[0].getFolderName() != goThroughAll(toGetMail).getFolderName())
		return goThroughAll(toGetMail);
	return m_Folders[0];
}

std::vector<Attachment> User::openMail(int userFolderPosition, int mailPosition)
{
	std::cout << m_Folders[userFolderPosition].getMailFolder(mailPosition).getMailAllData("open");
	int a = 0;
	return m_Folders[userFolderPosition].getMailFolder(mailPosition).getMail().getAllAttachments();
}

UserFolder& User::goThroughFrom(Mail mail)
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
	return m_Folders[0];
}

UserFolder& User::goThroughSubject(Mail mail)
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
				if (subject.npos != subject.find(subjectFilters[j]))
					return m_Folders[i];
			}
		}
	}
	return m_Folders[0];
}

UserFolder& User::goThroughContent(Mail mail)
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
				if (content.npos != content.find(contentFilters[j]))
					return m_Folders[i];
			}
		}
	}
	return m_Folders[0];
}


UserFolder& User::goThroughAll(Mail mail)
{
	std::string content = mail.getSubject() + mail.getTextBody();
	std::vector<std::string> contentFilters = m_Folders[4].getSpamFilter();
	int size = contentFilters.size();
	if (0 == size)
		return m_Folders[0];
	else
	{
		for (int j = 0; j < size; j++)
		{
			if (content.npos != content.find(contentFilters[j]))
				return m_Folders[4];
		}
	}
	return m_Folders[0];
}

UserFolder& User::operator[](int num)
{
	return m_Folders[num];
}

User::User(std::string filename)
{
	if ("config.txt" == filename)
	{
		std::ifstream in(filename);
		std::string buffer;

		// General
		std::getline(in, buffer);

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

		std::string folderPath = "./" + m_userMail;
		try {
			if (!std::filesystem::exists(folderPath)) {
				std::filesystem::create_directories(folderPath);
			}
		}
		catch (const std::filesystem::filesystem_error& e) {
			std::cout << "Error creating folder: " << e.what() << "\n";
		}

		for (int i = 0; i < 5; i++)
		{
			folderPath = "./" + m_userMail + "/" + m_Folders[i].getFolderName();
			try {
				if (!std::filesystem::exists(folderPath)) {
					std::filesystem::create_directories(folderPath);
				}
			}
			catch (const std::filesystem::filesystem_error& e) {
				std::cout << "Error creating folder: " << e.what() << "\n";
			}
		}

		// Filters
		while ("Filter:" != buffer)
		{
			std::getline(in, buffer);
		}

		while (!in.eof())
		{
			std::getline(in, buffer);
			if ("" == buffer)
				continue;
			getFolderFromFilter(buffer).addFilter(buffer);
		}
	}
	else if ("config.json" == filename)
	{
		std::string buffer;
		std::ifstream inFile(filename);
		std::getline(inFile, buffer);
		std::getline(inFile, buffer);
		std::getline(inFile, buffer);
		std::getline(inFile, buffer);
		
		// Username
		buffer = buffer.substr(buffer.find(':') + 1);
		setUsername(buffer.substr(buffer.find('"') + 1, buffer.find('<') - buffer.find('"') - 1));

		// UserMail
		setUserMail(buffer.substr(buffer.find('<') + 1, buffer.find('>') - buffer.find('<') - 1));

		// Password
		std::getline(inFile, buffer);
		buffer = buffer.substr(buffer.find(':') + 1);
		setPassword(buffer.substr(buffer.find('"') + 1, buffer.find_last_of('"') - buffer.find('"') - 1));

		// Server IP
		std::getline(inFile, buffer);
		buffer = buffer.substr(buffer.find(':') + 1);
		setServerIP(buffer.substr(buffer.find('"') + 1, buffer.find_last_of('"') - buffer.find('"') - 1).c_str());

		// SMTP
		std::getline(inFile, buffer);
		buffer = buffer.substr(buffer.find(':') + 1);
		setPortSMTP(std::stoi(buffer.substr(buffer.find(' ') + 1, buffer.find_last_of(',') - buffer.find(' ') - 1)));

		// POP3
		std::getline(inFile, buffer);
		buffer = buffer.substr(buffer.find(':') + 1);
		setPortPOP3(std::stoi(buffer.substr(buffer.find(' ') + 1, buffer.find_last_of(',') - buffer.find(' ') - 1)));

		// Autoload
		std::getline(inFile, buffer);
		setAutoLoad(std::stoi(buffer.substr(buffer.find_last_of(' ') + 1)));

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

		std::string folderPath = "./" + m_userMail;
		try {
			if (!std::filesystem::exists(folderPath)) {
				std::filesystem::create_directories(folderPath);
			}
		}
		catch (const std::filesystem::filesystem_error& e) {
			std::cout << "Error creating folder: " << e.what() << "\n";
		}

		for (int i = 0; i < 5; i++)
		{
			folderPath = "./" + m_userMail + "/" + m_Folders[i].getFolderName();
			try {
				if (!std::filesystem::exists(folderPath)) {
					std::filesystem::create_directories(folderPath);
				}
			}
			catch (const std::filesystem::filesystem_error& e) {
				std::cout << "Error creating folder: " << e.what() << "\n";
			}
		}

		// Filters
		while (!inFile.eof())
		{
			while (!inFile.eof()
				&& buffer.npos == buffer.find("From")
				&& buffer.npos == buffer.find("Subject")
				&& buffer.npos == buffer.find("Content")
				&& buffer.npos == buffer.find("Spam"))
			{
				std::getline(inFile, buffer);
			}

			if (inFile.eof())
			{
				break;
			}
			std::string fromJsonToTxt;
			fromJsonToTxt = buffer.substr(buffer.find_first_of('"') + 1, buffer.find_first_of(':') - buffer.find_first_of('"') - 2) + ":";
			if (fromJsonToTxt.npos != fromJsonToTxt.find("From"))
			{
				buffer = buffer.substr(buffer.find('[') + 1);
				while (buffer.npos != buffer.find(','))
				{
					fromJsonToTxt += " " + buffer.substr(buffer.find_first_of('"') + 1, buffer.find_first_of(',') - buffer.find_first_of('"') - 2) + ",";
					buffer = buffer.substr(buffer.find(',') + 1);
					if (buffer.npos != buffer.find(','))
					{
						fromJsonToTxt += " " + buffer.substr(buffer.find_first_of('"') + 1, buffer.find_last_of('"') - buffer.find_first_of('"') - 1);
						break;
					}
				}
			}
			else
			{
				buffer = buffer.substr(buffer.find('[') + 1);
				while (buffer.npos != buffer.find(','))
				{
					fromJsonToTxt += " " + buffer.substr(buffer.find_first_of('"'), buffer.find_first_of(',') - buffer.find_first_of('"')) + ",";
					buffer = buffer.substr(buffer.find(',') + 1);
					if (buffer.npos != buffer.find(','))
					{
						fromJsonToTxt += " " + buffer.substr(buffer.find_first_of('"'), buffer.find_last_of('"') - buffer.find_first_of('"') + 1);
						break;
					}
				}
			}
			std::getline(inFile, buffer);
			buffer = buffer.substr(buffer.find(':') + 1);
			fromJsonToTxt += " - To folder: " + buffer.substr(buffer.find_first_of('"') + 1, buffer.find_last_of('"') - buffer.find_first_of('"') - 1);
			getFolderFromFilter(fromJsonToTxt).addFilter(fromJsonToTxt);
			std::getline(inFile, buffer);
		}
	}
}

void User::moveMailToFolder(MailFolder mail, UserFolder& toFolder)
{
	mail.setMailName(toFolder.getSizeOfListMail());
	std::string folderPath = "./" + m_userMail + '/' + toFolder.getFolderName() + "/" + mail.getMailName();
	try {
		if (!std::filesystem::exists(folderPath)) {
			std::filesystem::create_directories(folderPath);
			mail.mailToFolder(folderPath);
		}
	}
	catch (const std::filesystem::filesystem_error& e) {
		std::cout << "Error creating folder: " << e.what() << "\n";
	}
}

UserFolder& User::getFolderFromFilter(std::string filter)
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
