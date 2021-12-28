#include "server.h"


void Server::ProcessClient(Socket::Client &&client_sock) {
    SessionState session_state;
    while (session_state.running) {
        Query::QType query = Query::DeserializeTransfer(client_sock.Recv());

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
