//
// Created by arthur wesley on 12/11/21.
//

#ifndef SENTE_LITERAL_H
#define SENTE_LITERAL_H


#include "Token.h"

namespace sente::GTP {

    class Literal : public Token{

    };

    class Vertex : public Literal{
    public:

        Vertex(const std::string& vertex);

        unsigned getX() const;
        unsigned getY() const;

    private:

        unsigned x;
        unsigned y;

    };
}


#endif //SENTE_LITERAL_H
