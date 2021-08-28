//
// Created by arthur wesley on 8/27/21.
//

#include <sstream>

#include "../Include/Utils/SGFNode.h"

namespace sente {
    namespace utils {

        SGFNode::SGFNode(const Move &move) {
            this->move = move;
        }

        Move SGFNode::getMove() const {
            return move;
        }

        void SGFNode::addCommand(SGFCommand command, const std::string &value) {
            payload[command] = value;
        }

        std::string SGFNode::removeCommand(SGFCommand command) {
            std::string result = payload[command];
            payload.erase(command);
            return result;
        }

        bool SGFNode::hasCommand(SGFCommand command) const {
            return payload.find(command) != payload.end();
        }

        std::string SGFNode::getCommand(SGFCommand command) const {
            return payload.at(command);
        }

        SGFNode::operator std::string() const {

            std::stringstream acc;

            if (move != Move()){
                acc << std::string(move) << std::endl;
            }

            for (const auto& command : payload){
                acc << toStr(command.first) << "[" << command.second << "]" << std::endl;
            }

            return acc.str();
        }

        bool SGFNode::operator==(const SGFNode &other) const {
            return move == other.move;
        }
    }
}