//
// Created by arthur wesley on 12/12/21.
//

#ifndef SENTE_HOST_H
#define SENTE_HOST_H

#include "Parser.h"

#include "../../Game/GoGame.h"

namespace sente::GTP {

    class Host {
    public:

        std::string evaluate(std::string text);

    private:

        unsigned commandIndex;

        GoGame game;

        std::string errorMessage(const std::string& message) const;

        static std::string protocolVersion();
        static std::string name();
        static std::string knownCommands();

    };
}


#endif //SENTE_HOST_H
