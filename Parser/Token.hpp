//
// Created by user on 16-August-2023.
//

#ifndef PL0_COMPILER_TOKEN_HPP
#define PL0_COMPILER_TOKEN_HPP

#include <string>

namespace Parser {

    enum class TokenType
    {
        CONST,
        VAR,
        PROCEDURE,
        CALL,
        BEGIN,
        END,
        IF,
        THEN,
        ELSE,
        WHILE,
        DO,
        READ,
        WRITE,
        ODD,
    };

    class Token
    {
            TokenType type;
            std::string value;
        public:
            Token(TokenType type, std::string value)
                    : type(type)
                      , value(std::move(value))
            {}
    };
}

#endif //PL0_COMPILER_TOKEN_HPP
