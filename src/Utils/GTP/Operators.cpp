//
// Created by arthur wesley on 12/18/21.
//

#include <iostream>
#include <filesystem>


#include "Operators.h"
#include "../SGF/SGF.h"

namespace sente::GTP {

    Response protocolVersion(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments){
        (void) self;
        (void) arguments;
        return {true, "2"};
    }

    Response name(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments){
        (void) arguments;
        return {true, self->getEngineName()};
    }
    Response version(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments){
        (void) arguments;
        return {true, self->getEngineVersion()};
    }
    Response knownCommand(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments){
        auto commands = self->getCommands();
        if (commands.find(arguments[1]->getText()) == commands.end()){
            return {true, "false"};
        }
        else {
            return {true, "true"};
        }
    }
    Response listCommands(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments){
        (void) arguments;
        std::stringstream commands;

        for (const auto& command : self->getCommands()){
            commands << command.first << std::endl;
        }

        return {true, commands.str()};

    }
    Response quit(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments){
        (void) arguments;
        self->setActive(false);
        return {true, ""};
    }
    Response boardSize(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments){
        // reset the board
        auto* size = (Integer*) arguments[1].get();
        if (size->getValue() == 9 or size->getValue() == 13 or size->getValue() == 19){
            self->masterGame = GoGame(size->getValue(), self->masterGame.getRules(), self->masterGame.getKomi());
            return {true, ""};
        }
        else {
            return {false, "unacceptable size"};
        }
    }
    Response clearBoard(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments){
        (void) arguments;
        // reset the board
        self->masterGame = GoGame(self->masterGame.getSide(), self->masterGame.getRules(), self->masterGame.getKomi());
        return {true, ""};
    }
    Response komi(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments){
        auto* newKomi = (Float*) arguments[1].get();
        self->masterGame.setKomi(newKomi->getValue());
        return {true, ""};
    }
    Response play(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments){

        // generate a move from the arguments
        Move move(*(Color*) arguments[1].get(), *(Vertex*) arguments[2].get(), self->masterGame.getSide());

        if (self->masterGame.isAddLegal(move.getMove())){
            if (self->masterGame.isLegal(move.getMove())){
                // play the stone
                self->masterGame.playStone(move.getMove());
                return {true, ""};
            }
            else {
                self->masterGame.addStone(move.getMove());
                return {true, ""};
            }
        }
        else {
            // if the move is illegal, report it
            return {false, "illegal move"};
        }
    }
    Response genMove(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments){
        (void) self;
        (void) arguments;
        return {false, "cannot generate move; no engine bound to generate move"};
    }
    Response showBoard(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments){
        (void) arguments;
        return {true, "\n" + std::string(self->masterGame)};
    }
    Response undoOnce(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments){
        (void) arguments;
        if (not self->masterGame.isAtRoot()){
            self->masterGame.stepUp(1);
            return {true, ""};
        }
        else {
            return {false, "cannot undo"};
        }
    }
    Response undoMultiple(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments){
        auto* steps = (Integer*) arguments[1].get();
        if (self->masterGame.getMoveSequence().size() >= steps->getValue()){
            self->masterGame.stepUp(steps->getValue());
            return {true, ""};
        }
        else {
            return {false, "cannot undo"};
        }
    }

    Response baseLoadSGF(Engine* self, const std::string& filePath){

        auto pathStr = std::filesystem::path(filePath);

        if (std::filesystem::exists(filePath)){

            // load the text from the file
            std::ifstream filePointer(filePath);
            std::string SGFText = std::string((std::istreambuf_iterator<char>(filePointer)),
                                              std::istreambuf_iterator<char>());

            // generate the move tree
            auto tree = sente::SGF::loadSGF(SGFText, false, true, true);

            // set the engine's game to be the move tree
            self->masterGame = GoGame(tree);
            self->setGTPDisplayFlags();

            return {true, ""};
        }
        else {
            return {false, "cannot load file"};
        }

    }

    Response loadSGF1(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments){
        auto* pathStr = (String*) arguments[1].get();
        auto response = baseLoadSGF(self, pathStr->getText());

        self->masterGame.playDefaultSequence();

        return response;
    }

    Response loadSGF2(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments){
        // load the board
        auto* pathStr = (String*) arguments[1].get();
        auto* moves = (Integer*) arguments[2].get();
        auto response = baseLoadSGF(self, pathStr->getText());

        // play out the sequence
        auto moveSequence = self->masterGame.getDefaultSequence();

        unsigned movesAdvanced = std::min(moves->getValue(), unsigned(moveSequence.size()));

        moveSequence = std::vector<sente::Move>(moveSequence.begin(), moveSequence.begin() + movesAdvanced);

        self->masterGame.playMoveSequence(moveSequence);

        return response;
    }

}
