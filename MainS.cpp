#include <iostream>
#include "HeaderS.h"
#include <map>
#include <vector>

using namespace std;

int main()
{
    string dArray[4];
    string* sqlConDate = SQL_Connect_Data(dArray); // Ввести массив с данными для подключения к базе данных MySQL

    // Создать таблицу клиентов
    createQuery(sqlConDate, "CREATE TABLE IF NOT EXISTS Customers (id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(50) NOT NULL, sername VARCHAR(50) NOT NULL, email VARCHAR(100), hash VARCHAR(100));");
    // Создать таблицу сообщений
    createQuery(sqlConDate, "CREATE TABLE IF NOT EXISTS Messages (id INT AUTO_INCREMENT PRIMARY KEY, id_sender INT NOT NULL, id_receiver INT NOT NULL, text VARCHAR(500), mess_date DATETIME NOT NULL, read_mess TINYINT(1) NOT NULL DEFAULT 0, sent_mess TINYINT NOT NULL DEFAULT 0);");
    // Создать таблицу данных подключения
    createQuery(sqlConDate, "CREATE TABLE IF NOT EXISTS Connect_data (MESSAGE_LENGTH INT, PORT INT, server_address VARCHAR(20));");

    // Проверить пуста ли таблица данных подключения
    char* ifEmpty = createQueryOut(sqlConDate, "SELECT EXISTS(SELECT 1 FROM Connect_data);");

    if (*ifEmpty == '0') // Если таблица с данными подключения пуста
    {
        createQuery(sqlConDate, "INSERT INTO Connect_data (MESSAGE_LENGTH, PORT, server_address) VALUES (1024, 7777, '127.0.0.1');"); // Заполнить таблицу данными подключения
    }

    createQuery(sqlConDate, "SELECT * FROM Connect_data");

    string curName;          //  Текущее имя пользователя
    string curSername;       //  Текущая фамилия пользователя
    string curEmail;         //  Текущий адрес электронной почты пользователя
    int hash;                //  Текущий хэш пользователя
    bool persPresence = false;       //  Если есть пользователь с таким именем и паролем
    bool nameInChatAlready = false;  //  Если пользователь с таким именем уже есть в чате

    string contProcNumber = "0";     // указатель действия, тип 'string' для обеспечения устойчивости программы

    Chat presChat;

    while (true)
    {
        while (!persPresence)  // Пользователь не зашел и не зарегистрировался
        {
            while (contProcNumber != "1" && contProcNumber != "2" && contProcNumber != "3")  //  Защита против некорректного ввода
            {
                cout << "Choose the action, please: 1 - to enter; 2 - to log in; 3 - exit." << endl;
                getline(cin, contProcNumber);
            }

            if (contProcNumber == "1")
            {
                presChat.entrChat(persPresence, curName, hash);      // Зайти в чат
                contProcNumber = "0";
            }

            if (contProcNumber == "2")
            {
                presChat.regChat(nameInChatAlready, persPresence, curName, curSername, curEmail, hash, sqlConDate);    // Зарегистрироваться в чате
                contProcNumber = "0";
            }

            if (contProcNumber == "3") break;  //  Выйти из цикла
        }
        if (contProcNumber == "3") break;      //  Выйти из программы

        while (true)                       //  Цикл продолжается пока пользователь не вышел из профиля или из программы
        {
            while (contProcNumber != "1" && contProcNumber != "2")  //  защита от некорректного ввода
            {
                cout << "Choose the action, please: 1 - start the dialog; 2 - quit the profile." << endl;
                getline(cin, contProcNumber);
            }

            if (contProcNumber == "2")     //  если выбран выход
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
