#pragma once

#include "database.h"
#include "server_sock.h"
#include "../query.h"
#include "../answer.h"

#include <future>
#include <string>
//
//
//class Session {
//private:
//
//};
//
//class Server {
//private:
//    struct SessionData {
//        std::string login;
//        bool running = true;
//    };
//
//    Database db_;
//    const int N_CLIENTS_TO_PROCESS = 1000;
//
//    Answer::GetMail ProcessQuery(SessionData& session_data, Query::GetMail&) {
//        std::vector<Letter> letters = db_.GetMail(session_data.login);
//        return {letters};
//    }
//
//    Answer::AnswerT ProcessQuery(SessionData& session_data, Query::QueryT query) {
//        Answer::AnswerT result;
//        std::visit([this, &session_data, &result](auto query) {
//            result = this->ProcessQuery(session_data, query);
//        }, query);
//        return result;
//    }
//
//    void ProcessClient(Socket::Client& client_sock) {
//        SessionData session_data;
//        while (session_data.running) {
//            Query::QueryT query = Query::DeserializeTransfer(client_sock.Recv());
//            Answer::AnswerT answer = ProcessQuery(session_data, query);
//            client_sock.Send(Answer::SerializeForTransfer(answer));
//        }
//    }
//
//public:
//    void Run() {
//        Socket::Listener listen_sock("8080");
//        std::vector<std::future<void>> client_processes;
//        for (int _ = 0; _ < N_CLIENTS_TO_PROCESS; ++_) {
//            Socket::Client client_sock = listen_sock.Accept();
//            client_processes.push_back(std::async(&Server::ProcessClient, *this, std::ref(client_sock)));
//        }
//    }
//};


struct SessionState {
    std::string user_login;
    bool running = true;
};


class QueryProcessor {
private:
    Database& db_;
    SessionState& session_state_;
public:
    Answer::GetMail operator ()(Query::GetMail);
    Answer::SendLetter operator ()(Query::SendLetter);
    Answer::Authorize operator ()(Query::Authorize);
    Answer::Terminate operator ()(Query::Terminate);
public:
    QueryProcessor(Database& db, SessionState& session_state);
    Answer::AnsT operator ()(Query::QType);
};


class Server {
private:
    Database& db_;

    void ProcessClient(Socket::Client&& client_sock);

public:
    Server(Database& db);

    void Run();
};
