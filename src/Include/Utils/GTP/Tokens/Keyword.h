//
// Created by arthur wesley on 12/11/21.
//

#ifndef SENTE_KEYWORD_H
#define SENTE_KEYWORD_H

#include <map>

#include "Token.h"

namespace sente::GTP {

    enum KeywordName {
        BLACK,
        WHITE
    };

    std::map<std::string, KeywordName> keywords{
            {"black", BLACK},
            {"white", WHITE},
            {"b", BLACK},
            {"w", WHITE}
    };

    class Keyword : public Token{
    public:

        Keyword(std::string)

    };
}


#endif //SENTE_KEYWORD_H
