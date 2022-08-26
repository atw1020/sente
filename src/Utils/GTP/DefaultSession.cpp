//
// Created by arthur wesley on 12/18/21.
//

#include <iostream>


#include "../SGF/SGF.h"
#include "DefaultSession.h"

namespace sente::GTP {

    std::unordered_map<std::string, std::vector<std::pair<CommandMethod, std::vector<ArgumentPattern>>>> builtins = {
            {"protocol_version", {{&protocolVersion, {{"operation", STRING}}}}},
            {"name", {{&name, {{"operation", STRING}}}}},
            {"version", {{&version, {{"operation", STRING}}}}},
            {"known_command", {{&knownCommand, {{"operation", STRING}, {"command", STRING}}}}},
            {"list_commands", {{&listCommands, {{"operation", STRING}}}}},
            {"quit", {{&quit, {{"operation", STRING}}}}},
            {"exit", {{&quit, {{"operation", STRING}}}}},
            {"boardsize", {{&boardSize, {{"operation", STRING}, {"size", INTEGER}}}}},
            {"clear_board", {{&clearBoard, {{"operation", STRING}}}}},
            {"komi", {{&komi, {{"operation", STRING}, {"komi", FLOAT}}}}},
            {"play", {{&play, {{"operation", STRING}, {"move", MOVE}}}}},
            {"undo", {{&undoOnce, {{"operation", STRING}}},
                      {&undoMultiple, {{"operation", STRING}, {"moves", INTEGER}}}}},
            {"showboard", {{&showBoard, {{"operation", STRING}}}}},
            {"loadsgf", {{&loadSGF1, {{"operation", STRING}, {"file", STRING}}},
                         {&loadSGF2, {{"operation", STRING}, {"file", STRING}, {"moves", INTEGER}}}}}
    };

    DefaultSession::DefaultSession(const std::string& engineName,
                                   const std::string& engineVersion): Session(engineName, engineVersion){
        // set the default commands
        commands = builtins;
    }

    Response DefaultSession::protocolVersion(const std::vector<std::shared_ptr<Token>>& arguments){
        (void) arguments;
        return {true, "2"};
    }

    Response DefaultSession::name(const std::vector<std::shared_ptr<Token>>& arguments){
        (void) arguments;
        return {true, getEngineName()};
    }
    Response DefaultSession::version(const std::vector<std::shared_ptr<Token>>& arguments){
        (void) arguments;
        return {true, getEngineVersion()};
    }
    Response DefaultSession::knownCommand(const std::vector<std::shared_ptr<Token>>& arguments){
        auto commands = getCommands();
        if (commands.find(arguments[1]->getText()) == commands.end()){
            return {true, "false"};
        }
        else {
            return {true, "true"};
        }
    }
    Response DefaultSession::listCommands(const std::vector<std::shared_ptr<Token>>& arguments){
        (void) arguments;
        std::stringstream commands;

        auto registeredCommands = getCommands();

        for (auto command = registeredCommands.begin(); command != registeredCommands.end();){
            commands << command->first;

            // only add the newline if we need to
            if (++command != registeredCommands.end()){
                commands << std::endl;
            }
        }

        return {true, commands.str()};

    }
    Response DefaultSession::quit(const std::vector<std::shared_ptr<Token>>& arguments){
        (void) arguments;
        setActive(false);
        return {true, ""};
    }
    Response DefaultSession::boardSize(const std::vector<std::shared_ptr<Token>>& arguments){
        // reset the board
        auto* size = (Integer*) arguments[1].get();
        if (size->getValue() == 9 or size->getValue() == 13 or size->getValue() == 19){
            masterGame = GoGame(size->getValue(), masterGame.getRules(), masterGame.getKomi(),
                                      {sente::Move::nullMove});
            setGTPDisplayFlags();
            return {true, ""};
        }
        else {
            return {false, "unacceptable size"};
        }
    }
    Response DefaultSession::clearBoard(const std::vector<std::shared_ptr<Token>>& arguments){
        (void) arguments;
        // reset the board
        masterGame.resetBoard();
        setGTPDisplayFlags();
        return {true, ""};
    }
    Response DefaultSession::komi(const std::vector<std::shared_ptr<Token>>& arguments){
        auto* newKomi = (Float*) arguments[1].get();
        masterGame.setKomi(newKomi->getValue());
        return {true, ""};
    }
    Response DefaultSession::play(const std::vector<std::shared_ptr<Token>>& arguments){

        // generate a move from the arguments
        sente::Move move = ((Move*) arguments[1].get())->getMove(masterGame.getSide());

        if (masterGame.isLegal(move)){
            // play the stone
            masterGame.playStone(move);
            return {true, ""};
        }
        else if (masterGame.isGTPLegal(move)){
            // play the stone
            masterGame.addStone(move);
            return {true, ""};
        }
        else {
            // if the move is illegal, report it
            return {false, "illegal move"};
        }
    }
    Response DefaultSession::genMove(const std::vector<std::shared_ptr<Token>>& arguments){
        (void) arguments;
        throw std::runtime_error("genmove has not been implemented by this engine, please register a valid function");
    }
    Response DefaultSession::showBoard(const std::vector<std::shared_ptr<Token>>& arguments){
        (void) arguments;
        return {true, "\n" + std::string(masterGame)};
    }
    Response DefaultSession::undoOnce(const std::vector<std::shared_ptr<Token>>& arguments){
        (void) arguments;
        if (not masterGame.isAtRoot()){
            masterGame.stepUp(1);
            setGTPDisplayFlags();
            return {true, ""};
        }
        else {
            return {false, "cannot undo"};
        }
    }
    Response DefaultSession:undoMultiple(const std::vector<std::shared_ptr<Token>>& arguments){
        auto* steps = (Integer*) arguments[1].get();
        if (masterGame.getMoveSequence().size() >= steps->getValue()){
            masterGame.stepUp(steps->getValue());
            setGTPDisplayFlags();
            return {true, ""};
        }
        else {
            return {false, "cannot undo"};
        }
    }

    Response DefaultSession::baseLoadSGF(const std::string& filePath){

        // load the text from the file
        std::ifstream filePointer(filePath);

        if (not filePointer.good()){
            return {false, "cannot load file"};
        }

        std::string SGFText = std::string((std::istreambuf_iterator<char>(filePointer)),
                                          std::istreambuf_iterator<char>());

        // generate the move tree
        auto tree = sente::SGF::loadSGF(SGFText, false, true, true);

        // set the engine's game to be the move tree
        masterGame = GoGame(tree);
        setGTPDisplayFlags();

        return {true, ""};

    }

    Response DefaultSession::loadSGF1(const std::vector<std::shared_ptr<Token>>& arguments){
        auto* pathStr = (String*) arguments[1].get();
        // run hte basic SGF loading
        auto response = baseLoadSGF(pathStr->getText());

        masterGame.playDefaultSequence();

        return response;
    }

    Response DefaultSession::loadSGF2(const std::vector<std::shared_ptr<Token>>& arguments){
        // load the board
        auto* pathStr = (String*) arguments[1].get();
        auto* moves = (Integer*) arguments[2].get();
        auto response = baseLoadSGF(pathStr->getText());

        // play out the sequence
        auto moveSequence = masterGame.getDefaultSequence();

        unsigned movesAdvanced = std::min(moves->getValue(), unsigned(moveSequence.size()));

        moveSequence = std::vector<sente::Move>(moveSequence.begin(), moveSequence.begin() + movesAdvanced);

        masterGame.playMoveSequence(moveSequence);

        return response;
    }

}
