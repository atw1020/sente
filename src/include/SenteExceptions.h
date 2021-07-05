//
// Created by arthur wesley on 7/5/21.
//

#ifndef SENTE_SENTEEXCEPTIONS_H
#define SENTE_SENTEEXCEPTIONS_H

#include <exception>

#include "Move.h"

namespace sente{
    namespace utils{

        enum IllegalMoveType {
            OFF_BOARD,
            OCCUPIED_POINT,
            SELF_CAPTURE,
            KO_POINT
        };

        class InvalidSGFException : public std::domain_error{
        public:
            explicit InvalidSGFException(const std::string &message);
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
}

#endif //SENTE_SENTEEXCEPTIONS_H
