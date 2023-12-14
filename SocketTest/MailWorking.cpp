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
    //UserFolder folderWorking = person.getFolder(1);
    //std::string PathWantToSave = "";
    //std::string wantToSaveFile = "";
    //folderWorking.savedFileLocally(folderWorking.getMail(0), PathWantToSave, wantToSaveFile);
    //explain: the number is the order of file want to save ( the second file)
}

bool MailWorking::openMail()
{

    return false;
}

void display(MailWorking& test, User& usertest, bool& isDone)
{
    while (true)
    {
        std::string choice;
        std::cout << "Please pick an option from the menu: " << std::endl;
        std::cout << "1. To send an email" << std::endl;
        std::cout << "2. To see the list of received emails" << std::endl;
        std::cout << "3. Quit" << std::endl;
        std::cout << "You chose: ";
        getline(std::cin, choice);

        if ("1" == choice)
        {
            std::string buffer;
            std::cout << "This is data for email sending: (if none for any field, press enter to skip)" << std::endl;
            std::cout << "To: ";
            std::vector<std::string> to;
            getline(std::cin, buffer);
            if (buffer.npos != buffer.find_first_not_of(' '))
            {
                std::string newReceiver;
                if (buffer.npos != buffer.find(','))
                {
                    newReceiver = buffer.substr(buffer.find(' ') + 1, buffer.find(','));
                    buffer = buffer.substr(buffer.find(',') + 1);
                }
                else
                {
                    newReceiver = buffer.substr(buffer.find(' ') + 1);
                }
                to.push_back(newReceiver);
                while (buffer.npos != buffer.find(','))
                {
                    newReceiver = buffer.substr(buffer.find(' ') + 1, buffer.find(','));
                    buffer = buffer.substr(buffer.find(',') + 1);
                    to.push_back(newReceiver);
                    if (buffer.npos != buffer.find(','))
                    {
                        newReceiver = buffer.substr(buffer.find(' ') + 1);
                        to.push_back(newReceiver);
                    }
                }
            }
            std::cout << "CC: ";
            std::vector<std::string> cc;
            getline(std::cin, buffer);
            if (buffer.npos != buffer.find_first_not_of(' '))
            {
                std::string newReceiver;
                if (buffer.npos != buffer.find(','))
                {
                    newReceiver = buffer.substr(buffer.find(' ') + 1, buffer.find(','));
                    buffer = buffer.substr(buffer.find(',') + 1);
                }
                else
                {
                    newReceiver = buffer.substr(buffer.find(' ') + 1);
                }
                cc.push_back(newReceiver);
                while (buffer.npos != buffer.find(','))
                {
                    newReceiver = buffer.substr(buffer.find(' ') + 1, buffer.find(','));
                    buffer = buffer.substr(buffer.find(',') + 1);
                    cc.push_back(newReceiver);
                    if (buffer.npos != buffer.find(','))
                    {
                        newReceiver = buffer.substr(buffer.find(' ') + 1);
                        cc.push_back(newReceiver);
                    }
                }
            }
            std::cout << "BCC: ";
            std::vector<std::string> bcc;
            getline(std::cin, buffer);
            if (buffer.npos != buffer.find_first_not_of(' '))
            {
                std::string newReceiver;
                if (buffer.npos != buffer.find(','))
                {
                    newReceiver = buffer.substr(buffer.find(' ') + 1, buffer.find(','));
                    buffer = buffer.substr(buffer.find(',') + 1);
                }
                else
                {
                    newReceiver = buffer.substr(buffer.find(' ') + 1);
                }
                bcc.push_back(newReceiver);
                while (buffer.npos != buffer.find(','))
                {
                    newReceiver = buffer.substr(buffer.find(' ') + 1, buffer.find(','));
                    buffer = buffer.substr(buffer.find(',') + 1);
                    bcc.push_back(newReceiver);
                    if (buffer.npos != buffer.find(','))
                    {
                        newReceiver = buffer.substr(buffer.find(' ') + 1);
                        bcc.push_back(newReceiver);
                    }
                }
            }

            std::cout << "Subject: ";
            std::string newSubject;
            getline(std::cin, newSubject);

            std::cout << "Content: ";
            std::string newContent;
            getline(std::cin, newContent);

            std::cout << "Send file? (1. yes, 2. no): ";
            getline(std::cin, buffer);
            std::vector<Attachment> Attas;
            if ("1" == buffer)
            {
                int size = 0;
                
                std::cout << "Number of files: ";
                getline(std::cin, buffer);
                int numOfFiles = stoi(buffer);
                for (int i = 0; i < numOfFiles; i++)
                {
                    std::cout << "The file number " << i + 1 << "'s location: ";
                    getline(std::cin, buffer);
                    Attachment newatta(buffer, size);
                    Attas.push_back(newatta);
                }
            }
            
            
            Mail email(usertest.getUserMail(), to, cc, bcc, newSubject, newContent, Attas);

            test.sendMail(usertest.getServerIP().c_str(), usertest.getPortSMTP(), usertest, email);
            std::cout << std::endl;
        }

        else if ("2" == choice)
        {

            test.receiveMail(usertest.getServerIP().c_str(), usertest.getPortPOP3(), usertest);
            std::cout << std::endl;
        }

        else if ("3" == choice)
        {
            isDone = true;
            return;

            std::string buffer;
            std::cout << "Here is a list of folders in your mailbox: " << std::endl;
            std::cout << "1. Inbox" << std::endl;
            std::cout << "2. Project" << std::endl;
            std::cout << "3. Important" << std::endl;
            std::cout << "4. Work" << std::endl;
            std::cout << "5. Spam" << std::endl;
            std::cout << "Which folder do you want to access: (Enter nothing to return to menu)" << std::endl;
            getline(std::cin, buffer);
            if ("1" == buffer)
            {
                std::cout << "Here is the list of mails in your Inbox: " << std::endl;
            }
            else if ("2" == buffer)
            {
                std::cout << "Here is the list of mails in your Project: " << std::endl;
            }
            else if ("3" == buffer)
            {
                std::cout << "Here is the list of mails in your Important: " << std::endl;
            }
            else if ("4" == buffer)
            {
                std::cout << "Here is the list of mails in your Work: " << std::endl;
            }
            else if ("5" == buffer)
            {
                std::cout << "Here is the list of mails in your Spam: " << std::endl;
            }
            else continue;

            // for-loop
            // list

            std::cout << "Which email do you want to read: (Enter nothing to return to menu or 0 to show list again)" << std::endl;
            getline(std::cin, buffer);
            std::cout << "Email number " << buffer << "'s contents: " << std::endl;

            // Email data

            // check for attachment files

            // std::cout << "In this email, there exist(s) attachment file(s), do you want to save to local computer: << std::endl;


        }
        std::cout << std::endl;
    }
}

void Menu::start()
{
    MailWorking test;
    User person("config.txt");

    bool isDone = false;

    std::thread mainDisplay(display, std::ref(test), std::ref(person), std::ref(isDone));

    while (false == isDone)
    {
        int seconds = person.getAutoLoad();
        std::this_thread::sleep_for(std::chrono::seconds(seconds));
        test.getPOP3().receiveMail(person);
    }

    mainDisplay.join();
}

POP3& MailWorking::getPOP3()
{
    return p_POP3_Client;
}