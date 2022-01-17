//
// Created by arthur wesley on 7/5/21.
//

#ifndef SENTE_SENTEEXCEPTIONS_H
#define SENTE_SENTEEXCEPTIONS_H

#include <ciso646>
#include <exception>

#include "../Game/Move.h"

namespace sente::utils{

        enum IllegalMoveType {
            OFF_BOARD,
            OCCUPIED_POINT,
            WRONG_COLOR,
            SELF_CAPTURE,
            KO_POINT
        };

        class FileNotFoundException : public std::domain_error{
        public:
            explicit FileNotFoundException(const std::string &message) : std::domain_error(message){}
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

#endif //SENTE_SENTEEXCEPTIONS_H
