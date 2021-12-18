//
// Created by arthur wesley on 12/18/21.
//

#ifndef SENTE_OPERATORS_H
#define SENTE_OPERATORS_H

#include "Host.h"

namespace sente::GTP {

    Response protocolVersion(Host* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response name(Host* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response version(Host* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response knownCommand(Host* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response listCommands(Host* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response quit(Host* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response boardSize(Host* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response clearBoard(Host* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response komi(Host* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response play(Host* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response genMove(Host* self, const std::vector<std::shared_ptr<Token>>& arguments);
}

#endif //SENTE_OPERATORS_H
