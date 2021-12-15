//
// Created by arthur wesley on 12/12/21.
//

#include "../../Include/Utils/GTP/Host.h"

#include <vector>

#include "../../Include/Utils/GTP/Parser.h"
#include "../../Include/JavaUtils.h"
#include "../../Include/Utils/GTP/Tokens/Seperator.h"
#include "../../Include/Utils/GTP/Tokens/Operator.h"
#include "../../Include/Utils/GTP/Tokens/Literal.h"

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
                arguments = std::vector<std::shared_ptr<Token>>(arguments.begin() + 2, arguments.end());
            }
            else {
                candidate = arguments[0];
                arguments = std::vector<std::shared_ptr<Token>>(arguments.begin() + 1, arguments.end());
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

    std::string Host::tooManyArgumentsError(const std::vector<std::shared_ptr<Token>> &arguments,
                                            unsigned expectedArguments) const {
        std::stringstream message;

        assert(arguments.size() > expectedArguments);

        message << "got " << arguments.size() - expectedArguments << " unexpected arguments";

        for (unsigned i = expectedArguments; i < arguments.size(); i++){
            message << std::endl << "unexpected argument: " << arguments[i]->getText();
        }

        return errorMessage(message.str());

    }

    std::string Host::notEnoughArgumentsError(const std::vector<std::shared_ptr<Token>> &arguments,
                                              const std::vector<std::string> argumentNames) const {

        std::stringstream message;

        assert(arguments.size() < argumentNames.size());

        message << "missing " << argumentNames.size() - arguments.size() << " arguments";

        for (unsigned i = arguments.size(); i < argumentNames.size(); i++){
            message << std::endl << "missing argument \"" << argumentNames[i] << "\"";
        }

        return errorMessage(message.str());
    }

    std::string Host::protocolVersion(const std::vector<std::shared_ptr<Token>>& arguments) {
        if (arguments.size() == 0){
            return statusMessage("2");
        }
        else {
            return errorMessage("unexpected arguments");
        }
    }

    std::string Host::name(const std::vector<std::shared_ptr<Token>>& arguments) {
        return "=Engine running with Sente";
    }

    std::string Host::listCommands(const std::vector<std::shared_ptr<Token>>& arguments) {
        std::stringstream response;

        // TODO: check to see if each item should be on a newline
        for (const auto& item : operators){
            response << item.first << std::endl;
        }

        return statusMessage(response.str());
    }

    std::string Host::knownCommand(const std::vector<std::shared_ptr<Token>>& arguments) {

        if (instanceof<String>(arg)){
            auto* command = (String*) arg;
            if (operators.find(arg->getText()) != operators.end()){
                return statusMessage("true");
            }
            else {
                return statusMessage("false");
            }
        }
        else {
            return statusMessage("false");
        }

    }

    std::string Host::boardSize(const std::vector<std::shared_ptr<Token>>& arguments) {
        if (instanceof<Integer>(token)){
            unsigned side = ((Integer*) token)->getValue();
            game = GoGame(side, game.getRules(), game.getKomi());
            return statusMessage("");
        }
        else {
            return errorMessage("unacceptable size");
        }
    }

}