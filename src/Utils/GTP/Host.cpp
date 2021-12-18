//
// Created by arthur wesley on 12/12/21.
//

#include "../../Include/Utils/GTP/Host.h"

#include <set>
#include <utility>
#include <vector>

#include "../../Include/Utils/GTP/Tokens/Operator.h"

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

            if (candidate->getTokenType() == OPERATOR){
                // execute the command
                auto* command = (Operator*) candidate.get();

                switch (command->getName()){
                    case PROTOCOL_VERSION:
                        response = protocolVersion(arguments);
                        break;
                    case NAME:
                        response = name(arguments);
                        break;
                    case KNOWN_COMMAND:
                        response = knownCommand(arguments);
                        break;
                    case BOARD_SIZE:
                        response = boardSize(arguments);
                        break;
                    case LIST_COMMANDS:
                        response = listCommands(arguments);
                        break;
                    case CLEAR_BOARD:
                        response = clearBoard(arguments);
                        break;
                    case VERSION:
                        response = version(arguments);
                        break;
                    default:
                        response = {false, "Unimplemented GTP command \"" + candidate->getText() + "\""};
                        break;
                    case QUIT:
                        // exit the function
                        return outputStream.str();
                }

            }
            else {
                response = {false, "unknown command"};
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


    Response Host::protocolVersion(const std::vector<std::shared_ptr<Token>>& arguments) {

        std::vector<std::vector<ArgumentPattern>> argumentPatterns = {
                {{"command", OPERATOR}}
        };

        if (argumentsMatch(*argumentPatterns.begin(), arguments)){
            return {true, "2"};
        }
        else {
            return invalidArgumentsErrorMessage(argumentPatterns, arguments);
        }

    }

    Response Host::name(const std::vector<std::shared_ptr<Token>>& arguments) {
        std::vector<std::vector<ArgumentPattern>> argumentPatterns = {
                {{"command", OPERATOR}}
        };

        if (argumentsMatch(*argumentPatterns.begin(), arguments)){
            return {true, engineName};
        }
        else {
            return invalidArgumentsErrorMessage(argumentPatterns, arguments);
        }
    }

    Response Host::listCommands(const std::vector<std::shared_ptr<Token>>& arguments) {

        std::vector<std::vector<ArgumentPattern>> argumentPatterns = {
                {{"command", OPERATOR}}
        };

        if (argumentsMatch(*argumentPatterns.begin(), arguments)){
            std::stringstream response;

            // TODO: check to see if each item should be on a newline
            for (const auto& item : OPERATORS){
                response << item.first << std::endl;
            }

            return {true, response.str()};
        }
        else {
            return invalidArgumentsErrorMessage(argumentPatterns, arguments);
        }
    }

    Response Host::knownCommand(const std::vector<std::shared_ptr<Token>>& arguments) {

        std::vector<std::vector<ArgumentPattern>> argumentPatterns = {
                {{"command", OPERATOR}, {"command", OPERATOR}},
                {{"command", OPERATOR}, {"string", STRING}}
        };

        if (argumentsMatch(*argumentPatterns.begin(), arguments)){

            if (arguments[1]->getTokenType() == OPERATOR){
                return {true, "true"};
            }
            else {
                return {true, "false"};
            }

        }
        else {
            return invalidArgumentsErrorMessage(argumentPatterns, arguments);
        }

    }

    Response Host::boardSize(const std::vector<std::shared_ptr<Token>>& arguments) {

        // TODO: check to make sure that boardSize clears the board
        std::vector<std::vector<ArgumentPattern>> argumentPatterns = {
                {{"command", OPERATOR}, {"command", INTEGER}}
        };

        if (argumentsMatch(*argumentPatterns.begin(), arguments)){
            unsigned side = ((Integer*) arguments[1].get())->getValue();
            if (side == 9 or side == 13 or side == 19){
                game = GoGame(side, game.getRules(), game.getKomi());
                return {true, ""};
            }
            else {
                return {false, "unacceptable size"};
            }
        }
        else {
            return invalidArgumentsErrorMessage(argumentPatterns, arguments);
        }
    }

    Response Host::clearBoard(const std::vector<std::shared_ptr<Token>> &arguments) {
        std::vector<std::vector<ArgumentPattern>> argumentPatterns = {
                {{"command", OPERATOR}}
        };

        if (argumentsMatch(*argumentPatterns.begin(), arguments)){
            game = GoGame(game.getBoard().getSide(), game.getRules(), game.getKomi());
            return {true, ""};
        }
        else {
            return invalidArgumentsErrorMessage(argumentPatterns, arguments);
        }

    }

    Response Host::komi(const std::vector<std::shared_ptr<Token>>& arguments) {

        std::vector<std::vector<ArgumentPattern>> argumentPatterns = {
                {{"command", OPERATOR}},
                {{"new komi", FLOAT}}
        };

        if (argumentsMatch(*argumentPatterns.begin(), arguments)){
            float komi = ((Float*) arguments[1].get())->getValue();
            game.setKomi(komi);
            return {true, ""};
        }
        else {
            return invalidArgumentsErrorMessage(argumentPatterns, arguments);
        }

    }

    Response Host::play(const std::vector<std::shared_ptr<Token>>& arguments){
        std::vector<std::vector<ArgumentPattern>> argumentPatterns = {
                {{"command", OPERATOR}},
                {{"color", COLOR}},
                {{"vertex", VERTEX}}
        };

        if (argumentsMatch(*argumentPatterns.begin(), arguments)){
            auto* color = (Color*) arguments[1].get();
            auto* vertex = (Vertex*) arguments[1].get();

            Stone moveColor = color->getColor() == BLACK ? Stone::BLACK : Stone::EMPTY;

            if (game.getActivePlayer() == moveColor){
                // if it is the active player's turn, play a move
                game.playStone(vertex->getX(), vertex->getY());
            }
            else {
                game.addStone(Move(vertex->getX(), vertex->getY(), moveColor));
            }

            return {true, ""};

        }
        else {
            return invalidArgumentsErrorMessage(argumentPatterns, arguments);
        }

    }

    Response Host::version(const std::vector<std::shared_ptr<Token>>& arguments){
        std::vector<std::vector<ArgumentPattern>> argumentPatterns = {
                {{"command", OPERATOR}}
        };

        if (argumentsMatch(*argumentPatterns.begin(), arguments)){
            return {true, "0.4.0"};
        }
        else {
            return invalidArgumentsErrorMessage(argumentPatterns, arguments);
        }
    }

}