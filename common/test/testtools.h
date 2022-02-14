#pragma once

#include "common/source/letter.h"

#include <cassert>

Letter operator "" _L(const char* letter_str, size_t);

#define ASSERT_THROWS(expression, exception)    \
do {                                            \
    try {                                       \
        expression;                             \
        assert(false);                          \
    } catch (exception&) {}                     \
} while (false)

// TODO: ASSERT_EQUAL with print
