#include "MailWorking.h"

bool MailWorking::sendMail(const char* IP, int PORT, User person, Mail mail, bool connected)
{
    if (!connected)
    {
        if (p_SMTP_Client.login(IP, PORT, person) == false) {
            return false;
        }
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

void MailWorking::closeSMTPconnection()
{
    p_SMTP_Client.disconnectServer();
}

void display(MailWorking* test, User& usertest, bool& isDone, bool& connectedToPOP3)
{
    bool connectedToSMTP = false;
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

            test->sendMail(usertest.getServerIP().c_str(), usertest.getPortSMTP(), usertest, email, connectedToSMTP);
            std::cout << std::endl;
        }

        else if ("2" == choice)
        {
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
            int folderNumber = std::stoi(buffer) - 1;

        openFolderAgain:
            usertest.openFolder(folderNumber);

            std::cout << "Which email do you want to read: (Enter nothing to return to menu or 0 to show list again)" << std::endl;
            getline(std::cin, buffer);
            if ("0" == buffer)
                goto openFolderAgain;
            if ("" == buffer)
                continue;
            std::cout << "Email number " << buffer << "'s contents: " << std::endl;
            int mailFolderNumber = std::stoi(buffer) - 1;
            std::vector<Attachment> attachmentsTemp = usertest.openMail(folderNumber, mailFolderNumber);
            int numberOfAttachments = attachmentsTemp.size();
            if (0 != numberOfAttachments)
            {
                if (1 != numberOfAttachments)
                    std::cout << "In this email, there exist " << numberOfAttachments << " attachment files, do you want to save to local computer:" << std::endl;
                else std::cout << "In this email, there exists 1 attachment file, do you want to save to local computer: (yes or no)" << std::endl;
                getline(std::cin, buffer);
                if ("yes" == buffer)
                {
                    std::cout << "Enter your desired local file path for the downloaded attachment: ";
                    getline(std::cin, buffer);
                    for (int i = 0; i < numberOfAttachments; i++)
                    {
                        usertest.getFolder(folderNumber).savedFileLocally(usertest.getFolder(folderNumber).getMailFolder(mailFolderNumber), buffer, attachmentsTemp[i].getFilename() + "." + attachmentsTemp[i].getFileType());
                    }
                }
            }
            for (int i = 0; i < 5; i++)
            {
                int numberOfMails = usertest.getSizeOfFolder(i);
                for (int j = 0; j < numberOfMails; j++)
                {
                    if ("Read" == usertest.getFolder(i).getMailFolder(j).getMail().getReadStatus())
                    {
                        std::string readStatus;
                        std::string filePath = "./" + usertest.getUserMail() + "/" + usertest.getFolder(i).getFolderName() + "/"
                            + "msg" + std::to_string(j + 1) + "/" + "msg" + std::to_string(j + 1) + ".txt";
                        std::ifstream inFile(filePath);
                        bool stopAdd = false;
                        while (!inFile.eof())
                        {
                            std::getline(inFile, readStatus);
                            if ("READ" == readStatus)
                            {
                                stopAdd = true;
                            }
                        }
                        inFile.close();
                        if (!stopAdd)
                        {
                            std::ofstream outFile(filePath, std::ios::app);
                            outFile << "READ";
                        }
                    }
                }
            }
        }

        else if ("3" == choice)
        {
            connectedToPOP3 = false;
            isDone = true;
            return;
        }
        std::cout << std::endl;
    }
}

void Menu::start()
{
    MailWorking* test = new MailWorking();
    std::string fileToRead;
    //std::ifstream testFile("config.txt");
    //if (testFile.is_open())
    //{
    //    fileToRead = "config.txt";
    //}
    std::ifstream testFile("config.json");
    if (testFile.is_open())
    {
        fileToRead = "config.json";
    }
    User person(fileToRead);
    test->receiveMail(person.getServerIP().c_str(), person.getPortPOP3(), person);
    bool isDone = false;
    bool connectedToPOP3 = true;
    std::thread mainDisplay(display, std::ref(test), std::ref(person), std::ref(isDone), std::ref(connectedToPOP3));
    int seconds = person.getAutoLoad();
    while (!isDone)
    {
        while (connectedToPOP3)
        {
            std::this_thread::sleep_for(std::chrono::seconds(seconds));
            test->getPOP3().receiveMail(person);
        }
    }
    mainDisplay.join();
}

POP3& MailWorking::getPOP3()
{
    return p_POP3_Client;
}