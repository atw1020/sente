//
// Created by arthur wesley on 12/12/21.
//

#ifndef SENTE_HOST_H
#define SENTE_HOST_H

#include "Parser.h"

#include <string>
#include <variant>

#include "../../Game/GoGame.h"

#include "Tokens/Literal.h"

namespace sente::GTP {

    class Host {
    public:

        Host();

        std::string evaluate(std::string text);

    private:

        GoGame game;

        unsigned commandIndex;

        std::string engineName = "Engine using Sente GTP";

        std::string errorMessage(const std::string& message) const;
        std::string statusMessage(const std::string& message) const;

        static bool argumentsMatch(const std::vector<std::pair<std::string, std::variant<literalType, tokenType>>>& expectedTypes,
                                   const std::vector<std::shared_ptr<Token>>& arguments);
        std::string invalidArgumentsErrorMessage(const std::unordered_set<std::vector<std::pair<std::string, std::variant<literalType, tokenType>>>>& argumentPatterns,
                                                 const std::vector<std::shared_ptr<Token>>& arguments) const;

        std::string protocolVersion(const std::vector<std::shared_ptr<Token>>& arguments);
        std::string name(const std::vector<std::shared_ptr<Token>>& arguments);
        std::string knownCommand(const std::vector<std::shared_ptr<Token>>& arguments);
        std::string listCommands(const std::vector<std::shared_ptr<Token>>& arguments);
        std::string boardSize(const std::vector<std::shared_ptr<Token>>& arguments);
        std::string resetBoard(const std::vector<std::shared_ptr<Token>>& arguments);

    };
}


#endif //SENTE_HOST_H
