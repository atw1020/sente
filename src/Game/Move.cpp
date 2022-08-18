//
// Created by arthur wesley on 6/27/21.
//

#include <iostream>

#include "pybind11/pybind11.h"

#include "Move.h"

namespace py = pybind11;

namespace sente {

//    Stone operator not(const Stone& stone){
//        switch(stone){
//            case EMPTY:
//                return EMPTY;
//            case BLACK:
//                return WHITE;
//            case WHITE:
//                return BLACK;
//        }
//    }

    Stone oppositeColor(const Stone& stone){
        switch(stone) {
            case EMPTY:
                return EMPTY;
            case BLACK:
                return WHITE;
            case WHITE:
                return BLACK;
        }
    }

    Vertex::Vertex(unsigned int x, unsigned int y) {
        this->x = x; this->y = y;
    }

    unsigned Vertex::getX() const {
        return x;
    }

    unsigned Vertex::getY() const {
        return y;
    }

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

    const Move Move::passBlack = Move(19, 19, BLACK);
    const Move Move::passWhite = Move(19, 19, WHITE);

    const Move Move::resignBlack = Move(19, -1, BLACK);
    const Move Move::resignWhite = Move(19, -1, WHITE);

    const Move Move::nullMove = Move();

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
            return {unsigned(sgf[3] - 'a'), unsigned(sgf[4] - 'a'), stone};
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

    Move::Move(Vertex vertex, Stone stone) {
        x = vertex.getX();
        y = vertex.getY();
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

    Vertex Move::getVertex() const {
        return {x, y};
    }

    std::vector<Vertex> Move::getAdjacentMoves(unsigned boardSize) const{

        std::vector<Vertex> adjacents;

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

    std::string Move::toSGF() const {

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
            str << ""; // resignation is not recorded as a move
        }
        else {
            str << '[' << char('a' + x) << char('a' + y) << ']';
        }

        return str.str();

    }

    Move::operator std::string() const {
        char first;

        if (stone == EMPTY){
            return "[Placeholder Move]";
        }

        // determine the letter
        if (x + 'A' < 'I'){
            first = 'A' + x;
        }
        else {
            first = 'B' + x;
        }

        // add the letter to the second co-ord
        std::string message = (stone == BLACK ? "B " : "W ") + std::to_string(y);
        message.insert(message.begin() + 2, first);

        return message;
    }

    void Move::flipOriginY(unsigned int side) {
        y = side - 1 - y;
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