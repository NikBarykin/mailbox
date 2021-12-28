#include "server.h"


QueryProcessor::QueryProcessor(Database &db, SessionState &session_state)
: db_(db), session_state_(session_state) {}

Answer::AnsT QueryProcessor::operator()(Query::GetMail) {
    return Query::GetMail::AnsT{db_.GetMail(session_state_.user_login)};
}


void Server::ProcessClient(Socket::Client &&client_sock) {
    SessionState session_state;
    QueryProcessor query_processor(db_, session_state);
    while (session_state.running) {
        Query::QType query = Query::DeserializeTransfer(client_sock.Recv());
        Answer::AnsT answer = visit(query_processor, query);
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
