#include "Header.h"

#pragma region Attachment

void Attachment::setFilename(std::string name)
{
	m_filename = name;
}

void Attachment::setFileType(std::string name)
{
	m_fileType = name;
}

std::vector<char> Attachment::readAttachmentFileContent(std::string filename, int& size)
{
	std::ifstream infileName(filename, std::ios::binary);
	if (!infileName.is_open()) {
		std::cout << "Cannot open file " << filename << std::endl;
		return {};
	}

	infileName.seekg(0, std::ios::end);
	std::size_t fileSize = infileName.tellg();
	infileName.seekg(0, std::ios::beg);

	size += fileSize;

	std::vector<char> fileContents((std::istreambuf_iterator<char>(infileName)),
		std::istreambuf_iterator<char>());
	return fileContents;
}

void Attachment::setFileContent(std::vector<char> attachmentData)
{
	m_inFileContent = attachmentData;
}

std::string Attachment::getFileType()
{
	return m_fileType;
}

std::string Attachment::getFilename()
{
	return m_filename;
}

std::vector<char>& Attachment::getInFileContent()
{
	return m_inFileContent;
}

std::size_t Attachment::getSizeOfFile()
{
	return m_inFileContent.size();
}

Attachment::Attachment()
{
	// do nothing
}

void Attachment::deleteInvalidAttachment(Attachment nameAttachment)
{
	//perform the delete attachment
}

Attachment::Attachment(std::string filename, int& size)
{
	std::string LocalWorking = filename;
	int backwardSlashLocation = LocalWorking.find('/');
	int dotLocation = LocalWorking.find_last_of('.');
	setFilename(LocalWorking.substr(backwardSlashLocation + 1, dotLocation - backwardSlashLocation - 1));
	setFileType(LocalWorking.substr(dotLocation + 1));
	m_inFileContent = readAttachmentFileContent(filename, size);
}

#pragma endregion Attachment

#pragma region Mail

std::string Mail::getFrom()
{
	return m_From;
}

std::string Mail::getTo(int num)
{
	return m_To[num];
}

int Mail::getToSize()
{
	return m_To.size();
}

std::string Mail::getCC(int num)
{
	return m_CC[num];
}

int Mail::getCCSize()
{
	return m_CC.size();
}

std::string Mail::getBCC(int num)
{
	return m_BCC[num];
}

int Mail::getBCCSize()
{
	return m_BCC.size();
}

std::string Mail::getSubject()
{
	return m_Subject;
}

std::string Mail::getTextBody()
{
	return m_TextBody;
}

std::string Mail::getReadStatus()
{
	return (m_isRead ? "Read" : "Unread");
}

std::string Mail::getAttachmentFilename(int num)
{
	return m_attachments[num].getFilename();
}

std::string Mail::getAttachmentFileType(int num)
{
	return m_attachments[num].getFileType();
}

std::vector<char> Mail::getAttachment(std::string filename)
{
	for (int i = 0; i < getSizeOfAttachments();i++) {
		std::string checkingFilename = m_attachments[i].getFilename() + "."
			+ m_attachments[i].getFileType();
		if (filename == checkingFilename) {
			return m_attachments[i].getInFileContent();
		}
	}
	return {};
}

std::vector<char> Mail::getAttachment1(int num)
{
	return m_attachments[num].getInFileContent();
}

std::vector<Attachment> Mail::getAllAttachments()
{
	return m_attachments;
}

void Mail::setFrom(std::string name)
{
	m_From = name;
}

void Mail::setTo(std::string name, int num)
{
	m_To.push_back(name);
}

void Mail::setCC(std::string name, int num)
{
	m_CC.push_back(name);
}

void Mail::setBCC(std::string name, int num)
{
	m_BCC.push_back(name);
}

void Mail::setSubject(std::string data)
{
	m_Subject = data;
}

void Mail::setbaseUnread()
{
	m_isRead = false;
}

void Mail::setMailAttachment(std::vector<char> attachmentData,int num)
{
	m_attachments[num].setFileContent(attachmentData);
}

size_t Mail::getSizeOfAttachments()
{
	return m_attachments.size();
}

void Mail::setAsRead()
{
	m_isRead = true;
}

void Mail::setAsUnread()
{
	m_isRead = false;
}

void Mail::setBodyText(std::string data)
{
	m_TextBody = data;
}

