#include "server/server_sock.h"
#include "client/client_sock.h"

#include <cassert>
#include <future>
#include <vector>
#include <iostream>


void TestServerOneThread() {
    Socket::Listener listen_sock("8080");
    Socket::Client client_sock = listen_sock.Accept();
    std::string input = client_sock.Recv();
    assert(input == "Hello, server!");
    client_sock.Send("Hello, client!");
    std::string large_str = client_sock.Recv();
    assert(large_str == std::string(2048, 'n'));
}

void TestClientOneThread() {
    Socket::Server server_sock("localhost", "8080");
    server_sock.Send("Hello, server!");
    std::string input = server_sock.Recv();
    assert(input == "Hello, client!");
    std::string large_str(2048, 'n');
    server_sock.Send(large_str);
}


void TestSocket() {
    std::vector<std::future<void>> futures;
    futures.push_back(std::async(TestServerOneThread));
    TestClientOneThread();
}


int main() {
    WSALib wsa_lib;
    TestSocket();
}
