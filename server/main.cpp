#include "socket.h"
#include "../letter.h"

#include <unordered_map>
#include <string>
#include <vector>

using namespace std;


class Server {
public:
    using LetterId = size_t;
private:
    unordered_map<string, vector<LetterId>> mailboxes;
    vector<Letter> letters;
public:
    void AddLetter(Letter letter) {
        mailboxes[letter.to].push_back(letters.size());
        letters.push_back(move(letter));
    }
    vector<Letter> GetLetters(const string& login) const {
        vector<Letter> result;
        for (LetterId id : mailboxes.at(login)) {
            result.push_back(letters[id]);
        }
        return result;
    }
};


int main() {
    Socket::Listener listen_sock("8080");
    unordered_map<string, Letter> data;
    for (size_t i = 0; i < 100; ++i) {
        Socket::Client client_sock = listen_sock.AcceptConnection();
        string login = client_sock.recv();
        while (true) {
            string query = client_sock.recv();
            if (query == "terminate") {
                break;
            } else if (query == "GetMail") {
                client_sock.send(data[login]);
            } else if (query == "Send") {
                data[login] = Letter();
                client_sock.send("OK");
            } else {
                throw runtime_error("Bad query: " + query);
            }
        }
    }
    return 0;
}
