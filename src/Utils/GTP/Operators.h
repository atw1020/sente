//
// Created by arthur wesley on 12/18/21.
//

#ifndef SENTE_OPERATORS_H
#define SENTE_OPERATORS_H

#include "Session.h"

namespace sente::GTP {

    Response protocolVersion(Session* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response name(Session* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response version(Session* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response knownCommand(Session* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response listCommands(Session* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response quit(Session* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response boardSize(Session* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response clearBoard(Session* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response komi(Session* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response play(Session* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response genMove(Session* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response showBoard(Session* self, const std::vector<std::shared_ptr<Token>>& arguments);

    Response undoOnce(Session* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response undoMultiple(Session* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response loadSGF1(Session* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response loadSGF2(Session* self, const std::vector<std::shared_ptr<Token>>& arguments);

}

#endif //SENTE_OPERATORS_H
