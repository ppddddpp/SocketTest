#include "FolderWorking.h"

#pragma region UserFolder
std::string UserFolder::getMailData(int num)
{
	std::string mailData = m_ListOfMail[num].getMailAllData("open");
	return mailData;
}

void MailFolder::setDataMailAttachment(std::vector<char> mailAttachment, int num)
{
	m_mail.setMailAttachment(mailAttachment, num);
}

MailFolder UserFolder::getMailAttachment(int num)
{
	return {};
}

MailFolder& UserFolder::getMailFolder(int num)
{
	return m_ListOfMail[num];
}

int UserFolder::getSizeOfListMail()
{
	return m_ListOfMail.size();
}

std::string UserFolder::getFolderName()
{
	return m_folderName;
}

void UserFolder::addMailToList(MailFolder& mail)
{
	m_ListOfMail.push_back(mail);
}

void UserFolder::saveMail(MailFolder mail)
{
	m_ListOfMail.push_back(mail);
}

void UserFolder::openMailChosen(int num)
{
	m_ListOfMail[num].openMail();
}

void UserFolder::savedFileLocally(MailFolder mail, std::string& localFilePath, std::string fileWantToSave)
{
	std::vector<char> fileContent = mail.getMailAttachment(fileWantToSave);
	std::string placeToSave = localFilePath + '/' + fileWantToSave;
	Mail temp;
	std::ofstream writeFile(placeToSave, std::ios::binary);
	std::string fileData = "";
	for (char i : fileContent)
		fileData += i;
	std::vector <unsigned char> decodedFileData = temp.base64_decode(fileData);
	for (char i : decodedFileData)
	{
		char buf[1] = { i };
		writeFile.write(buf, sizeof(buf));
	}
	writeFile.close();
	std::cout << "File downloaded and saved locally at: " << placeToSave << std::endl;
}

std::string UserFolder::getWorkingUserPath()
{
	return "/." + getFolderName();
}

void MailFolder::extractMailDataDownloaded(std::string data)
{
	std::string localWorkingData = data;
	std::string mailDataExacted = "";
	
	std::stringstream issTo(localWorkingData);

	


	std::vector<char>attachmentDataExtracted;
	int countFile = -1;
	size_t startPosAttachment = localWorkingData.find("[STARTOFATTACHMENT]" + '\r\n');
	size_t endPosAttachment = localWorkingData.find("[ENDOFATTACHMENT]" + '\r\n');
	while (startPosAttachment != std::string::npos && endPosAttachment != std::string::npos)
	{
		countFile++;
		std::string extractedAttachment = localWorkingData.substr(startPosAttachment + std::strlen("[STARTOFATTACHMENT]"),
			endPosAttachment - startPosAttachment - std::strlen("[STARTOFATTACHMENT]"));

		std::vector<char> attachmentData(extractedAttachment.begin(), extractedAttachment.end());
		attachmentDataExtracted.insert(attachmentDataExtracted.end(), attachmentData.begin(), attachmentData.end());

		setDataMailAttachment(attachmentDataExtracted, countFile);

		localWorkingData = localWorkingData.substr(endPosAttachment + std::strlen("[ENDOFATTACHMENT]"));
		
		startPosAttachment = localWorkingData.find("[STARTOFATTACHMENT]");
		endPosAttachment = localWorkingData.find("[ENDOFATTACHMENT]");

	}
}

void UserFolder::addFilter(std::string filter)
{
	if ('r' == filter[1]) // From
	{
		while (filter.npos != filter.find('@'))
		{
			int startOfEmailAddr = filter.find_first_of(' ');
			m_fromFilter.push_back(filter.substr(startOfEmailAddr + 1, 
				filter.find_first_of(',') - startOfEmailAddr - 1));
			filter = filter.substr(filter.find_first_of(',') + 1);
			if (filter.npos == filter.find(','))
			{
				int startOfEmailAddr = filter.find_first_of(' ');
				if ("..." != filter.substr(startOfEmailAddr + 1,
					filter.find_first_of('-') - startOfEmailAddr - 2))
				{
					m_fromFilter.push_back(filter.substr(startOfEmailAddr + 1,
						filter.find_first_of('-') - startOfEmailAddr - 2));
				}
			}
		}
	}
	else if ('u' == filter[1]) // Subject
	{
		while (filter.npos != filter.find('"'))
		{
			int startOfEmailAddr = filter.find_first_of('"');
			m_subjectFilter.push_back(filter.substr(startOfEmailAddr + 1,
				filter.find_first_of(',') - startOfEmailAddr - 2));
			filter = filter.substr(filter.find_first_of(',') + 1);
			if (filter.npos == filter.find(','))
			{
				int startOfEmailAddr = filter.find_first_of(' ');
				m_subjectFilter.push_back(filter.substr(startOfEmailAddr + 1,
					filter.find_last_of('"') - startOfEmailAddr - 1));
			}
		}
	}
	else if ('o' == filter[1]) // Content
	{
		while (filter.npos != filter.find('"'))
		{
			int startOfEmailAddr = filter.find_first_of('"');
			m_subjectFilter.push_back(filter.substr(startOfEmailAddr + 1,
				filter.find_first_of(',') - startOfEmailAddr - 2));
			filter = filter.substr(filter.find_first_of(',') + 1);
			if (filter.npos == filter.find(','))
			{
				int startOfEmailAddr = filter.find_first_of(' ');
				m_contentFilter.push_back(filter.substr(startOfEmailAddr + 1,
					filter.find_last_of('"') - startOfEmailAddr - 1));
			}
		}
	}
	else if ('p' == filter[1]) // Spam
	{
		while (filter.npos != filter.find('"'))
		{
			int startOfEmailAddr = filter.find_first_of('"');
			m_spamFilter.push_back(filter.substr(startOfEmailAddr + 1,
				filter.find_first_of(',') - startOfEmailAddr - 2));
			filter = filter.substr(filter.find_first_of(',') + 1);
			if (filter.npos == filter.find(','))
			{
				int startOfEmailAddr = filter.find_first_of(' ');
				m_fromFilter.push_back(filter.substr(startOfEmailAddr + 1,
					filter.find_last_of('"') - startOfEmailAddr - 1));
			}
		}
	}
}

