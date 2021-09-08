//
// Created by arthur wesley on 8/27/21.
//

#ifndef SENTE_SGFNODE_H
#define SENTE_SGFNODE_H

#include "SGFProperty.h"
#include "../Game/Move.h"

namespace sente {
    namespace utils {

        class SGFNode {
        public:

            explicit SGFNode() = default;
            explicit SGFNode(const Move& move);

            Move getMove() const;

            void addCommand(SGFProperty command, const std::string& value);
            void setCommand(SGFProperty command, const std::vector<std::string>& value);

            std::vector<std::string> removeCommand(SGFProperty command);

            bool hasCommand(SGFProperty command) const;
            bool isEmpty() const;
            std::vector<SGFProperty> getInvalidCommands(unsigned version) const;

            std::unordered_map<SGFProperty, std::vector<std::string>> getAttributes() const;

            std::vector<std::string> getCommand(SGFProperty command) const;

            explicit operator std::string() const;
            bool operator==(const SGFNode& other) const;

        private:

            Move move;
            std::unordered_map<SGFProperty, std::vector<std::string>> attributes;

        };
    }
}


#endif //SENTE_SGFNODE_H
