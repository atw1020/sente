//
// Created by arthur wesley on 7/5/21.
//

#include "include/SenteExceptions.h"

namespace sente{

    namespace utils{

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
                case WRONG_COLOR:
                    message = "It is not currently " + std::string(move.getStone() == BLACK ? "black" : "white") + "'s turn\n";
                    break;
                case SELF_CAPTURE:
                    message = "The Desired move " + std::string(move) + " would result in a self-capture\n";
                    break;
                case KO_POINT:
                    message = "The Desired move " + std::string(move) + " lies on a Ko point\n";
            }

            return message.c_str();
        }

        InvalidSGFException::InvalidSGFException(const std::string &message) : std::domain_error(message){}
    }
}
