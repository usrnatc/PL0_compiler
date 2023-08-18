//
// Created by user on 16-August-2023.
//

#ifndef PL0_COMPILER_PROCEDURENODE_HPP
#define PL0_COMPILER_PROCEDURENODE_HPP

#include "AST.hpp"
#include "Visitor.hpp"

namespace AST {

    class ProcedureNode : public ASTNode
    {
        public:
            ~ProcedureNode() override = default;
            virtual void accept(class ProcedureVisitor &visitor) = 0;

            void accept(class Visitor &visitor) override
            {
                auto procedure_visitor = dynamic_cast<ProcedureVisitor *>(&visitor);

                if (procedure_visitor) {
                    accept(*procedure_visitor);
                } else {
                    throw std::runtime_error("Invalid visitor");
                }
            }
    };

    class Procedure : public ProcedureNode
    {
            std::string name;
            std::vector<std::unique_ptr<StatementNode>> statements;
        public:
            Procedure(std::string name,
                      std::vector<std::unique_ptr<StatementNode>> statements)
                    : name(std::move(name))
                      , statements(std::move(statements))
            {}

            void accept(class ProcedureVisitor &visitor) override
            {
                visitor.visit(*this);
            }

            [[nodiscard]] const std::string &getName() const
            { return name; }

            [[nodiscard]] const std::vector<std::unique_ptr<StatementNode>> &
            getStatements() const
            { return statements; }

            [[nodiscard]] std::string toString() const override
            {
                std::ostringstream oss;
                oss << "procedure " << name << " {" << std::endl;
                for (const auto &statement: statements) {
                    oss << statement->toString() << std::endl;
                }
                oss << "}" << std::endl;
                return oss.str();
            }
    };
}

#endif //PL0_COMPILER_PROCEDURENODE_HPP
