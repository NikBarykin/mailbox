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


Query::QueryT ReadQuery(istream& input = cin) {
    string query_type;
    getline(input, query_type);
    if (query_type == "Terminate") {
        return Query::Terminate{};
    }
}

void ProcessAnswer(Answer::AnswerT answer, ostream& output = cout) {
    auto letters = get<Answer::GetMail>(answer).mail;
    for (const Letter& letter : letters) {
        output << "From: " << letter.from << endl;
        output << "Body:\n" << letter.body << endl;
        output << string(30, '-') << endl;
    }
}

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
        Query::QueryT query = ReadQuery();
        if (holds_alternative<Query::Authorize>(query)) {
            cout << "You've already authorized!" << endl;
            continue;
        }
        server_sock.Send(Query::SerializeForTransfer(query));
        string serialized_answer = server_sock.Recv();
        auto process_answer = [&serialized_answer](auto query) {
            auto answer = Answer::DeserializeTransfer<query::AnsT>(serialized_answer);
            ProcessAnswer(answer);
        };
        visit(process_answer, query);
        if (holds_alternative<Query::Terminate>(query)) {
            break;
        }
    }
}
