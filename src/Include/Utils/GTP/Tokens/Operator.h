//
// Created by arthur wesley on 12/11/21.
//

#ifndef SENTE_OPERATOR_H
#define SENTE_OPERATOR_H

#include <map>

#include "Token.h"

#define OPERATORS std::map<std::string, OperatorName> { \
    {"protocol_version", PROTOCOL_VERSION}, \
    {"name", NAME}, \
    {"version", VERSION}, \
    {"known_command", KNOWN_COMMAND}, \
    {"list_commands", LIST_COMMANDS}, \
    {"quit", QUIT}, \
    {"boardsize", BOARD_SIZE}, \
    {"clear_board", CLEAR_BOARD}, \
    {"komi", KOMI}, \
    {"play", PLAY}, \
    {"genmove", GEN_MOVE} \
}

namespace sente::GTP {

    enum OperatorName {
        PROTOCOL_VERSION,
        NAME,
        VERSION,
        KNOWN_COMMAND,
        LIST_COMMANDS,
        QUIT,
        BOARD_SIZE,
        CLEAR_BOARD,
        KOMI,
        PLAY,
        GEN_MOVE
    };

    class Operator : public Token {
    public:

        explicit Operator(const std::string&);
        ~Operator() override;

        OperatorName getName() const;

        tokenType getTokenType() const final;

    private:

        OperatorName name;

    };

}


#endif //SENTE_OPERATOR_H
