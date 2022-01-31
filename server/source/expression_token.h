#pragma once

#include <string>
#include <vector>
#include <memory>

// TODO: discard letter-filter-words, though it is a generic code that can be used outside of letter-filter context
namespace LetterFilter {
    struct Token {};

    // TODO: maybe
    // using TokenHandler = std::unique_ptr<Token>;

    template <int precedence, class Func>
    struct BinaryOperator : public Token {
        static constexpr int precedence_ = precedence;
        using Func_ = Func;
    };

    using LogicalAnd    = BinaryOperator<1, std::logical_and<>>;
    using LogicalOr     = BinaryOperator<0, std::logical_or<>>;

    using Equal         = BinaryOperator<2, std::equal_to<>>;
    using NotEqual      = BinaryOperator<2, std::not_equal_to<>>;
    using Less          = BinaryOperator<2, std::less<>>;
    using LessEqual     = BinaryOperator<2, std::less_equal<>>;
    using Greater       = BinaryOperator<2, std::greater<>>;
    using GreaterEqual  = BinaryOperator<2, std::greater_equal<>>;

    struct Operand : public Token {};

    struct LetterProperty : public Operand {
        std::string property_name;
        LetterProperty(std::string property_name)
        : property_name(std::move(property_name)) {}
    };

    struct Literal : public Operand {
        std::string value_str;
        Literal(std::string value_str)
        : value_str(std::move(value_str)) {}
    };

    struct LeftParenthesis : public Token {};
//    std::unique_ptr<Token> MakeLeftParenthesis() {
//        return std::make_unique<LeftParenthesis>()
//    }
    struct RightParenthesis : public Token {};

    std::vector<std::unique_ptr<Token>> ParseTokens(std::string_view);
    std::vector<std::unique_ptr<Token>> MakePostfixNotationFromInfix(
            const std::vector<std::unique_ptr<Token>> &);
}
