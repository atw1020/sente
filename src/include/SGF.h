//
// Created by arthur wesley on 7/2/21.
//

#ifndef SENTE_SGF_H
#define SENTE_SGF_H

#include <string>

#include "Tree.h"
#include "GoComponents.h"

/**
 *
 * SGF file format specifications
 *
 * https://www.red-bean.com/sgf/go.html
 *
 */
namespace sente {
    namespace utils {

        Tree <Move> getSGFMoves(const std::string &SGFText);

        Rules getSGFRules(const std::string &SGFText);

        std::unique_ptr<_board> getSGFBoardSize(const std::string &SGFText);

        std::string toSGF(Tree <Move> moves, std::unordered_map<std::string, std::string> &attributes);

        std::unordered_map<std::string, std::string> getMetadata(const std::string& SGFText);

    }
}


#endif //SENTE_SGF_H
