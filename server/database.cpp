#include "database.h"


class Database {
public:
    using LetterId = size_t;
    using UserId = size_t;
private:
    std::vector<Letter> letters_;
//    std::vector<std::string> users_;

    std::unordered_map<std::string, std::vector<LetterId>> mailboxes_;
public:
    std::vector<Letter> GetMail(std::string login) const {
        std::vector<Letter> result;
        result.reserve(mailboxes_.at(login).size());
        for (LetterId letter_id : mailboxes_.at(login)) {
            result.push_back(letters_[letter_id]);
        }
        return result;
    }
    Database& AddLetter(Letter letter) {

    }
};