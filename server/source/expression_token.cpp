#include "expression_token.h"

#include <unordered_map>
#include <functional>
#include <stack>


namespace LetterFilter {
    template<class TokenT>
    std::unique_ptr<Token> MakeToken() {
        return std::make_unique<TokenT>();
    }

    std::vector<std::unique_ptr<Token>> ParseLetterFilterTokens(std::string_view sv) {
        static const std::unordered_map<std::string, std::function<std::unique_ptr<Token> ()>> literal_tokens = {
                {"(", MakeToken<LeftParenthesis>},
                {")", MakeToken<RightParenthesis>},
        };

        std::vector<std::unique_ptr<Token>> tokens;

        while (!sv.empty()) {
            if (sv.front() == ' ') {
                sv.remove_prefix(1);
                continue;
            }
            if (std::isalpha(sv.front())) {
                std::string property_name;
                while (!sv.empty() && std::isalpha(sv.front())) {
                    property_name.push_back(sv.front());
                    sv.remove_prefix(1);
                }
                tokens.push_back(std::make_unique<LetterProperty>(std::move(property_name)));
            } else if (sv.front() == '\"') {
                sv.remove_prefix(1);
                size_t pos = sv.find('\"');
                if (pos == std::string_view::npos) {
                    throw std::runtime_error("Invalid literal: \"" + std::string(sv));
                }
                tokens.push_back(std::make_unique<Literal>(std::string(sv.substr(pos))));
                sv.remove_prefix(pos + 1);
            } else {
                std::string greatest_token;
                for (const auto & [token_str, _] : literal_tokens) {
                    if (token_str.size() > greatest_token.size() && sv.starts_with(token_str)) {
                        greatest_token = token_str;
                    }
                }
                if (greatest_token.empty()) {
                    throw std::runtime_error("Failed to parse heading token from suffix " + std::string(sv));
                }
                tokens.push_back(literal_tokens.at(greatest_token)());
            }
        }
        return tokens;
    }

    std::vector<std::unique_ptr<Token>> MakePostfixNotationFromInfix(
            const std::vector<std::unique_ptr<Token>> & tokens) {
        std::vector<std::unique_ptr<Token>> postfix_notation;
        std::stack<std::unique_ptr<Token>> operators;
        // TODO: add validation (infix notation should look like this: operand operator operand ... operand + parenthesis)
        for (auto & token_ptr : tokens) {
            if (auto op_ptr = dynamic_cast<BinaryOperator*>(token_ptr.get()); op_ptr) {
                while (!operators.empty()) {
                    auto top_ptr = dynamic_cast<BinaryOperator*>(operators.top().get());
                    if (!top_ptr || top_ptr->Precedence() < op_ptr->Precedence()) {
                        break;
                    }
                    postfix_notation.push_back(std::move(operators.top()));
                    operators.pop();
                }
                operators.push(std::move(token_ptr));
            } else if (dynamic_cast<LeftParenthesis*>(token_ptr.get())) {
                operators.push(std::move(token_ptr));
            } else if (dynamic_cast<RightParenthesis*>(token_ptr.get())) {
                while (!operators.empty() && !dynamic_cast<LeftParenthesis*>(operators.top().get())) {
                    postfix_notation.push_back(std::move(operators.top()));
                    operators.pop();
                }
                if (operators.empty()) {
                    throw std::runtime_error("Unclosed parenthesis");
                }
            } else if (dynamic_cast<Operand*>(token_ptr.get())) {
                postfix_notation.push_back(std::move(token_ptr));
            }
        }
        return postfix_notation;
    }
}
