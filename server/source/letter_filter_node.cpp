#include "letter_filter_node.h"
#include "letter_filter_token.h"

#include <stack>
#include <cassert>
#include <typeindex>

using namespace std;

namespace LetterFilter::Node {
    Node::~Node() = default;

    template<typename PropertyNodeT>
    std::shared_ptr<Property> MakeProperty(Token::TokenHandler) {
        return std::make_shared<PropertyNodeT>();
    }

    template<>
    std::shared_ptr<Property> MakeProperty<StringLiteral>(Token::TokenHandler token) {
        return std::make_shared<StringLiteral>(dynamic_cast<Token::StringLiteral&>(*token).value);
    }


    template<typename ConditionNodeT>
    std::shared_ptr<Logical> MakeCondition(std::shared_ptr<Property> left, std::shared_ptr<Property> right) {
        return std::make_shared<ConditionNodeT>(left, right);
    }

    template<typename LogicalCombinationT>
    std::shared_ptr<Logical> MakeLogicalCombination(std::shared_ptr<Logical> left, std::shared_ptr<Logical> right) {
        return std::make_shared<LogicalCombinationT>(left, right);
    }

    std::shared_ptr<Logical> BuildTree(const std::vector<Token::TokenHandler> &tokens_in_postfix_notation) {
        using PropertyMaker = std::function<std::shared_ptr<Property>(Token::Token * )>;
        static const unordered_map<type_index, function<shared_ptr<Property>(Token::TokenHandler)>> property_makers = {
                {typeid(Token::FromProperty),  MakeProperty<From>},
                {typeid(Token::BodyProperty),  MakeProperty<Body>},
                {typeid(Token::StringLiteral), MakeProperty<StringLiteral>},
        };

        using ConditionMaker = std::function<std::shared_ptr<Logical>(std::shared_ptr<Property>,
                                                                      std::shared_ptr<Property>)>;
        static const std::unordered_map<std::type_index, ConditionMaker> condition_makers = {
                {typeid(Token::Equal),    MakeCondition<Equal>},
                {typeid(Token::NotEqual), MakeCondition<NotEqual>},
                {typeid(Token::NotEqual), MakeCondition<Less>},
                {typeid(Token::NotEqual), MakeCondition<Greater>},
                {typeid(Token::NotEqual), MakeCondition<LessEqual>},
                {typeid(Token::NotEqual), MakeCondition<GreaterEqual>},
        };

        using LogicalCombinationMaker = std::function<std::shared_ptr<Logical>(std::shared_ptr<Logical>,
                                                                               std::shared_ptr<Logical>)>;
        static const std::unordered_map<std::type_index, LogicalCombinationMaker> logical_combination_makers = {
                {typeid(Token::LogicalAnd), MakeLogicalCombination<LogicalAnd>},
                {typeid(Token::LogicalOr),  MakeLogicalCombination<LogicalOr>},
        };

        std::stack<NodeHandler> nodes;

        for (const auto &token_handler: tokens_in_postfix_notation) {
            std::type_index token_type = typeid(*token_handler);
            if (property_makers.count(token_type)) {
                nodes.push(property_makers.at(token_type)(token_handler));
            } else if (condition_makers.count(token_type)) {
                auto right = std::dynamic_pointer_cast<Property>(nodes.top());
                nodes.pop();
                auto left = std::dynamic_pointer_cast<Property>(nodes.top());
                nodes.pop();
                // TODO: make normal exception
                assert(left && right);
                nodes.push(condition_makers.at(token_type)(left, right));
            } else if (logical_combination_makers.count(token_type)) {
                auto right = std::dynamic_pointer_cast<Logical>(nodes.top());
                nodes.pop();
                auto left = std::dynamic_pointer_cast<Logical>(nodes.top());
                nodes.pop();
                assert(left && right);
                nodes.push(logical_combination_makers.at(token_type)(left, right));
            } else {
                throw std::runtime_error(std::string("Bad token type: ") + token_type.name());
            }
        }

        if (nodes.size() != 1) {
            throw std::runtime_error("Failed to build one node from token sequence");
        }
        auto tree = std::dynamic_pointer_cast<Logical>(nodes.top());
        if (!tree) {
            throw std::runtime_error("Failed to build logical node from token sequence");
        }
        return tree;
    }
}
