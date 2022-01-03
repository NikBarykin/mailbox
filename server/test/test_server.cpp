#include "test_server.h"
#include "server/source/server.h"
#include "client/source/client_sock.h"

#include <iostream>
#include <future>
#include <cassert>


namespace {
    void RunTestServer() {
        Database db;
        Server server(db, "8080");
        server.Run();
    }

//    struct QuerySender {
//        Socket::Server& server_sock;
//        explicit QuerySender(Socket::Server& server_sock): server_sock(server_sock) {}
//        template <class QueryType>
//        Answer::Any SendQuery(QueryType query) {
//            server_sock.Send(query.SerializeForTransfer());
//            return Answer::DeserializeTransfer<typename QueryType::Answer>(server_sock.Recv());
//        }
//    };

    void ClientTest(std::string a_name, std::string b_name) {
        assert(a_name != b_name);

        Socket::Server a_serv_sock("localhost", "8080");
        Socket::Server b_serv_sock("localhost", "8080");

        auto send_query = []<class QueryType>(Socket::Server& server_sock, QueryType query) {
            server_sock.Send(query.SerializeForTransfer());
            return Answer::DeserializeTransfer<typename QueryType::Answer>(server_sock.Recv());
        };

        Answer::Any ans1 = send_query(a_serv_sock, Query::GetMail{});
        assert(std::holds_alternative<Answer::Error>(ans1));

        Answer::Any ans2 = send_query(a_serv_sock, Query::Authorize{a_name, a_name});
        assert(std::get<Answer::Authorize>(ans2).authorization_succeed);

        Answer::Any ans3 = send_query(a_serv_sock, Query::Authorize{a_name, b_name});
        assert(!std::get<Answer::Authorize>(ans3).authorization_succeed);

        Letter l1{a_name, b_name, "Hey, " + b_name};
        Answer::Any ans4 = send_query(a_serv_sock, Query::SendLetter{l1});
        assert(std::holds_alternative<Answer::SendLetter>(ans4));

        Answer::Any ans5 = send_query(b_serv_sock, Query::GetMail{});
        assert(std::holds_alternative<Answer::Error>(ans5));

        Answer::Any ans6 = send_query(b_serv_sock, Query::Authorize{b_name, b_name});
        assert(std::holds_alternative<Answer::Authorize>(ans6));

        Answer::Any ans7 = send_query(b_serv_sock, Query::GetMail{});
        assert(std::holds_alternative<Answer::GetMail>(ans7));
        auto b_mail = std::get<Answer::GetMail>(ans7).mail;
        assert(b_mail == std::vector<Letter>{l1});

        Letter l2{b_name, a_name, "Hey, " + a_name};
        Answer::Any ans8 = send_query(b_serv_sock, Query::SendLetter{l2});
        assert(std::holds_alternative<Answer::SendLetter>(ans8));

        Letter l3{b_name, a_name, "How was your day?:p"};
        Answer::Any ans9 = send_query(b_serv_sock, Query::SendLetter{l3});
        assert(std::holds_alternative<Answer::SendLetter>(ans9));

        Answer::Any ans10 = send_query(b_serv_sock, Query::Terminate{});
        assert(std::holds_alternative<Answer::Terminate>(ans10));

        // b's session is terminated
        Answer::Any ans11 = send_query(b_serv_sock, Query::GetMail{});
        assert(std::holds_alternative<Answer::Error>(ans11));

        Answer::Any ans12 = send_query(a_serv_sock, Query::GetMail{});
        assert(std::holds_alternative<Answer::GetMail>(ans12));
        auto a_mail = std::get<Answer::GetMail>(ans12).mail;
        assert(a_mail == std::vector<Letter>({l2, l3}));

        Answer::Any ans13 = send_query(a_serv_sock, Query::Terminate{});
        assert(std::holds_alternative<Answer::Terminate>(ans13));
    }


    void Test() {
        std::future<void> server_future = std::async(RunTestServer);
        std::vector<std::future<void>> client_futures;
        client_futures.push_back(std::async(ClientTest, "a", "b"));
        client_futures.push_back(std::async(ClientTest, "x", "y"));
        client_futures.push_back(std::async(ClientTest, "p", "q"));
        client_futures.push_back(std::async(ClientTest, "v", "w"));
    }
}


void TestServer() {
    Test();
    std::cerr << "TestServer: OK" << std::endl;
}
