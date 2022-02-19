#pragma once

#include "common/source/letter.h"

#include <cassert>

// TODO: maybe rename to testing_utils

Letter operator "" _L(const char* letter_str, size_t);

#define ASSERT_THROWS(expression, exception)    \
do {                                            \
    try {                                       \
        expression;                             \
        assert(false);                          \
    } catch (exception&) {}                     \
} while (false)

// TODO: ASSERT_EQUAL with print
