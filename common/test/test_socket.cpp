#include "test_socket.h"

#include <cassert>
#include <future>
#include <vector>
#include <iostream>

#include "common/source/socket.h"
#include "server/source/server_sock.h"
#include "client/source/client_sock.h"
#include "testing_utility.h"


namespace {
    void TestServerOneThread() {
        Socket::Listener listen_sock("8080");
        Socket::Client client_sock = listen_sock.Accept();

        std::string input = client_sock.Recv();
        assert(input == "Hello, server!");
        client_sock.Send("Hello, client!");

        std::string large_str = client_sock.Recv();
        assert(large_str == std::string(2048, 'x'));

        assert(client_sock.Recv() == "Compressed message to server");
        client_sock.Send("Compressed message to client", true);
    }

    void TestClientOneThread() {
        Socket::Server server_sock("localhost", "8080");

        server_sock.Send("Hello, server!");
        std::string input = server_sock.Recv();
        assert(input == "Hello, client!");

        std::string large_str(2048, 'x');
        server_sock.Send(large_str);

        server_sock.Send("Compressed message to server", true);
        assert(server_sock.Recv() == "Compressed message to client");
    }

    void TestGeneral() {
        std::future<void> server_fut = std::async(TestServerOneThread);
        TestClientOneThread();
        server_fut.get();
    }

    void TestExceptions() {
        // Connection failed
        ASSERT_THROWS(Socket::Server("localhost", "1234"), Socket::SocketError);
    }
}


void TestSocket() {
    TestGeneral();
    TestExceptions();
    std::cerr << "TestSocket: OK" << std::endl;
}
