#pragma once

#include "../letter.h"


#include <vector>
#include <string>
#include <unordered_map>


class Database {
public:
    using LetterId = size_t;
    using UserId = std::string;
private:
    std::vector<Letter> letters_;
    std::unordered_map<UserId, std::vector<LetterId>> letters_by_destination_;
public:
    std::vector<Letter> GetMail(UserId user_id) const;
    Database& AddLetter(Letter letter);
};
