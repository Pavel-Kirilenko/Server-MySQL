#include "HeaderS.h"
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>

#define MESSAGE_LENGTH 1024 // data buffer's max size
#define PORT 7777 // port's number

using namespace std;

/***** Функции взаимодействия с базой данных *****/

string* SQL_Connect_Data(string dArray[]) // Ввести данные для подключения к MySQL
{
    string sHostName, sUserName, sMySQLPass, sDataBaseName;
    cout << "Input the host (for example 'localhost'):" << endl;
    getline(cin, sHostName);
    cout << "Input user's name: " << endl;
    getline(cin, sUserName);
    cout << "Input MySQL's password: " << endl;
    getline(cin, sMySQLPass);
    cout << "Input database's name: " << endl;
    getline(cin, sDataBaseName);
    dArray[0] = sHostName;
    dArray[1] = sUserName;
    dArray[2] = sMySQLPass;
    dArray[3] = sDataBaseName;
    return dArray;
}

void createQuery(string* sqlConDate, const char* query) // Создать запрос для базы данных.
{
    const char* hostName = sqlConDate[0].c_str();
    const char* userName = sqlConDate[1].c_str();
    const char* mySQLPass = sqlConDate[2].c_str();
    const char* dataBaseName = sqlConDate[3].c_str();

    MYSQL mysql;
    mysql_init(&mysql);

    if (!mysql_real_connect(&mysql, hostName, userName, mySQLPass, dataBaseName, NULL, NULL, 0)) {
        cout << "Error: can't connect to database " << mysql_error(&mysql) << endl;
        return;
    }

    mysql_set_character_set(&mysql, "utf8");

    if (mysql_query(&mysql, query)) {
        cout << "Query error: " << mysql_error(&mysql) << endl;
    }

    mysql_close(&mysql);
}

char* createQueryOut(string* sqlConDate, const char* query) // Создать запрос для базы данных с выводом значения типа char*
{
    const char* hostName = sqlConDate[0].c_str();
    const char* userName = sqlConDate[1].c_str();
    const char* mySQLPass = sqlConDate[2].c_str();
    const char* dataBaseName = sqlConDate[3].c_str();

    MYSQL mysql;
    mysql_init(&mysql);

    if (!mysql_real_connect(&mysql, hostName, userName, mySQLPass, dataBaseName, NULL, NULL, 0)) {
        cout << "Error: can't connect to database " << mysql_error(&mysql) << endl;
        return nullptr;
    }

    mysql_set_character_set(&mysql, "utf8");

    if (mysql_query(&mysql, query)) {
        cout << "Query error: " << mysql_error(&mysql) << endl;
    }

    MYSQL_RES* res = mysql_store_result(&mysql);
    char* result = nullptr;

    if (res) {
        MYSQL_ROW row = mysql_fetch_row(res);
        if (row) {
            result = strdup(row[0]); // Используем strdup для копирования строки
        }
        mysql_free_result(res);
    }
    else {
        cout << "Ошибка MySql номер " << mysql_error(&mysql) << endl;
    }

    mysql_close(&mysql);
    return result;
}

/***** Функции и классы серверного соединения *****/

void userReg(string& curName, string& curSername, string& curEmail, int& hash, std::string* sqlConDate) // Ввод данных пользователя во время регистрации
{
    cout << "Type your name (less than 15 symbols including spaces): ";
    getline(cin, curName);
    cout << "Type your surname (less than 15 symbols including spaces): ";
    getline(cin, curSername);
    cout << "Type your email (less than 100 symbols including spaces): ";
    getline(cin, curEmail);
    cout << "Type your password (less than 15 symbols including spaces): ";
    string password;
    getline(cin, password);
    hash = (stoi(password) % 50) + (stoi(password) % 49); // Вычислить хэш от пароля

    // Вставка данных в таблицу Customers
    string query = "INSERT INTO Customers (name, sername, email, hash) VALUES ('" + curName + "', '" + curSername + "', '" + curEmail + "', '" + to_string(hash) + "');";
    createQuery(sqlConDate, query.c_str());
}

void userEnt(string& curName, int& hash) // Ввод данных пользователя во время входа в чат
{
    cout << "Type your name (less than 15 symbols including spaces): ";
    getline(cin, curName);
    cout << "Type your password (less than 15 symbols including spaces): ";
    string password;
    getline(cin, password);
    hash = (stoi(password) % 50) + (stoi(password) % 49); // Вычислить хэш от пароля
}

