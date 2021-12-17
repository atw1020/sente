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

    typedef std::pair<bool, std::string> Response;
    typedef std::pair<std::string, std::variant<literalType, tokenType>> ArgumentPattern;

    class Host {
    public:

        Host(const std::string& engineName = "Engine using Sente GTP");

        std::string evaluate(const std::string& text);

    private:

        GoGame game;

        std::string engineName;

        std::string errorMessage(const std::string& message) const;
        std::string errorMessage(const std::string& message, unsigned i) const;
        std::string statusMessage(const std::string& message) const;
        std::string statusMessage(const std::string& message, unsigned i) const;

        static bool argumentsMatch(const std::vector<ArgumentPattern>& expectedTypes,
                                   const std::vector<std::shared_ptr<Token>>& arguments);
        static Response invalidArgumentsErrorMessage(const std::vector<std::vector<ArgumentPattern>>& argumentPatterns,
                                                 const std::vector<std::shared_ptr<Token>>& arguments) ;

        static Response protocolVersion(const std::vector<std::shared_ptr<Token>>& arguments);
        Response name(const std::vector<std::shared_ptr<Token>>& arguments);
        static Response knownCommand(const std::vector<std::shared_ptr<Token>>& arguments);
        static Response listCommands(const std::vector<std::shared_ptr<Token>>& arguments);
        Response boardSize(const std::vector<std::shared_ptr<Token>>& arguments);
        Response clearBoard(const std::vector<std::shared_ptr<Token>>& arguments);
        Response komi(const std::vector<std::shared_ptr<Token>>& arguments);
        Response play(const std::vector<std::shared_ptr<Token>>& arguments);

    };
}


#endif //SENTE_HOST_H
