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
        self->game = GoGame(size->getValue(), self->game.getRules(), self->game.getKomi());
        return {true, ""};
    }
    Response clearBoard(Host* self, const std::vector<std::shared_ptr<Token>>& arguments){
        // reset the board
        self->game = GoGame(self->game.getBoard().getSide(), self->game.getRules(), self->game.getKomi());
        return {true, ""};
    }
    Response komi(Host* self, const std::vector<std::shared_ptr<Token>>& arguments){
        return {true, std::to_string(self->game.getKomi())};
    }
    Response play(Host* self, const std::vector<std::shared_ptr<Token>>& arguments){
        // TODO: implement
        return {true, ""};
    }
    Response genMove(Host* self, const std::vector<std::shared_ptr<Token>>& arguments){
        // TODO: implement
        return {true, ""};
    }

}
