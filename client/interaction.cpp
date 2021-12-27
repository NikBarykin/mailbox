#include "interaction.h"
#include "client_sock.h"
#include "../query.h"
#include "../answer.h"
#include "../letter.h"

#include <string>
#include <vector>
#include <iostream>
#include <variant>

using namespace std;


void RunInteraction() {
    Socket::Server server_sock("localhost", "8080");
    cout << "login:" << endl;
    string login;
    getline(cin, login);
    cout << "password:" << endl;
    string password;
    getline(cin, password);
    server_sock.Send(Query::Authorize{login, password}.SerializeForTransfer());
    Answer::Authorize answer = Answer::Authorize::DeserializeTransfer(server_sock.Recv());
    if (!answer.authorization_succeed) {
        cout << "Wrong password!" << endl;
        return;
    }

    while (true) {
        Query::QueryT query = ParseQuery(cin);
        server_sock.Send(Query::SerializeForTransfer(query));
        Answer::AnswerT answer = Answer::DeserializeTransfer(server_sock.Recv());
        ProcessAnswer(answer);
        if (holds_alternative<Query::Terminate>(query)) {
            break;
        }
    }
}
