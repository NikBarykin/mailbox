#include "letter_filter_token.h"
#include "letter_filter.h"

#include <unordered_map>
#include <functional>
#include <stack>
#include <regex>


namespace LetterFilter::Token {
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
                {"in", MakeToken<ContainedIn>},
                {"(" , MakeToken<LeftParenthesis>},
                {")" , MakeToken<RightParenthesis>},
                {"from", MakeToken<FromProperty>},
                {"body", MakeToken<BodyProperty>},
                {"date", MakeToken<DateProperty>},
        };

        std::vector<TokenHandler> resulting_tokens;

        while (!filter_str.empty()) {
            if (std::isspace(filter_str.front())) {
                filter_str.remove_prefix(1);
                continue;
            }

            std::string_view largest_token;
            for (const auto &[token_str, _]: constant_tokens) {
                if (token_str.size() > largest_token.size() && filter_str.substr(0, token_str.size()) == token_str) {
                    largest_token = token_str;
                }
            }
            if (!largest_token.empty()) {
                filter_str.remove_prefix(largest_token.size());
                resulting_tokens.push_back(constant_tokens.at(largest_token)());
                continue;
            }

            // String literal
            if (char start_ch = filter_str.front(); start_ch == '\"' || start_ch == '\'') {
                filter_str.remove_prefix(1);
                bool escape = false;
                std::string literal_value;
                while (true) {
                    if (filter_str.empty()) {
                        throw ParseError("Failed to parse string literal, unmatched quotation mark");
                    }
                    char cur_ch = filter_str.front();
                    filter_str.remove_prefix(1);
                    if (escape) {
                        escape = false;
                    } else if (cur_ch == '\\') {
                        escape = true;
                        continue;
                    } else if (cur_ch == start_ch) {
                        break;
                    }
                    literal_value.push_back(cur_ch);
                }
                resulting_tokens.push_back(std::make_shared<StringLiteral>(std::move(literal_value)));
            } else { // Date literal or invalid token
                // [\s\S] means any character including newline ('.' doesn't match newline)
                std::regex date_regex{"(" + std::string(Date::regex) + ")[\\s\\S]*"};
                std::match_results<std::string_view::const_iterator> match_res;

                if (!std::regex_match(filter_str.cbegin(), filter_str.cend(), match_res, date_regex)) {
                    throw ParseError("Failed to parse heading token from suffix: " + std::string(filter_str));
                }

                std::string date_str = match_res[1];
                filter_str.remove_prefix(date_str.size());
                resulting_tokens.push_back(std::make_shared<DateLiteral>(Date::ParseFrom(date_str)));
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
                    throw ParseError("Wrong token order, expected operator but found operand");
                }
                expect_operand = false;
            } else if (std::dynamic_pointer_cast<Operator>(token_handler)) {
                if (expect_operand) {
                    throw ParseError("Wrong token order, expected operand but found operator");
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
                    throw ParseError("Unmatched right parenthesis");
                }
                operator_order.pop(); // Pop left parenthesis
            } else if (std::dynamic_pointer_cast<Operand>(token_handler)) {
                postfix_notation.push_back(token_handler);
            }
        }
        while (!operator_order.empty()) {
            if (std::dynamic_pointer_cast<LeftParenthesis>(operator_order.top())) {
                throw ParseError("Unmatched left parenthesis");
            }
            postfix_notation.push_back(operator_order.top());
            operator_order.pop();
        }
        return postfix_notation;
    }
}
