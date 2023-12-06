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

std::vector<uint8_t> Mail::readAttachmentFileContent(std::string filename)
{
	std::ifstream infileName(filename, std::ios::binary);
	if (!infileName.is_open()) {
		std::cout << "Cannot open file";
		return {};
		// may using the std::exception later 
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

#pragma endregion Mail