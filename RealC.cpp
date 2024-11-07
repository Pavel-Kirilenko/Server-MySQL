#include"HeaderC.h"
#include<iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>

#define MESSAGE_LENGTH 1024 // data buffer's max size
#define PORT 7777 // port's number

using namespace std;

/***** Функции и классы серверного соединения *****/

void userReg(string& curName, string& curSername, string& curEmail, int hash) // Ввод данных пользователя во время регистрации
{
    cout << "Type you name (less than 15 symbols including spaces): ";
    getline(cin, curName);
    cout << "Type you sername (less than 15 symbols including spaces): ";
    getline(cin, curSername);
    cout << "Type you email (less than 15 symbols including spaces): ";
    getline(cin, curEmail);
    cout << "Type your password (less than 15 symbols including spaces): ";
    string password;
    getline(cin, password);
    hash = (stoi(password) % 50) + (stoi(password) % 49); // Вычислить хэш от пароля
}

void userEnt(string& curName, int hash) // Ввод данных пользователя во время входа в чат
{
    cout << "Type you name (less than 15 symbols including spaces): ";
    getline(cin, curName);
    cout << "Type your password (less than 15 symbols including spaces): ";
    string password;
    getline(cin, password);
    hash = (stoi(password) % 50) + (stoi(password) % 49); // Вычислить хэш от пароля
}

void Chat::entrChat(bool& persPresence, string& curName, int hash) // Зарегистрироваться в чате
{
    if (persArray.size()) // Если массив не пустой
    {
        userEnt(curName, hash); // Ввести данные пользователя во время регистрации
        map <string, int>::iterator it = persArray.find(curName);
        if (it != persArray.end() && it->second == hash) persPresence = true; // Успешная регистрация
        if (persPresence == false)
        {
            cout << "The user whith this name and password not found. Check the input" << endl;
        }
    }
    else
    {
        cout << "There are no users in the chat yet" << endl;
    }
}
void Chat::regChat(bool& nameInChatAlready, bool& persPresence, string& curName, string& curSername, string& curEmail, int hash) // Зарегистрироваться в чате
{
    int socket_file_descriptor;
    userReg(curName, curSername, curEmail, hash); // Ввод данных пользователя во время регистрации
    map <string, int>::iterator it = persArray.find(curName);
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

        // Отправляем данные о регистрации на сервер
        string message = "register " + curName + "," + curSername + "," + curEmail; // Формат: "register имя,фамилия,email"
        ssize_t bytes = write(socket_file_descriptor, message.c_str(), message.size());
        if (bytes >= 0) {
            cout << "User  registration data sent to server." << endl;
        }
        persPresence = true; // Успешная регистрация
    }
}

void Chat::sendMess(string curName) // Отправить сообщение
{
    cout << "to quit the dialog type 'end'" << endl;
    int socket_file_descriptor, connection;
    struct sockaddr_in serveraddress, client;
    char message[MESSAGE_LENGTH];
    string suffix = "_(" + curName + " is writing to you)";
    const char* suf = suffix.c_str();
    // создаем сокет

    socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_file_descriptor == -1) {
        cout << "Creation of Socket failed!" << endl;
        return;
    }

    // определяем адрес сервера
    serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    // определяем номер порта
    serveraddress.sin_port = htons(PORT);
    // используем IPv4
    serveraddress.sin_family = AF_INET;
    // подключаемся к серверу
    connection = connect(socket_file_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress));
    if (connection == -1) {
        cout << "Connection with the server failed.!" << endl;
        return;
    }
    // взаимодействие с сервером
    while (1) {
        bzero(message, sizeof(message));
        cout << "Enter the message: " << endl;
        cin.getline(message, MESSAGE_LENGTH - 37);
        if ((strncmp(message, "end", 3)) == 0) {
            write(socket_file_descriptor, message, sizeof(message));
            cout << "Exit." << endl;
            break;
        }
        strcat(message, suf);
        ssize_t bytes = write(socket_file_descriptor, message, strlen(message));
        // если >= 0  байты переданы, значит передача успешна
        if (bytes >= 0) {
            cout << "     ***     " << endl;
        }
        bzero(message, sizeof(message));
        // ожидание ответа от сервера
        read(socket_file_descriptor, message, sizeof(message));
        cout << "Message: " << message << endl;
    }
    // закрываем сокет, останавливаем соединение
    close(socket_file_descriptor);
    return;
}
