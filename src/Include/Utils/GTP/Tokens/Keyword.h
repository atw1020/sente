//
// Created by arthur wesley on 12/11/21.
//

#ifndef SENTE_KEYWORD_H
#define SENTE_KEYWORD_H

#include <map>

#include "Token.h"

namespace sente::GTP {

    class Keyword : public Token{
    public:

        Keyword(std::string);
        ~Keyword() override;

        tokenType getTokenType() const final;

    };
}


#endif //SENTE_KEYWORD_H
