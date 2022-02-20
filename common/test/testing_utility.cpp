#include "testing_utility.h"

#include <sstream>
#include <random>
#include <ctime>


Letter operator "" _l(const char* letter_str, size_t) {
    std::istringstream iss(letter_str);
    std::string from, to, body, date_str;

    std::getline(iss, from      , ' ');
    std::getline(iss, to        , ' ');
    std::getline(iss, body      , ' ');
    std::getline(iss, date_str  , ' ');

    assert(iss.eof());

    Date date = date_str.empty() ? Date::CurrentDate() : Date::ParseFrom(date_str);
    return {from, to, body, date};
}

Date operator "" _d(const char* date_str, size_t) {
    return Date::ParseFrom(date_str);
}

Letter GenRandomLetter(size_t seed) {
    std::mt19937 rnd(seed);
    Letter l  = {"", "", "", Date(rnd() % 10 + 1, rnd() % 5 + 1, rnd() % 10 + 1)};
    for (int i = 0; i < 10; ++i) {
        l.from.push_back('a' + rnd() % 10);
        l.to.push_back('a' + rnd() % 10);
        l.body.push_back('a' + rnd() % 10);
    }
    return l;
}


namespace Query {
    bool operator==(const Query::GetMail &lhs, const Query::GetMail &rhs) {
        return lhs.letter_filter == rhs.letter_filter;
    }

    bool operator==(const Query::SendLetter &lhs, const Query::SendLetter &rhs) {
        return lhs.letter == rhs.letter;
    }

    bool operator==(const Query::Authorize &lhs, const Query::Authorize &rhs) {
        return lhs.login == rhs.login && lhs.password == rhs.password;
    }

    bool operator==(const Query::Terminate &lhs, const Query::Terminate &rhs) {
        return true;
    }
}


namespace Answer {
    bool operator==(const Answer::GetMail &lhs, const Answer::GetMail &rhs) {
        return lhs.mail == rhs.mail;
    }

    bool operator==(const Answer::SendLetter &lhs, const Answer::SendLetter &rhs) {
        return true;
    }

    bool operator==(const Answer::Authorize &lhs, const Answer::Authorize &rhs) {
        return lhs.authorized_login == rhs.authorized_login;
    }

    bool operator==(const Answer::Terminate &lhs, const Answer::Terminate &rhs) {
        return true;
    }

    bool operator==(const Answer::Error &lhs, const Answer::Error &rhs) {
        return lhs.message == rhs.message;
    }
}
