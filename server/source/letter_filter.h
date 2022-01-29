#pragma once

#include "common/source/letter.h"

#include <string>
#include <memory>


class LetterFilterNode {
public:
    virtual bool CheckLetter(const Letter &) const = 0;
};

class LetterFilter {
private:
    std::unique_ptr<LetterFilterNode> filter_tree;
public:
    LetterFilter(const std::string & filter_str);
    bool operator()(const Letter &);
};
