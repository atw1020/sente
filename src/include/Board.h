//
// Created by arthur wesley on 6/28/21.
//

#ifndef SENTE_BOARD_H
#define SENTE_BOARD_H

#include <array>
#include <sstream>

#include "Move.h"
#include "Group.h"

namespace sente {

    class _board {
    public:
        virtual bool isOnBoard(const Move& move) const = 0;
        virtual bool isEmpty(const Move& move) const = 0;

        virtual void captureStone(const Move&) = 0;
        virtual void playStone(const Move& move) = 0;

        virtual unsigned getSide() const = 0;

        virtual Move getSpace(unsigned x, unsigned y) const = 0;

        virtual explicit operator std::string() const = 0;

    };

    template<unsigned side>
    class Board : public _board{
    public:

        Board() = default;

        explicit Board(std::array<std::array<Stone, side>, side> stones){
            for (unsigned i = 0; i < side; i++){
                for (unsigned j = 0; j < side; j++){
                    board[i][j] = Move(i, j, stones[i][j]);
                }
            }
        }

        bool isOnBoard(const Move& move) const override {

            bool xInRange = 0 <= move.getX() and move.getX() < side;
            bool yInRange = 0 <= move.getY() and move.getY() < side;

            return xInRange and yInRange;
        }
        bool isEmpty(const Move& move) const override {
            return board[move.getX()][move.getY()].getStone() == EMPTY;
        }

        void playStone(const Move& move) override{
            board[move.getX()][move.getY()] = move;
        }

        void captureStone(const Move& move) override{
            board[move.getX()][move.getY()] = Move(move.getX(), move.getY(), EMPTY);
        }

        bool isStar(unsigned x, unsigned y) const;

        unsigned getSide() const override{
            return side;
        }

        Move getSpace(unsigned int x, unsigned int y) const override {
            return board[x][y];
        }

        bool operator==(const Board<side>& other) const{
            return board == other.board;
        }

        explicit operator std::string() const override{

            std::stringstream accumulator;

            for (unsigned i = 0; i < side; i++){

                // accumulate the beginning of the row
                if (i + 1 < 10){
                    accumulator << " ";
                }

                accumulator << i + 1;

                for (unsigned j = 0; j < side; j++){

                    accumulator << "  ";

                    switch(board[i][j].getStone()){
                        case BLACK:
                            accumulator << "#";
                            break;
                        case WHITE:
                            accumulator << "O";
                            break;
                        case EMPTY:
                            // check if we are on a star point
                            if (isStar(i, j)){
                                accumulator << "*";
                            }
                            else {
                                accumulator << ".";
                            }
                    }
                }
                accumulator << std::endl;
            }

            accumulator << "  ";

            for (unsigned j = 0; j < side; j++){
                accumulator << "  " << char('A' + j);
            }

            accumulator << std::endl;

            return accumulator.str();

        }

    private:

        std::array<std::array<Move, side>, side> board;

    };

}



#endif //SENTE_BOARD_H
