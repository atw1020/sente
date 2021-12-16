//
// Created by arthur wesley on 12/11/21.
//

#include "../../../Include/Utils/GTP/Tokens/Token.h"

namespace sente::GTP {

    std::string toString(tokenType type){
        switch (type){
            case OPERATOR:
                return "command";
            case KEYWORD:
                return "keyword";
            case SEPERATOR:
                return "seperator";
            case LITERAL:
                return "literal";
        }
    }

    Token::Token(std::string text) {
        this->text = std::move(text);
    }

    std::string Token::getText() {
        return text;
    }

}