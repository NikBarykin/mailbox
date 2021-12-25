#include "client_sock.h"
#include "../letter.h"

#include <string>
#include <vector>
#include <iostream>

using namespace std;


//void SendLetter(Letter);

//vector<Letter> GetMail();

int main() {
    WSALib wsa_lib;
    Socket::Server sock("localhost", "8080");
    cout << "Login?" << endl;
    string login;
    getline(cin, login);
    sock.Send(login);
    while (true) {
        string command;
        getline(cin, command);
        if (command == "Exit") {
            sock.Send("terminate");
            break;
        } else if (command == "GetMail") {
            sock.Send("GetMail");
            cout << sock.Recv() << endl;
        } else if (command == "Send") {
            sock.Send("Send");
//            cout << sock.Recv() << endl;
        } else {
            cout << "Unknown command!" << endl;
        }
    }
    return 0;
}
