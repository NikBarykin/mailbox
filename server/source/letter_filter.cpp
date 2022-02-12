#include "letter_filter.h"
#include "letter_filter_token.h"
#include "letter_filter_node.h"


namespace LetterFilter {
    class Filter {
    private:
        std::shared_ptr<Node::Logical> filter_tree;
    public:
        explicit Filter(std::string_view filter_str) {
            auto tokens = Token::Tokenize(filter_str);
            tokens = Token::MakePostfixNotationFromInfix(tokens);
            filter_tree = Node::BuildTree(tokens);
        }

        bool operator()(const Letter &letter) {
            return filter_tree->Evaluate(letter);
        }
    };

    std::function<bool (const Letter &)> ParseFilter(std::string_view filter_str) {
        if (std::all_of(filter_str.begin(), filter_str.end(), isspace)) {
            return [](const Letter &) { return true; };
        }
        return Filter{filter_str};
    }
}
