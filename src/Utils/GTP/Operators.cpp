//
// Created by arthur wesley on 12/18/21.
//

#include <iostream>
#include <filesystem>


#include "Operators.h"
#include "../SGF/SGF.h"

namespace sente::GTP {

    Response protocolVersion(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments){
        return {true, "2"};
    }

    Response name(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments){
        return {true, self->engineName};
    }
    Response version(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments){
        return {true, self->engineVersion};
    }
    Response knownCommand(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments){
        if (self->commands.find(arguments[1]->getText()) == self->commands.end()){
            return {true, "false"};
        }
        else {
            return {true, "true"};
        }
    }
    Response listCommands(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments){
        std::stringstream commands;

        for (const auto& command : self->commands){
            commands << command.first << std::endl;
        }

        return {true, commands.str()};

    }
    Response quit(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments){
        self->active = false;
        return {true, ""};
    }
    Response boardSize(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments){
        // reset the board
        auto* size = (Integer*) arguments[1].get();
        if (size->getValue() == 9 or size->getValue() == 13 or size->getValue() == 19){
            self->game = GoGame(size->getValue(), self->game.getRules(), self->game.getKomi());
            self->setGTPDisplayFlags();
            return {true, ""};
        }
        else {
            return {false, "unacceptable size"};
        }
    }
    Response clearBoard(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments){
        // reset the board
        self->game = GoGame(self->game.getSide(), self->game.getRules(), self->game.getKomi());
        self->setGTPDisplayFlags();
        return {true, ""};
    }
    Response komi(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments){
        auto* newKomi = (Float*) arguments[1].get();
        self->game.setKomi(newKomi->getValue());
        return {true, ""};
    }
    Response play(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments){

        // generate a move from the arguments
        Move move(*(Color*) arguments[1].get(), *(Vertex*) arguments[2].get(), self->game.getSide());

        if (self->game.isAddLegal(move.getMove())){
            if (self->game.isLegal(move.getMove())){
                // play the stone
                self->game.playStone(move.getMove());
                return {true, ""};
            }
            else {
                self->game.addStone(move.getMove());
                return {true, ""};
            }
        }
        else {
            // if the move is illegal, report it
            return {false, "illegal move"};
        }
    }
    Response genMove(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments){
        return {false, "cannot generate move; no engine bound to generate move"};
    }
    Response showBoard(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments){
        std::cout << "entering showBoard" << std::endl;
        return {true, "\n" + std::string(self->game)};
    }
    Response undoOnce(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments){
        if (not self->game.isAtRoot()){
            self->game.stepUp(1);
            return {true, ""};
        }
        else {
            return {false, "cannot undo"};
        }
    }
    Response undoMultiple(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments){
        auto* steps = (Integer*) arguments[1].get();
        if (self->game.getMoveSequence().size() >= steps->getValue()){
            self->game.stepUp(steps->getValue());
            return {true, ""};
        }
        else {
            return {false, "cannot undo"};
        }
    }

    Response baseLoadSGF(Engine* self, std::string filePath){

        auto pathStr = std::filesystem::path(filePath);

        if (std::filesystem::exists(filePath)){

            // load the text from the file
            std::ifstream filePointer(filePath);
            std::string SGFText = std::string((std::istreambuf_iterator<char>(filePointer)),
                                              std::istreambuf_iterator<char>());

            // generate the move tree
            auto tree = sente::SGF::loadSGF(SGFText, false, true, true);

            // set the engine's game to be the move tree
            self->game = GoGame(tree);
            self->setGTPDisplayFlags();

            std::cout << "leaving loadSGF" << std::endl;

            return {true, ""};
        }
        else {
            return {false, "cannot load file"};
        }

    }

    Response loadSGF1(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments){
        auto* pathStr = (String*) arguments[1].get();
        auto response = baseLoadSGF(self, pathStr->getText());

        std::cout << "playing sequence" << std::endl;
        self->game.playDefaultSequence();

        return response;
    }

    Response loadSGF2(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments){
        // load the board
        auto* pathStr = (String*) arguments[1].get();
        auto* moves = (Integer*) arguments[2].get();
        auto response = baseLoadSGF(self, pathStr->getText());

        // play out the sequence
        auto moveSequence = self->game.getDefaultSequence();

        unsigned movesAdvanced = std::min(moves->getValue(), unsigned(moveSequence.size()));

        moveSequence = std::vector<sente::Move>(moveSequence.begin(), moveSequence.begin() + movesAdvanced);

        self->game.playMoveSequence(moveSequence);

        return response;
    }

}
