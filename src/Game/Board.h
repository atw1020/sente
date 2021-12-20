//
// Created by arthur wesley on 6/28/21.
//

#ifndef SENTE_BOARD_H
#define SENTE_BOARD_H

#include <array>
#include <sstream>
#include <ciso646>
#include <iostream>

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include "Move.h"

#ifdef __CYGWIN__
#define WHITE_STONE " O "
#define BLACK_STONE " # "
#else
#define WHITE_STONE " ⚪"
#define BLACK_STONE " ⚫"
#endif

namespace py = pybind11;

namespace sente {

    class _board {
    public:

        virtual ~_board() {};

        virtual bool isOnBoard(const Move& move) const = 0;
        virtual bool isEmpty(const Move& move) const = 0;

        virtual void captureStone(const Move&) = 0;
        virtual void playStone(const Move& move) = 0;

        virtual unsigned getSide() const = 0;

        virtual Move getSpace(unsigned x, unsigned y) const = 0;
        virtual Move getSpace(Point point) const = 0;

        virtual Stone getStone(unsigned x, unsigned y) const = 0;
        virtual Stone getStone(Point point) const = 0;

        virtual explicit operator std::string() const = 0;

        bool lowerLeftOrigin;
        bool useASCII;

    };

    template<unsigned side>
    class Board : public _board{
    public:

        Board() = default;

        Board(const Board& other){
            for (unsigned i = 0; i < side; i++){
                for (unsigned j = 0; j < side; j++){
                    board[i][j] = other.board[i][j];
                }
            }
            useASCII = false;
            lowerLeftOrigin = false;
        }

        explicit Board(std::array<std::array<Stone, side>, side> stones){
            for (unsigned i = 0; i < side; i++){
                for (unsigned j = 0; j < side; j++){
                    board[i][j] = stones[i][j];
                }
            }
            useASCII = false;
            lowerLeftOrigin = false;
        }

        [[nodiscard]] bool isOnBoard(const Move& move) const override {

            bool xInRange = 0 <= move.getX() and move.getX() < side;
            bool yInRange = 0 <= move.getY() and move.getY() < side;

            return xInRange and yInRange;
        }
        [[nodiscard]] bool isEmpty(const Move& move) const override {
            return board[move.getX()][move.getY()] == EMPTY;
        }

        void playStone(const Move& move) override{
            board[move.getX()][move.getY()] = move.getStone();
        }

        void captureStone(const Move& move) override{
            board[move.getX()][move.getY()] = EMPTY;
        }

        [[nodiscard]] bool isStar(unsigned x, unsigned y) const;

        [[nodiscard]] unsigned getSide() const override{
            return side;
        }

        [[nodiscard]] Move getSpace(unsigned int x, unsigned int y) const override {
            if (not isOnBoard(Move(x, y, BLACK))){
                throw std::out_of_range("Move not on board");
            }
            return Move(x, y, board[x][y]);
        }
        [[nodiscard]] Move getSpace(Point point) const override {
            return getSpace(point.first, point.second);
        }

        [[nodiscard]] Stone getStone(unsigned x, unsigned y) const override {
            return board[x][y];
        }
        [[nodiscard]] Stone getStone(Point point) const override {
            return getStone(point.first, point.second);
        }

        bool operator ==(const Board<side>& other) const {

            for (unsigned i = 0; i < side; i++){
                for (unsigned j = 0; j < side; j++){
                    if (board[i][j] != other.board[i][j]){
                        return false;
                    }
                }
            }
            return true;
        }


        py::array_t<int8_t> numpy(){

            // allocate the buffer and object
            auto result = py::array_t<int8_t>(side * side);
            auto buffer = result.request(true);

            auto* ptr = (int8_t*) buffer.ptr;

            for (unsigned i = 0; i < side; i++){
                for (unsigned j = 0; j < side; j++){
                    unsigned offset = j * side + i;
                    switch (board[i][j]){
                        case BLACK:
                            ptr[offset] = 1;
                            break;
                        case WHITE:
                            ptr[offset] = -1;
                            break;
                        case EMPTY:
                            ptr[offset] = 0;
                            break;
                    }
                }
            }

            result.resize({side, side});

            return result;

        }

        explicit operator std::string() const override{

            std::stringstream accumulator;

            for (unsigned i = 0; i < side; i++){

                // accumulate the beginning of the row
                if (i + 1 < 10){
                    accumulator << " ";
                }

                accumulator << i + 1 << " ";

                for (unsigned j = 0; j < side; j++){

                    unsigned rowIndex = lowerLeftOrigin ? side - 1 - i : i;

                    switch(board[rowIndex][j]){
                        case BLACK:
                            accumulator << " ⚫";
                            break;
                        case WHITE:
                            accumulator << " ⚪";
                            break;
                        case EMPTY:
                            // check if we are on a star point
                            if (isStar(rowIndex, j)){
                                accumulator << " *";
                            }
                            else {
                                accumulator << " .";
                            }
                            if (j != side - 1){
                                accumulator << " ";
                            }
                    }
                }
                accumulator << std::endl;
            }

            accumulator << "  ";

            for (unsigned j = 0; j < side; j++){
                if ('A' + j < 'I'){
                    accumulator << "  " << char('A' + j);
                }
                else {
                    accumulator << "  " << char('B' + j);
                }
            }

            return accumulator.str();

        }

    private:
        std::array<std::array<Stone, side>, side> board;

    };

}

#endif //SENTE_BOARD_H
