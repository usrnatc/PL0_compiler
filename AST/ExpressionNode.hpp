//
// Created by user on 16-August-2023.
//

#ifndef PL0_COMPILER_EXPRESSIONNODE_HPP
#define PL0_COMPILER_EXPRESSIONNODE_HPP

#include "AST.hpp"
#include "Visitor.hpp"
#include <stdexcept>
#include <memory>

namespace AST {

    class ExpressionNode : public ASTNode
    {
        public:
            ~ExpressionNode() override = default;
            virtual void accept(class ExpressionVisitor &visitor) = 0;

            void accept(class Visitor &visitor) override
            {
                auto expression_visitor = dynamic_cast<ExpressionVisitor *>(&visitor);

                if (expression_visitor) {
                    accept(*expression_visitor);
                } else {
                    throw std::runtime_error("Invalid visitor");
                }
            }
    };

    class ConstantExpression : public ExpressionNode
    {
            int value;
        public:
            explicit ConstantExpression(int value)
                    : value(value)
            {}

            void accept(class ExpressionVisitor &visitor) override
            {
                visitor.visit(*this);
            }

            [[nodiscard]] int getValue() const
            { return value; }

            [[nodiscard]] std::string toString() const override
            { return std::to_string(value); }
    };

    class VariableExpression : public ExpressionNode
    {
            std::string name;
        public:
            explicit VariableExpression(std::string name)
                    : name(std::move(name))
            {}

            void accept(class ExpressionVisitor &visitor) override
            {
                visitor.visit(*this);
            }

            [[nodiscard]] const std::string &getName() const
            { return name; }

            [[nodiscard]] std::string toString() const override
            { return name; }
    };

    class BinaryExpression : public ExpressionNode
    {
            std::unique_ptr<ExpressionNode> left;
            std::unique_ptr<ExpressionNode> right;
            std::string op;
        public:
            BinaryExpression(std::unique_ptr<ExpressionNode> left,
                             std::unique_ptr<ExpressionNode> right,
                             std::string op)
                    : left(std::move(left))
                      , right(std::move(right))
                      , op(std::move(op))
            {}

            void accept(class ExpressionVisitor &visitor) override
            {
                visitor.visit(*this);
            }

            [[nodiscard]] const std::string &getOp() const
            { return op; }

            [[nodiscard]] const ExpressionNode &getLeft() const
            { return *left; }

            [[nodiscard]] const ExpressionNode &getRight() const
            { return *right; }

            [[nodiscard]] std::string toString() const override
            { return left->toString() + " " + op + " " + right->toString(); }
    };

    class UnaryExpression : public ExpressionNode
    {
            std::unique_ptr<ExpressionNode> expression;
            std::string op;
        public:
            UnaryExpression(std::unique_ptr<ExpressionNode> expression,
                            std::string op)
                    : expression(std::move(expression))
                      , op(std::move(op))
            {}

            void accept(class ExpressionVisitor &visitor) override
            {
                visitor.visit(*this);
            }

            [[nodiscard]] const std::string &getOp() const
            { return op; }

            [[nodiscard]] const ExpressionNode &getExpression() const
            { return *expression; }

            [[nodiscard]] std::string toString() const override
            { return op + expression->toString(); }
    };
}
#endif //PL0_COMPILER_EXPRESSIONNODE_HPP
