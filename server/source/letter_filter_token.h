#pragma once

#include <string>
#include <vector>
#include <memory>

#include "common/source/date.h"


namespace LetterFilter::Token {
    struct Token {
        virtual ~Token() = 0;
    };
    inline Token::~Token() = default;

    using TokenHandler = std::shared_ptr<Token>;

    struct Operator : public Token {
        virtual int Precedence() const = 0;
    };

    struct LogicalAnd : public Operator {
        int Precedence() const final { return 1; };
    };

    struct LogicalOr : public Operator {
        int Precedence() const final { return 0; };
    };

    struct Condition : public Operator {
        int Precedence() const final { return 2; };
    };

    struct Equal        : public Condition {};
    struct NotEqual     : public Condition {};
    struct Less         : public Condition {};
    struct Greater      : public Condition {};
    struct LessEqual    : public Condition {};
    struct GreaterEqual : public Condition {};
    struct ContainedIn  : public Condition {};

    struct Operand : public Token {};

    struct LeftParenthesis : public Token {};
    struct RightParenthesis : public Token {};

    struct FromProperty : public Operand {};
    struct BodyProperty : public Operand {};
    struct DateProperty : public Operand {};

    struct StringLiteral : public Operand {
        std::string value;
        explicit StringLiteral(std::string value) : value(std::move(value)) {}
    };

    struct DateLiteral : public Operand {
        Date value;
        explicit DateLiteral(Date value) : value(value) {}
    };

    std::vector<TokenHandler> Tokenize(std::string_view filter_str);
    std::vector<TokenHandler> MakePostfixNotationFromInfix(const std::vector<TokenHandler> &);
}
