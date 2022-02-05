#include "letter_filter.h"
#include "letter_filter_token.h"
#include "letter_filter_node.h"

namespace LetterFilter {
    class Filter {
    private:
        std::shared_ptr<Node::LogicalNode> filter_tree;
    public:
        Filter(const std::string &filter_str);

        bool operator()(const Letter &);
    };

    LetterFilter::Filter::Filter(const std::string &filter_str) {
        auto tokens = Token::Tokenize(filter_str);
        tokens = Token::MakePostfixNotationFromInfix(tokens);
        filter_tree = Node::BuildTree(tokens);
    }

    bool LetterFilter::Filter::operator()(const Letter &letter) {
        return filter_tree->Check(letter);
    }

    std::function<bool (const Letter &)> ParseFilter(const std::string &filter_str) {
        return Filter(filter_str);
    }
}
