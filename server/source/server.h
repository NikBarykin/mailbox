#pragma once

#include "database.h"
#include "server_sock.h"
#include "../../query.h"
#include "../../answer.h"

#include <future>
#include <string>


struct SessionState {
    std::string user_login;
    bool running = true;
};


class QueryProcessor {
private:
    Database& db_;
    SessionState& session_state_;
public:
    QueryProcessor(Database& db, SessionState& session_state);

    Answer::Any operator ()(Query::GetMail);
    Answer::Any operator ()(Query::SendLetter);
    Answer::Any operator ()(Query::Authorize);
    Answer::Any operator ()(Query::Terminate);

    Answer::Any operator ()(Query::Any);
};


class Server {
private:
    Database& db_;

    void ProcessClient(Socket::Client&& client_sock);

public:
    explicit Server(Database& db);

    void Run();
};
