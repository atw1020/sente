//
// Created by arthur wesley on 12/11/21.
//

#ifndef SENTE_OPERATOR_H
#define SENTE_OPERATOR_H

#include <vector>

#include "Literals.h"

namespace sente::GTP {

    class Operator : public Token {
    public:

        Operator();

        std::vector<Literal> getArguments();

        virtual std::string getResponse() = 0;

    protected:

        std::vector<Literal> arguments;

    };
}


#endif //SENTE_OPERATOR_H
