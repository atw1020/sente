//
// Created by arthur wesley on 12/11/21.
//

#ifndef SENTE_TOKEN_H
#define SENTE_TOKEN_H

#include <string>


namespace sente::GTP {

    enum tokenType {
        SEPERATOR,
        LITERAL
    };

    std::string toString(tokenType type);

    class Token {
    public:

        explicit Token(std::string text);
        virtual ~Token();

        virtual std::string getText();

        virtual tokenType getTokenType() const = 0;

    protected:

        std::string text;

    };
}


#endif //SENTE_TOKEN_H
