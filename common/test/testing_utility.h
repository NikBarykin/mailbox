#pragma once

#include <cassert>
#include <ctime>

#include "common/source/letter.h"
#include "common/source/query.h"
#include "common/source/answer.h"


Letter operator "" _l(const char* letter_str, size_t);
Date operator "" _d(const char* date_str, size_t);

Letter GenRandomLetter(size_t seed = time(nullptr) + 123);


namespace Query {
    bool operator==(const Query::GetMail &lhs,      const Query::GetMail &rhs);
    bool operator==(const Query::SendLetter &lhs,   const Query::SendLetter &rhs);
    bool operator==(const Query::Authorize &lhs,    const Query::Authorize &rhs);
    bool operator==(const Query::Terminate &lhs,    const Query::Terminate &rhs);
}


namespace Answer {
    bool operator==(const Answer::GetMail &lhs,     const Answer::GetMail &rhs);
    bool operator==(const Answer::SendLetter &lhs,  const Answer::SendLetter &rhs);
    bool operator==(const Answer::Authorize &lhs,   const Answer::Authorize &rhs);
    bool operator==(const Answer::Terminate &lhs,   const Answer::Terminate &rhs);
    bool operator==(const Answer::Error &lhs,       const Answer::Error &rhs);
}


#define ASSERT_THROWS(expression, exception)    \
do {                                            \
    try {                                       \
        expression;                             \
        assert(false);                          \
    } catch (exception&) {}                     \
} while (false)

// TODO: ASSERT_EQUAL with print
