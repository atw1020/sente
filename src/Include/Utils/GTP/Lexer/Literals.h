//
// Created by arthur wesley on 12/11/21.
//

#ifndef SENTE_LITERALS_H
#define SENTE_LITERALS_H

#include "Token.h"

namespace sente::GTP {

    enum Color {
        BLACK,
        WHITE
    };

    class Literal : public Token{

    };

    class StoneColor : public Literal {
    public:

    private:
        Color color;
    };

    class Vertex : public Literal {
    public:

        Vertex(std::string value);

    private:

        unsigned x;
        unsigned y;

    };

    class String: public Literal {
    public:

    };
}


#endif //SENTE_LITERALS_H
