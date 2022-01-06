//
// Created by arthur wesley on 7/2/21.
//

#ifndef SENTE_SGF_H
#define SENTE_SGF_H

#include <string>

#include "../Tree.h"
#include "SGFProperty.h"
#include "../../Game/GoGame.h"
#include "../../Game/GoComponents.h"

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
namespace sente::SGF {

    // GoGame loadSGF(const std::string &SGFText);
    utils::Tree<SGFNode> loadSGF(const std::string& SGFText, bool disableWarnings,
                                                      bool ignoreIllegalProperties,
                                                      bool fixFileFormat);

    std::string dumpSGF(const GoGame& game);
    // std::string dumpSGF(const Tree<SGFNode>& game);


}


#endif //SENTE_SGF_H
