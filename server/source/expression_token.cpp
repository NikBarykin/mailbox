#include "expression_token.h"

#include <unordered_map>
#include <functional>
#include <stack>


namespace LetterFilter {
    Token::~Token() = default;

    template<class TokenT>
    std::shared_ptr<Token> MakeToken() {
        return std::make_shared<TokenT>();
    }

    std::vector<std::shared_ptr<Token>> ParseLetterFilterTokens(std::string_view sv) {
        static const std::unordered_map<std::string, std::function<std::shared_ptr<Token> ()>> constant_tokens = {
                {"(", MakeToken<LeftParenthesis>},
                {")", MakeToken<RightParenthesis>},
                {"==", MakeToken<Equal>},
                {"||", MakeToken<LogicalOr>},
                {"&&", MakeToken<LogicalAnd>},
        };
        static const std::unordered_map<std::string, std::function<std::shared_ptr<Token> ()>> property_tokens = {
                {"from", MakeToken<LetterSenderName>},
                {"body", MakeToken<LetterBody>},
        };
//        static const std::unordered_map<std::string, std::function<std::shared_ptr<Token> ()>> literal_tokens = {
//                {"from", MakeToken<LetterSenderName>},
//        };

        std::vector<std::shared_ptr<Token>> tokens;

        while (!sv.empty()) {
            if (std::isspace(sv.front())) {
                sv.remove_prefix(1);
                continue;
            }
            if (std::isalpha(sv.front())) {
                std::string property_name;
                while (!sv.empty() && std::isalpha(sv.front())) {
                    property_name.push_back(sv.front());
                    sv.remove_prefix(1);
                }
                if (!property_tokens.count(property_name)) {
                    throw std::runtime_error("Unknown property: " + property_name);
                }
                tokens.push_back(property_tokens.at(property_name)());
            } else if (sv.front() == '\"') {
                sv.remove_prefix(1);
                size_t pos = sv.find('\"');
                if (pos == std::string_view::npos) {
                    throw std::runtime_error("Invalid literal: \"" + std::string(sv));
                }
                tokens.push_back(std::make_shared<Literal>(std::string(sv.substr(0, pos))));
                sv.remove_prefix(pos + 1);
            } else {
                std::string greatest_token;
                for (const auto & [token_str, _] : constant_tokens) {
                    if (token_str.size() > greatest_token.size() && sv.starts_with(token_str)) {
                        greatest_token = token_str;
                    }
                }
                if (greatest_token.empty()) {
                    throw std::runtime_error("Failed to parse heading token from suffix: " + std::string(sv));
                }
                sv.remove_prefix(greatest_token.size());
                tokens.push_back(constant_tokens.at(greatest_token)());
            }
        }
        return tokens;
    }

    std::vector<std::shared_ptr<Token>> MakePostfixNotationFromInfix(const std::vector<std::shared_ptr<Token>> & tokens) {
        std::vector<std::shared_ptr<Token>> postfix_notation;
        std::stack<std::shared_ptr<Token>> operators;

        // Token sequence validation (if we remove parenthesis it should look like this:
        // operand operator operand ... operator operand)
        bool expect_operand = true;
        for (const auto & token_handler : tokens) {
            if (dynamic_cast<Operand*>(token_handler.get())) {
                if (!expect_operand) {
                    throw std::runtime_error("Expected operator but found operand");
                }
                expect_operand = false;
            } else if (dynamic_cast<BinaryOperator*>(token_handler.get())) {
                if (expect_operand) {
                    throw std::runtime_error("Expected operand but found operator");
                }
                expect_operand = true;
            }
        }

        for (auto && token_handler : tokens) {
            if (auto op_ptr = std::dynamic_pointer_cast<BinaryOperator>(token_handler); op_ptr) {
                while (!operators.empty()) {
                    auto top_ptr = std::dynamic_pointer_cast<BinaryOperator>(operators.top());
                    if (!top_ptr || top_ptr->Precedence() < op_ptr->Precedence()) {
                        break;
                    }
                    postfix_notation.push_back(operators.top());
                    operators.pop();
                }
                operators.emplace(token_handler);
            } else if (std::dynamic_pointer_cast<LeftParenthesis>(token_handler)) {
                operators.emplace(token_handler);
            } else if (std::dynamic_pointer_cast<RightParenthesis>(token_handler)) {
                while (!operators.empty() && !std::dynamic_pointer_cast<LeftParenthesis>(operators.top())) {
                    postfix_notation.push_back(operators.top());
                    operators.pop();
                }
                if (operators.empty()) {
                    throw std::runtime_error("Unmatched right parenthesis");
                }
                operators.pop(); // Pop left parenthesis
            } else if (std::dynamic_pointer_cast<Operand>(token_handler)) {
                postfix_notation.push_back(token_handler);
            }
        }
        while (!operators.empty()) {
            if (std::dynamic_pointer_cast<LeftParenthesis>(operators.top())) {
                throw std::runtime_error("Unmatched left parenthesis");
            }
            postfix_notation.push_back(operators.top());
            operators.pop();
        }
        return postfix_notation;
    }
}
