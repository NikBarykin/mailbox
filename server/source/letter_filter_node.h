#pragma once

#include "common/source/letter.h"
#include "letter_filter_token.h"

#include <memory>
#include <functional>
#include <any>
#include <variant>


namespace LetterFilter::Node {
    struct Node {
        virtual ~Node() = 0;
    };

    using NodeHandler = std::shared_ptr<Node>;
    using PropertyT = std::variant<std::string>;

    struct Property : public Node {
        virtual PropertyT GetProperty(const Letter &) const = 0;
    };

    struct From : public Property {
        PropertyT GetProperty(const Letter &letter) const override {
            return letter.from;
        }
    };

    struct Body : public Property {
        PropertyT GetProperty(const Letter &letter) const override {
            return letter.body;
        }
    };

    struct StringLiteral : public Property {
        std::string value;

        StringLiteral(std::string value) : value(value) {}

        PropertyT GetProperty(const Letter &) const final { return value; }
    };

    struct Logical : public Node {
        virtual bool Evaluate(const Letter &) const = 0;
    };

    template<typename CombinationRule>
    class LogicalCombination : public Logical {
        CombinationRule rule;
        std::shared_ptr<Logical> left, right;

    public:
        LogicalCombination(std::shared_ptr<Logical> left, std::shared_ptr<Logical> right)
        : left(left), right(right) {}

        bool Evaluate(const Letter &letter) const final {
            return rule(left->Evaluate(letter),
                        right->Evaluate(letter));
        }
    };

    using LogicalAnd =  LogicalCombination<std::logical_and<>>;
    using LogicalOr  =  LogicalCombination<std::logical_or<>>;

    template<typename ConditionVisitor>
    class Condition : public Logical {
        ConditionVisitor visitor;
        std::shared_ptr<Property> left, right;

    public:
        Condition(std::shared_ptr<Property> left, std::shared_ptr<Property> right)
        : left(left), right(right) {}

        bool Evaluate(const Letter &letter) const final {
            PropertyT lhs = left->GetProperty(letter);
            PropertyT rhs = right->GetProperty(letter);
            return std::visit<bool>([this, &rhs]<typename PT>(const PT &lhs) {
                return this->visitor(lhs, std::get<PT>(rhs));
            }, lhs);
        }
    };

    using Equal        = Condition<std::equal_to<>>;
    using NotEqual     = Condition<std::not_equal_to<>>;
    using Less         = Condition<std::less<>>;
    using Greater      = Condition<std::greater<>>;
    using LessEqual    = Condition<std::less_equal<>>;
    using GreaterEqual = Condition<std::greater_equal<>>;

    std::shared_ptr<Logical> BuildTree(const std::vector<Token::TokenHandler> &tokens_in_postfix_notation);
}
