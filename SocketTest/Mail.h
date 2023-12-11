#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

class Attachment {
private:
	std::string m_filename;
	std::string m_fileType;
	std::vector<char> m_inFileContent;
public:
	void setFilename(std::string name);
	void setFileType(std::string name);
	std::vector<char> readAttachmentFileContent(std::string filename, int& size);
public:
	std::string getFileType();
	std::string getFilename();
	std::vector<char>& getInFileContent();
	std::size_t getSizeOfFile();
public:
	Attachment();
	void deleteInvaidAttachment(Attachment nameAttachment);
	Attachment(std::string filename, int& size);
};

class Mail : public Attachment {
private:
	std::string m_From;
	std::vector<std::string> m_To;
	std::vector<std::string> m_CC;
	std::vector<std::string> m_BCC;
	std::string m_Subject;
	std::string m_TextBody;
	std::vector<Attachment> m_attachments;
	int m_attachmentsSize;
	bool m_hadRead;
public:
	std::string getFrom();
	std::string getTo(int num);
	std::string getCC(int num);
	std::string getBCC(int num);
	std::string getSubject();
	std::string getTextBody();
public:
	std::string getAttachmentFilename(int num);
	std::string getAttachmentFileType(int num);
	std::vector<char> getAttachment(std::string filename);
	std::vector<char> getAttachment1(int num);
public:
	void setFrom(std::string name);
	void setTo(std::string name, int num);
	void setCC(std::string name, int num);
	void setBCC(std::string name, int num);
	void setSubject(std::string data);
	void setBodyText(std::string data);
	void setbaseUnread();
	void addAttachment(std::string& filename);
	int sizeofTo();
	int sizeofCC();
	int sizeofBCC();
	size_t getSizeOfAttachments();
	void setAsRead();
	void setAsUnread();

public:
	std::string to_base64(std::vector<char>& data);
public:
	Mail();
	Mail(std::string From,
		std::vector<std::string> To,
		std::vector<std::string> CC,
		std::vector<std::string> BCC,
		std::string Subject,
		std::string TextBody,
		std::vector<Attachment> attachments);
	std::string getBasicMailData();
	std::string getAllMailData(std::string purpose);
};

