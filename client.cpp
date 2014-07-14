#include <ctime>
#include <QHostAddress>
#include <sstream>
#include <iostream>
#include <string>
#include "client.h"
#include "basedatamaker.h"
#include "basedata.h"

void trimStr(std::string& s)
{
    size_t p = s.find_first_not_of(" \t");
    s.erase(0, p);

    p = s.find_last_not_of(" \t");
    if (std::string::npos != p)
       s.erase(p+1);
}

Client::Client(QObject* parent): QObject(parent)
{
  connect(&client, SIGNAL(connected()),
    this, SLOT(startTransfer()));
}

Client::~Client()
{
  client.close();
}

void Client::start(QString address, quint16 port)
{
  QHostAddress addr(address);
  client.connectToHost(addr, port);
}

void Client::startTransfer()
{
    while (true)
    {
        std::cout << "Enter 0 to send XRF data.\n";
        std::cout << "Enter 1 to send EMS data.\n";
        std::cout << "Enter 2 to send LIBS data.\n";
        std::cout << "Enter X to exit.\n";
        std::cout << "Please enter your choice now. ";

        std::string choice;
        std::getline (std::cin, choice);
        BaseData* bd = 0;

        trimStr(choice);
        if (choice == "x" || choice == "X")
            exit(0);

        // objects populated with sample data.
        //
        else if (choice == "0")
            bd = BaseDataMaker::MakeData(XRF);
        else if (choice == "1")
            bd = BaseDataMaker::MakeData(EMS);
        else if (choice == "2")
            bd = BaseDataMaker::MakeData(LIBS);
        else
            continue;

        QByteArray qArray;
        bool createBufferSuccessful = BaseDataMaker::GenerateBufferForSending(bd, qArray);

        if (!createBufferSuccessful)
        {
            delete bd;
            std::cout << "Unable to create buffer.\n";
            continue;
        }
        client.write(qArray);
        client.flush();
        std::cout << bd->GetName() + " data sent.\n\n";
        delete bd;
    }
}
