//
// Created by user on 16-August-2023.
//

#ifndef PL0_COMPILER_VISITOR_HPP
#define PL0_COMPILER_VISITOR_HPP

#include "AST.hpp"
#include "StatementNode.hpp"
#include "ExpressionNode.hpp"

namespace AST {

    class Visitor
    {
        public:
            virtual ~Visitor() = default;
    };

    class ExpressionVisitor : public Visitor
    {
        public:
            ~ExpressionVisitor() override = default;
            virtual void visit(ExpressionNode &node) = 0;
    };

    class StatementVisitor : public Visitor
    {
        public:
            ~StatementVisitor() override = default;
            virtual void visit(StatementNode &node) = 0;
    };

    class ProcedureVisitor : public Visitor
    {
        public:
            ~ProcedureVisitor() override = default;
            virtual void visit(ProcedureNode &node) = 0;
    };
}

#endif //PL0_COMPILER_VISITOR_HPP
