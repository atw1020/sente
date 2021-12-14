//
// Created by arthur wesley on 12/11/21.
//

#ifndef SENTE_OPERATOR_H
#define SENTE_OPERATOR_H

#include "Token.h"

namespace sente::GTP {

    class Operator : public Token {
    public:

        explicit Operator(std::string value) : Token(std::move(value)) {};

    };

}


#endif //SENTE_OPERATOR_H
