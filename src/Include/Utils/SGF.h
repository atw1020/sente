//
// Created by arthur wesley on 7/2/21.
//

#ifndef SENTE_SGF_H
#define SENTE_SGF_H

#include <string>

#include "Tree.h"
#include "SGFCommands.h"
#include "../Game/GoGame.h"
#include "../Game/GoComponents.h"

#ifdef __CYGWIN__
#include <ciso646>
#endif

/**
 *
 * SGF file format specifications
 *
 * https://www.red-bean.com/sgf/properties.html
 *
 */
namespace sente {
    namespace utils {

        // GoGame loadSGF(const std::string &SGFText);
        Tree<SGFNode> loadSGF(const std::string& SGFText);

        std::string dumpSGF(const GoGame& game);
        // std::string dumpSGF(const Tree<SGFNode>& game);

    }
}


#endif //SENTE_SGF_H
