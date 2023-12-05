#include "Mail.h"

std::string Mail::AllOfMailContent(std::string MyMail)
{
	{
		std::string mailContent = "From: " + MyMail + "\n";
		mailContent += "To: " + _To + "\n";
		mailContent += "Cc: " + _CC + "\n";
		mailContent += "Bcc: " + _BCC + "\n";
		mailContent += "Subject: " + _Subject + "\n";
		mailContent += "\n\n";
		mailContent += _TextBody + "\n\n";

		//sending the attachment 

		return mailContent;
	}
}
