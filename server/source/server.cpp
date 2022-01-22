#include "server.h"
#include "query_processor.h"

// TODO: function instead of query_processor,
//  though it would be more elegant (query processing isn't an entity)
void Server::ProcessClient(Socket::Client &&client_sock) {
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
    std::vector<std::future<void>> client_futures;

    while (n_clients_to_process--) {
        client_futures.push_back(std::async(&Server::ProcessClient, this, listen_sock.Accept()));
    }

    for (auto& future : client_futures) {
        future.get();
    }
}
