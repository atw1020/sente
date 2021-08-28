//
// Created by arthur wesley on 8/27/21.
//

#ifndef SENTE_SGFNODE_H
#define SENTE_SGFNODE_H

#include "SGFCommands.h"
#include "../Game/Move.h"

namespace sente {
    namespace utils {

        class SGFNode {
        public:

            SGFNode() = default; // root node constructor
            explicit SGFNode(const Move& move); // create from a move

            Move getMove() const;

            void addCommand(SGFCommand command, const std::string& value);
            std::string removeCommand(SGFCommand command);

            bool hasCommand(SGFCommand command) const;
            std::string getCommand(SGFCommand command) const;

            explicit operator std::string() const;

        private:

            Move move;
            std::unordered_map<SGFCommand, std::string> payload;

        };
    }
}


#endif //SENTE_SGFNODE_H
