#include "letter_filter.h"
#include "expression_token.h"


LetterFilter::LetterFilter::LetterFilter(const std::string & filter_str) {
    auto tokens = ::LetterFilter::ParseLetterFilterTokens(filter_str);
    tokens = ::LetterFilter::MakePostfixNotationFromInfix(std::move(tokens));
    filter_tree = BuildTree(tokens);
}

bool LetterFilter::LetterFilter::operator()(const Letter & letter) {
    return filter_tree->Check(letter);
}
