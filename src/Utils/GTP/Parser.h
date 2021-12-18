//
// Created by arthur wesley on 12/11/21.
//

#ifndef SENTE_PARSER_H
#define SENTE_PARSER_H

#include <string>
#include <vector>

#include "Tokens/Token.h"

namespace sente::GTP {

    std::string preprocess(const std::string& text);

    std::vector<std::shared_ptr<Token>> parse(const std::string& text);
    std::shared_ptr<Token> parseToken(const std::string& toParse);

}

#endif //SENTE_PARSER_H
