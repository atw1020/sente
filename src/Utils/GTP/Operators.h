//
// Created by arthur wesley on 12/18/21.
//

#ifndef SENTE_OPERATORS_H
#define SENTE_OPERATORS_H

#include "GTPSession.h"

namespace sente::GTP {

    Response protocolVersion(GTPSession* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response name(GTPSession* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response version(GTPSession* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response knownCommand(GTPSession* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response listCommands(GTPSession* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response quit(GTPSession* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response boardSize(GTPSession* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response clearBoard(GTPSession* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response komi(GTPSession* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response play(GTPSession* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response genMove(GTPSession* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response showBoard(GTPSession* self, const std::vector<std::shared_ptr<Token>>& arguments);

    Response undoOnce(GTPSession* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response undoMultiple(GTPSession* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response loadSGF1(GTPSession* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response loadSGF2(GTPSession* self, const std::vector<std::shared_ptr<Token>>& arguments);

}

#endif //SENTE_OPERATORS_H
