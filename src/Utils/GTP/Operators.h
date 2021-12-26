//
// Created by arthur wesley on 12/18/21.
//

#ifndef SENTE_OPERATORS_H
#define SENTE_OPERATORS_H

#include "Interpreter.h"

namespace sente::GTP {

    Response protocolVersion(Interpreter* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response name(Interpreter* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response version(Interpreter* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response knownCommand(Interpreter* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response listCommands(Interpreter* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response quit(Interpreter* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response boardSize(Interpreter* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response clearBoard(Interpreter* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response komi(Interpreter* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response play(Interpreter* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response genMove(Interpreter* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response showBoard(Interpreter* self, const std::vector<std::shared_ptr<Token>>& arguments);

    Response undoOnce(Interpreter* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response undoMultiple(Interpreter* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response loadSGF1(Interpreter* self, const std::vector<std::shared_ptr<Token>>& arguments);
    Response loadSGF2(Interpreter* self, const std::vector<std::shared_ptr<Token>>& arguments);

}

#endif //SENTE_OPERATORS_H
