#pragma once
#include <string>
#include <vector> 
#include <iostream>
#include "Mail.h"

class MailFolder 
{
private:
	Mail mail;
public:
	//adding set mail set attachments
public:
	void openMail();
	std::string getMailBasicData();
	std::string getMailAllData(std::string purpose);
public:
	std::vector<char> getMailAttachment(std::string filename);
public:
	std::ofstream mailToFile(std::string filename);
	std::ofstream attachmentToFile(std::string filename);
};

class UserFolder
{
private:
	std::string m_folderName;
	std::vector<MailFolder> m_ListOfMail;
public:
	int getSizeOfListMail();
	void addMailToList(MailFolder& mail);
	void saveMail(MailFolder mail);
	void deleteMail(MailFolder mail);
public:
	MailFolder getMailAttachment(int num);
	MailFolder getMail(int num);
	std::string getMailData(int num);
public:
	MailFolder operator[](int num) {
		return m_ListOfMail[num];
	}
public:
	void openMailChosen(int num);
public:
	void savedFileLocally(MailFolder mail, std::string& localFilePath, std::string fileWantToSave);
	std::string getWorkingUserPath();
public:
	std::vector<MailFolder> MailSubjectSameAs(std::string name);
	std::vector<MailFolder> UnreadMail();
};


//Exact path for windows

#include <windows.h>
std::string wideToNarrow(const std::wstring& wideStr);
std::string getExecutablePath();


//Exact path for linux

//#include <unistd.h>
//#include <limits.h>
//std::string getExecutablePath() {
//	char buffer[PATH_MAX];
//	ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
//	if (len != -1) {
//		buffer[len] = '\0';
//		std::string::size_type pos = std::string(buffer).find_last_of("/");
//		return std::string(buffer).substr(0, pos);
//	}
//	return "";
//}

//std::string path = std::experimental::filesystem::current_path();

//std::filesystem::path prepend_exe_path(const std::string& filename, const std::string& exe_path = "");
//std::filesystem::path prepend_exe_path(const std::string& filename, const std::string& exe_path)
//{
//	static auto exe_parent_path = fs::path(exe_path).parent_path();
//	return exe_parent_path / filename;
//}

//#include <cstdlib>
//std::string getExecutablePath() {
//	char buffer[PATH_MAX];
//	if (realpath("/proc/self/exe", buffer) != nullptr) {
//		return std::string(buffer);
//	}
//	return std::string();  
//}