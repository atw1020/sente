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

        virtual tokenType getType() const;
    };

    class Integer : public Literal {
    public:

        explicit Integer(const std::string& literal);

        unsigned getValue() const;

        tokenType getType() const;

    private:

        unsigned value;
    };

    class Vertex : public Literal{
    public:

        explicit Vertex(const std::string& vertex);

        unsigned getX() const;
        unsigned getY() const;

        tokenType getType() const;

    private:

        unsigned x;
        unsigned y;

    };

    class String : public Literal {
    public:
        explicit String(const std::string& text);
        tokenType getType() const;
    };

}


#endif //SENTE_LITERAL_H
