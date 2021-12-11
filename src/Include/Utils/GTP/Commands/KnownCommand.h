//
// Created by arthur wesley on 12/11/21.
//

#ifndef SENTE_KNOWNCOMMAND_H
#define SENTE_KNOWNCOMMAND_H

#include "../Lexer/Operator.h"

namespace sente::GTP {

    class KnownCommand : public Operator{
    public:

        KnownCommand(const String& command);

        std::string getResponse();

    };
}


#endif //SENTE_KNOWNCOMMAND_H