void Chat::entrChat(bool& persPresence, string& curName, int& hash) // Войти в чат
{
    if (persArray.size()) // Если массив не пустой
    {
        userEnt(curName, hash); // Ввести данные пользователя во время регистрации
        map<string, int>::iterator it = persArray.find(curName);
        if (it != persArray.end() && it->second == hash) {
            persPresence = true; // Успешная регистрация
        }
        if (!persPresence) {
            cout << "The user with this name and password not found. Check the input" << endl;
        }
    }
    else {
        cout << "There are no users in the chat yet" << endl;
    }
}

void Chat::regChat(bool& nameInChatAlready, bool& persPresence, string& curName, string& curSername, string& curEmail, int& hash, std::string* sqlConDate) // Зарегистрироваться в чате
{
    userReg(curName, curSername, curEmail, hash, sqlConDate); // Ввод данных пользователя во время регистрации
    map<string, int>::iterator it = persArray.find(curName);
    if (it != persArray.end() || curName.empty()) {
        nameInChatAlready = true; // Если имя уже есть в чате или имя пустое
    }
    if (nameInChatAlready) { // Регистрация отклонена
        cout << "The user with this name is already in the chat or the data is not correct (empty name or password). Repeat the input" << endl;
        nameInChatAlready = false;
    }
    else {
        persArray.insert({ curName, hash }); // Вставка данных пользователя в массив
        cout << "Welcome to the chat, " << curName << "!" << endl;
        persPresence = true;
    }
}

void Chat::sendMess(string curName, std::string* sqlConDate) // Отправить сообщение
{
    struct sockaddr_in serveraddress, client;
    socklen_t length;
    int sockert_file_descriptor, connection, bind_status, connection_status;
    char message[MESSAGE_LENGTH];
    string suffix = "_(" + curName + " is writing to you)";
    const char* suf = suffix.c_str();

    // Создаем сокет
    sockert_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (sockert_file_descriptor == -1) {
        cout << "Socket creation failed.!" << endl;
        return;
    }

    serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_family = AF_INET;

    // Привязываем сокет
    bind_status = bind(sockert_file_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress));
    if (bind_status == -1) {
        cout << "Socket binding failed.!" << endl;
        return;
    }

    // Начинаем слушать
    connection_status = listen(sockert_file_descriptor, 5);
    if (connection_status == -1) {
        cout << "Socket is unable to listen for new connections.!" << endl;
        return;
    }
    else {
        cout << "Server is listening for new connection: " << endl;
    }

    length = sizeof(client);
    connection = accept(sockert_file_descriptor, (struct sockaddr*)&client, &length);
    if (connection == -1) {
        cout << "Server is unable to accept the data from client.!" << endl;
        return;
    }

    // Установление связи
    while (true) {
        bzero(message, MESSAGE_LENGTH);
        read(connection, message, sizeof(message));
        if (strncmp("end", message, 3) == 0) {
            cout << "Client Exited." << endl;
            cout << "Server is Exiting..!" << endl;
            break;
        }

        // Если сообщение начинается с "register", обрабатываем регистрацию
        if (strncmp(message, "register", 8) == 0) {
            string userData(message + 9); // Получаем данные пользователя после "register "
            size_t pos = userData.find(',');
            string curName = userData.substr(0, pos);
            userData.erase(0, pos + 1);
            pos = userData.find(',');
            string curSername = userData.substr(0, pos);
            userData.erase(0, pos + 1);
            string curEmail = userData; // Остальные данные - это email

            // Вставка данных в таблицу Customers
            int hash = (curName.length() % 50) + (curName.length() % 49); // Простой хэш
            string query = "INSERT INTO Customers (name, sername, email, hash) VALUES ('" + curName + "', '" + curSername + "', '" + curEmail + "', '" + to_string(hash) + "');";
            createQuery(sqlConDate, query.c_str());
            cout << "User  registered: " << curName << endl;
            continue; // Продолжаем цикл, чтобы ожидать следующее сообщение
        }

        cout << "Message: " << message << endl;

        // Запись сообщения в базу данных
        string query = "INSERT INTO Messages (id_sender, id_receiver, text, mess_date, read_mess, sent_mess) VALUES (1, 2, '" + string(message) + "', NOW(), 0, 1);";
        createQuery(sqlConDate, query.c_str());

        // Отправка ответа клиенту
        bzero(message, MESSAGE_LENGTH);
        cout << "Enter the message: " << endl;
        cin.getline(message, MESSAGE_LENGTH - 37);
        strcat(message, suf);
        ssize_t bytes = write(connection, message, sizeof(message));
        if (bytes >= 0) {
            cout << "     ***     " << endl;
        }
    }

    close(sockert_file_descriptor);
    return;
}
