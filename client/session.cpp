#include "session.h"
#include "../socket.h"
#include "client_sock.h"
#include "read_query.h"


Answer::AnsT DeserializeAnswer(std::string serialized_answer, Query::QType query) {
    return std::visit([&serialized_answer]())
    return Answer::DeserializeTransfer<typename QType::AnsT>(serialized_answer);
}


void RunSession(std::istream& input, std::ostream& output) {
    Socket::Server server_sock("localhost", "8080");
    SessionState session_state;
    while (session_state.running) {
        Query::QType query = ReadQuery(input);
        server_sock.Send(Query::SerializeForTransfer(query));
        std::string serialized_answer = server_sock.Recv();
        Answer::AnsT answer = std::visit(DeserializeAnswer, )
        Answer::AnsT answer = DeserializeAnswer(server_sock.Recv(), query);
    }
}
