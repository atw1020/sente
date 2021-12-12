//
// Created by arthur wesley on 12/11/21.
//

#ifndef SENTE_PARSER_H
#define SENTE_PARSER_H

#include <set>
#include <string>
#include <vector>

#include "Tokens/Token.h"

namespace sente::GTP {

    std::set<std::string> operators{
        "",
    };
    std::set<std::string> keywords{
        "black",
        "white",
        "b",
        "w"
    };

    std::string preprocess(const std::string& text);

    std::vector<std::unique_ptr<Token>> parse(const std::string& text);
    std::unique_ptr<Token> parseToken(const std::string& toParse);

}

#endif //SENTE_PARSER_H
