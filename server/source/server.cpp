#include "server.h"


QueryProcessor::QueryProcessor(Database &db, SessionState &session_state)
: db_(db), session_state_(session_state) {}

Answer::Any QueryProcessor::operator()(Query::GetMail) {
    return Query::GetMail::Any{db_.GetMail(session_state_.user_login)};
}

Answer::Any QueryProcessor::operator()(Query::Any query) {
    return std::visit(*this, query);
}


void Server::ProcessClient(Socket::Client &&client_sock) {
    SessionState session_state;
    QueryProcessor query_processor(db_, session_state);
    while (session_state.running) {
        Query::Any query = Query::DeserializeTransfer(client_sock.Recv());
        Answer::Any answer = query_processor(query);
        client_sock.Send(Answer::SerializeForTransfer(answer));
    }
}

Server::Server(Database &db): db_(db) {}

void Server::Run() {
    Socket::Listener listen_sock("8080");
    std::vector<std::future<void>> client_futures;
    for (int _ = 0; _ < 10; ++_) {
        client_futures.push_back(std::async(&Server::ProcessClient, this, listen_sock.Accept()));
    }
}