#include "Mail.h"

#pragma region Attachment

std::string Attachment::getFilename()
{
	return m_filename;
}

std::vector<uint8_t>& Attachment::getInFileContent()
{
	return m_inFileContent;
}

std::size_t Attachment::getSizeOfFile()
{
	return m_inFileContent.size();
}

Attachment::Attachment(std::string filename, std::vector<uint8_t> fileContent)
{
	m_filename = filename;
	m_inFileContent = fileContent;
}

#pragma endregion Attachment

#pragma region Mail

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

std::vector<uint8_t> Mail::getAttachment(int num)
{
	return m_attachments[num].getInFileContent();
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

std::string Mail::getAllMailData()
{
	std::string ThingToSend;
	//edit the string to send: 
	ThingToSend += getSubject();
	ThingToSend += '\n\n\n';
	ThingToSend += getTextBody();

	//this send method should reconsider ?
	//send attachments
	std::string AttachmentAsStr = "";
	for (int i = 0; i < getSizeOfAttachments(); i++) {
		std::vector<uint8_t>tempAttachment = getAttachment(i);
		AttachmentAsStr = to_base64(tempAttachment);
		AttachmentAsStr += "[ENDOFATTACHMENT]";
	}
	ThingToSend += AttachmentAsStr;
	return ThingToSend;
}

std::vector<uint8_t> Mail::readAttachmentFileContent(std::string filename)
{
	std::ifstream infileName(filename, std::ios::binary);
	if (!infileName.is_open()) {
		throw std::runtime_error("Cannot open file: " + filename);
	}

	infileName.seekg(0, std::ios::end);
	std::size_t fileSize = infileName.tellg();
	infileName.seekg(0, std::ios::beg);

	int maxSize = 3 * 1024 * 1024;
	if (fileSize > maxSize) {
		throw std::runtime_error("Attachment file size exceeds the allowed limit");
	}

	std::vector<uint8_t> fileContents((std::istreambuf_iterator<char>(infileName)),
		std::istreambuf_iterator<char>());
	return fileContents;
}

void Mail::addAttachment(std::string& filename)
{
	std::vector<uint8_t> fileContents = readAttachmentFileContent(filename);
	m_attachments.emplace_back(filename, fileContents);
}

std::string Mail::to_base64(std::vector<uint8_t>& data)
{
	const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	std::string ouStr;
	size_t i = 0;

	while (i < data.size()) {
		uint32_t octet_1 = 0;
		uint32_t octet_2 = 0;
		uint32_t octet_3 = 0;

		if (i < data.size()) {
			octet_1 = static_cast<uint8_t>(data[i++]);
		}

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

Mail Mail::toMailFormmat(std::string data)
{
	//may be need this
	Mail newMail;
	return newMail;
}

#pragma endregion Mail