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

/***** Ôóíêöèè âçàèìîäåéñòâèÿ ñ áàçîé äàííûõ *****/

string* SQL_Connect_Data(string dArray[]) // Ââåñòè äàííûå äëÿ ïîäêëþ÷åíèÿ ê MySQL
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

void createQuery(string* sqlConDate, const char* query) // Ñîçäàòü çàïðîñ äëÿ áàçû äàííûõ.
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

char* createQueryOut(string* sqlConDate, const char* query) // Ñîçäàòü çàïðîñ äëÿ áàçû äàííûõ ñ âûâîäîì çíà÷åíèÿ òèïà char*
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
            result = strdup(row[0]); // Èñïîëüçóåì strdup äëÿ êîïèðîâàíèÿ ñòðîêè
        }
        mysql_free_result(res);
    }
    else {
        cout << "Îøèáêà MySql íîìåð " << mysql_error(&mysql) << endl;
    }

    mysql_close(&mysql);
    return result;
}

/***** Ôóíêöèè è êëàññû ñåðâåðíîãî ñîåäèíåíèÿ *****/

void userReg(string& curName, string& curSername, string& curEmail, int& hash, std::string* sqlConDate) // Ââîä äàííûõ ïîëüçîâàòåëÿ âî âðåìÿ ðåãèñòðàöèè
{
    cout << "Type your name (less than 15 symbols including spaces): ";
    getline(cin, curName);
    cout << "Type your surname (less than 15 symbols including spaces): ";
    getline(cin, curSername);
    cout << "Type your email (less than 100 symbols including spaces): ";
    getline(cin, curEmail);
    cout << "Type your password (numbers only!): ";
    string password;
    getline(cin, password);
    hash = (stoi(password) % 50) + (stoi(password) % 49); // Âû÷èñëèòü õýø îò ïàðîëÿ

    // Âñòàâêà äàííûõ â òàáëèöó Customers
    string query = "INSERT INTO Customers (name, sername, email, hash) VALUES ('" + curName + "', '" + curSername + "', '" + curEmail + "', '" + to_string(hash) + "');";
    createQuery(sqlConDate, query.c_str());
}

void userEnt(string& curName, int& hash) // Ââîä äàííûõ ïîëüçîâàòåëÿ âî âðåìÿ âõîäà â ÷àò
{
    cout << "Type your name (less than 15 symbols including spaces): ";
    getline(cin, curName);
    cout << "Type your password (less than 15 symbols including spaces): ";
    string password;
    getline(cin, password);
    hash = (stoi(password) % 50) + (stoi(password) % 49); // Âû÷èñëèòü õýø îò ïàðîëÿ
}

void Chat::entrChat(bool& persPresence, string& curName, int& hash) // Âîéòè â ÷àò
{
    if (persArray.size()) // Åñëè ìàññèâ íå ïóñòîé
    {
        userEnt(curName, hash); // Ââåñòè äàííûå ïîëüçîâàòåëÿ âî âðåìÿ ðåãèñòðàöèè
        map<string, int>::iterator it = persArray.find(curName);
        if (it != persArray.end() && it->second == hash) {
            persPresence = true; // Óñïåøíàÿ ðåãèñòðàöèÿ
        }
        if (!persPresence) {
            cout << "The user with this name and password not found. Check the input" << endl;
        }
    }
    else {
        cout << "There are no users in the chat yet" << endl;
    }
}

void Chat::regChat(bool& nameInChatAlready, bool& persPresence, string& curName, string& curSername, string& curEmail, int& hash, std::string* sqlConDate) // Çàðåãèñòðèðîâàòüñÿ â ÷àòå
{
    userReg(curName, curSername, curEmail, hash, sqlConDate); // Ââîä äàííûõ ïîëüçîâàòåëÿ âî âðåìÿ ðåãèñòðàöèè
    map<string, int>::iterator it = persArray.find(curName);
    if (it != persArray.end() || curName.empty()) {
        nameInChatAlready = true; // Åñëè èìÿ óæå åñòü â ÷àòå èëè èìÿ ïóñòîå
    }
    if (nameInChatAlready) { // Ðåãèñòðàöèÿ îòêëîíåíà
        cout << "The user with this name is already in the chat or the data is not correct (empty name or password). Repeat the input" << endl;
        nameInChatAlready = false;
    }
    else {
        persArray.insert({ curName, hash }); // Âñòàâêà äàííûõ ïîëüçîâàòåëÿ â ìàññèâ
        cout << "Welcome to the chat, " << curName << "!" << endl;
        persPresence = true;
    }
}

void Chat::sendMess(string curName, std::string* sqlConDate) // Îòïðàâèòü ñîîáùåíèå
{
    struct sockaddr_in serveraddress, client;
    socklen_t length;
    int sockert_file_descriptor, connection, bind_status, connection_status;
    char message[MESSAGE_LENGTH];
    string suffix = "_(" + curName + " is writing to you)";
    const char* suf = suffix.c_str();

    // Ñîçäàåì ñîêåò
    sockert_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (sockert_file_descriptor == -1) {
        cout << "Socket creation failed.!" << endl;
        return;
    }

    serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_family = AF_INET;

    // Ïðèâÿçûâàåì ñîêåò
    bind_status = bind(sockert_file_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress));
    if (bind_status == -1) {
        cout << "Socket binding failed.!" << endl;
        return;
    }

    // Íà÷èíàåì ñëóøàòü
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

    // Óñòàíîâëåíèå ñâÿçè
    while (true) {
        bzero(message, MESSAGE_LENGTH);
        read(connection, message, sizeof(message));
        if (strncmp("end", message, 3) == 0) {
            cout << "Client Exited." << endl;
            cout << "Server is Exiting..!" << endl;
            break;
        }

        // Åñëè ñîîáùåíèå íà÷èíàåòñÿ ñ "register", îáðàáàòûâàåì ðåãèñòðàöèþ
        if (strncmp(message, "register", 8) == 0) {
            string userData(message + 9); // Ïîëó÷àåì äàííûå ïîëüçîâàòåëÿ ïîñëå "register "
            size_t pos = userData.find(',');
            string curName = userData.substr(0, pos);
            userData.erase(0, pos + 1);
            pos = userData.find(',');
            string curSername = userData.substr(0, pos);
            userData.erase(0, pos + 1);
            string curEmail = userData; // Îñòàëüíûå äàííûå - ýòî email

            // Âñòàâêà äàííûõ â òàáëèöó Customers
            int hash = (curName.length() % 50) + (curName.length() % 49); // Ïðîñòîé õýø
            string query = "INSERT INTO Customers (name, sername, email, hash) VALUES ('" + curName + "', '" + curSername + "', '" + curEmail + "', '" + to_string(hash) + "');";
            createQuery(sqlConDate, query.c_str());
            cout << "User  registered: " << curName << endl;
            continue; // Ïðîäîëæàåì öèêë, ÷òîáû îæèäàòü ñëåäóþùåå ñîîáùåíèå
        }

        cout << "Message: " << message << endl;

        // Çàïèñü ñîîáùåíèÿ â áàçó äàííûõ
        string query = "INSERT INTO Messages (id_sender, id_receiver, text, mess_date, read_mess, sent_mess) VALUES (1, 2, '" + string(message) + "', NOW(), 0, 1);";
        createQuery(sqlConDate, query.c_str());

        // Îòïðàâêà îòâåòà êëèåíòó
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
