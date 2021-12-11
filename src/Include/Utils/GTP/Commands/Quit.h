//
// Created by arthur wesley on 12/11/21.
//

#ifndef SENTE_QUIT_H
#define SENTE_QUIT_H

#include "../Lexer/Operator.h"

namespace sente::GTP {

    class Quit : public Operator{
    public:

        std::string getResponse();

    };
}


#endif //SENTE_QUIT_H
