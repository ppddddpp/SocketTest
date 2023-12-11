#pragma once
#include <filesystem>
#include <string>
#include <vector> 
#include <iostream>
#include "Mail.h"

class UserFolder
{
private:
	std::string m_folderName;
	std::vector<Mail> m_ListOfMail;
public:
	void openMail();
	Mail getMail(int num);
	int getSizeOfListMail();
	void addMailToList(Mail& mail);
	void saveMail(Mail mail);
	void deleteMail(Mail mail);
public:
	Mail operator[](int num) {
		return m_ListOfMail[num];
	}
	std::string getMailData(int num);

public:
	void savedFileLocally(Mail mail, std::string& localFilePath, std::string fileWantToSave);
	void getSubjectForArrange(std::string name);
};

