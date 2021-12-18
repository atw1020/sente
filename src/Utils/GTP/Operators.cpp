//
// Created by arthur wesley on 12/18/21.
//

#include "../../Include/Utils/GTP/Operators.h"

namespace sente::GTP {

    std::string protocolVersion(Host* self, const std::vector<std::shared_ptr<Token>>& arguments){
        return "2";
    }

    std::string name(Host* self, const std::vector<std::shared_ptr<Token>>& arguments){
        return self->engineName;
    }
    std::string version(Host* self, const std::vector<std::shared_ptr<Token>>& arguments){
        return self->engineVersion;
    }
    std::string knownCommand(Host* self, const std::vector<std::shared_ptr<Token>>& arguments){
        if (self->commands.find(arguments[1]->getText()) == self->commands.end()){
            return "false";
        }
        else {
            return "true";
        }
    }
    std::string listCommands(Host* self, const std::vector<std::shared_ptr<Token>>& arguments){
        std::stringstream commands;

        for (const auto& command : self->commands){
            commands << command.first << std::endl;
        }

        return commands.str();

    }
    std::string quit(Host* self, const std::vector<std::shared_ptr<Token>>& arguments){
        return "";
    }
    std::string boardSize(Host* self, const std::vector<std::shared_ptr<Token>>& arguments){
        // reset the board
        auto* size = (Integer*) arguments[1].get();
        self->game = GoGame(size->getValue(), self->game.getRules(), self->game.getKomi());
        return "";
    }
    std::string clearBoard(Host* self, const std::vector<std::shared_ptr<Token>>& arguments){
        // reset the board
        self->game = GoGame(self->game.getBoard().getSide(), self->game.getRules(), self->game.getKomi());
        return "";
    }
    std::string komi(Host* self, const std::vector<std::shared_ptr<Token>>& arguments){
        return std::to_string(self->game.getKomi());
    }
    std::string play(Host* self, const std::vector<std::shared_ptr<Token>>& arguments){
        return "";
    }
    std::string genMove(Host* self, const std::vector<std::shared_ptr<Token>>& arguments){
        return "";
    }

}
