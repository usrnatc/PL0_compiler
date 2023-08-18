//
// Created by user on 16-August-2023.
//

#ifndef PL0_COMPILER_SYMBOLENTRY_HPP
#define PL0_COMPILER_SYMBOLENTRY_HPP

#include "Type.hpp"
#include "Scope.hpp"
#include "../AST/ExpressionNode.hpp"
#include "../Parser/Location.hpp"
#include <string>
#include <memory>

namespace Symbol {

    class SymbolEntry
    {
        protected:
            std::string name;
            std::unique_ptr<Scope> scope;
            std::unique_ptr<Type> type;
            bool resolved;

            virtual std::string
            toString(const std::string &kind, const std::string &sep)
            {
                return kind + " " + name + sep + type->toString() +
                       (scope == nullptr ? "" : " level " + std::to_string(
                               scope->getLevel()));
            }

        public:
            SymbolEntry(std::string name, std::unique_ptr<Type> type,
                        bool resolved)
                    : name(std::move(name))
                      , type(std::move(type))
                      , resolved(resolved)
            {}

            virtual ~SymbolEntry() = default;

            [[nodiscard]] const std::string &getName() const
            {
                return name;
            }

            [[nodiscard]] const Type &getType()
            {
                if (!resolved)
                    resolve();

                return *type;
            }

            void setType(std::unique_ptr<Type> t)
            {
                this->type = std::move(t);
            }

            virtual void resolve()
            {
                if (!resolved) {
                    type = type.resolveType();
                    resolved = true;
                }
            }
    };

    class ConstantEntry : public SymbolEntry
    {
            int value;
            std::unique_ptr<AST::ConstantExpression> tree;


            enum Status
            {
                Unresolved, Resolving, Resolved
            };

        protected:
            Status status;

        public:
            ConstantEntry(std::string name, std::unique_ptr<Type> t,
                          int val)
                    : SymbolEntry(std::move(name), std::move(t), true)
                      , value(val)
            {
                status = Resolved;
            }

            ConstantEntry(std::string name, std::unique_ptr<Type> t,
                          const AST::ConstantExpression &tree)
                    : SymbolEntry(std::move(name), std::move(t), false)
            {
                this->tree = std::make_unique<AST::ConstantExpression>(tree);
                value = 0x80808080; // garbage value
                status = Unresolved;
            }

            void resolve() override
            {
                switch (status) {
                    case Unresolved:
                        status = Resolving;
                        value = tree->getValue();
                        type = tree->getType();
                        status = Resolved;
                        resolved = true;
                        break;
                    case Resolving:
                        // TODO: circular reference error
                        status = Resolved;
                        resolved = true;
                        break;
                    case Resolved:
                        break;
                }
            }

            [[nodiscard]] int getValue()
            {
                if (!resolved)
                    resolve();

                return value;
            }

            std::string toString()
            {
                return SymbolEntry::toString("CONST", ":") + " = " +
                       std::to_string(value);
            }
    };

    class ProcedureEntry : public SymbolEntry
    {
    };
}

#endif //PL0_COMPILER_SYMBOLENTRY_HPP
