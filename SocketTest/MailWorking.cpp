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
    int choose = 10;
    person.getMailData(choose);
    std::string PathWantToSave = "";
    person.saveFileLocally(person.getMail(choose), PathWantToSave, 2);
    //explain: the number is the order of file want to save ( the second file)
}

