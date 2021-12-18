//
// Created by arthur wesley on 12/12/21.
//

#ifndef SENTE_ENGINE_H
#define SENTE_ENGINE_H

#include "Parser.h"

#include <string>
#include <variant>

#include "../../Game/GoGame.h"

#include "Tokens/Literal.h"

namespace sente::GTP {

    struct Engine;

    typedef std::pair<bool, std::string> Response;
    typedef std::pair<std::string, std::variant<literalType, tokenType>> ArgumentPattern;
    typedef Response (*CommandMethod)(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments);

    struct Engine {

        Engine(const std::string& engineName, const std::string& engineVersion);

        GoGame game;
        std::string engineName;
        std::string engineVersion;

        std::string evaluate(const std::string& text);

        void registerCommand(const std::string& commandName, CommandMethod method,
                             std::vector<ArgumentPattern> argumentPattern);

        std::unordered_map<std::string, std::vector<std::pair<CommandMethod, std::vector<ArgumentPattern>>>> commands;

        Response evaluateCommand(const std::string& command, const std::vector<std::shared_ptr<Token>>& arguments);

        std::string errorMessage(const std::string& message) const;
        std::string errorMessage(const std::string& message, unsigned i) const;
        std::string statusMessage(const std::string& message) const;
        std::string statusMessage(const std::string& message, unsigned i) const;

        static bool argumentsMatch(const std::vector<ArgumentPattern>& expectedTypes,
                                   const std::vector<std::shared_ptr<Token>>& arguments);
        static Response invalidArgumentsErrorMessage(const std::vector<std::vector<ArgumentPattern>>& argumentPatterns,
                                                 const std::vector<std::shared_ptr<Token>>& arguments);

    };
}


#endif //SENTE_ENGINE_H
