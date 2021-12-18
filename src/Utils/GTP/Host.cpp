//
// Created by arthur wesley on 12/12/21.
//

#include "../../Include/Utils/GTP/Host.h"

#include <set>
#include <utility>
#include <vector>

namespace sente::GTP {

    Host::Host(const std::string& engineName) : game(19, CHINESE, determineKomi(CHINESE)){
        this->engineName = engineName;
    }

    std::string Host::evaluate(const std::string& text) {

        auto tokens = parse(text);

        std::stringstream outputStream;

        unsigned start = 0;

        // iterate through the tokens
        for (unsigned index = 0; index < tokens.size(); index++){

            Response response;

            // keep incrementing until we find a seperator
            while (tokens[index++]->getTokenType() != SEPERATOR and index < tokens.size()) {}

            // echo the tokens into the output
            for (unsigned i = start; i < index; i++){
                outputStream << tokens[i]->getText();
                // add a space if the next item is not a seperator (newline)
                if (i + 1 < tokens.size()){
                    if (tokens[i + 1]->getTokenType() != SEPERATOR){
                        outputStream << " ";
                    }
                }
            }

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
                        response = evaluateCommand(literal->getText(), arguments);
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

    std::string Host::errorMessage(const std::string& message) const {
        return "? " + message + "\n\n";
    }

    std::string Host::errorMessage(const std::string &message, unsigned id) const {
        return "?" + std::to_string(id) + " " + message + "\n\n";
    }

    std::string Host::statusMessage(const std::string &message) const {
        return "= " + message + "\n\n";
    }

    std::string Host::statusMessage(const std::string &message, unsigned id) const {
        return "=" + std::to_string(id) + " " + message + "\n\n";
    }

    bool Host::argumentsMatch(const std::vector<ArgumentPattern> &expectedArguments,
                                const std::vector<std::shared_ptr<Token>> &arguments) {

        if (arguments.size() != expectedArguments.size()){
            return false;
        }

        for (unsigned i = 0; i < arguments.size(); i++){
            // check to see if the argument is a literal
            if (std::holds_alternative<tokenType>(expectedArguments[i].second)){
                // if we have a token, check to see if we expect a token
                if (arguments[i]->getTokenType() != std::get<tokenType>(expectedArguments[i].second)){
                    return false;
                }
            }
            else {
                // if we have a literal, cast the argument to a literal and see if we have
                // a literal
                auto argument = (Literal*) arguments[i].get();
                if (argument->getLiteralType() != std::get<literalType>(expectedArguments[i].second)){
                    return false;
                }
            }
        }

        return true;

    }

    Response Host::invalidArgumentsErrorMessage(const std::vector<std::vector<ArgumentPattern>>& argumentPatterns,
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
                    if (std::holds_alternative<tokenType>(candidate[i].second)){
                        if (arguments[i]->getTokenType() != std::get<tokenType>(candidate[i].second)){
                            message << std::endl << "candidate pattern not valid: expected " << toString(std::get<tokenType>(candidate[i].second))
                                    << " in position " << i << ", got" << toString(arguments[i]->getTokenType());
                        }
                    }
                    else {
                        auto argument = (Literal*) arguments[i].get();
                        if (argument->getLiteralType() != std::get<literalType>(candidate[i].second)){
                            message << std::endl << "candidate pattern not valid: expected " << toString(std::get<tokenType>(candidate[i].second))
                                    << " in position " << i << ", got" << toString(arguments[i]->getTokenType());
                        }
                    }
                }
            }

        }

        return {false, message.str()};

    }

    Response Host::evaluateCommand(const std::string& command, const std::vector<std::shared_ptr<Token>>& arguments){

        // generate a list of possible argument patterns
        std::vector<std::vector<ArgumentPattern>> patterns;

        for (auto& definition : commands[command]){
            patterns.push_back(definition.second);
        }

        // find a matching pattern
        auto iter = std::find_if(patterns.begin(), patterns.end(), this->argumentsMatch);

        if (iter != patterns.end()){
            // look up the matching function in the table and evaluate it
            return {true, commands[command][iter - patterns.begin()].first(this, arguments)};
        }
        else {
            return invalidArgumentsErrorMessage(patterns, arguments);
        }

    }

}