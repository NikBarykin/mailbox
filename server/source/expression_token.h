#pragma once

#include <string>
#include <vector>
#include <memory>

// TODO: discard letter-filter-words, though it is a generic code that can be used outside of letter-filter context
namespace LetterFilter {
    struct Token {
        virtual ~Token() = 0;
    };

    // TODO: maybe
    // using TokenHandler = std::shared_ptr<Token>;

    struct BinaryOperator : public Token {
        constexpr virtual int Precedence() const = 0;
    };

    struct LogicalAnd : public BinaryOperator {
        constexpr int Precedence() const final { return 1; };
    };

    struct LogicalOr : public BinaryOperator {
        constexpr int Precedence() const final { return 0; };
    };

    // TODO: maybe comparison
    struct Comparator : public BinaryOperator {
        constexpr int Precedence() const final { return 2; };
    };

    struct Equal : public Comparator {};
    struct NotEqual : public Comparator {};

    struct Operand : public Token {};

    struct LetterSenderName : public Operand {};
    struct LetterBody : public Operand {};

    // TODO: deduce value type
    struct Literal : public Operand {
        std::string value_str;
        Literal(std::string value_str) : value_str(std::move(value_str)) {}
    };

    struct LeftParenthesis : public Token {};
    struct RightParenthesis : public Token {};

    std::vector<std::shared_ptr<Token>> ParseLetterFilterTokens(std::string_view);
    std::vector<std::shared_ptr<Token>> MakePostfixNotationFromInfix(const std::vector<std::shared_ptr<Token>> &);
}
