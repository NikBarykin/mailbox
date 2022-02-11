#include "letter_filter_token.h"

#include <unordered_map>
#include <functional>
#include <stack>


namespace LetterFilter::Token {
    Token::~Token() = default;

    template<class TokenType>
    TokenHandler MakeToken() {
        return std::make_shared<TokenType>();
    }

    std::vector<TokenHandler> Tokenize(std::string_view filter_str) {
        static const std::unordered_map<std::string_view, std::function<TokenHandler()>> constant_tokens = {
                {"&&", MakeToken<LogicalAnd>},
                {"||", MakeToken<LogicalOr>},
                {"==", MakeToken<Equal>},
                {"!=", MakeToken<NotEqual>},
                {"<" , MakeToken<Less>},
                {">" , MakeToken<Greater>},
                {"<=", MakeToken<LessEqual>},
                {">=", MakeToken<GreaterEqual>},
                {"(" , MakeToken<LeftParenthesis>},
                {")" , MakeToken<RightParenthesis>},
        };

        static const std::unordered_map<std::string, std::function<TokenHandler()>> property_tokens = {
                {"from", MakeToken<FromProperty>},
                {"body", MakeToken<BodyProperty>},
        };

        std::vector<TokenHandler> resulting_tokens;

        while (!filter_str.empty()) {
            if (std::isspace(filter_str.front())) {
                filter_str.remove_prefix(1);
                continue;
            }
            if (std::isalpha(filter_str.front())) {
                std::string property_name;
                while (!filter_str.empty() && std::isalpha(filter_str.front())) {
                    property_name.push_back(filter_str.front());
                    filter_str.remove_prefix(1);
                }
                try {
                    resulting_tokens.push_back(property_tokens.at(property_name)());
                } catch (std::out_of_range&) {
                    throw std::runtime_error("Unknown property: " + property_name);
                }
            } else if (filter_str.front() == '\"') {
                filter_str.remove_prefix(1);
                size_t pos = filter_str.find('\"');
                if (pos == std::string_view::npos) {
                    throw std::runtime_error("Invalid string literal: \"" + std::string(filter_str));
                }
                resulting_tokens.push_back(std::make_shared<StringLiteral>(std::string(filter_str.substr(0, pos))));
                filter_str.remove_prefix(pos + 1);
            } else {
                std::string_view greatest_token;
                for (const auto &[token_str, _]: constant_tokens) {
                    if (token_str.size() > greatest_token.size() && filter_str.starts_with(token_str)) {
                        greatest_token = token_str;
                    }
                }
                if (greatest_token.empty()) {
                    throw std::runtime_error("Failed to parse heading token from suffix: " + std::string(filter_str));
                }
                filter_str.remove_prefix(greatest_token.size());
                resulting_tokens.push_back(constant_tokens.at(greatest_token)());
            }
        }
        return resulting_tokens;
    }

    std::vector<TokenHandler> MakePostfixNotationFromInfix(const std::vector<TokenHandler> &tokens) {
        std::vector<TokenHandler> postfix_notation;
        std::stack<TokenHandler> operator_order;

        // Token sequence validation
        // if we remove parenthesis it should look like this: operand operator operand ... operator operand
        bool expect_operand = true;
        for (const auto &token_handler: tokens) {
            if (std::dynamic_pointer_cast<Operand>(token_handler)) {
                if (!expect_operand) {
                    throw std::runtime_error("Expected operator but found operand");
                }
                expect_operand = false;
            } else if (std::dynamic_pointer_cast<Operator>(token_handler)) {
                if (expect_operand) {
                    throw std::runtime_error("Expected operand but found operator");
                }
                expect_operand = true;
            }
        }

        for (const auto& token_handler: tokens) {
            if (auto op_ptr = std::dynamic_pointer_cast<Operator>(token_handler); op_ptr) {
                while (!operator_order.empty()) {
                    auto top_ptr = std::dynamic_pointer_cast<Operator>(operator_order.top());
                    if (!top_ptr || top_ptr->Precedence() < op_ptr->Precedence()) {
                        break;
                    }
                    postfix_notation.push_back(operator_order.top());
                    operator_order.pop();
                }
                operator_order.emplace(token_handler);
            } else if (std::dynamic_pointer_cast<LeftParenthesis>(token_handler)) {
                operator_order.emplace(token_handler);
            } else if (std::dynamic_pointer_cast<RightParenthesis>(token_handler)) {
                while (!operator_order.empty() && !std::dynamic_pointer_cast<LeftParenthesis>(operator_order.top())) {
                    postfix_notation.push_back(operator_order.top());
                    operator_order.pop();
                }
                if (operator_order.empty()) {
                    throw std::runtime_error("Unmatched right parenthesis");
                }
                operator_order.pop(); // Pop left parenthesis
            } else if (std::dynamic_pointer_cast<Operand>(token_handler)) {
                postfix_notation.push_back(token_handler);
            }
        }
        while (!operator_order.empty()) {
            if (std::dynamic_pointer_cast<LeftParenthesis>(operator_order.top())) {
                throw std::runtime_error("Unmatched left parenthesis");
            }
            postfix_notation.push_back(operator_order.top());
            operator_order.pop();
        }
        return postfix_notation;
    }
}
