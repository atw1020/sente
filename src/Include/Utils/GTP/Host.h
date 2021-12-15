//
// Created by arthur wesley on 12/12/21.
//

#ifndef SENTE_HOST_H
#define SENTE_HOST_H

#include "Parser.h"

#include <string>

#include "../../Game/GoGame.h"

namespace sente::GTP {

    class Host {
    public:

        std::string evaluate(std::string text);

    private:

        unsigned commandIndex;

        GoGame game;

        std::string errorMessage(const std::string& message) const;
        std::string statusMessage(const std::string& message) const;

        std::string tooManyArgumentsError(const std::vector<std::shared_ptr<Token>>& arguments,
                                          unsigned expectedArguments) const;
        std::string notEnoughArgumentsError(const std::vector<std::shared_ptr<Token>>& arguments,
                                            const std::vector<std::string> argumentNames) const;

        std::string protocolVersion(const std::vector<std::shared_ptr<Token>>& arguments);
        std::string name(const std::vector<std::shared_ptr<Token>>& arguments);
        std::string knownCommand(const std::vector<std::shared_ptr<Token>>& arguments);
        std::string listCommands(const std::vector<std::shared_ptr<Token>>& arguments);
        std::string boardSize(const std::vector<std::shared_ptr<Token>>& arguments);

    };
}


#endif //SENTE_HOST_H
