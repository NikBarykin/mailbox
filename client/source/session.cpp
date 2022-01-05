#include "session.h"
#include "general/source/socket.h"
#include "client_sock.h"
#include "query_reader.h"
#include "answer_processor.h"


void RunSession(std::string nodename, std::string servname,
                std::istream& input, std::ostream& output) {
    Socket::Server server_sock(nodename, servname);
    SessionState session_state;
    QueryReader query_reader(input, output);
    AnswerProcessor answer_processor(session_state, output);

    while (session_state.running) {
        Query::Any query = query_reader();
        server_sock.Send(Query::SerializeForTransfer(query));

        std::string serialized_answer = server_sock.Recv();
        Answer::Any answer = Query::DeserializeQueryAnswer(query, serialized_answer);
        answer_processor(answer);
    }
}
