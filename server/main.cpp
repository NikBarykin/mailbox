#include "server_sock.h"
#include "database.h"

#include <unordered_map>
#include <string>
#include <vector>
#include <sstream>

using namespace std;


vector<string> ParseQuery(string query) {
    vector<string> tokens;
    istringstream input(query);
    for (string token; getline(input, token, ' '); ) {
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    return tokens;
}


int main(int argc, char* argv[]) {
    WSALib wsa_lib;
    Socket::Listener listen_sock("8080");
    int n_clients_to_process = argc < 2 ? 5 : stoi(argv[1]);
    unordered_map<string, string> passwords;
    unordered_map<string, vector<pair<string, string>>> mail;
    for (size_t i = 0; i < n_clients_to_process; ++i) {
        Socket::Client client_sock = listen_sock.Accept();
        auto authorize_tokens = ParseQuery(client_sock.Recv());
        string login = authorize_tokens[1], password = authorize_tokens[2];
        if (passwords.count(login) && passwords[login] != password) {
            client_sock.Send("Wrong password");
            continue;
        }
        if (!passwords.count(login)) {
            passwords.emplace(login, password);
        }
        client_sock.Send("Password correct");
        while (true) {
            auto query_tokens = ParseQuery(client_sock.Recv());
            string q_type = query_tokens[0];
            if (q_type == "Exit") {
                break;
            } else if (q_type == "GetMail") {
                ostringstream answer;
                for (auto [sender, body] : mail[login]) {
                    answer << sender << ":\n";
                    answer << body << "\n";
                }
                client_sock.Send(answer.str());
            } else if (q_type == "SendLetter") {
                string destination = query_tokens[1];
                string body = query_tokens[2];
                mail[destination].emplace_back(login, body);
            } else {
                throw runtime_error("Bad query: " + q_type);
            }
        }
    }
    return 0;
}
