//
// Created by arthur wesley on 12/18/21.
//

#ifndef SENTE_OPERATORS_H
#define SENTE_OPERATORS_H

#include "Engine.h"

namespace sente::GTP {

    Response protocolVersion(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response name(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response version(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response knownCommand(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response listCommands(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response quit(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response boardSize(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response clearBoard(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response komi(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response play(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response genMove(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response showBoard(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments);

    Response undoOnce(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response undoMultiple(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response loadSGF1(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response loadSGF2(Engine* self, const std::vector<std::shared_ptr<Token>>& arguments);

}

#endif //SENTE_OPERATORS_H