std::string Mail::getAllMailData(std::string purpose)
{
	std::string ThingToSend = "";
	if (purpose == "open" || purpose == "check" || purpose == "save") {
		if (purpose == "open")
		{
			setAsRead();
		}
		std::string AttachmentInfo = " List of attachments: \r\n";
		std::string AttachmentAsStr = "";
		for (int i = 0; i < getSizeOfAttachments(); i++) {
			AttachmentInfo = getAttachmentFilename(i) + "."
				+ getAttachmentFileType(i) + "\r\n";
		}
		ThingToSend += "To: ";
		for (std::string i : m_To)
		{
			ThingToSend += i + ", ";
		}
		ThingToSend[ThingToSend.size() - 2] = ' ';
		ThingToSend += "\r\n";

		ThingToSend += "Cc: ";
		for (std::string i : m_CC)
		{
			ThingToSend += i + ", ";
		}
		ThingToSend[ThingToSend.size() - 2] = ' ';
		ThingToSend += "\r\n";

		ThingToSend += "From: " + m_From;
		ThingToSend += "\r\n";

		ThingToSend += getSubject();
		ThingToSend += getTextBody();
		return ThingToSend;
	}
	ThingToSend += "To: ";
	for (std::string i : m_To)
	{
		ThingToSend += i + ", ";
	}
	ThingToSend[ThingToSend.size() - 2] = ' ';
	ThingToSend += "\r\n";

	ThingToSend += "Cc: ";
	for (std::string i : m_CC)
	{
		ThingToSend += i + ", ";
	}
	ThingToSend[ThingToSend.size() - 2] = ' ';
	ThingToSend += "\r\n";

	ThingToSend += "From: " + m_From;
	ThingToSend += "\r\n";

	ThingToSend += "Subject: " + getSubject();

	ThingToSend += "\r\n";
	ThingToSend += "\r\n";

	ThingToSend += "Body:" + getTextBody();

	if (purpose == "send") {
		//send attachments
		for (int i = 0; i < getSizeOfAttachments(); i++) {
			std::string AttachmentInfo = "\r\nBASE64!\r\n";
			std::string AttachmentAsStr = "";
			AttachmentAsStr = " [STARTOFATTACHMENT]\r\n";
			AttachmentInfo += "Filename:" + getAttachmentFilename(i) + "\r\n";
			AttachmentInfo += "FileType:" + getAttachmentFileType(i) + "\r\n";
			std::vector<char>tempAttachment = getAttachment1(i);
			std::string FileData = to_base64(tempAttachment);
			for (int i = 0; i < FileData.size(); i += 100)
			{
				AttachmentAsStr += FileData.substr(i, 100) + "\r\n";
			}
			AttachmentAsStr += " [ENDOFATTACHMENT]\r\n";
			ThingToSend = ThingToSend + AttachmentInfo + AttachmentAsStr;
		}
	}
	return ThingToSend;
}

void Mail::addAttachmentToSend(std::string& filename)
{
	int size = m_attachmentsSize;
	Attachment attachmentNeedToAdd = Attachment(filename, size);
	int maxSize = 3 * 1024 * 1024;
	if (size > maxSize) {
		std::cout << "Attachment file size is too large ! 3MB is maximum" << std::endl;
		Attachment::deleteInvalidAttachment(attachmentNeedToAdd);
	}
	m_attachments.emplace_back(attachmentNeedToAdd);
}

Mail::Mail()
{
	m_isRead = false;
}

Mail::Mail(std::string data)
{
	m_isRead = false;
	std::stringstream mailData(data);
	std::string buffer;
	getline(mailData, buffer);

	// To
	int count = 0;
	getline(mailData, buffer);
	while (buffer.npos != buffer.find(','))
	{
		setTo(buffer.substr(buffer.find(' ') + 1, buffer.find(',') - buffer.find(' ') - 1), count);
		count++;
		buffer = buffer.substr(buffer.find(','));
	}
	setTo(buffer.substr(buffer.find(' ') + 1, buffer.find_last_of(' ') - buffer.find(' ') - 1), count);

	// Cc
	count = 0;
	getline(mailData, buffer);
	while (buffer.npos != buffer.find(','))
	{
		setCC(buffer.substr(buffer.find(' ') + 1, buffer.find(',') - buffer.find(' ') - 1), count);
		count++;
		buffer = buffer.substr(buffer.find(','));
	}
	setCC(buffer.substr(buffer.find(' ') + 1, buffer.find_last_of(' ') - buffer.find(' ') - 1), count);

	// From
	getline(mailData, buffer);
	setFrom(buffer.substr(buffer.find(' ') + 1, buffer.find_last_of('\r') - buffer.find(' ') - 1));

	// Subject
	getline(mailData, buffer);
	buffer = buffer.substr(0, buffer.size() - 1);
	setSubject(buffer);

	// Contents
	std::string bodyText = " ";
	getline(mailData, buffer);
	buffer = buffer.substr(0, buffer.size() - 1);
	while ("BASE64!" != buffer && !mailData.eof())
	{
		buffer += '\n';
		bodyText += buffer;
		getline(mailData, buffer);
		if (".\r" == buffer)
			break;
		buffer = buffer.substr(0, buffer.size() - 1);
	}
	setBodyText(bodyText);

	// Attachments
	while (!mailData.eof())
	{
		Attachment newAttachment;
		getline(mailData, buffer);
		if (buffer == "")
		{
			break;
		}
		newAttachment.setFilename(buffer.substr(buffer.find(':') + 1, buffer.find('\r') - buffer.find(':') - 1));
		getline(mailData, buffer);
		newAttachment.setFileType(buffer.substr(buffer.find(':') + 1, buffer.find('\r') - buffer.find(':') - 1));
		getline(mailData, buffer);
		std::vector<char> inFileContents;
		getline(mailData, buffer);
		bool foundEndOFAttach = false;
		while (!foundEndOFAttach)
		{
			int size = buffer.size() - 1;
			for (int i = 0; i < size; i++)
			{
				inFileContents.push_back(buffer[i]);
			}
			getline(mailData, buffer);
			if (buffer.npos != buffer.find("[ENDOFATTACHMENT]"))
			{
				foundEndOFAttach = true;
			}
		}
		newAttachment.setFileContent(inFileContents);
		m_attachments.push_back(newAttachment);
		while ("BASE64!\r" != buffer && !mailData.eof())
		{
			getline(mailData, buffer);
		}
	}
}

