#include <iostream>
#include "HeaderS.h"
#include <map>
#include <vector>

using namespace std;

int main()
{
    string dArray[4];
    string* sqlConDate = SQL_Connect_Data(dArray); // ������ ������ � ������� ��� ����������� � ���� ������ MySQL

    // ������� ������� ��������
    createQuery(sqlConDate, "CREATE TABLE IF NOT EXISTS Customers (id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(50) NOT NULL, sername VARCHAR(50) NOT NULL, email VARCHAR(100), hash VARCHAR(100));");
    // ������� ������� ���������
    createQuery(sqlConDate, "CREATE TABLE IF NOT EXISTS Messages (id INT AUTO_INCREMENT PRIMARY KEY, id_sender INT NOT NULL, id_receiver INT NOT NULL, text VARCHAR(500), mess_date DATETIME NOT NULL, read_mess TINYINT(1) NOT NULL DEFAULT 0, sent_mess TINYINT NOT NULL DEFAULT 0);");
    // ������� ������� ������ �����������
    createQuery(sqlConDate, "CREATE TABLE IF NOT EXISTS Connect_data (MESSAGE_LENGTH INT, PORT INT, server_address VARCHAR(20));");

    // ��������� ����� �� ������� ������ �����������
    char* ifEmpty = createQueryOut(sqlConDate, "SELECT EXISTS(SELECT 1 FROM Connect_data);");

    if (*ifEmpty == '0') // ���� ������� � ������� ����������� �����
    {
        createQuery(sqlConDate, "INSERT INTO Connect_data (MESSAGE_LENGTH, PORT, server_address) VALUES (1024, 7777, '127.0.0.1');"); // ��������� ������� ������� �����������
    }

    createQuery(sqlConDate, "SELECT * FROM Connect_data");

    string curName;          //  ������� ��� ������������
    string curSername;       //  ������� ������� ������������
    string curEmail;         //  ������� ����� ����������� ����� ������������
    int hash;                //  ������� ��� ������������
    bool persPresence = false;       //  ���� ���� ������������ � ����� ������ � �������
    bool nameInChatAlready = false;  //  ���� ������������ � ����� ������ ��� ���� � ����

    string contProcNumber = "0";     // ��������� ��������, ��� 'string' ��� ����������� ������������ ���������

    Chat presChat;

    while (true)
    {
        while (!persPresence)  // ������������ �� ����� � �� �����������������
        {
            while (contProcNumber != "1" && contProcNumber != "2" && contProcNumber != "3")  //  ������ ������ ������������� �����
            {
                cout << "Choose the action, please: 1 - to enter; 2 - to log in; 3 - exit." << endl;
                getline(cin, contProcNumber);
            }

            if (contProcNumber == "1")
            {
                presChat.entrChat(persPresence, curName, hash);      // ����� � ���
                contProcNumber = "0";
            }

            if (contProcNumber == "2")
            {
                presChat.regChat(nameInChatAlready, persPresence, curName, curSername, curEmail, hash, sqlConDate);    // ������������������ � ����
                contProcNumber = "0";
            }

            if (contProcNumber == "3") break;  //  ����� �� �����
        }
        if (contProcNumber == "3") break;      //  ����� �� ���������

        while (true)                       //  ���� ������������ ���� ������������ �� ����� �� ������� ��� �� ���������
        {
            while (contProcNumber != "1" && contProcNumber != "2")  //  ������ �� ������������� �����
            {
                cout << "Choose the action, please: 1 - start the dialog; 2 - quit the profile." << endl;
                getline(cin, contProcNumber);
            }

            if (contProcNumber == "2")     //  ���� ������ �����
            {
                cout << endl;
                persPresence = false;
                contProcNumber = "0";
                break;
            }

            if (contProcNumber == "1")      //  if sending a message is chosen
            {
                presChat.sendMess(curName, sqlConDate);
                contProcNumber = "0";
            }
        }
        if (contProcNumber == "3") break;      //  exit from programme
    }
    return 0;
}
