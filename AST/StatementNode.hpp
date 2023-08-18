//
// Created by user on 16-August-2023.
//

#ifndef PL0_COMPILER_STATEMENTNODE_HPP
#define PL0_COMPILER_STATEMENTNODE_HPP

#include "AST.hpp"
#include "Visitor.hpp"
#include "ProcedureNode.hpp"
#include <stdexcept>
#include <memory>
#include <vector>
#include <sstream>

namespace AST {

    class StatementNode : public ASTNode
    {
        public:
            ~StatementNode() override = default;
            virtual void accept(class StatementVisitor &visitor) = 0;

            void accept(class Visitor &visitor) override
            {
                auto statement_visitor = dynamic_cast<StatementVisitor *>(&visitor);

                if (statement_visitor) {
                    accept(*statement_visitor);
                } else {
                    throw std::runtime_error("Invalid visitor");
                }
            }
    };

    class AssignmentStatement : public StatementNode
    {
            std::string name;
            std::unique_ptr<ExpressionNode> expression;
        public:
            AssignmentStatement(std::string name,
                                std::unique_ptr<ExpressionNode> expression)
                    : name(std::move(name))
                      , expression(std::move(expression))
            {}

            void accept(class StatementVisitor &visitor) override
            {
                visitor.visit(*this);
            }

            [[nodiscard]] const std::string &getName() const
            { return name; }

            [[nodiscard]] const ExpressionNode &getExpression() const
            { return *expression; }

            [[nodiscard]] std::string toString() const override
            { return name + " = " + expression->toString(); }
    };

    class CallStatement : public StatementNode
    {
            std::unique_ptr<ProcedureNode> procedure;

        public:
            explicit CallStatement(std::unique_ptr<ProcedureNode> procedure)
                    : procedure(std::move(procedure))
            {}

            void accept(class StatementVisitor &visitor) override
            {
                visitor.visit(*this);
            }

            [[nodiscard]] const ProcedureNode &getProcedure() const
            { return *procedure; }

            [[nodiscard]] std::string toString() const override
            { return "CALL " + procedure->toString(); }
    };

    class BlockStatement : public StatementNode
    {
            std::vector<std::unique_ptr<StatementNode>> statements;

        public:
            explicit BlockStatement(
                    std::vector<std::unique_ptr<StatementNode>> statements)
                    : statements(std::move(statements))
            {}

            void accept(class StatementVisitor &visitor) override
            {
                visitor.visit(*this);
            }

            [[nodiscard]] const std::vector<std::unique_ptr<StatementNode>> &
            getStatements() const
            { return statements; }

            [[nodiscard]] std::string toString() const override
            {
                std::ostringstream oss;

                oss << "BEGIN" << std::endl;
                for (const auto &statement: statements) {
                    oss << statement->toString() << std::endl;
                }
                oss << "END";
                return oss.str();
            }
    };

    class IfStatement : public StatementNode
    {
            std::unique_ptr<ExpressionNode> condition;
            std::unique_ptr<StatementNode> then_statement;

        public:
            IfStatement(std::unique_ptr<ExpressionNode> condition,
                        std::unique_ptr<StatementNode> then_statement)
                    : condition(std::move(condition))
                      , then_statement(std::move(then_statement))
            {}

            void accept(class StatementVisitor &visitor) override
            {
                visitor.visit(*this);
            }

            [[nodiscard]] const ExpressionNode &getCondition() const
            { return *condition; }

            [[nodiscard]] const StatementNode &getThenStatement() const
            { return *then_statement; }

            [[nodiscard]] std::string toString() const override
            {
                std::ostringstream oss;

                oss << "IF " << condition->toString() << " THEN" << std::endl;
                oss << then_statement->toString();
                return oss.str();
            }
    };

    class IfElseStatement : public StatementNode
    {
            std::unique_ptr<ExpressionNode> condition;
            std::unique_ptr<StatementNode> then_statement;
            std::unique_ptr<StatementNode> else_statement;

        public:
            IfElseStatement(std::unique_ptr<ExpressionNode> condition,
                            std::unique_ptr<StatementNode> then_statement,
                            std::unique_ptr<StatementNode> else_statement)
                    : condition(std::move(condition))
                      , then_statement(std::move(then_statement))
                      , else_statement(std::move(else_statement))
            {}

            void accept(class StatementVisitor &visitor) override
            {
                visitor.visit(*this);
            }

            [[nodiscard]] const ExpressionNode &getCondition() const
            { return *condition; }

            [[nodiscard]] const StatementNode &getThenStatement() const
            { return *then_statement; }

            [[nodiscard]] const StatementNode &getElseStatement() const
            { return *else_statement; }

            [[nodiscard]] std::string toString() const override
            {
                std::ostringstream oss;

                oss << "IF " << condition->toString() << " THEN" << std::endl;
                oss << then_statement->toString() << std::endl;
                oss << "ELSE" << std::endl;
                oss << else_statement->toString();
                return oss.str();
            }
    };

    class WhileStatement : public StatementNode
    {
            std::unique_ptr<ExpressionNode> condition;
            std::unique_ptr<StatementNode> statement;

        public:
            WhileStatement(std::unique_ptr<ExpressionNode> condition,
                           std::unique_ptr<StatementNode> statement)
                    : condition(std::move(condition))
                      , statement(std::move(statement))
            {}

            void accept(class StatementVisitor &visitor) override
            {
                visitor.visit(*this);
            }

            [[nodiscard]] const ExpressionNode &getCondition() const
            { return *condition; }

            [[nodiscard]] const StatementNode &getStatement() const
            { return *statement; }

            [[nodiscard]] std::string toString() const override
            {
                std::ostringstream oss;

                oss << "WHILE " << condition->toString() << " DO" << std::endl;
                oss << statement->toString();
                return oss.str();
            }
    };

    class ReadStatement : public StatementNode
    {
            std::unique_ptr<ExpressionNode> expression;

        public:
            explicit ReadStatement(std::unique_ptr<ExpressionNode> expression)
                    : expression(std::move(expression))
            {}

            void accept(class StatementVisitor &visitor) override
            {
                visitor.visit(*this);
            }

            [[nodiscard]] const ExpressionNode &getExpression() const
            { return *expression; }

            [[nodiscard]] std::string toString() const override
            { return "READ " + expression->toString(); }
    };

    class WriteStatement : public StatementNode
    {
            std::unique_ptr<ExpressionNode> expression;

        public:
            explicit WriteStatement(std::unique_ptr<ExpressionNode> expression)
                    : expression(std::move(expression))
            {}

            void accept(class StatementVisitor &visitor) override
            {
                visitor.visit(*this);
            }

            [[nodiscard]] const ExpressionNode &getExpression() const
            { return *expression; }

            [[nodiscard]] std::string toString() const override
            { return "WRITE " + expression->toString(); }
    };
}

#endif //PL0_COMPILER_STATEMENTNODE_HPP
