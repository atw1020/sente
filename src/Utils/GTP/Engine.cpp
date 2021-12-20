//
// Created by arthur wesley on 12/12/21.
//

#include "Engine.h"

#include <set>
#include <utility>
#include <vector>
#include <iostream>

#include "Operators.h"

namespace sente::GTP {

    Engine::Engine(const std::string& engineName, const std::string& engineVersion)
        : game(19, CHINESE, determineKomi(CHINESE)){
        this->engineName = engineName;
        this->engineVersion = engineVersion;

        // initialize the base commands' sente implements

        commands = {
                {"protocol_version", {{&protocolVersion, {{"operation", STRING}}}}},
                {"name", {{&name, {{"operation", STRING}}}}},
                {"version", {{&version, {{"operation", STRING}}}}},
                {"known_command", {{&knownCommand, {{"operation", STRING}, {"command", STRING}}}}},
                {"list_commands", {{&listCommands, {{"operation", STRING}}}}},
                {"quit", {{&quit, {{"operation", STRING}}}}},
                {"exit", {{&quit, {{"operation", STRING}}}}},
                {"boardsize", {{&boardSize, {{"operation", STRING}, {"size", INTEGER}}}}},
                {"clearboard", {{&clearBoard, {{"operation", STRING}}}}},
                {"komi", {{&komi, {{"operation", STRING}, {"komi", FLOAT}}}}},
                {"play", {{&play, {{"operation", STRING}, {"color", COLOR}, {"vertex", VERTEX}}}}},
                {"genmove", {{&genMove, {{"operation", STRING}, {"color", COLOR}}}}},
                {"showboard", {{&showBoard, {{"operation", STRING}}}}},
        };

        // flip the co-ordinate label for the board
        std::cout << "setting ASCII mode and lower left" << std::endl;
        game.setASCIIMode(true);
        game.setLowerLeftCornerCoOrdinates(true);

    }

    std::string Engine::interpret(const std::string& text) {

        auto tokens = parse(text);

        std::stringstream outputStream;

        unsigned start = 0;

        // iterate through the tokens
        for (unsigned index = 0; index < tokens.size(); index++){

            Response response;

            // keep incrementing until we find a seperator
            while (tokens[index++]->getTokenType() != SEPERATOR and index < tokens.size()) {}

            // slice the tokens and put them into a list
            auto arguments = std::vector<std::shared_ptr<Token>>(tokens.begin() + start, tokens.begin() + index - 1);

            // begin interpreting by checking to see if the first element is an integer literal
            std::shared_ptr<Token> candidate;

            bool precedingID = false;
            unsigned id;

            if (arguments[0]->getTokenType() == LITERAL){
                auto* literal = (Literal*) arguments[0].get();
                precedingID = literal->getLiteralType() == INTEGER;
            }

            if (precedingID){
                id = ((Integer*) arguments[0].get())->getValue();
                candidate = arguments[1];
                arguments = std::vector<std::shared_ptr<Token>>(arguments.begin() + 1, arguments.end());
            }
            else {
                candidate = arguments[0];
            }

            if (candidate->getTokenType() == LITERAL){
                // make sure we have a string literal
                auto* literal = (Literal*) candidate.get();

                if (literal->getLiteralType() == STRING){

                    // check to see if a command exists
                    if (commands.find(literal->getText()) != commands.end()){
                        // check the arguments for the command
                        response = execute(literal->getText(), arguments);
                    }
                    else {
                        response = {false, "unknown command"};
                    }

                }

            }

            if (response.first){
                // if we successfully execute the command
                if (precedingID){
                    // if there is a preceding ID, include it in the answer
                    outputStream << statusMessage(response.second, id);
                }
                else {
                    outputStream << statusMessage(response.second);
                }
            }
            else {
                if (precedingID){
                    outputStream << errorMessage(response.second, id);
                }
                else {
                    outputStream << errorMessage(response.second);
                }
            }

        }

        return outputStream.str();
    }

    void Engine::registerCommand(const std::string& commandName, CommandMethod method,
                                 std::vector<ArgumentPattern> argumentPattern){
        if (commands.find(commandName) == commands.end()){
            // create a new vector
            commands[commandName] = {{method, argumentPattern}};
        }
        else {
            // append to an existing vector
            commands[commandName].push_back({method, argumentPattern});
        }
    }

    std::string Engine::errorMessage(const std::string& message) const {
        return "? " + message + "\n\n";
    }

    std::string Engine::errorMessage(const std::string &message, unsigned id) const {
        return "?" + std::to_string(id) + " " + message + "\n\n";
    }

    std::string Engine::statusMessage(const std::string &message) const {
        return "= " + message + "\n\n";
    }

    std::string Engine::statusMessage(const std::string &message, unsigned id) const {
        return "=" + std::to_string(id) + " " + message + "\n\n";
    }

    bool Engine::argumentsMatch(const std::vector<ArgumentPattern> &expectedArguments,
                                const std::vector<std::shared_ptr<Token>> &arguments) {

        if (arguments.size() != expectedArguments.size()){
            return false;
        }

        for (unsigned i = 0; i < arguments.size(); i++){
            // if we have a literal, cast the argument to a literal and see if we have
            // a literal
            auto argument = (Literal*) arguments[i].get();
            if (argument->getLiteralType() != expectedArguments[i].second){
                return false;
            }
        }

        return true;

    }

    Response Engine::invalidArgumentsErrorMessage(const std::vector<std::vector<ArgumentPattern>>& argumentPatterns,
                                                  const std::vector<std::shared_ptr<Token>> &arguments) {

        std::stringstream message;

        std::vector<std::vector<ArgumentPattern>> candidates;

        // first, find all the candidate patterns (patters with the correct number of arguments)
        for (const auto& pattern : argumentPatterns){
            if (pattern.size() == arguments.size()){
                candidates.push_back(pattern);
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

            message << ", got " << arguments.size() - 1;

        }
        else {

            message << "no viable argument pattern for command \"" << arguments[0]->getText() << "\"";

            for (const auto& candidate : candidates){
                // find the error
                for (unsigned i = 0; i < arguments.size(); i++){
                    auto argument = (Literal*) arguments[i].get();
                    if (argument->getLiteralType() != candidate[i].second){
                        message << std::endl << "candidate pattern not valid: expected " << toString(candidate[i].second)
                                << " in position " << i << ", got " << toString(argument->getLiteralType());
                    }
                }
            }

        }

        return {false, message.str()};

    }

    Response Engine::execute(const std::string &command, const std::vector<std::shared_ptr<Token>> &arguments) {

        // generate a list of possible argument patterns
        std::vector<std::vector<ArgumentPattern>> patterns;

        for (auto& definition : commands[command]){
            patterns.push_back(definition.second);
        }

        // find a matching pattern
        auto iter = std::find_if(patterns.begin(), patterns.end(),
                                 [arguments](const std::vector<ArgumentPattern>& pattern){
            return argumentsMatch(pattern, arguments);
        });

        if (iter != patterns.end()){
            // look up the matching function in the table and evaluate it
            return commands[command][iter - patterns.begin()].first(this, arguments);
        }
        else {
            return invalidArgumentsErrorMessage(patterns, arguments);
        }

    }

}