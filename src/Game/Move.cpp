//
// Created by arthur wesley on 6/27/21.
//

#include <iostream>

#include "pybind11/pybind11.h"

#include "../Include/Game/Move.h"

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

    const Move Move::passBlack = Move(-1, 19, BLACK);
    const Move Move::passWhite = Move(-1, 19, WHITE);

    const Move Move::resignBlack = Move(19, -1, BLACK);
    const Move Move::resignWhite = Move(19, -1, WHITE);

    Move Move::fromSGF(std::string sgf){

        Stone stone = sgf[1] == 'B' ? BLACK : WHITE;

        // check to see if the brackets are empty
        if (sgf[3] == ']'){
            if (stone == BLACK){
                return passBlack;
            }
            else {
                return passWhite;
            }
        }
        else {
            // reverse the order of the co-ordinates
            return {unsigned(sgf[4] - 'a'), unsigned(sgf[3] - 'a'), stone};
        }

    }

    Move Move::pass(Stone player) {
        return player == BLACK ? passBlack : passWhite;
    }

    Move Move::resign(Stone player){
        return player == BLACK ? resignBlack : resignWhite;
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

    bool Move::isPass() const {
        return *this == passBlack or *this == passWhite;
    }

    bool Move::isResign() const {
        return *this == resignBlack or *this == resignWhite;
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

        if (isPass()){
            str << "[]";
        }
        else if (isResign()){

        }
        else {
            str << '[' << char('a' + y) << char('a' + x) << ']';
        }

        return str.str();

    }
}

namespace std {

    size_t hash<sente::Move>::operator()(const sente::Move &move) const noexcept {

        size_t hash1 = hash<unsigned>()(move.getX());
        size_t hash2 = hash<unsigned>()(move.getX());
        size_t hash3 = hash<unsigned>()(move.getStone());

        return ((hash1 xor (hash2 << 1)) >> 1) xor (hash3 << 1);
    }

    size_t std::hash<sente::Stone>::operator()(const sente::Stone &stone) const noexcept {
        return size_t(stone);
    }
}