//
// Created by arthur wesley on 12/11/21.
//

#ifndef SENTE_NAME_H
#define SENTE_NAME_H

#include "../Lexer/Operator.h"

namespace sente::GTP {

    class Name : public Operator {
    public:

        std::string getResponse();

    };
}


#endif //SENTE_NAME_H
