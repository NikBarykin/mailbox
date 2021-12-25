#include "server_sock.h"
#include "../letter.h"

#include <unordered_map>
#include <string>
#include <vector>
#include <sstream>

using namespace std;


//
//class Server {
//public:
//    using LetterId = size_t;
//private:
//    unordered_map<string, vector<LetterId>> mailboxes;
//    vector<Letter> letters;
//public:
//    void AddLetter(Letter letter) {
//        mailboxes[letter.to].push_back(letters.size());
//        letters.push_back(move(letter));
//    }
//    vector<Letter> GetLetters(const string& user_id) const {
//        vector<Letter> result;
//        for (LetterId id : mailboxes.at(user_id)) {
//            result.push_back(letters[id]);
//        }
//        return result;
//    }
//};


string GetFriend(string name) {
    return name == "Nikita" ? "Tumen" : "Nikita";
}


int main() {
    WSALib wsa_lib;
    Socket::Listener listen_sock("8080");
    unordered_map<string, vector<string>> data;
    for (size_t i = 0; i < 5; ++i) {
        Socket::Client client_sock = listen_sock.Accept();
        string login = client_sock.Recv();
        while (true) {
            string query = client_sock.Recv();
            if (query == "terminate") {
                break;
            } else if (query == "GetMail") {
                ostringstream answer_out;
                for (string record : data[login]) {
                    answer_out << record << endl;
                }
                client_sock.Send(answer_out.str());
            } else if (query == "Send") {
                string friend_name = GetFriend(login);
                data[friend_name].push_back(login + " send you \"Hello!\"");
            } else {
                throw runtime_error("Bad query: " + query);
            }
        }
    }
    return 0;
}
