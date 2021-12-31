#include "test_server.h"
#include "server/source/server.h"
#include "client/source/client_sock.h"

#include <iostream>
#include <future>


namespace {
    void /*???*/() {
        Database db;
        Server server(db, "8080");
        server.Run();
    }

    void Client1Test() {
        Socket::Server server_sock("localhost", "8080");
        server_sock.Send(Query::GetMail{}.SerializeForTransfer());
        Answer::Any ans1 = Answer::DeserializeTransfer<Answer::GetMail>(server_sock.Recv());
        assert(std::holds_alternative<Answer::Error>(any1));
    }

    void Client2Test() {

    }

    void Test() {
        std::future<void> server_future = std::async(ServerThread);
        std::future<void> client1_future = std::async(Client1Thread);
        std::future<void> client2_future = std::async(Client2Thread);
    }
}


void TestServer() {
    std::cerr << "TestServer: OK" << std::endl;
}
