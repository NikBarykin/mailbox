#include "session.h"
#include "common/source/socket.h"
#include "client_sock.h"
#include "build_query.h"
#include "cycle_outcome_processing.h"


void RunSession(std::string nodename, std::string servname,
                std::istream& input, std::ostream& output) {
    Socket::Server server_sock(nodename, servname);
    SessionState session_state;

    while (session_state.running) {
        Query::Any query = BuildQuery(input, output, session_state);
        server_sock.Send(Query::SerializeForTransfer(query));

        std::string serialized_answer = server_sock.Recv();
        Answer::Any answer = Query::DeserializeQueryAnswer(query, serialized_answer);
        ProcessCycleOutcome(query, answer, session_state, output);
    }
}
