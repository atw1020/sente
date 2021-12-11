//
// Created by arthur wesley on 12/11/21.
//

#ifndef SENTE_PROTOCOLVERSION_H
#define SENTE_PROTOCOLVERSION_H

#include "../../Lexer/Operator.h"

namespace sente::GTP {

    class ProtocolVersion : public Operator{

        std::string getResponse();

    };
}


#endif //SENTE_PROTOCOLVERSION_H
