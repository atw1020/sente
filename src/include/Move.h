//
// Created by arthur wesley on 6/27/21.
//

#ifndef SENTE_MOVE_H
#define SENTE_MOVE_H

#include <vector>
#include <string>
#include <sstream>
#include <exception>
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

    Stone getOpponent(Stone player);

    enum IllegalMoveType {
        OFF_BOARD,
        OCCUPIED_POINT,
        SELF_CAPTURE,
        KO_POINT
    };

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

    class IllegalMoveException : public std::exception{
    public:

        IllegalMoveException(IllegalMoveType type, const Move& move);

        const char* what() const noexcept override;

    private:

        IllegalMoveType type;
        Move move;

    };
}

namespace std {

    template<>
    struct hash<sente::Move> {
        size_t operator()(const sente::Move& move) const noexcept;
    };
}

#endif //SENTE_MOVE_H
