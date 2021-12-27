#include "interaction.h"
#include "server_sock.h"
#include "database.h"
#include "../query.h"

#include <unordered_map>
#include <string>
#include <vector>
#include <sstream>
#include <future>

using namespace std;
//
//
//vector<string> ParseQuery(string query) {
//    vector<string> tokens;
//    istringstream input(query);
//    for (string token; getline(input, token, ' '); ) {
//        if (!token.empty()) {
//            tokens.push_back(token);
//        }
//    }
//    return tokens;
//}


void ProcessClient(Socket::Client& client_sock, Database& db) {
    SessionData session_data;
    while (!session_data.terminated) {
        auto& query = Query::ParseTransfer(client_sock.Recv());
        auto answer = ProcessQuery(query, db, session_data);
        client_sock.Send(answer.TransferString());
    }
}


void RunInteraction() {
    Socket::Listener listen_sock("8080");
    vector<future<void>> client_processes;
    Database db;
    int n_clients_to_process = 5;
    while (n_clients_to_process--) {
        Socket::Client client_sock = listen_sock.Accept();
        client_processes.push_back(async(ProcessClient, ref(client_sock), ref(db)));
    }
}