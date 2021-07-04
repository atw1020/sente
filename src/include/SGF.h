//
// Created by arthur wesley on 7/2/21.
//

#ifndef SENTE_SGF_H
#define SENTE_SGF_H

#include <string>

#include "Tree.h"
#include "GoComponents.h"

namespace sente_utils {

    class InvalidSGFException : public std::domain_error{
    public:
        explicit InvalidSGFException(const std::string &message);
        InvalidSGFException(const InvalidSGFException& other, const std::string& fileName);
    };

    std::unique_ptr<Tree<sente::Move>> getSGFMoves(const std::string& SGFText);
    sente::Rules getSGFRules(const std::string& SGFText);
    std::unique_ptr<sente::_board> getSGFBoardSize(const std::string& SGFText);

}

#endif //SENTE_SGF_H
