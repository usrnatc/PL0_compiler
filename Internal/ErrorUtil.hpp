//
// Created by user on 18-August-2023.
//

#ifndef PL0_COMPILER_ERRORUTIL_HPP
#define PL0_COMPILER_ERRORUTIL_HPP

#include <stdexcept>
#include <string>

namespace Internal {

    class not_implemented : public std::logic_error
    {
        public:
            explicit not_implemented(const std::string &msg)
                    : std::logic_error(msg + " not implemented")
            {}
    };
}

#endif //PL0_COMPILER_ERRORUTIL_HPP
