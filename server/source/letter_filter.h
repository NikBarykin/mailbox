#pragma once

#include "common/source/letter.h"

#include <string_view>
#include <functional>


namespace LetterFilter {
    std::function<bool (const Letter &)> ParseFilter(std::string_view filter_str);
}
