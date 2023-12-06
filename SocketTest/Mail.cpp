#include "Mail.h"

Mail::Mail()
{
	m_TextBody = "Hello";
}

std::string Mail::AllOfMailContent(std::string MyMail)
{
	{
		std::string mailContent = "From: " + MyMail + "\n";
		//mailContent += "To: " + m_To + "\n";
		//mailContent += "Cc: " + m_CC + "\n";
		//mailContent += "Bcc: " + m_BCC + "\n";
		//mailContent += "Subject: " + m_Subject + "\n";
		//mailContent += "\n\n";
		//mailContent += m_TextBody + "\n\n";

		//sending the attachment 

		return mailContent;
	}
}
