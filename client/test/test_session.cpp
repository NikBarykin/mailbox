#include "test_session.h"

#include <cassert>
#include <iostream>
#include <sstream>
#include <future>

#include "client/source/session.h"
#include "server/source/server_sock.h"
#include "common/source/query.h"
#include "client/source/client_sock.h"
#include "common/test/testing_utility.h"


namespace {
    void RunTestSession() {
        std::istringstream input(R"(SendLetter
B
C

Authorize
A
A
SendLetter
B
C

GetMail

Authorize
B
B
GetMail
from == "A"
GetMail
from != "A"
SendLetter
A
D

Authorize
A
A
GetMail
body == "D"
Terminate
)");
        std::ostringstream output;

        RunSession("localhost", "8080", input, output);

        std::ostringstream expected_output;
        expected_output << "Query name:\n"
                        << "Letter addressee:\n"
                        << "Letter body: (empty line marks end of letter)\n\n"
                        << "Server error: Not authorized\n\n"
                        << "Query name:\n"
                        << "Login:\n"
                        << "Password:\n\n"
                        << "Authorized successfully\n\n"
                        << "Query name:\n"
                        << "Letter addressee:\n"
                        << "Letter body: (empty line marks end of letter)\n\n"
                        << "Letter sent successfully\n\n"
                        << "Query name:\n"
                        << "Letter filter:\n\n"
                        << "Your mailbox is empty\n\n"
                        << "Query name:\n"
                        << "Login:\n"
                        << "Password:\n\n"
                        << "Authorized successfully\n\n"
                        << "Query name:\n"
                        << "Letter filter:\n\n"
                        << "1.\n"
                        << "Date: " << Date::CurrentDate().AsString() << '\n'
                        << "From: A\n"
                        << "C\n\n"
                        << "Query name:\n"
                        << "Letter filter:\n\n"
                        << "Your mailbox is empty\n\n"
                        << "Query name:\n"
                        << "Letter addressee:\n"
                        << "Letter body: (empty line marks end of letter)\n\n"
                        << "Letter sent successfully\n\n"
                        << "Query name:\n"
                        << "Login:\n"
                        << "Password:\n\n"
                        << "Authorized successfully\n\n"
                        << "Query name:\n"
                        << "Letter filter:\n\n"
                        << "1.\n"
                        << "Date: " << Date::CurrentDate().AsString() << '\n'
                        << "From: B\n"
                        << "D\n\n"
                        << "Query name:\n\n"
                        << "Session terminated\n\n";

        assert(output.str() == expected_output.str());
    }

    void ServerTest() {
        Socket::Listener listen_sock("8080");
        Socket::Client client_sock = listen_sock.Accept();

        Query::Any query1 = Query::DeserializeTransfer(client_sock.Recv());
        Query::Any expected1 = Query::SendLetter{" B C"_l};
        assert(query1 == expected1);

        client_sock.Send(Answer::Error{"Not authorized"}.SerializeForTransfer());

        Query::Any query2 = Query::DeserializeTransfer(client_sock.Recv());
        Query::Any expected2 = Query::Authorize{"A", "A"};
        assert(query2 == expected2);

        client_sock.Send(Answer::Authorize{"A"}.SerializeForTransfer());

        Query::Any query3 = Query::DeserializeTransfer(client_sock.Recv());
        Query::Any expected3 = Query::SendLetter{"A B C"_l};
        assert(query3 == expected3);

        client_sock.Send(Answer::SendLetter{}.SerializeForTransfer());

        auto query4 = Query::DeserializeTransfer(client_sock.Recv());
        assert(std::holds_alternative<Query::GetMail>(query4));

        client_sock.Send(Answer::GetMail{}.SerializeForTransfer());

        Query::Any query5 = Query::DeserializeTransfer(client_sock.Recv());
        Query::Any expected5 = Query::Authorize{"B", "B"};
        assert(query5 == expected5);

        client_sock.Send(Answer::Authorize{"B"}.SerializeForTransfer());

        auto query6 = Query::DeserializeTransfer(client_sock.Recv());
        assert(std::holds_alternative<Query::GetMail>(query6));

        client_sock.Send(Answer::GetMail{{"A B C"_l}}.SerializeForTransfer());

        auto query6_2 = Query::DeserializeTransfer(client_sock.Recv());
        assert(std::get<Query::GetMail>(query6_2).letter_filter == "from != \"A\"");

        client_sock.Send(Answer::GetMail{{}}.SerializeForTransfer());

        Query::Any query7 = Query::DeserializeTransfer(client_sock.Recv());
        Query::Any expected7 = Query::SendLetter{"B A D"_l};
        assert(query7 == expected7);

        client_sock.Send(Answer::SendLetter{}.SerializeForTransfer());

        Query::Any query8 = Query::DeserializeTransfer(client_sock.Recv());
        Query::Any expected8 = Query::Authorize{"A", "A"};
        assert(query8 == expected8);

        client_sock.Send(Answer::Authorize{"A"}.SerializeForTransfer());

        auto query9 = Query::DeserializeTransfer(client_sock.Recv());
        assert(std::holds_alternative<Query::GetMail>(query9));

        client_sock.Send(Answer::GetMail{{"B A D"_l}}.SerializeForTransfer());

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
