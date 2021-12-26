#include "client_sock.h"
#include "../letter.h"

#include <string>
#include <vector>
#include <iostream>

using namespace std;


int main() {
    WSALib wsa_lib;
    Socket::Server server_sock("localhost", "8080");
    cout << "login:" << endl;
    string login;
    getline(cin, login);
    cout << "password:" << endl;
    string password;
    getline(cin, password);
    server_sock.Send("Authorize " + login + " " + password);
    if (server_sock.Recv() == "Wrong password") {
        cout << "Wrong password!" << endl;
        return 0;
    }
    while (true) {
        string command;
        getline(cin, command);
        if (command == "Exit") {
            server_sock.Send("Exit");
            break;
        } else if (command == "GetMail") {
            server_sock.Send("GetMail");
            cout << server_sock.Recv() << endl;
        } else if (command.rfind("SendLetter", 0) == 0) {
            server_sock.Send(command);
        } else {
            cout << "Unknown command!" << endl;
        }
    }
    return 0;
}
