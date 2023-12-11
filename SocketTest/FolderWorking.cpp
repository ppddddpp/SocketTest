#include "FolderWorking.h"

#pragma region UserFolder

Mail UserFolder::getMail(int num)
{
	return m_ListOfMail[num];
}

void UserFolder::addMailToList(Mail& mail)
{
	m_ListOfMail.push_back(mail);
}

std::string UserFolder::getMailData(int num)
{
	std::string mailData = m_ListOfMail[num].getAllMailData("open");
	return mailData;
}

void UserFolder::openMail()
{
	//function to output all mail details
	int count = 1;
	for (int i = 0; i < getSizeOfListMail(); i++) {
		std::string basicMailData = m_ListOfMail[i].getBasicMailData();
		std::cout << count << "." << basicMailData << std::endl;
		count++;
	}
}

int UserFolder::getSizeOfListMail()
{
	return m_ListOfMail.size();
}

void UserFolder::saveMail(Mail mail)
{
	m_ListOfMail.push_back(mail);
}

void UserFolder::deleteMail(Mail mail)
{

}

void UserFolder::savedFileLocally(Mail mail, std::string& localFilePath, std::string fileWantToSave)
{
	std::vector<char> fileContent = mail.getAttachment(fileWantToSave);
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

void UserFolder::getSubjectForArrange(std::string name)
{
	for (int i = 0; i < getSizeOfListMail(); i++) {
		if (name == m_ListOfMail[i].getSubject()) {
			std::cout << m_ListOfMail[i].getBasicMailData();
		}
	}
}
#pragma endregion UserFolder