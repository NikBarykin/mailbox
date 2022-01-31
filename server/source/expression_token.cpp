#include "expression_token.h"

#include <unordered_map>
#include <functional>


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
}
