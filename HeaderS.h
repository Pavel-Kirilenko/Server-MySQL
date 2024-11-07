#pragma once
#include <string>
#include <map>
#include <vector>
#include "mysql.h"

/***** Функции взаимодействия с базой данных *****/

std::string* SQL_Connect_Data(std::string dArray[]); // Ввести данные для подключения к MySQL

void createQuery(std::string* sqlConDate, const char* query); // Создать запрос для базы данных

char* createQueryOut(std::string* sqlConDate, const char* query); // Создать запрос для базы данных с выводом значения типа int

/***** Функции и классы серверного соединения *****/

void userReg(std::string& curName, std::string& curSername, std::string& curEmail, int& hash, std::string* sqlConDate); // Ввод данных пользователя во время регистрации

void userEnt(std::string& curName, int& hash); // Ввод данных пользователя во время входа в чат

class Chat
{
    std::map <std::string, int> persArray; // Массив с данными пользователя
public:
    void entrChat(bool& persPresence, std::string& curName, int& hash); // Войти в чат
    void regChat(bool& nameInChatAlready, bool& persPresence, std::string& curName, std::string& curSername, std::string& curEmail, int& hash, std::string* sqlConDate); // Зарегистрироваться в чате
    void sendMess(std::string curName, std::string* sqlConDate); // отправить сообщение
};

