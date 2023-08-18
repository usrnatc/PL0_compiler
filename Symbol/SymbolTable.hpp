//
// Created by user on 16-August-2023.
//

#ifndef LATTICE_SYMBOLTABLE_HPP
#define LATTICE_SYMBOLTABLE_HPP

#include "Scope.hpp"
#include "SymbolEntry.hpp"
#include <memory>

namespace Symbol {

    class SymbolTable
    {
            std::shared_ptr<Scope> currentScope;

        public:
            SymbolTable()
            {
                ProcedureEntry predefined = ProcedureEntry("<predefined>", nullptr);
            }
    };
}

#endif //LATTICE_SYMBOLTABLE_HPP
