#include "Mail.h"

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
}

void Attachment::deleteInvaidAttachment(Attachment nameAttachment)
{
	//perform the delete attachment
}

Attachment::Attachment(std::string filename, int& size)
{
	std::string LocalWorking = filename;
	int dotLocation = LocalWorking.find('.');
	setFilename(LocalWorking.substr(0, dotLocation));
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

std::string Mail::getCC(int num)
{
	return m_CC[num];
}

std::string Mail::getBCC(int num)
{
	return m_BCC[num];
}

std::string Mail::getSubject()
{
	return m_Subject;
}

std::string Mail::getTextBody()
{
	return m_TextBody;
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
	m_hadRead = false;
}

int Mail::sizeofTo()
{
	return m_To.size();
}

int Mail::sizeofCC()
{
	return m_CC.size();
}

int Mail::sizeofBCC()
{
	return m_BCC.size();
}

size_t Mail::getSizeOfAttachments()
{
	return m_attachments.size();
}

void Mail::setAsRead()
{
	m_hadRead = true;
}

void Mail::setAsUnread()
{
	m_hadRead = false;
}

void Mail::setBodyText(std::string data)
{
	m_TextBody = data;
}

std::string Mail::getAllMailData(std::string purpose)
{
	std::string ThingToSend = "";

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
	ThingToSend += "\r\n";
	ThingToSend += "\r\n";
	ThingToSend += getTextBody();

	if (purpose == "send" || purpose == "check") {
		//send attachments
		std::string AttachmentInfo = " BASE64!\r\n";
		std::string AttachmentAsStr = "";
		for (int i = 0; i < getSizeOfAttachments(); i++) {
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
		}
		ThingToSend = ThingToSend + AttachmentInfo + AttachmentAsStr;
	}
	else if (purpose == "open") {
		std::string AttachmentInfo = " List of attachments: \r\n";
		std::string AttachmentAsStr = "";
		for (int i = 0; i < getSizeOfAttachments(); i++) {
			AttachmentInfo = getAttachmentFilename(i) + "." 
				+ getAttachmentFileType(i) + "\r\n";
		}
		setAsRead();
	}
	return ThingToSend;
}

void Mail::addAttachment(std::string& filename)
{
	int size = m_attachmentsSize;
	Attachment attachmentNeedToAdd = Attachment(filename, size);
	int maxSize = 3 * 1024 * 1024;
	if (size > maxSize) {
		std::cout << "Attachment file size is too large ! 3MB is maximum" << std::endl;
		Attachment::deleteInvaidAttachment(attachmentNeedToAdd);
	}
	m_attachments.emplace_back(attachmentNeedToAdd);
}

Mail::Mail()
{
	m_hadRead = false;
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
		addAttachment(filename);
		m_attachmentsSize += i.getSizeOfFile();
	}
}

std::string Mail::getBasicMailData()
{
	std::string data = "";
	if (m_hadRead == false) {
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

		/*if (i < data.size()) {
			octet_1 = static_cast<uint8_t>(data[i++]);
		}*/

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


#pragma endregion Mail
