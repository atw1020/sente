//
// Created by arthur wesley on 7/2/21.
//

#ifndef SENTE_SGF_H
#define SENTE_SGF_H

#include <string>

#include "Tree.h"
#include "GoComponents.h"

namespace sente_utils {

    Tree<sente::Move> getSGFMoves(const std::string& SGFText);

    sente::Rules getSGFRules(const std::string& SGFText);
    unsigned getSGFBoardSize(const std::string& SGFText);

}

#endif //SENTE_SGF_H
