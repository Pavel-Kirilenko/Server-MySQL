#pragma once
#include <string>
#include <map>
#include <vector>

/***** Функции и классы серверного соединения *****/

void userReg(std::string& curName, std::string& curSername, std::string& curEmail, int hash); // Ввод данных пользователя во время регистрации

void userEnt(std::string& curName, int hash); // Ввод данных пользователя во время входа в чат

class Chat
{
	std::map <std::string, int> persArray; // Массив с данными пользователя
public:
	void entrChat(bool& persPresence, std::string& curName, int hash); // Войти в чат
	void regChat(bool& nameInChatAlready, bool& persPresence, std::string& curName, std::string& curSername, std::string& curEmail, int hash); // Зарегистрироваться в чате
	void sendMess(std::string curName); // отправить сообщение
};