//
// Created by arthur wesley on 12/18/21.
//

#include "../../Include/Utils/GTP/Operators.h"

namespace sente::GTP {

    Response protocolVersion(Host* self, const std::vector<std::shared_ptr<Token>>& arguments){
        return {true, "2"};
    }

    Response name(Host* self, const std::vector<std::shared_ptr<Token>>& arguments){
        return {true, self->engineName};
    }
    Response version(Host* self, const std::vector<std::shared_ptr<Token>>& arguments){
        return {true, self->engineVersion};
    }
    Response knownCommand(Host* self, const std::vector<std::shared_ptr<Token>>& arguments){
        if (self->commands.find(arguments[1]->getText()) == self->commands.end()){
            return {true, "false"};
        }
        else {
            return {true, "true"};
        }
    }
    Response listCommands(Host* self, const std::vector<std::shared_ptr<Token>>& arguments){
        std::stringstream commands;

        for (const auto& command : self->commands){
            commands << command.first << std::endl;
        }

        return {true, commands.str()};

    }
    Response quit(Host* self, const std::vector<std::shared_ptr<Token>>& arguments){
        return {true, ""};
    }
    Response boardSize(Host* self, const std::vector<std::shared_ptr<Token>>& arguments){
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
    Response clearBoard(Host* self, const std::vector<std::shared_ptr<Token>>& arguments){
        // reset the board
        self->game = GoGame(self->game.getBoard().getSide(), self->game.getRules(), self->game.getKomi());
        return {true, ""};
    }
    Response komi(Host* self, const std::vector<std::shared_ptr<Token>>& arguments){
        auto* newKomi = (Float*) arguments[1].get();
        self->game.setKomi(newKomi->getValue());
        return {true, ""};
    }
    Response play(Host* self, const std::vector<std::shared_ptr<Token>>& arguments){
        // cast the argument to a move
        auto* move = (Move*) arguments[1].get();
        if (self->game.isLegal(move->getMove())){
            return {true, ""};
        }
        else {
            // if the move is illegal, report it
            return {false, "illegal move"};
        }
    }
    Response genMove(Host* self, const std::vector<std::shared_ptr<Token>>& arguments){
        return {true, ""};
    }

}
