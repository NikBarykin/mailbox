#pragma once

#include "../../letter.h"


#include <vector>
#include <string>
#include <unordered_map>
#include <mutex>
#include <optional>


// TODO: Make multi-thread support better by choosing more appropriate data structures
// TODO: Constructor from Letter container

class Database {
public:
    using LetterId = size_t;
    using UserId = size_t;

private:
    mutable std::mutex user_mutex_;
    std::unordered_map<std::string, std::string> user_passwords_;
    std::unordered_map<std::string, UserId> user_ids_;

    UserId GiveId(std::string login);

public:
    std::optional<UserId> Authorize(std::string login, std::string password);

private:
    mutable std::mutex letter_mutex_;
    std::vector<Letter> letters_;
    std::unordered_map<UserId, std::vector<LetterId>> letters_by_destination_;

public:
    std::vector<Letter> GetMail(UserId user_id) const;
    Database& AddLetter(Letter letter);
};
