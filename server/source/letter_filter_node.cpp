#include "letter_filter_node.h"
#include "letter_filter_token.h"

#include <stack>
#include <cassert>
#include <typeindex>


namespace LetterFilter {
    namespace Node {
        Node::~Node() = default;

        template<typename NodeT>
        std::shared_ptr<Property> MakePropertyNode(Token::Token *) {
            return std::make_shared<NodeT>();
        }

        template<typename NodeT>
        std::shared_ptr<Property> MakeLiteralNode(Token::Token *token) {
            return std::make_shared<NodeT>(dynamic_cast<Token::StringLiteral *>(token)->value);
        }

        using PropertyMaker = std::function<std::shared_ptr<Property>(Token::Token * )>;
        static const std::unordered_map<std::type_index, PropertyMaker> property_makers = {
                {typeid(Token::FromProperty),  MakePropertyNode<From>},
                {typeid(Token::BodyProperty),  MakePropertyNode<Body>},
                {typeid(Token::StringLiteral), MakeLiteralNode<StringLiteral>},
        };

        template<typename LimitT>
        std::shared_ptr<Logical>
        MakeLimit(std::shared_ptr<Property> left, std::shared_ptr<Property> right) {
            return std::make_shared<LimitT>(left, right);
        }

        using LimitationNodeMaker = std::function<std::shared_ptr<Logical>(std::shared_ptr<Property>,
                                                                               std::shared_ptr<Property>)>;
        static const std::unordered_map<std::type_index, LimitationNodeMaker> limitation_makers = {
                {typeid(Token::Equal),    MakeLimit<EqualLimitation>},
                {typeid(Token::NotEqual), MakeLimit<NotEqualLimitation>},
        };

        template<typename CombT>
        std::shared_ptr<Logical> MakeComb(std::shared_ptr<Logical> left, std::shared_ptr<Logical> right) {
            return std::make_shared<CombT>(left, right);
        }

        using CombinatorNodeMaker = std::function<std::shared_ptr<Logical>(std::shared_ptr<Logical>,
                                                                               std::shared_ptr<Logical>)>;
        static const std::unordered_map<std::type_index, CombinatorNodeMaker> comb_makers = {
                {typeid(Token::LogicalAnd), MakeComb<AndCombinator>},
                {typeid(Token::LogicalOr),  MakeComb<OrCombinator>},
        };

//    template<typename T, typename U>
//    std::shared_ptr<T> UniquePtrForcedDynamicCast(std::shared_ptr<U>& ptr) {
//        auto result = std::shared_ptr<T>(dynamic_cast<T*>(ptr.release()));
//        if (!result) {
//            throw std::runtime_error(std::string("Failed to perform unique_ptr dynamic_cast: ")
//                                     + typeid(U).name() + " -> " + typeid(T).name());
//        }
//        return result;
//    }

        std::shared_ptr<Logical> BuildTree(const std::vector<Token::TokenHandler> &tokens_in_postfix_notation) {
            std::stack<NodeHandler> nodes;

            for (const auto &token_handler: tokens_in_postfix_notation) {
                Token::Token *token_ptr = token_handler.get();
                std::type_index token_type = typeid(*token_ptr);
                if (property_makers.count(token_type)) {
                    nodes.push(property_makers.at(token_type)(token_ptr));
                } else if (limitation_makers.count(token_type)) {
                    auto right = std::dynamic_pointer_cast<Property>(nodes.top());
                    nodes.pop();
                    auto left = std::dynamic_pointer_cast<Property>(nodes.top());
                    nodes.pop();
                    // TODO: make normal exception
                    assert(left && right);
                    nodes.push(limitation_makers.at(token_type)(left, right));
                } else if (comb_makers.count(token_type)) {
                    auto right = std::dynamic_pointer_cast<Logical>(nodes.top());
                    nodes.pop();
                    auto left = std::dynamic_pointer_cast<Logical>(nodes.top());
                    nodes.pop();
                    assert(left && right);
                    nodes.push(comb_makers.at(token_type)(left, right));
                } else {
                    throw std::runtime_error(std::string("Bad token type: ") + typeid(*token_ptr).name());
                }
            }
            auto resulting_node = std::dynamic_pointer_cast<Logical>(nodes.top());
            if (nodes.size() != 1 || !resulting_node) {
                throw std::runtime_error("Bad tokens, failed to combine one logical node from them");
            }
            return resulting_node;
        }
    }
}
