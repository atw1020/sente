//
// Created by arthur wesley on 6/27/21.
//

#ifndef SENTE_MOVE_H
#define SENTE_MOVE_H

#include <vector>
#include <string>
#include <sstream>
#include <ciso646>
#include <functional>
#include <pybind11/pytypes.h>

namespace py = pybind11;

namespace sente {

    enum Stone {
        EMPTY,
        BLACK,
        WHITE
    };

//     Stone operator not(const Stone& stone);

    Stone oppositeColor(const Stone& stone);

    // TODO add tests for Vertex
    class Vertex{
    public:

        Vertex(unsigned first, unsigned second);

        unsigned getX() const;
        unsigned getY() const;

    private:
        unsigned x;
        unsigned y;

    };

    Stone getOpponent(Stone player);

    class Move {
    public:

        Move();
        // Move(const Move& move) = delete;
        Move(unsigned x, unsigned y, Stone stone);
        Move(Stone stone, unsigned x, unsigned y);
        Move(Vertex vertex, Stone stone);

        const static Move passBlack;
        const static Move resignBlack;

        const static Move passWhite;
        const static Move resignWhite;

        const static Move nullMove;

        static Move fromSGF(std::string sgf);
        static Move pass(Stone player);
        static Move resign(Stone player);

        // TODO: see if making these fields public improves performance
        unsigned getX() const;
        unsigned getY() const;

        Stone getStone() const;

        bool isPass() const;
        bool isResign() const;

        bool operator==(const Move& other) const;
        bool operator!=(const Move& other) const;

        Vertex getVertex() const;
        std::vector<Vertex> getAdjacentMoves(unsigned boardSize) const;

        explicit operator std::string() const;
        std::string toSGF() const;

        void flipOriginY(unsigned side);

    private:

        unsigned x;
        unsigned y;

        Stone stone;

    };
}

namespace std {

    template<>
    struct hash<sente::Move> {
        size_t operator()(const sente::Move& move) const noexcept;
    };

    template<>
    struct hash<sente::Stone> {
        size_t operator()(const sente::Stone& stone) const noexcept;
    };
}

#endif //SENTE_MOVE_H