UserFolder::UserFolder(std::string folderName)
{
	m_folderName = folderName;
}

#pragma endregion UserFolder

#pragma region MailFolder
std::string MailFolder::getMailBasicData()
{
	return m_mail.getBasicMailData();
}

std::string MailFolder::getMailAllData(std::string purpose)
{
	std::string tempData = m_mail.getAllMailData(purpose);
	return tempData;
}

Mail MailFolder::getMail()
{
	return m_mail;
}

void MailFolder::setMailName(int count)
{
	std::stringstream builder;
	builder << "msg" << std::to_string(count);
	m_folderName = builder.str();
}

void MailFolder::setMailAttachment(std::vector<char> attachmentName, int num)
{
	m_mail.setMailAttachment(attachmentName, num);
}

std::string MailFolder::getMailName()
{
	return m_folderName;
}

void MailFolder::openMail()
{

	m_mail.getAllMailData("open");
}

std::vector<char> MailFolder::getMailAttachment(std::string filename)
{
	return m_mail.getAttachment(filename);
}

void MailFolder::mailToFolder(std::string folderName)
{
	std::string mailPath = "./" + folderName + "./" + getMailName() + ".txt";
	std::ofstream mailAsFile(mailPath);

	// To
	mailAsFile << "To: ";
	for (int i = 0; i < m_mail.getToSize(); i++)
	{
		if (m_mail.getToSize() - 1 == i)
			mailAsFile << m_mail.getTo(i);
		else mailAsFile << m_mail.getTo(i) << ", ";
	}
	mailAsFile << '\n';

	// Cc
	mailAsFile << "CC: ";
	for (int i = 0; i < m_mail.getCCSize(); i++)
	{
		if (m_mail.getToSize() - 1 == i)
			mailAsFile << m_mail.getCC(i);
		else mailAsFile << m_mail.getCC(i) << ", ";
	}
	mailAsFile << '\n';

	// From
	mailAsFile << "From: ";
	mailAsFile << m_mail.getFrom();
	mailAsFile << '\n';

	//Subject
	mailAsFile << m_mail.getSubject();
	mailAsFile << '\n';
	mailAsFile << '\n';

	//Contents
	mailAsFile << m_mail.getTextBody();
	return;
}

void MailFolder::attachmentToFolder(std::string folderName, int attachmentNumber)
{
	std::string mailPath = "./" + folderName + "./" + m_mail.getAttachmentFilename(attachmentNumber) + "." + m_mail.getAttachmentFileType(attachmentNumber);
	std::ofstream mailAsFile(mailPath, std::ios::binary);

	std::string fileData = "";
	for (char i : m_mail.getAttachment1(attachmentNumber))
	{
		fileData += i;
	}
	std::vector <unsigned char> decodedFileData = m_mail.base64_decode(fileData);
	for (char i : decodedFileData)
	{
		char buf[1] = {i};
		mailAsFile.write(buf, sizeof(buf));
	}
	return;
}

MailFolder::MailFolder(Mail mail)
{
	m_mail = mail;
}

MailFolder::MailFolder()
{
	// do nothing
}


#pragma endregion MailFolder

std::vector<std::string> UserFolder::getFromFilter()
{
	return m_fromFilter;
}

std::vector<std::string> UserFolder::getSubjectFilter()
{
	return m_subjectFilter;
}

std::vector<std::string> UserFolder::getContentFilter()
{
	return m_contentFilter;
}

std::vector<std::string> UserFolder::getSpamFilter()
{
	return m_spamFilter;
}
