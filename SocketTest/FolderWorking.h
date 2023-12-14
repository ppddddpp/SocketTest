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
	std::string getMailName();
	std::string getMailAllData(std::string purpose);
	Mail getMail();
public:
	void setMailName(int count);
	void setMailSubject(std::string data);
	void setMailTo(std::string data, int num);
	void setMailCC(std::string data, int num);
	void setMailBCC(std::string data, int num);
	void setMailBody(std::string data);
	void setMailAttachment(std::vector<char> attachmentName, int num);
public:
	std::vector<char> getMailAttachment(std::string filename);
	void extractMailDataDownloaded(std::string data);
	void setDataMailAttachment(std::vector<char> mailAttachment, int num);
public:
	void mailToFolder(std::string folderName);
	void attachmentToFolder(std::string folderName, int attachmentNumber);
public:
	MailFolder(Mail mail);
	MailFolder();
};

class UserFolder
{
private:
	std::vector<std::string> m_fromFilter;
	std::vector<std::string> m_subjectFilter;
	std::vector<std::string> m_contentFilter;
	std::vector<std::string> m_spamFilter;
	std::string m_folderName;
	std::vector<MailFolder> m_ListOfMail;
public:
	int getSizeOfListMail();
	std::string getFolderName();
	void addMailToList(MailFolder& mail);
	void saveMail(MailFolder mail);
	void deleteMail(MailFolder mail);
public:
	void setMailSubject(std::string data);
	void setMailTo(std::string data, int num);
	void setMailCC(std::string data, int num);
	void setMailBCC(std::string data, int num);
	void setMailBody(std::string data);
	std::vector<std::string> getFromFilter();
	std::vector<std::string> getSubjectFilter();
	std::vector<std::string> getContentFilter();
	std::vector<std::string> getSpamFilter();
public:
	MailFolder getMailAttachment(int num);
	MailFolder& getMailFolder(int num);
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
	void addFilter(std::string filter);
public:
	UserFolder(std::string folderName);
};