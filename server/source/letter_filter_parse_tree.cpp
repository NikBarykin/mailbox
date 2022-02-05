#include "letter_filter_parse_tree.h"
#include "expression_token.h"

#include <stack>
#include <cassert>
#include <typeindex>


namespace LetterFilter {
    template<typename NodeT>
    std::unique_ptr<PropertyNode> MakePropertyNode(Token *) {
        return std::make_unique<NodeT>();
    }

    template<typename LimitT>
    std::unique_ptr<LogicalNode> MakeLimit(std::unique_ptr<PropertyNode> left, std::unique_ptr<PropertyNode> right) {
        return std::make_unique<LimitT>(std::move(left), std::move(right));
    }

    template<typename CombT>
    std::unique_ptr<LogicalNode> MakeComb(std::unique_ptr<LogicalNode> left, std::unique_ptr<LogicalNode> right) {
        return std::make_unique<CombT>(std::move(left), std::move(right));
    }

    template<typename T, typename U>
    std::unique_ptr<T> UniquePtrForcedDynamicCast(std::unique_ptr<U>& ptr) {
        auto result = std::unique_ptr<T>(dynamic_cast<T*>(ptr.release()));
        if (!result) {
            throw std::runtime_error(std::string("Failed to perform unique_ptr dynamic_cast: ")
                                     + typeid(U).name() + " -> " + typeid(T).name());
        }
        return result;
    }

    std::unique_ptr<LogicalNode> BuildTree(const std::vector<std::unique_ptr<Token>> & tokens_in_postfix_notation) {
        std::stack<NodeHandler> nodes;

        using PropertyNodeMaker = std::function<std::unique_ptr<PropertyNode>(Token *)>;
        static const std::unordered_map<std::type_index, PropertyNodeMaker> property_makers = {
                {typeid(LetterSenderName), MakePropertyNode<SenderNameNode>},
        };

        using LimitationNodeMaker = std::function<std::unique_ptr<LogicalNode>(std::unique_ptr<PropertyNode>,
                                                                               std::unique_ptr<PropertyNode>)>;
        static const std::unordered_map<std::type_index, LimitationNodeMaker> limitation_makers = {
                {typeid(Equal), MakeLimit<EqLimitation>},
        };

        using CombinatorNodeMaker = std::function<std::unique_ptr<LogicalNode>(std::unique_ptr<LogicalNode>,
                                                                               std::unique_ptr<LogicalNode>)>;
        static const std::unordered_map<std::type_index, CombinatorNodeMaker> comb_makers = {
                {typeid(LogicalAnd), MakeComb<AndCombinator>},
        };

        for (const auto &token_handler: tokens_in_postfix_notation) {
            Token *token_ptr = token_handler.get();
            std::type_index token_type = typeid(*token_ptr);
            if (property_makers.count(token_type)) {
                nodes.push(property_makers.at(token_type)(token_ptr));
            } else if (limitation_makers.count(token_type)) {
//                auto right = std::unique_ptr<PropertyNode>(dynamic_cast<PropertyNode *>(nodes.top().release()));
                auto right = UniquePtrForcedDynamicCast<PropertyNode>(nodes.top());
                nodes.pop();
                auto left = UniquePtrForcedDynamicCast<PropertyNode>(nodes.top());
                nodes.pop();
                // TODO: make normal exception
                assert(left && right);
                nodes.push(limitation_makers.at(token_type)(std::move(left), std::move(right)));
            } else if (comb_makers.count(token_type)) {
                auto right = UniquePtrForcedDynamicCast<LogicalNode>(nodes.top());
                nodes.pop();
                auto left = UniquePtrForcedDynamicCast<LogicalNode>(nodes.top());
                nodes.pop();
                nodes.push(comb_makers.at(token_type)(std::move(left), std::move(right)));
            } else {
                throw std::runtime_error(std::string("Bad token type: ") + typeid(*token_ptr).name());
            }
        }
        if (nodes.size() != 1) {
            throw std::runtime_error("Bad tokens, failed to combine one logical node from them");
        }
        return UniquePtrForcedDynamicCast<LogicalNode>(nodes.top());
    }
}
