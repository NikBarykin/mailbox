#include "server.h"
#include "query_processor.h"

void Server::ProcessClient(Socket::Client &client_sock) {
    SessionState session_state;
    QueryProcessor query_processor(db_, session_state);
    while (session_state.running) {
        Query::Any query = Query::DeserializeTransfer(client_sock.Recv());
        Answer::Any answer = query_processor(query);
        client_sock.Send(Answer::SerializeForTransfer(answer));
    }
}

Server::Server(Database &db, std::string servname)
: db_(db), servname_(servname) {}

void Server::Run(size_t n_clients_to_process) {
    Socket::Listener listen_sock(servname_);
    Socket::Client client_sock = listen_sock.Accept();
//    std::vector<Socket::Client> client_sockets = {listen_sock.Accept()};
    std::vector<std::future<void>> client_futures;
    client_futures.push_back(std::async(&Server::ProcessClient, this, std::ref(client_sock)));
//    n_clients_to_process = 1;
//    while (n_clients_to_process--) {
//        client_sockets.emplace_back(listen_sock.Accept());
//        client_futures.push_back(std::async(&Server::ProcessClient, this, std::ref(client_sockets.back())));
//    }

    for (auto& future : client_futures) {
        future.get();
    }
}
