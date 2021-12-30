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


class Server {
private:
    Database& db_;
    std::string servname_;

    void ProcessClient(Socket::Client&& client_sock);
public:
    explicit Server(Database& db, std::string servname);

    void Run(size_t n_clients_to_process = -1);
};
