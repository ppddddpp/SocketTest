#include "MailWorking.h"

bool MailWorking::sendMail(const char* IP, int PORT, std::string username, std::string password, Mail mail)
{
    if (p_SMTP_Client.login(IP, PORT, username, password) == false) {
        return false;
    }
    p_SMTP_Client.sendMail(mail);
    return true;
}

bool MailWorking::receiveMail(const char* IP, int PORT, std::string username, std::string password)
{
    if (p_POP3_Client.login(IP, PORT, username, password) == false) {
        return false;
    }
    bool exist;
    std::vector<Mail> tempReceiveMail = p_POP3_Client.reciveMail();
    for (int i = 0; i < tempReceiveMail.size(); i++) {
        exist = false;
        for (int j = 0; j < m_ListOfMail.size(); j++) {
            //checking the mail are same
            //if they are the same 
            exist = true;
        }
        if (!exist) {
            m_ListOfMail.push_back(tempReceiveMail[i]);
        }
    }
    return true;
}

