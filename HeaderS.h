#pragma once
#include <string>
#include <map>
#include <vector>
#include "mysql.h"

/***** ������� �������������� � ����� ������ *****/

std::string* SQL_Connect_Data(std::string dArray[]); // ������ ������ ��� ����������� � MySQL

void createQuery(std::string* sqlConDate, const char* query); // ������� ������ ��� ���� ������

char* createQueryOut(std::string* sqlConDate, const char* query); // ������� ������ ��� ���� ������ � ������� �������� ���� int

/***** ������� � ������ ���������� ���������� *****/

void userReg(std::string& curName, std::string& curSername, std::string& curEmail, int& hash, std::string* sqlConDate); // ���� ������ ������������ �� ����� �����������

void userEnt(std::string& curName, int& hash); // ���� ������ ������������ �� ����� ����� � ���

class Chat
{
    std::map <std::string, int> persArray; // ������ � ������� ������������
public:
    void entrChat(bool& persPresence, std::string& curName, int& hash); // ����� � ���
    void regChat(bool& nameInChatAlready, bool& persPresence, std::string& curName, std::string& curSername, std::string& curEmail, int& hash, std::string* sqlConDate); // ������������������ � ����
    void sendMess(std::string curName, std::string* sqlConDate); // ��������� ���������
};

