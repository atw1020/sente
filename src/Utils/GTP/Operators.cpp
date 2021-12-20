//
// Created by arthur wesley on 12/18/21.
//

#include <iostream>
#include "Operators.h"

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
            return {true, ""};
        }
        else {
            return {false, "unacceptable size"};
        }
    }
    Response clearBoard(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments){
        // reset the board
        self->game = GoGame(self->game.getBoard()->getSide(), self->game.getRules(), self->game.getKomi());
        return {true, ""};
    }
    Response komi(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments){
        auto* newKomi = (Float*) arguments[1].get();
        self->game.setKomi(newKomi->getValue());
        return {true, ""};
    }
    Response play(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments){
        // generate a move from the arguments
        Move move(*(Color*) arguments[1].get(), *(Vertex*) arguments[2].get());
        if (self->game.isLegal(move.getMove())){
            self->game.playStone(move.getMove());
            return {true, ""};
        }
        else {
            // if the move is illegal, report it
            return {false, "illegal move"};
        }
    }
    Response genMove(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments){
        return {true, ""};
    }
    Response showBoard(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments){
        return {true, std::string(self->game)};
    }

}
