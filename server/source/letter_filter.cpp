#include "letter_filter.h"
#include "expression_token.h"

//
//std::vector<Token> Parse(const std::string & filter_str) {
//
//}


LetterFilter::LetterFilter::LetterFilter(const std::string & filter_str) {
    auto tokens = ::LetterFilter::ParseTokens(filter_str);
    tokens = ::LetterFilter::MakePostfixNotationFromInfix(tokens);
    filter_tree = BuildTree(tokens);
}
