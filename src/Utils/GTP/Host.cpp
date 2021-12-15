//
// Created by arthur wesley on 12/12/21.
//

#include "../../Include/Utils/GTP/Host.h"

#include <vector>
#include <set>

#include "../../Include/Utils/GTP/Parser.h"
#include "../../Include/JavaUtils.h"
#include "../../Include/Utils/GTP/Tokens/Seperator.h"
#include "../../Include/Utils/GTP/Tokens/Operator.h"

namespace sente::GTP {

    std::string Host::evaluate(std::string text) {

        auto tokens = parse(text);

        std::stringstream response;

        unsigned start = 0;

        // iterate through the tokens
        for (unsigned index = 0; index < tokens.size(); index++){

            // keep incrementing until we find a seperator
            while (not instanceof<Seperator>(tokens[index++].get())) {}

            auto arguments = std::vector<std::shared_ptr<Token>>(tokens.begin() + start, tokens.end() + index);

            // begin parsing by checking to see if the first element is an integer literal

            std::shared_ptr<Token> candidate;

            if (instanceof<Integer>(arguments[0].get())){
                candidate = arguments[1];
                // TODO: check for invalid command number
                arguments = std::vector<std::shared_ptr<Token>>(arguments.begin() + 1, arguments.end());
            }
            else {
                candidate = arguments[0];
            }

            if (instanceof<Operator>(&candidate)){
                // execute the command
                auto* command = (Operator*) candidate.get();

                switch (command->getName()){
                    case PROTOCOL_VERSION:
                        response << protocolVersion(arguments);
                        break;
                    case NAME:
                        response << name(arguments);
                        break;
                    case KNOWN_COMMAND:
                        response << knownCommand(arguments);
                        break;
                    case BOARD_SIZE:
                        response << boardSize(arguments);
                        break;
                    case VERSION:
                        // TODO: implement
                    default:
                        response << errorMessage("Unimplemented GTP command \"" + candidate->getText() + "\"");
                        continue;
                    case QUIT:
                        // exit the function
                        return response.str();
                }
            }
            else {
                response << errorMessage("Unknown GTP command \"" + candidate->getText() + "\"");
                continue;
            }

        }

        return response.str();
    }

    std::string Host::errorMessage(const std::string& message) const {
        return "?" + std::to_string(commandIndex) + " " + message + "\n\n";
    }

    std::string Host::statusMessage(const std::string &message) const {
        return "=" + std::to_string(commandIndex) + " " + message + "\n\n";
    }

    bool Host::argumentsMatch(const std::vector<std::pair<std::string, tokenType>> &expectedArguments,
                                const std::vector<std::shared_ptr<Token>> &arguments) {

        if (arguments.size() != expectedArguments.size()){
            return false;
        }

        for (unsigned i = 0; i < arguments.size(); i++){
            if (arguments[i]->getType() != expectedArguments[i].second){
                return false;
            }
        }

        return true;

    }

    std::string Host::invalidArgumentsErrorMessage(const std::unordered_set<std::vector<std::pair<std::string, tokenType>>>& argumentPatterns,
                                                   const std::vector<std::shared_ptr<Token>> &arguments) const {

        std::stringstream message;

        std::unordered_set<std::vector<std::pair<std::string, tokenType>>> candidates;

        // first, find all the candidate patterns (patters with the correct number of arguments)
        for (const auto& pattern : argumentPatterns){
            if (pattern.size() == arguments.size()){
                candidates.insert(pattern);
            }
        }

        if (candidates.empty()){
            // if there are no valid candidates, give an error based on the number of arguments
            message << "invalid number of arguments for command \"" << arguments[0]->getText() << "\"; expected ";

            std::set<unsigned> expectedArguments;

            for (const auto& pattern : argumentPatterns){
                expectedArguments.insert(pattern.size() - 1); // minus 1 because the first argument is the operator
            }

            message << *expectedArguments.begin();

            if (expectedArguments.size() > 2){
                for (auto argumentCount = ++expectedArguments.begin();
                     argumentCount != --expectedArguments.end(); argumentCount++){
                    message << ", " << *argumentCount;
                }
            }

            if (expectedArguments.size() > 1){
                message << "or " << *(--expectedArguments.end());
            }

            message << " arguments, got " << arguments.size();

        }
        else {

            message << "no viable argument pattern for command \"" << arguments[0]->getText() << "\"";

            for (const auto& candidate : candidates){
                // find the error
                for (unsigned i = 0; i < arguments.size(); i++){
                    if (arguments[i]->getType() != candidate[i].second){
                        message << std::endl << "candidate pattern not valid: expected " << toString(candidate[i].second)
                        << " in position " << i << ", got" << toString(arguments[i]->getType());
                    }
                }
            }

        }

        return errorMessage(message.str());

    }


    std::string Host::protocolVersion(const std::vector<std::shared_ptr<Token>>& arguments) {

        std::unordered_set<std::vector<std::pair<std::string, tokenType>>> argumentPatterns = {
                {{"command", OPERATOR}}
        };

        if (argumentsMatch(*argumentPatterns.begin(), arguments)){
            return statusMessage("2");
        }
        else {
            return invalidArgumentsErrorMessage(argumentPatterns, arguments);
        }

    }

    std::string Host::name(const std::vector<std::shared_ptr<Token>>& arguments) {
        std::unordered_set<std::vector<std::pair<std::string, tokenType>>> argumentPatterns = {
                {{"command", OPERATOR}}
        };

        if (argumentsMatch(*argumentPatterns.begin(), arguments)){
            return statusMessage(engineName);
        }
        else {
            return invalidArgumentsErrorMessage(argumentPatterns, arguments);
        }
    }

    std::string Host::listCommands(const std::vector<std::shared_ptr<Token>>& arguments) {

        std::unordered_set<std::vector<std::pair<std::string, tokenType>>> argumentPatterns = {
                {{"command", OPERATOR}}
        };

        if (argumentsMatch(*argumentPatterns.begin(), arguments)){
            std::stringstream response;

            // TODO: check to see if each item should be on a newline
            for (const auto& item : operators){
                response << item.first << std::endl;
            }

            return statusMessage(response.str());
        }
        else {
            return invalidArgumentsErrorMessage(argumentPatterns, arguments);
        }
    }

    std::string Host::knownCommand(const std::vector<std::shared_ptr<Token>>& arguments) {

        std::unordered_set<std::vector<std::pair<std::string, tokenType>>> argumentPatterns = {
                {{"command", OPERATOR}, {"command", OPERATOR}},
                {{"command", OPERATOR}, {"string", LITERAL_STRING}}
        };

        if (std::any_of(argumentPatterns.begin(), argumentPatterns.end(),this->argumentsMatch)){

            if (arguments[1]->getType() == OPERATOR){
                return statusMessage("true");
            }
            else {
                return statusMessage("false");
            }

        }
        else {
            return invalidArgumentsErrorMessage(argumentPatterns, arguments);
        }

    }

    std::string Host::boardSize(const std::vector<std::shared_ptr<Token>>& arguments) {

        std::unordered_set<std::vector<std::pair<std::string, tokenType>>> argumentPatterns = {
                {{"command", OPERATOR}, {"command", LITERAL_INTEGER}}
        };

        if (argumentsMatch(*argumentPatterns.begin(), arguments)){
            unsigned side = ((Integer*) arguments[1].get())->getValue();
            game = GoGame(side, game.getRules(), game.getKomi());
            return statusMessage("");
        }
        else {
            return invalidArgumentsErrorMessage(argumentPatterns, arguments);
        }
    }
}