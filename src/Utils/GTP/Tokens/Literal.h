//
// Created by arthur wesley on 12/11/21.
//

#ifndef SENTE_LITERAL_H
#define SENTE_LITERAL_H


#include "Token.h"
#include "../../../Game/Move.h"

namespace sente::GTP {

    enum LiteralType {
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
        virtual LiteralType getLiteralType() const = 0;
    };

    std::string toString(LiteralType type);

    class Integer final : public Literal {
    public:

        explicit Integer(const std::string& literal);
        ~Integer() final;

        unsigned getValue() const;

        LiteralType getLiteralType() const final;

    private:

        unsigned value;
    };

    class Vertex final : public Literal{
    public:

        explicit Vertex(const std::string& vertex);
        ~Vertex() final;

        unsigned getX() const;
        unsigned getY() const;

        sente::Vertex toVertex(unsigned side);

        LiteralType getLiteralType() const final;

    private:

        unsigned x;
        unsigned y;
    };

    class String final : public Literal {
    public:
        String(const std::string& value);
        ~String() final;

        LiteralType getLiteralType() const final;
    };

    enum GoColor {
        BLACK,
        WHITE
    };

    class Color final : public Literal {
    public:
        explicit Color(std::string text);
        ~Color() final;

        LiteralType getLiteralType() const final;
        Stone getStone() const;

        static bool isColor(std::string text);

    private:
        GoColor color;
    };

    class Float final : public Literal {
    public:
        explicit Float(const std::string& text);
        ~Float() final;

        float getValue() const;

        LiteralType getLiteralType() const final;
    private:
        float value;
    };

    class Move final : public Literal {
    public:
        Move(const Color& color, const Vertex& vertex);

        sente::Move getMove(unsigned side);

        LiteralType getLiteralType() const final;
    private:

        bool flipped;

        sente::Move move;

    };

    class Boolean final : public Literal {
    public:
        explicit Boolean(std::string text);

        bool getValue() const;

        LiteralType getLiteralType() const final;

    private:
        bool value;
    };

}


#endif //SENTE_LITERAL_H
