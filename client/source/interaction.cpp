#include "interaction.h"
#include "client_sock.h"
#include "../../query.h"
#include "../../answer.h"
#include "../../letter.h"
#include "read_query.h"
#include "process_answer.h"

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
        optional<Query::QType> query = ReadQuery();
        if (!query) {
            cout << "Failed to read query!";
        }
        if (holds_alternative<Query::Authorize>(*query)) {
            cout << "You've already authorized!" << endl;
            continue;
        }
        server_sock.Send(Query::SerializeForTransfer(*query));
        string serialized_answer = server_sock.Recv();
        auto get_answer = [&serialized_answer]<class QueryType>(QueryType query) {
            return Answer::DeserializeTransfer<typename QueryType::AnsT>(serialized_answer);
        };
        auto answer = visit(get_answer, *query);
        if (holds_alternative<Query::Terminate>(query)) {
            break;
        }
    }
}
