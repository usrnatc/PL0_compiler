cmake_minimum_required(VERSION 3.25)
project(PL0_compiler)

set(CMAKE_CXX_STANDARD 20)

add_executable(
        PL0_compiler
        main.cpp
        AST/Visitor.hpp
        AST/AST.hpp
        AST/StatementNode.hpp
        AST/ExpressionNode.hpp
        AST/ProcedureNode.hpp
        Symbol/Type.hpp
        Parser/Token.hpp
        Symbol/Predefined.hpp
        Symbol/Scope.hpp
        Symbol/SymbolTable.hpp
        Symbol/SymbolEntry.hpp
        Parser/Location.hpp)
