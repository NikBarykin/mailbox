#pragma once

#include "general/source/letter.h"


#include <vector>
#include <string>
#include <unordered_map>
#include <mutex>
#include <optional>


// TODO: Make multi-thread support better by choosing more appropriate data structures,
//  wrapping data in proxy data structures with mutex inside
// TODO: Constructor from Letter container
// TODO: Maybe. Separate authorization and registration, remove opportunity to write letters to nonexistent users

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
    // If user doesn't have and id GetId creates new id and returns it
    UserId GetId(std::string login);

    std::optional<UserId> Authorize(std::string login, std::string password);

private:
    mutable std::mutex letter_mutex_;
    std::vector<Letter> letters_;
    std::unordered_map<UserId, std::vector<LetterId>> letters_by_recipient_;

public:
    std::vector<Letter> GetMail(UserId user_id) const;

    Database& AddLetter(Letter letter);

    template<class LetterIt>
    Database& AddLetters(LetterIt letter_begin, LetterIt letter_end);
};


// Implementations

template<class LetterIt>
Database& Database::AddLetters(LetterIt letter_begin, LetterIt letter_end) {
    for (LetterIt letter_it = letter_begin; letter_it != letter_end; ++letter_it) {
        AddLetter(*letter_it);
    }
    return *this;
}
