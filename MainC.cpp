#include <iostream>
#include "HeaderC.h"
#include <map>
#include <vector>

using namespace std;

int main()
{
	string curName; // Текущее имя пользователя
	string curSername; // Текущая фамилия пользователя
	string curEmail; // Текущий адрес электронной почты пользователя
	int hash; // Текущий хэш пользователя
	int messNumber = 0; // Номер текущего сообщения (от ноля и в порядке появления)
	bool persPresence = false; // Если есть пользователь с таким именем и паролем
	bool nameInChatAlready = false; // Если пользователь с таким именем уже есть в чате

	string contProcNumber = "0";     // action pointer, type 'string' is for program sustainability

	Chat presChat;


	while (true)
	{
		while (persPresence == false)  // while the user is not entered or loged in
		{
			while (contProcNumber != "1" && contProcNumber != "2" && contProcNumber != "3")  //  protection from incorrect input
			{
				cout << "Choose the action, please: 1 - to enter; 2 - to log in; 3 - exit." << endl;
				getline(cin, contProcNumber);
			}
			persPresence = false;
			if (contProcNumber == "1")
			{
				presChat.entrChat(persPresence, curName, hash);      // to enter in the chat
				contProcNumber = "0";
			}

			if (contProcNumber == "2")
			{
				presChat.regChat(nameInChatAlready, persPresence, curName, curSername, curEmail, hash);    // to log in the chat
				contProcNumber = "0";
			}

			if (contProcNumber == "3") break;  //  exit from the cycle
		}
		if (contProcNumber == "3") break;      //  exit from the programme
		while (true)                       //  the cycle is continiuing unless the user exits from the profile or the programme
		{
			while (contProcNumber != "1" && contProcNumber != "2" && contProcNumber != "3")  //  protection from incorrect input
			{
				cout << "Choose the action, please: 1 - start the dialoge (you have to send the message first); 2 - quit the profile." << endl;
				getline(cin, contProcNumber);
			}
			if (contProcNumber == "2")     //  if exit chosen
			{
				cout << endl;
				persPresence = false;
				contProcNumber = "0";
				break;
			}
			if (contProcNumber == "1")      //  if sending a message is chosen
			{
				presChat.sendMess(curName);
				contProcNumber = "0";
			}
			if (contProcNumber == "2")      //  exit from programme
			{
				contProcNumber = "3";
				break;
			}
		}
		if (contProcNumber == "3") break;      //  exit from programme
	}
	return 0;
}
