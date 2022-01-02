#include "test_socket.h"
#include "general/source/socket.h"
#include "server/source/server_sock.h"
#include "client/source/client_sock.h"

#include <cassert>
#include <future>
#include <vector>
#include <iostream>


namespace {
    void TestServerOneThread() {
        Socket::Listener listen_sock("8080");
        Socket::Client client_sock = listen_sock.Accept();
        std::string input = client_sock.Recv();
        assert(input == "Hello, server!");
        client_sock.Send("Hello, client!");
    }

    void TestClientOneThread() {
        Socket::Server server_sock("localhost", "8080");
        server_sock.Send("Hello, server!");
        std::string input = server_sock.Recv();
        assert(input == "Hello, client!");
        std::string large_str(2048, 'n');
        try {
            server_sock.Send(large_str);
            assert(false);
        } catch (std::exception&) {}
    }


    void TestSocket() {
        WSALib wsa_lib;
        std::vector<std::future<void>> futures;
        futures.push_back(std::async(TestServerOneThread));
        futures.push_back(std::async(TestClientOneThread));
    }
}


void TestSocket() {
    std::cerr << "TestSocket: OK" << std::endl;
}