Mail::Mail(std::string From, 
	std::vector<std::string> To, 
	std::vector<std::string> CC,
	std::vector<std::string> BCC,
	std::string Subject, 
	std::string TextBody, 
	std::vector<Attachment> attachments)
{
	setFrom(From);
	for (std::string i : To)
	{
		setTo(i, 0);
	}
	
	for (std::string i : CC)
	{
		setCC(i, 0);
	}
	
	for (std::string i : BCC)
	{
		setBCC(i, 0);
	}
	setSubject(Subject);
	setBodyText(TextBody);
	m_attachmentsSize = 0;
	for (Attachment i : attachments)
	{
		std::string filename = i.getFilename() + '.' + i.getFileType();
		addAttachmentToSend(filename);
		m_attachmentsSize += i.getSizeOfFile();
	}
}

std::string Mail::getBasicMailData()
{
	std::string data = "";
	if (m_isRead == false) {
		data += "(Unread)";
	}
	else {
		data += "(Read)";
	}
	data = data + " " + getFrom() + " , " + getSubject();
	return data;
}

std::string Mail::to_base64(std::vector<char>& data)
{
	const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	std::string ouStr;
	size_t i = 0;

	while (i < data.size()) {
		uint32_t octet_1 = 0;
		uint32_t octet_2 = 0;
		uint32_t octet_3 = 0;

		octet_1 = static_cast<uint8_t>(data[i++]);

		if (i < data.size()) {
			octet_2 = static_cast<uint8_t>(data[i++]);
		}

		if (i < data.size()) {
			octet_3 = static_cast<uint8_t>(data[i++]);
		}

		uint32_t triple = (octet_1 << 16) + (octet_2 << 8) + octet_3;

		ouStr.push_back(base64_chars[(triple >> 18) & 0x3F]);
		ouStr.push_back(base64_chars[(triple >> 12) & 0x3F]);

		if (i > data.size() + 1) {
			ouStr.push_back('=');
		}
		else {
			ouStr.push_back(base64_chars[(triple >> 6) & 0x3F]);
		}

		if (i > data.size()) {
			ouStr.push_back('=');
		}
		else {
			ouStr.push_back(base64_chars[triple & 0x3F]);
		}
	}
	return ouStr;
}

static inline bool is_base64(unsigned char c) {
	return (isalnum(c) || (c == '+') || (c == '/'));
}

typedef unsigned char BYTE;
std::vector<BYTE> Mail::base64_decode(std::string const& encoded_string) {
	const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	int in_len = encoded_string.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	BYTE char_array_4[4], char_array_3[3];
	std::vector<BYTE> ret;

	while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
		char_array_4[i++] = encoded_string[in_]; in_++;
		if (i == 4) {
			for (i = 0; i < 4; i++)
				char_array_4[i] = base64_chars.find(char_array_4[i]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; (i < 3); i++)
				ret.push_back(char_array_3[i]);
			i = 0;
		}
	}

	if (i) {
		for (j = i; j < 4; j++)
			char_array_4[j] = 0;

		for (j = 0; j < 4; j++)
			char_array_4[j] = base64_chars.find(char_array_4[j]);

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

		for (j = 0; (j < i - 1); j++) ret.push_back(char_array_3[j]);
	}

	return ret;
}

#pragma endregion Mail
