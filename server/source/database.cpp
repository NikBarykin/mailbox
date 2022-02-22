#include "database.h"
#include "letter_filter.h"


Database::UserId Database::GetId(std::string login) {
    std::lock_guard guard(user_mutex_);
    if (user_ids_.count(login)) {
        return user_ids_.at(login);
    }
    UserId new_id = user_ids_.size();
    user_ids_.emplace(login, new_id);
    return new_id;
}

std::optional<Database::UserId> Database::Authorize(std::string login, std::string password) {
    {
        std::lock_guard guard(user_mutex_);
        if (user_passwords_.count(login) && user_passwords_.at(login) != password) {
            return std::nullopt;
        }
        user_passwords_.emplace(login, password);
    }
    return GetId(login);
}

std::vector<Letter> Database::GetMail(UserId user_id, std::string_view filter_str) const {
    std::lock_guard guard(letter_mutex_);
    if (!letters_by_recipient_.count(user_id)) {
        return {};
    }
    // TODO: filter hashing
    auto filter = LetterFilter::ParseFilter(filter_str);
    std::vector<Letter> result;
    for (LetterId letter_id: letters_by_recipient_.at(user_id)) {
        const Letter & letter = letters_.at(letter_id);
        if (filter(letter)) {
            result.push_back(letter);
        }
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
