#include "MailWorking.h"

bool MailWorking::sendMail(const char* IP, int PORT, User person, Mail mail)
{
    if (p_SMTP_Client.login(IP, PORT, person) == false) {
        return false;
    }
    p_SMTP_Client.sendMail(mail);
    return true;
}

bool MailWorking::receiveMail(const char* IP, int PORT, User& person)
{
    if (p_POP3_Client.login(IP, PORT, person) == false) {
        return false;
    }
    p_POP3_Client.receiveMail(person);
}

