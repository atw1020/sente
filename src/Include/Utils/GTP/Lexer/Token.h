//
// Created by arthur wesley on 12/11/21.
//

#ifndef SENTE_TOKEN_H
#define SENTE_TOKEN_H

#include <string>

namespace sente::GTP {

    class Token {
    public:

        explicit Token(std::string);

        std::string getValue();

    protected:
        std::string value;
    };
}


#endif //SENTE_TOKEN_H
