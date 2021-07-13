//
// Created by arthur wesley on 6/27/21.
//

#ifndef SENTE_MOVE_H
#define SENTE_MOVE_H

#include <vector>
#include <string>
#include <sstream>
#include <functional>
#include <pybind11/pytypes.h>

namespace py = pybind11;

namespace sente {

    enum Stone {
        BLACK,
        WHITE,
        EMPTY
    };

    enum MoveType {
        PASS,
        RESIGN
    };

    typedef std::pair<unsigned, unsigned> Point;

    Stone getOpponent(Stone player);

    class Move {
    public:

        Move();
        // Move(const Move& move);
        Move(Stone stone, MoveType move);
        explicit Move(std::string sgf);
        Move(unsigned x, unsigned y, Stone stone);

        // TODO: see if making these fields public improves performance
        unsigned getX() const;
        unsigned getY() const;

        Stone getStone() const;

        bool operator==(const Move& other) const;
        bool operator!=(const Move& other) const;

        std::vector<std::pair<int, int>> getAdjacentMoves(unsigned boardSize) const;

        explicit operator std::string() const;

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
