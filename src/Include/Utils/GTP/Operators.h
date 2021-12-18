//
// Created by arthur wesley on 12/18/21.
//

#ifndef SENTE_OPERATORS_H
#define SENTE_OPERATORS_H

#include "Host.h"

namespace sente::GTP {

    std::string protocolVersion(Host* self, const std::vector<std::shared_ptr<Token>>& arguments);
    std::string name(Host* self, const std::vector<std::shared_ptr<Token>>& arguments);
    std::string version(Host* self, const std::vector<std::shared_ptr<Token>>& arguments);
    std::string knownCommand(Host* self, const std::vector<std::shared_ptr<Token>>& arguments);
    std::string listCommands(Host* self, const std::vector<std::shared_ptr<Token>>& arguments);
    std::string quit(Host* self, const std::vector<std::shared_ptr<Token>>& arguments);
    std::string boardSize(Host* self, const std::vector<std::shared_ptr<Token>>& arguments);
    std::string clearBoard(Host* self, const std::vector<std::shared_ptr<Token>>& arguments);
    std::string komi(Host* self, const std::vector<std::shared_ptr<Token>>& arguments);
    std::string play(Host* self, const std::vector<std::shared_ptr<Token>>& arguments);
    std::string genMove(Host* self, const std::vector<std::shared_ptr<Token>>& arguments);
}

#endif //SENTE_OPERATORS_H
