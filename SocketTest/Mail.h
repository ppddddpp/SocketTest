#pragma once
#include <string>
#include <vector>

class Attachment {
private:
	std::string m_filename;
	std::vector<uint8_t> m_inFileContent;
public:
	std::string getFilename() {
		return m_filename;
	}
	const std::vector<uint8_t>& getInFileContent() {
		return m_inFileContent;
	}
	std::size_t getSizeOfFile() {
		return m_inFileContent.size();
	}
public:
	Attachment();
};

class Mail {
private:
	std::string& m_To;
	std::string& m_CC;
	std::string& m_BCC;
	std::string& m_Subject;
	std::string& m_TextBody;
	std::vector<Attachment> m_attachments;
public:
	std::string getTo() {
		return m_To;
	}
	std::string getCC() {
		return m_CC;
	}
	std::string getBCC() {
		return m_BCC;
	}
	std::string getSubject() {
		return m_Subject;
	}
	std::string getTextBody() {
		return m_TextBody;
	}
	size_t getSizeOfAttachments() {
		return m_attachments.size();
	}

public:
	Mail();
	void addAttachment(const std::string& filename, const std::vector<uint8_t>& content);
	std::string AllOfMailContent(std::string MyMail);
};

