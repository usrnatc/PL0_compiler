//
// Created by user on 16-August-2023.
//

#ifndef PL0_COMPILER_SYMBOLTABLE_HPP
#define PL0_COMPILER_SYMBOLTABLE_HPP

#include "Scope.hpp"
#include "SymbolEntry.hpp"
#include <memory>
#include <stdexcept>

namespace Symbol {

    class SymbolTable
    {
            std::shared_ptr<Scope> currentScope;

        public:
            SymbolTable()
            {
            }
    };
}

#endif //PL0_COMPILER_SYMBOLTABLE_HPP
