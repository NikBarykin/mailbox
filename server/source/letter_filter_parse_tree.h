#pragma once

#include "common/source/letter.h"

#include <memory>
#include <functional>


namespace LetterFilter {
    class LogicalNode {
    public:
        virtual bool Check(const Letter &) const = 0;
    };

    template<class CombinationRule>
    class BinaryOperator : public LogicalNode {
    private:
        std::unique_ptr<LogicalNode> left_child, right_child;
    public:
        BinaryOperator(std::unique_ptr<LogicalNode> left_child, std::unique_ptr<LogicalNode> right_child)
        : left_child(std::move(left_child)), right_child(std::move(right_child)) {}

        virtual bool Check(const Letter & letter) const override {
            return CombinationRule()(left_child->Check(letter),
                                   right_child->Check(letter));
        }
    };

    using LogicalAndOperator = BinaryOperator<std::logical_and<bool>>;

    template<class LetterPropertyType>
    class PropertyNode {
    public:
        virtual LetterPropertyType Eval(const Letter &) const = 0;
    };

    template<class LetterPropertyType, class CompareRule>
    class PropertyComparator : public LogicalNode {
    public:
        std::unique_ptr<PropertyNode<LetterPropertyType>> left_child, right_child;
        virtual bool Check(const Letter & letter) const override {
            return CompareRule()(left_child->Eval(letter),
                                 right_child->Eval(letter));
        }
    };
}
