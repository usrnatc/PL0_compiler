//
// Created by user on 16-August-2023.
//

#ifndef LATTICE_TYPE_HPP
#define LATTICE_TYPE_HPP

namespace Symbol {

    class Type
    {
        public:
            virtual ~Type() = default;
    };

    class ScalarType : public Type
    {
        public:
            ~ScalarType() override = default;
    };

    class SubrangeType : public ScalarType
    {
        public:
            ~SubrangeType() override = default;
    };

    class ProductType : public Type
    {
        public:
            ~ProductType() override = default;
    };
}
#endif //LATTICE_TYPE_HPP
