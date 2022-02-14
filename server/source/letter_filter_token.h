#pragma once

#include <string>
#include <vector>
#include <memory>


namespace LetterFilter::Token {
    struct Token {
        virtual ~Token() = 0;
    };

    using TokenHandler = std::shared_ptr<Token>;

    struct Operator : public Token {
        constexpr virtual int Precedence() const = 0;
    };

    struct LogicalAnd : public Operator {
        constexpr int Precedence() const final { return 1; };
    };

    struct LogicalOr : public Operator {
        constexpr int Precedence() const final { return 0; };
    };

    struct Condition : public Operator {
        constexpr int Precedence() const final { return 2; };
    };

    // TODO: "IN" condition (like "Foo" IN body)
    struct Equal        : public Condition {};
    struct NotEqual     : public Condition {};
    struct Less         : public Condition {};
    struct Greater      : public Condition {};
    struct LessEqual    : public Condition {};
    struct GreaterEqual : public Condition {};

    struct Operand : public Token {};

    struct LeftParenthesis : public Token {};
    struct RightParenthesis : public Token {};

    struct FromProperty : public Operand {};
    struct BodyProperty : public Operand {};

    struct StringLiteral : public Operand {
        std::string value;
        explicit StringLiteral(std::string value) : value(std::move(value)) {}
    };

    std::vector<TokenHandler> Tokenize(std::string_view filter_str);
    std::vector<TokenHandler> MakePostfixNotationFromInfix(const std::vector<TokenHandler> &);
}
