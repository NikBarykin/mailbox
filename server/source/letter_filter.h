#pragma once

#include "common/source/letter.h"
#include "letter_filter_parse_tree.h"


#include <string>
#include <memory>


namespace LetterFilter {

    class LetterFilter {
    private:
        std::unique_ptr<LogicalNode> filter_tree;
    public:
        LetterFilter(const std::string &filter_str);

        bool operator()(const Letter &);
    };
}
