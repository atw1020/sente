//
// Created by arthur wesley on 7/2/21.
//

#ifndef SENTE_SGF_H
#define SENTE_SGF_H

#include <string>

#include "Tree.h"
#include "GoGame.h"
#include "GoComponents.h"

#ifdef __CYGWIN__
#include <ciso646>
#endif

/**
 *
 * SGF file format specifications
 *
 * https://www.red-bean.com/sgf/go.html
 *
 */
namespace sente {
    namespace utils {

        GoGame loadSGF(const std::string &SGFText);

        std::string dumpSGF(const GoGame& game, std::unordered_map<std::string, std::string> &attributes);

        std::unordered_map<std::string, std::string> getMetadata(const std::string& SGFText);

    }
}


#endif //SENTE_SGF_H
