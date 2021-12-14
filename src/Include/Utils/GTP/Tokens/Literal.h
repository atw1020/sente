//
// Created by arthur wesley on 12/11/21.
//

#ifndef SENTE_LITERAL_H
#define SENTE_LITERAL_H


#include "Token.h"

namespace sente::GTP {

    class Literal : public Token{
    public:
        explicit Literal(std::string text);
    };

    class Integer : public Literal {
    public:

        explicit Integer(const std::string& literal);

    private:

        unsigned value;

    };

    class Vertex : public Literal{
    public:

        explicit Vertex(const std::string& vertex);

        unsigned getX() const;
        unsigned getY() const;

    private:

        unsigned x;
        unsigned y;

    };

    class String : public Literal {
    public:
        explicit String(const std::string& text);
    };

}


#endif //SENTE_LITERAL_H
