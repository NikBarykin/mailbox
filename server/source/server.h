#pragma once

#include "database.h"
#include "server_sock.h"
#include "common/source/query.h"
#include "common/source/answer.h"

#include <future>
#include <string>


// TODO: class Session instead
struct SessionState {
    std::optional<Database::UserId> user_id = std::nullopt;
    bool running = true;
};


class Server {
private:
    Database& db_;
    std::string servname_;

    void ProcessClient(Socket::Client&& client_sock);

public:
    explicit Server(Database& db, std::string servname);

    void Run(size_t n_clients_to_process = -1);
};
