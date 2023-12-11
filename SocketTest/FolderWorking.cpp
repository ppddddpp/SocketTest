#include "FolderWorking.h"

#pragma region UserFolder
std::string UserFolder::getMailData(int num)
{
	std::string mailData = m_ListOfMail[num].getMailAllData("open");
	return mailData;
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
	return getExecutablePath();
}

#pragma endregion UserFolder

#pragma region MailFolder
std::string MailFolder::getMailBasicData()
{
	return mail.getBasicMailData();
}

std::string MailFolder::getMailAllData(std::string purpose)
{
	return mail.getAllMailData(purpose);
}

void MailFolder::openMail()
{
	mail.getAllMailData("open");
}

std::vector<char> MailFolder::getMailAttachment(std::string filename)
{
	return mail.getAttachment(filename);
}

std::ofstream MailFolder::mailToFile(std::string filename)
{
	std::ofstream mailAsFile(filename);

	//

	//questionable ? need to return file or path
	std::string mailFilePath = getExecutablePath();
	return mailAsFile;
}

std::ofstream MailFolder::attachmentToFile(std::string filename)
{
	std::ofstream attachmentAsFile(filename);
	
	//

	//questionable ? need to return file or path
	std::string attachmentFilePath = getExecutablePath();
	return attachmentAsFile;
}

#pragma endregion MailFolder

#pragma region GettingWorkingPath
std::string wideToNarrow(const std::wstring& wideStr)
{
	int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, NULL, 0, NULL, NULL);
	std::string result(bufferSize, 0);
	WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, &result[0], bufferSize, NULL, NULL);
	return result;
}

std::string getExecutablePath()
{
	wchar_t buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
	std::wstring wpath = std::wstring(buffer).substr(0, pos);
	return wideToNarrow(wpath);
}

#pragma endregion MailFolder