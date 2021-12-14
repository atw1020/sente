//
// Created by arthur wesley on 12/11/21.
//

#include "../../../Include/Utils/GTP/Tokens/Token.h"

namespace sente::GTP {

    Token::Token(std::string text) {
        this->text = std::move(text);
    }

}