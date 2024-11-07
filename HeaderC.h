#pragma once
#include <string>
#include <map>
#include <vector>

/***** ������� � ������ ���������� ���������� *****/

void userReg(std::string& curName, std::string& curSername, std::string& curEmail, int hash); // ���� ������ ������������ �� ����� �����������

void userEnt(std::string& curName, int hash); // ���� ������ ������������ �� ����� ����� � ���

class Chat
{
	std::map <std::string, int> persArray; // ������ � ������� ������������
public:
	void entrChat(bool& persPresence, std::string& curName, int hash); // ����� � ���
	void regChat(bool& nameInChatAlready, bool& persPresence, std::string& curName, std::string& curSername, std::string& curEmail, int hash); // ������������������ � ����
	void sendMess(std::string curName); // ��������� ���������
};