//
// Created by arthur wesley on 6/27/21.
//

#include <iostream>

#include "pybind11/pybind11.h"

#include "include/Move.h"

namespace py = pybind11;

namespace sente {


    Stone getOpponent(Stone player){
        if (player == BLACK){
            return WHITE;
        }
        else if (player == WHITE){
            return BLACK;
        }
        else {
            return EMPTY;
        }
    }

    /**
     *
     * default constructor (creates the base Ko point
     *
     */
    Move::Move() {
        x = 0;
        y = 0;
        stone = EMPTY;
    }

    Move::Move(Stone stone, MoveType type){
        if (type == PASS){
            // return a pass move for the player (move at unsigned -1, -1)
            x = -1;
            y = -1;
            this->stone = stone;
        }
        else {
            // return a resign move for the player (move at unsigned -2, -2)
            x = -2;
            y = -2;
            this->stone = stone;
        }
    }

    Move::Move(std::string sgf){

        // check to see if the brackets are empty
        if (sgf[3] == ']'){
            // instantiate a pass move
            x = -1;
            y = -1;
            if (sgf[1] == 'B'){
                stone = BLACK;
            }
            else {
                stone = WHITE;
            }
        }
        else {
            x = unsigned(sgf[3] - 'a');
            y = unsigned(sgf[4] - 'a');
            if (sgf[1] == 'B'){
                stone = BLACK;
            }
            else {
                stone = WHITE;
            }
        }

    }

    /**
     *
     * Particular move constructor
     *
     * @param x
     * @param y
     * @param stone
     */
    Move::Move(unsigned int x, unsigned int y, Stone stone) {
        this->x = x;
        this->y = y;
        this->stone = stone;
    }

    unsigned Move::getX() const{
        return x;
    }

    unsigned Move::getY() const{
        return y;
    }

    Stone Move::getStone() const{
        return stone;
    }

    bool Move::operator==(const Move& other) const{
        return other.x == x and other.y == y and other.stone == stone;
    }

    bool Move::operator!=(const Move& other) const{
        return other.x != x or other.y != y or other.stone != stone;
    }

    std::vector<std::pair<int, int>> Move::getAdjacentMoves(unsigned boardSize) const{

        std::vector<std::pair<int, int>> adjacents;

        if (x + 1 < boardSize){
            adjacents.emplace_back(x + 1, y);
        }
        if (x > 0){
            adjacents.emplace_back(x - 1, y);
        }
        if (y + 1 < boardSize){
            adjacents.emplace_back(x, y + 1);
        }
        if (y > 0){
            adjacents.emplace_back(x, y - 1);
        }

        return adjacents;
    }

    Move::operator std::string() const {

        std::stringstream str;

        switch (stone){
            case BLACK:
                str << "B";
                break;
            case WHITE:
                str << "W";
                break;
            default:
                return "[Placeholder Move]";
        }

        str << '[' << char('a' + x) << char('a' + y) << ']';
        return str.str();

    }

    IllegalMoveException::IllegalMoveException(IllegalMoveType type, const Move& move) {
        this->type = type;
        this->move = move;
    }

    const char* IllegalMoveException::what() const noexcept {

        std::string message;

        switch (type){
            case OFF_BOARD:
                message = "The Desired move " + std::string(move) + " is beyond the edge of the go board (check your board size)\n";
                break;
            case OCCUPIED_POINT:
                message = "The Desired move " + std::string(move) + " lies on an occupied point\n";
                break;
            case SELF_CAPTURE:
                message = "The Desired move " + std::string(move) + " would result in a self-capture\n";
                break;
            case KO_POINT:
                message = "The Desired move " + std::string(move) + " lies on a Ko point\n";
        }

        return message.c_str();
    }
}

namespace std {

    size_t hash<sente::Move>::operator()(const sente::Move &move) const noexcept {

        size_t hash1 = hash<unsigned>()(move.getX());
        size_t hash2 = hash<unsigned>()(move.getX());
        size_t hash3 = hash<unsigned>()(move.getStone());

        return ((hash1 xor (hash2 << 1)) >> 1) xor (hash3 << 1);
    }
}