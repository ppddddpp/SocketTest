#pragma once
#include <string>
#include <vector> 
#include <iostream>
#include <cstdio>
#include <sstream>
#include "Mail.h"

class MailFolder
{
private:
	Mail m_mail;
	std::string m_folderName;
public:
	//adding set mail set attachments
public:
	void openMail();
	std::string getMailBasicData();
	std::string getMailAllData(std::string purpose);
	void setMailName(int count);
	std::string getMailName();
public:
	std::vector<char> getMailAttachment(std::string filename);
public:
	std::ofstream mailToFolder(std::string filename);
	std::ofstream attachmentToFolder(std::string filename, int attachmentNumber);
public:
	MailFolder(Mail mail);
	MailFolder();
};

class UserFolder
{
private:
	std::string m_folderName;
	std::vector<MailFolder> m_ListOfMail;
public:
	int getSizeOfListMail();
	std::string getFolderName();
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
public:
	UserFolder(std::string folderName);
};