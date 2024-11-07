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

/***** Ôóíêöèè è êëàññû ñåðâåðíîãî ñîåäèíåíèÿ *****/

void userReg(string& curName, string& curSername, string& curEmail, int hash) // Ââîä äàííûõ ïîëüçîâàòåëÿ âî âðåìÿ ðåãèñòðàöèè
{
    cout << "Type you name (less than 15 symbols including spaces): ";
    getline(cin, curName);
    cout << "Type you sername (less than 15 symbols including spaces): ";
    getline(cin, curSername);
    cout << "Type you email (less than 15 symbols including spaces): ";
    getline(cin, curEmail);
    cout << "Type your password (numbers only!): ";
    string password;
    getline(cin, password);
    hash = (stoi(password) % 50) + (stoi(password) % 49); // Âû÷èñëèòü õýø îò ïàðîëÿ
}

void userEnt(string& curName, int hash) // Ââîä äàííûõ ïîëüçîâàòåëÿ âî âðåìÿ âõîäà â ÷àò
{
    cout << "Type you name (less than 15 symbols including spaces): ";
    getline(cin, curName);
    cout << "Type your password (less than 15 symbols including spaces): ";
    string password;
    getline(cin, password);
    hash = (stoi(password) % 50) + (stoi(password) % 49); // Âû÷èñëèòü õýø îò ïàðîëÿ
}

void Chat::entrChat(bool& persPresence, string& curName, int hash) // Çàðåãèñòðèðîâàòüñÿ â ÷àòå
{
    if (persArray.size()) // Åñëè ìàññèâ íå ïóñòîé
    {
        userEnt(curName, hash); // Ââåñòè äàííûå ïîëüçîâàòåëÿ âî âðåìÿ ðåãèñòðàöèè
        map <string, int>::iterator it = persArray.find(curName);
        if (it != persArray.end() && it->second == hash) persPresence = true; // Óñïåøíàÿ ðåãèñòðàöèÿ
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
void Chat::regChat(bool& nameInChatAlready, bool& persPresence, string& curName, string& curSername, string& curEmail, int hash) // Çàðåãèñòðèðîâàòüñÿ â ÷àòå
{
    int socket_file_descriptor;
    userReg(curName, curSername, curEmail, hash); // Ââîä äàííûõ ïîëüçîâàòåëÿ âî âðåìÿ ðåãèñòðàöèè
    map <string, int>::iterator it = persArray.find(curName);
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

        // Îòïðàâëÿåì äàííûå î ðåãèñòðàöèè íà ñåðâåð
        string message = "register " + curName + "," + curSername + "," + curEmail; // Ôîðìàò: "register èìÿ,ôàìèëèÿ,email"
        ssize_t bytes = write(socket_file_descriptor, message.c_str(), message.size());
        if (bytes >= 0) {
            cout << "User  registration data sent to server." << endl;
        }
        persPresence = true; // Óñïåøíàÿ ðåãèñòðàöèÿ
    }
}

void Chat::sendMess(string curName) // Îòïðàâèòü ñîîáùåíèå
{
    cout << "to quit the dialog type 'end'" << endl;
    int socket_file_descriptor, connection;
    struct sockaddr_in serveraddress, client;
    char message[MESSAGE_LENGTH];
    string suffix = "_(" + curName + " is writing to you)";
    const char* suf = suffix.c_str();
    // ñîçäàåì ñîêåò

    socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_file_descriptor == -1) {
        cout << "Creation of Socket failed!" << endl;
        return;
    }

    // îïðåäåëÿåì àäðåñ ñåðâåðà
    serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    // îïðåäåëÿåì íîìåð ïîðòà
    serveraddress.sin_port = htons(PORT);
    // èñïîëüçóåì IPv4
    serveraddress.sin_family = AF_INET;
    // ïîäêëþ÷àåìñÿ ê ñåðâåðó
    connection = connect(socket_file_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress));
    if (connection == -1) {
        cout << "Connection with the server failed.!" << endl;
        return;
    }
    // âçàèìîäåéñòâèå ñ ñåðâåðîì
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
        // åñëè >= 0  áàéòû ïåðåäàíû, çíà÷èò ïåðåäà÷à óñïåøíà
        if (bytes >= 0) {
            cout << "     ***     " << endl;
        }
        bzero(message, sizeof(message));
        // îæèäàíèå îòâåòà îò ñåðâåðà
        read(socket_file_descriptor, message, sizeof(message));
        cout << "Message: " << message << endl;
    }
    // çàêðûâàåì ñîêåò, îñòàíàâëèâàåì ñîåäèíåíèå
    close(socket_file_descriptor);
    return;
}
