#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

class Attachment {
private:
	std::string m_filename;
	std::vector<uint8_t> m_inFileContent;
public:
	std::string getFilename();
	std::vector<uint8_t>& getInFileContent();
	std::size_t getSizeOfFile();
public:
	Attachment(std::string filename,std::vector<uint8_t> fileContent);
};

class Mail {
private:
	std::vector<std::string> m_To;
	std::vector<std::string> m_CC;
	std::vector<std::string> m_BCC;
	std::string m_Subject;
	std::string m_TextBody;
	std::vector<Attachment> m_attachments;
public:
	std::string getTo(int num);
	std::string getCC(int num);
	std::string getBCC(int num);
	std::string getSubject();
	std::string getTextBody();
	std::vector<uint8_t> getAttachment(int num);
public:
	int sizeofTo();
	int sizeofCC();
	int sizeofBCC();
	size_t getSizeOfAttachments();
public:
	std::vector<uint8_t> readAttachmentFileContent(std::string filename);
	void addAttachment(std::string& filename);
};

