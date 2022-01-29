#include "letter_filter.h"


std::vector<Token> Parse(const std::string & filter_str) {

}


LetterFilter::LetterFilter(const std::string & filter_str) {
    auto tokens = Parse(filter_str);
    filter_tree = BuildTree(tokens);
}
