//
// Created by arthur wesley on 12/11/21.
//

#ifndef SENTE_TOKEN_H
#define SENTE_TOKEN_H

#include <string>


namespace sente::GTP {

    class Token {
    public:

        explicit Token(std::string text);

        std::string getText();

    protected:

        std::string text;

    };
}


#endif //SENTE_TOKEN_H
