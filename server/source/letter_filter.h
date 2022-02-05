#pragma once

#include "common/source/letter.h"
#include "letter_filter_parse_tree.h"


#include <string>
#include <memory>
#include <functional>


namespace LetterFilter {
    std::function<bool (const Letter &)> MakeFilter(const std::string &filter_str);
}
