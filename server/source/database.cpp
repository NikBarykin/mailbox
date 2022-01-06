#include "database.h"
#include "general/source/utils.h"

#include <mutex>
#include <cassert>


Database::UserId Database::GiveId(std::string login) {
    std::lock_guard guard(user_mutex_);
    assert(!user_ids_.count(login));
    UserId new_id = user_ids_.size();
    user_ids_.emplace(login, new_id);
    return new_id;
}

Database::UserId Database::GetId(std::string login) {
    const size_t* id_ptr;
    {
        std::lock_guard guard(user_mutex_);
        id_ptr = GetVal(user_ids_, login);
    }
    return id_ptr == nullptr ? GiveId(login) : *id_ptr;
}

std::optional<Database::UserId> Database::Authorize(std::string login, std::string password) {
    {
        std::lock_guard guard(user_mutex_);
        if (!user_passwords_.count(login)) {
            user_passwords_.emplace(login, password);
        }
        if (user_passwords_[login] != password) {
            return std::nullopt;
        }
    }
    return GetId(login);
}


std::vector<Letter> Database::GetMail(UserId user_id) const {
    std::lock_guard guard(letter_mutex_);
    std::vector<Letter> result;
    const std::vector<LetterId>* letter_ids = GetVal(letters_by_recipient_, user_id);
    if (letter_ids == nullptr) {
        return {};
    }
    result.reserve(letter_ids->size());
    for (LetterId letter_id: *letter_ids) {
        result.push_back(letters_[letter_id]);
    }
    return result;
}

Database &Database::AddLetter(Letter letter) {
    std::lock_guard guard(letter_mutex_);
    LetterId letter_id = letters_.size();
    UserId recipient_id = GetId(letter.to);
    letters_by_recipient_[recipient_id].push_back(letter_id);
    letters_.push_back(letter);
    return *this;
}
