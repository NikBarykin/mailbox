#include "test_session.h"
#include "client/source/session.h"
#include "server/source/server_sock.h"
#include "general/source/query.h"
#include "client/source/client_sock.h"


#include <cassert>
#include <iostream>
#include <sstream>
#include <future>


namespace {
    void Test1() {
        Socket::Server serv_sock("localhost", "8080");
    }

    void Test2() {
        Socket::Listener listen_sock("8080");
        Socket::Client client_sock = listen_sock.Accept();
    }


    void RunTestSession() {
        std::istringstream input(R"(SendLetter
B
C
\End-of-letter
Authorize
A
A
SendLetter
B
C
\End-of-letter
GetMail
Authorize
B
B
GetMail
SendLetter
A
D
\End-of-letter
Authorize
A
A
GetMail
Terminate
)");
        std::ostringstream output;

        RunSession("localhost", "8080", input, output);

        std::string expected_output = R"(Query name:
Letter recipient:
Letter body:

Server error: Not authorized

Query name:
Login:
Password:

Authorized successfully

Query name:
Letter recipient:
Letter body:

Letter sent successfully

Query name:

Your mailbox is empty

Query name:
Login:
Password:

Authorized successfully

Query name:

From: A
C

Query name:
Letter recipient:
Letter body:

Letter sent successfully

Query name:
Login:
Password:

Authorized successfully

Query name:

From: B
D

Query name:

Session terminated

)";
        assert(output.str() == expected_output);
    }

    void ServerTest() {
        Socket::Listener listen_sock("8080");
        Socket::Client client_sock = listen_sock.Accept();

        auto query1 = Query::DeserializeTransfer(client_sock.Recv());
        assert(std::holds_alternative<Query::SendLetter>(query1));
        assert(std::get<Query::SendLetter>(query1).letter == Letter({"", "B", "C"}));

        client_sock.Send(Answer::Error{"Not authorized"}.SerializeForTransfer());

        auto query2 = Query::DeserializeTransfer(client_sock.Recv());
        assert(std::holds_alternative<Query::Authorize>(query2));
        auto [login2, password2] = std::get<Query::Authorize>(query2);
        assert(login2 == "A" && password2 == "A");

        client_sock.Send(Answer::Authorize{"A"}.SerializeForTransfer());

        auto query3 = Query::DeserializeTransfer(client_sock.Recv());
        assert(std::holds_alternative<Query::SendLetter>(query3));
        assert(std::get<Query::SendLetter>(query3).letter == Letter({"A", "B", "C"}));

        client_sock.Send(Answer::SendLetter{}.SerializeForTransfer());

        auto query4 = Query::DeserializeTransfer(client_sock.Recv());
        assert(std::holds_alternative<Query::GetMail>(query4));

        client_sock.Send(Answer::GetMail{}.SerializeForTransfer());

        auto query5 = Query::DeserializeTransfer(client_sock.Recv());
        assert(std::holds_alternative<Query::Authorize>(query5));
        auto [login5, password5] = std::get<Query::Authorize>(query5);
        assert(login5 == "B" && password5 == "B");

        client_sock.Send(Answer::Authorize{"B"}.SerializeForTransfer());

        auto query6 = Query::DeserializeTransfer(client_sock.Recv());
        assert(std::holds_alternative<Query::GetMail>(query6));

        client_sock.Send(Answer::GetMail{{{"A", "B", "C"}}}.SerializeForTransfer());

        auto query7 = Query::DeserializeTransfer(client_sock.Recv());
        assert(std::holds_alternative<Query::SendLetter>(query7));
        assert(std::get<Query::SendLetter>(query7).letter == Letter({"B", "A", "D"}));

        client_sock.Send(Answer::SendLetter{}.SerializeForTransfer());

        auto query8 = Query::DeserializeTransfer(client_sock.Recv());
        assert(std::holds_alternative<Query::Authorize>(query8));
        auto [login8, password8] = std::get<Query::Authorize>(query8);
        assert(login8 == "A" && password8 == "A");

        client_sock.Send(Answer::Authorize{"A"}.SerializeForTransfer());

        auto query9 = Query::DeserializeTransfer(client_sock.Recv());
        assert(std::holds_alternative<Query::GetMail>(query9));

        client_sock.Send(Answer::GetMail{{{"B", "A", "D"}}}.SerializeForTransfer());

        auto query10 = Query::DeserializeTransfer(client_sock.Recv());
        assert(std::holds_alternative<Query::Terminate>(query10));

        client_sock.Send(Answer::Terminate{}.SerializeForTransfer());
    }

    void Test() {
        std::future<void> serv_fut = std::async(ServerTest);
        RunTestSession();
    }
}


void TestSession() {
    Test();
    std::cerr << "TestSession: OK" << std::endl;
}
