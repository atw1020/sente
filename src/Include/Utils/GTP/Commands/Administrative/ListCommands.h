//
// Created by arthur wesley on 12/11/21.
//

#ifndef SENTE_LISTCOMMANDS_H
#define SENTE_LISTCOMMANDS_H

#include "../../Lexer/Operator.h"

namespace sente::GTP {

    class ListCommands : public Operator {
    public:

        std::string getResponse();
    };
}


#endif //SENTE_LISTCOMMANDS_H
