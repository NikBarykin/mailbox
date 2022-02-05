#pragma once

#include "common/source/letter.h"
#include "expression_token.h"

#include <memory>
#include <functional>
#include <any>
#include <variant>


namespace LetterFilter {
    struct Node {
        virtual ~Node() = 0;
    };

    using NodeHandler = std::unique_ptr<Node>;
    using PropertyT = std::variant<std::string>;

    struct PropertyNode : public Node {
        virtual PropertyT GetProperty(const Letter &) const = 0;
    };

    struct SenderNameNode : public PropertyNode {
        PropertyT GetProperty(const Letter & letter) const override {
            return letter.from;
        }
    };

    struct StringLiteralNode : public PropertyNode {
        std::string value;
        StringLiteralNode(std::string value): value(value) {}
        PropertyT GetProperty(const Letter &) const override {
            return value;
        }
    };

    struct LogicalNode : public Node {
        virtual bool Check(const Letter &) const = 0;
    };

    template<typename LimitVisitor>
    class LimitationNode : public LogicalNode {
        LimitVisitor visitor;
        std::unique_ptr<PropertyNode> left, right;
    public:
        LimitationNode(std::unique_ptr<PropertyNode> left, std::unique_ptr<PropertyNode> right)
        : left(std::move(left)), right(std::move(right)) {}

        bool Check(const Letter & letter) const override {
            PropertyT lhs = left->GetProperty(letter);
            PropertyT rhs = right->GetProperty(letter);
            return std::visit<bool>([this, &rhs]<typename PT>(const PT & lhs) {
                return this->visitor(lhs, std::get<PT>(rhs));
            }, lhs);
        }
    };

    using LessLimitation = LimitationNode<std::less<>>;
    using EqLimitation = LimitationNode<std::equal_to<>>;

    template<typename LogicalCombinator>
    class LogicalCombineNode : public LogicalNode {
        LogicalCombinator combinator;
        std::unique_ptr<LogicalNode> left, right;
    public:
        LogicalCombineNode(std::unique_ptr<LogicalNode> left, std::unique_ptr<LogicalNode> right)
                : left(std::move(left)), right(std::move(right)) {}

        bool Check(const Letter & letter) const override {
            return combinator(left->Check(letter),
                              right->Check(letter));
        }
    };

    using AndCombinator = LogicalCombineNode<std::logical_and<>>;
    using OrCombinator = LogicalCombineNode<std::logical_or<>>;

    std::unique_ptr<LogicalNode> BuildTree(const std::vector<std::unique_ptr<Token>> & tokens_in_postfix_notation);
}
