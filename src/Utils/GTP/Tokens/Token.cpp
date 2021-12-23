//
// Created by arthur wesley on 12/11/21.
//

#include "Token.h"

namespace sente::GTP {

    std::string toString(tokenType type){
        switch (type){
            case SEPERATOR:
                return "seperator";
            case LITERAL:
                return "literal";
        }
    }

    Token::Token(std::string text) {
        this->text = std::move(text);
    }

    std::string Token::getText() const {
        return text;
    }

    Token::~Token() {}

}