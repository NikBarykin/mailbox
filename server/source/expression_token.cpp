#include "expression_token.h"

#include <unordered_map>
#include <functional>
#include <stack>


namespace LetterFilter {
    Token::~Token() = default;

    template<class TokenT>
    std::unique_ptr<Token> MakeToken() {
        return std::make_unique<TokenT>();
    }

    std::vector<std::unique_ptr<Token>> ParseLetterFilterTokens(std::string_view sv) {
        static const std::unordered_map<std::string, std::function<std::unique_ptr<Token> ()>> constant_tokens = {
                {"(", MakeToken<LeftParenthesis>},
                {")", MakeToken<RightParenthesis>},
                {"==", MakeToken<Equal>},
                {"||", MakeToken<LogicalOr>},
        };
        static const std::unordered_map<std::string, std::function<std::unique_ptr<Token> ()>> property_tokens = {
                {"from", MakeToken<LetterSenderName>},
        };
//        static const std::unordered_map<std::string, std::function<std::unique_ptr<Token> ()>> literal_tokens = {
//                {"from", MakeToken<LetterSenderName>},
//        };

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
                tokens.push_back(std::make_unique<Literal>(std::string(sv.substr(0, pos))));
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

    std::vector<std::unique_ptr<Token>> MakePostfixNotationFromInfix(std::vector<std::unique_ptr<Token>> && tokens) {
        std::vector<std::unique_ptr<Token>> postfix_notation;
        std::stack<std::unique_ptr<Token>> operators;

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
            Token * token_ptr = token_handler.release();
            if (auto op_ptr = dynamic_cast<BinaryOperator*>(token_ptr); op_ptr) {
                while (!operators.empty()) {
                    auto top_ptr = dynamic_cast<BinaryOperator*>(operators.top().get());
                    if (!top_ptr || top_ptr->Precedence() < op_ptr->Precedence()) {
                        break;
                    }
                    postfix_notation.push_back(std::move(operators.top()));
                    operators.pop();
                }
                operators.emplace(token_ptr);
            } else if (dynamic_cast<LeftParenthesis*>(token_ptr)) {
                operators.emplace(token_ptr);
            } else if (dynamic_cast<RightParenthesis*>(token_ptr)) {
                while (!operators.empty() && !dynamic_cast<LeftParenthesis*>(operators.top().get())) {
                    postfix_notation.push_back(std::move(operators.top()));
                    operators.pop();
                }
                if (operators.empty()) {
                    throw std::runtime_error("Unmatched right parenthesis");
                }
                operators.pop(); // Pop left parenthesis
            } else if (dynamic_cast<Operand*>(token_ptr)) {
                postfix_notation.emplace_back(token_ptr);
            }
        }
        while (!operators.empty()) {
            if (dynamic_cast<LeftParenthesis*>(operators.top().get())) {
                throw std::runtime_error("Unmatched left parenthesis");
            }
            postfix_notation.emplace_back(std::move(operators.top()));
            operators.pop();
        }
        return postfix_notation;
    }
}
