#include "letter_filter.h"
#include "expression_token.h"

namespace LetterFilter {
    class LetterFilter {
    private:
        std::shared_ptr<LogicalNode> filter_tree;
    public:
        LetterFilter(const std::string &filter_str);

        bool operator()(const Letter &);
    };

    LetterFilter::LetterFilter::LetterFilter(const std::string &filter_str) {
        auto tokens = ::LetterFilter::ParseLetterFilterTokens(filter_str);
        tokens = ::LetterFilter::MakePostfixNotationFromInfix(std::move(tokens));
        filter_tree = BuildTree(tokens);
    }

    bool LetterFilter::LetterFilter::operator()(const Letter &letter) {
        return filter_tree->Check(letter);
    }


    std::function<bool (const Letter &)> MakeFilter(const std::string &filter_str) {
        return LetterFilter(filter_str);
    }
}
