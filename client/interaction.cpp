#include "interaction.h"
#include "client_sock.h"
#include "../query.h"
#include "../letter.h"

#include <string>
#include <vector>
#include <iostream>

using namespace std;


void RunInteraction() {
    Socket::Server server_sock("localhost", "8080");
    cout << "login:" << endl;
    string login;
    getline(cin, login);
    cout << "password:" << endl;
    string password;
    getline(cin, password);
    server_sock.Send(Query::Authorize(login, password).TransferString());
    auto answer = Answer::ParseTranfer(server_sock.Recv());
    if (/* failed to authorize */) {
        cout << "Wrong password!" << endl;
        return;
    }
    while (true) {
        Query::Base& query = ParseQuery(cin);
        server_sock.Send(query.TransferString());
        auto answer = Answer::ParseTranfer(server_sock.Recv());
        ProcessAnswer(answer);
        if (/* type(query) == Query::Terminate */) {
            break;
        }
    }
}
