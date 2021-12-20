//
// Created by arthur wesley on 12/11/21.
//

#ifndef SENTE_LITERAL_H
#define SENTE_LITERAL_H


#include "Token.h"
#include "../../../Game/Move.h"

namespace sente::GTP {

    enum literalType {
        INTEGER,
        VERTEX,
        STRING,
        COLOR,
        FLOAT,
        MOVE,
        BOOLEAN
    };

    class Literal : public Token{
    public:

        explicit Literal(const std::string& text);

        tokenType getTokenType() const final;
        virtual literalType getLiteralType() const = 0;
    };

    std::string toString(literalType type);

    class Integer final : public Literal {
    public:

        explicit Integer(const std::string& literal);
        ~Integer() final;

        unsigned getValue() const;

        literalType getLiteralType() const final;

    private:

        unsigned value;
    };

    class Vertex final : public Literal{
    public:

        explicit Vertex(const std::string& vertex);
        ~Vertex() final;

        unsigned getX() const;
        unsigned getY() const;

        literalType getLiteralType() const final;

    private:

        unsigned x;
        unsigned y;
    };

    class String final : public Literal {
    public:
        String(const std::string& value);
        ~String() final;

        literalType getLiteralType() const final;
    };

    enum GoColor {
        BLACK,
        WHITE
    };

    class Color final : public Literal {
    public:
        explicit Color(std::string text);
        ~Color() final;

        literalType getLiteralType() const final;
        GoColor getColor() const;

        static bool isColor(std::string text);

    private:
        GoColor color;
    };

    class Float final : public Literal {
    public:
        explicit Float(const std::string& text);
        ~Float() final;

        float getValue() const;

        literalType getLiteralType() const final;
    private:
        float value;
    };

    class Move final : public Literal {
    public:
        Move(Color color, Vertex vertex, unsigned boardSize);

        sente::Move getMove();

        literalType getLiteralType() const final;
    private:

        sente::Move move;

    };

}


#endif //SENTE_LITERAL_H
