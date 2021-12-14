//
// Created by arthur wesley on 12/11/21.
//

#ifndef SENTE_PARSER_H
#define SENTE_PARSER_H

#include <map>
#include <string>
#include <vector>

#include "Tokens/Token.h"

namespace sente::GTP {

    enum OperatorName {
        PROTOCOL_VERSION,
        NAME,
        VERSION,
        KNOWN_COMMANDS,
        QUIT,
        BOARD_SIZE,
        CLEAR_BOARD,
        KOMI,
        PLAY,
        GEN_MOVE
    };

    enum KeywordName {
        BLACK,
        WHITE
    };

    std::map<std::string, OperatorName> operators{
            {"protocol_version", PROTOCOL_VERSION},
            {"name", NAME},
            {"version", VERSION},
            {"known_commands", KNOWN_COMMANDS},
            {"quit", QUIT},
            {"boardsize", BOARD_SIZE},
            {"clear_board", CLEAR_BOARD},
            {"komi", KOMI},
            {"play", PLAY},
            {"genmove", GEN_MOVE}
    };
    std::map<std::string, KeywordName> keywords{
            {"black", BLACK},
            {"white", WHITE},
            {"b", BLACK},
            {"w", WHITE}
    };

    std::string preprocess(const std::string& text);

    std::vector<std::unique_ptr<Token>> parse(const std::string& text);
    std::unique_ptr<Token> parseToken(const std::string& toParse);

}

#endif //SENTE_PARSER_H
