#include "FolderWorking.h"

#pragma region UserFolder
std::string UserFolder::getMailData(int num)
{
	std::string mailData = m_ListOfMail[num].getMailAllData("open");
	return mailData;
}

void UserFolder::setDataMailAttachment(std::vector<char> mailAttachment, int num)
{
	m_ListOfMail[num].setMailAttachment(mailAttachment, num);
}

MailFolder UserFolder::getMailAttachment(int num)
{
	return {};
}

MailFolder UserFolder::getMail(int num)
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

void UserFolder::deleteMail(MailFolder mail)
{

}

void UserFolder::setMailSubject(std::string data)
{
	
}

void UserFolder::openMailChosen(int num)
{
	m_ListOfMail[num].openMail();
}

void UserFolder::savedFileLocally(MailFolder mail, std::string& localFilePath, std::string fileWantToSave)
{
	std::vector<char> fileContent = mail.getMailAttachment(fileWantToSave);
	std::string placeToSave = localFilePath + "\\" + fileWantToSave;

	std::ofstream localFile(placeToSave, std::ios::binary);
	if (localFile.is_open()) {
		localFile.write(fileContent.data(), fileContent.size());
		localFile.close();
		std::cout << "File downloaded and saved locally at: " << placeToSave << std::endl;
	}
	else {
		std::cout << "Error: Cannot open the local file for writing" << std::endl;
	}
}


std::string UserFolder::getWorkingUserPath()
{
	return "/." + getFolderName();
}

void UserFolder::exactMailDataDownloaded(std::string data)
{
	std::string localWorkingData = data;
	std::string mailDataExacted = "";
	
	std::stringstream issTo(localWorkingData);

	


	std::vector<char>attachmentDataExacted;
	int countFile = -1;
	size_t startPosAttachment = localWorkingData.find("[STARTOFATTACHMENT]" + '\r\n');
	size_t endPosAttachment = localWorkingData.find("[ENDOFATTACHMENT]" + '\r\n');
	while (startPosAttachment != std::string::npos && endPosAttachment != std::string::npos)
	{
		countFile++;
		std::string extractedAttachment = localWorkingData.substr(startPosAttachment + std::strlen("[STARTOFATTACHMENT]"),
			endPosAttachment - startPosAttachment - std::strlen("[STARTOFATTACHMENT]"));

		std::vector<char> attachmentData(extractedAttachment.begin(), extractedAttachment.end());
		attachmentDataExacted.insert(attachmentDataExacted.end(), attachmentData.begin(), attachmentData.end());

		setDataMailAttachment(attachmentDataExacted, countFile);

		localWorkingData = localWorkingData.substr(endPosAttachment + std::strlen("[ENDOFATTACHMENT]"));
		
		startPosAttachment = localWorkingData.find("[STARTOFATTACHMENT]");
		endPosAttachment = localWorkingData.find("[ENDOFATTACHMENT]");

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
	return m_mail.getAllMailData(purpose);
}

void MailFolder::setMailName(int count)
{
	std::stringstream builder;
	builder << "(" << m_mail.getReadStatus() << ")" << " msg" << std::to_string(count);
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

std::ofstream MailFolder::mailToFolder(std::string folderName)
{
	std::string mailPath = "./" + folderName + "./" + getMailName() + ".txt";
	std::ofstream mailAsFile(mailPath);

	//

	//questionable file path or return void?
	std::string mailFilePath = mailPath;
	return std::ofstream(mailFilePath);
}

std::ofstream MailFolder::attachmentToFolder(std::string folderName, int attachmentNumber)
{
	std::string mailPath = "./" + folderName + "./" + m_mail.getAttachmentFilename(attachmentNumber) + "." + m_mail.getAttachmentFileType(attachmentNumber);
	std::ofstream mailAsFile(mailPath);

	//

	//questionable file path or return void?
	std::string mailFilePath = mailPath;
	return std::ofstream(mailFilePath);
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