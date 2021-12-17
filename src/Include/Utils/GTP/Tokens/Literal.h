//
// Created by arthur wesley on 12/11/21.
//

#ifndef SENTE_LITERAL_H
#define SENTE_LITERAL_H


#include "Token.h"

namespace sente::GTP {

    enum literalType {
        INTEGER,
        VERTEX,
        STRING,
        COLOR,
        FLOAT,
        BOOLEAN
    };

    class Literal : public Token{
    public:

        explicit Literal(const std::string& text);

        virtual tokenType getTokenType() const override final;
        virtual literalType getLiteralType() const;
    };

    std::string toString(literalType type);

    class Integer : public Literal {
    public:

        explicit Integer(const std::string& literal);

        unsigned getValue() const;

        literalType getLiteralType() const override;

    private:

        unsigned value;
    };

    class Vertex : public Literal{
    public:

        explicit Vertex(const std::string& vertex);

        unsigned getX() const;
        unsigned getY() const;

        literalType getLiteralType() const override;

    private:

        unsigned x;
        unsigned y;
    };

    class String : public Literal {
    public:
        String(const std::string& value);

        literalType getLiteralType() const;
    };

    enum GoColor {
        BLACK,
        WHITE
    };

    class Color : public Literal {
    public:
        explicit Color(std::string text);
        literalType getLiteralType() const;
        GoColor getColor() const;
    private:
        GoColor color;
    };

    class Float : public Literal {
    public:
        explicit Float(const std::string& text);

        float getValue() const;

        literalType getLiteralType() const;
    private:
        float value;
    };

}


#endif //SENTE_LITERAL_H
