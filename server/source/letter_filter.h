#pragma once

#include "common/source/letter.h"

#include <string_view>
#include <functional>
#include <stdexcept>


namespace LetterFilter {
    struct ParseError : public std::runtime_error {
        explicit ParseError(const std::string &msg)
        : std::runtime_error(msg) {}
    };

    // throw if we, for example use condition with invalid types (like ContainedIn with dates)
    // or checking date condition on letter with unspecified date
    struct ExecutionError : public std::runtime_error {
        explicit ExecutionError(const std::string &msg)
        : std::runtime_error(msg) {}
    };

    std::function<bool (const Letter &)> ParseFilter(std::string_view filter_str);
}
