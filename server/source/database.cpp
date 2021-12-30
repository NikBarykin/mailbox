#include "database.h"

#include <mutex>


std::vector<Letter> Database::GetMail(UserId user_id) const {
    std::lock_guard guard(data_mutex_);
    std::vector<Letter> result;
    const std::vector<LetterId>& letter_ids = letters_by_destination_.at(user_id);
    result.reserve(letter_ids.size());
    for (LetterId letter_id: letter_ids) {
        result.push_back(letters_[letter_id]);
    }
    return result;
}

Database &Database::AddLetter(Letter letter) {
    std::lock_guard guard(data_mutex_);
    LetterId letter_id = letters_.size();
    UserId destination_id = letter.to;
    letters_by_destination_[destination_id].push_back(letter_id);
    letters_.push_back(letter);
    return *this;
}
