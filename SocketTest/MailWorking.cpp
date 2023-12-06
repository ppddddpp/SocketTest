#include "MailWorking.h"

bool MailWorking::sendMail(const char* IP, int PORT, User person, Mail mail)
{
    if (p_SMTP_Client.login(IP, PORT, person) == false) {
        return false;
    }
    p_SMTP_Client.sendMail(mail);
    return true;
}

bool MailWorking::receiveMail(const char* IP, int PORT, User person)
{
    if (p_POP3_Client.login(IP, PORT, person) == false) {
        return false;
    }
    bool exist;
    std::vector<Mail> tempReceiveMail = p_POP3_Client.receiveMail();
    for (int i = 0; i < tempReceiveMail.size(); i++) {
        exist = false;
        for (int j = 0; j < person.getSizeOfListMail(); j++) {
            //checking the mail are same
            //if they are the same 
            exist = true;
        }
        if (!exist) {
            person.addMailToList(tempReceiveMail[i]);
        }
    }
    return true;
}

