//
// Created by arthur wesley on 12/11/21.
//

#ifndef SENTE_SEPERATOR_H
#define SENTE_SEPERATOR_H


#include "Token.h"

namespace sente::GTP {

    class Seperator : public Token{
    public:
        explicit Seperator(std::string text) : Token(std::move(text)) {};
    };

}


#endif //SENTE_SEPERATOR_H
