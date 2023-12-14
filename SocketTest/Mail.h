#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

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
	void setFileContent(std::vector<char> attachmentData);
	std::string getFileType();
	std::string getFilename();
	std::vector<char>& getInFileContent();
	std::size_t getSizeOfFile();
public:
	Attachment();
	void deleteInvalidAttachment(Attachment nameAttachment);
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
	bool m_isRead;
public:
	std::string getFrom();
	std::string getTo(int num);
	int getToSize();
	std::string getCC(int num);
	int getCCSize();
	std::string getBCC(int num);
	int getBCCSize();
	std::string getSubject();
	std::string getTextBody();
	std::string getReadStatus();
public:
	std::string getAttachmentFilename(int num);
	std::string getAttachmentFileType(int num);
	std::vector<char> getAttachment(std::string filename);
	std::vector<char> getAttachment1(int num);
	std::vector<Attachment> getAllAttachments();
public:
	void setFrom(std::string name);
	void setTo(std::string name, int num);
	void setCC(std::string name, int num);
	void setBCC(std::string name, int num);
	void setSubject(std::string data);
	void setBodyText(std::string data);
	void setbaseUnread();
	void setMailAttachment(std::vector<char> attachmentData, int num);
	void addAttachmentToSend(std::string& filename);
	size_t getSizeOfAttachments();
	void setAsRead();
	void setAsUnread();

public:
	std::string to_base64(std::vector<char>& data);
	std::vector<unsigned char> base64_decode(std::string const& encoded_string);
public:
	Mail();
	Mail(std::string data);
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

