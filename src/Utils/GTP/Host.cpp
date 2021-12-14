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
            }
            else {
                candidate = arguments[0];
            }

            if (instanceof<Operator>(&candidate)){
                // execute the command
                auto* command = (Operator*) candidate.get();

                switch (command->getName()){
                    case PROTOCOL_VERSION:
                        response << protocolVersion() << std::endl;
                        break;
                    case NAME:
                        response << name() << std::endl;
                        break;
                    case KNOWN_COMMANDS:
                        response << knownCommands() << std::endl;
                        break;
                    case QUIT:
                        // exit the function
                        return response.str();
                    case BOARD_SIZE:

                    case VERSION:
                        // TODO: implement
                    default:
                        response << errorMessage("Unimplemented GTP command \"" + candidate->getText() + "\"")
                                 << std::endl;
                        continue;
                }
            }
            else {
                response << errorMessage("Unknown GTP command \"" + candidate->getText() + "\"") << std::endl;
                continue;
            }

        }

        return response.str();
    }

    std::string Host::errorMessage(const std::string& message) const {
        return "?" + std::to_string(commandIndex) + message;
    }

    std::string Host::protocolVersion() {
        return "2";
    }

    std::string Host::name() {
        return "Engine running with Sente";
    }

    std::string Host::knownCommands() {
        std::stringstream response;

        for (const auto& item : operators){
            response << item.first << std::endl;
        }

        return response.str();
    }

}