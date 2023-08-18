//
// Created by user on 16-August-2023.
//

#ifndef LATTICE_AST_HPP
#define LATTICE_AST_HPP

#include <string>

namespace AST {

    class ASTNode
    {
        public:
            virtual ~ASTNode() = default;
            virtual void accept(class Visitor &visitor) = 0;
            [[nodiscard]] virtual std::string toString() const = 0;
    };

}

#endif //LATTICE_AST_HPP
