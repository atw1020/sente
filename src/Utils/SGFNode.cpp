//
// Created by arthur wesley on 8/27/21.
//

#include <sstream>
#include <pybind11/pybind11.h>

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
            if (command == B or command == W){
                if (value.empty()){
                    move = Move::pass(command == B ? BLACK : WHITE);
                }
                else {
                    // get the co-ordinates from the move
                    move = {unsigned(value[1] - 'a'), unsigned(value[0] - 'a'), command == B ? BLACK : WHITE};
                }
            }
            else {
                payload[command].push_back(value);
            }
        }

        void SGFNode::replaceCommand(SGFCommand command, const std::vector<std::string> &replacement) {
            payload[command] = replacement;
        }

        std::vector<std::string> SGFNode::removeCommand(SGFCommand command) {
            auto result = payload[command];
            payload.erase(command);
            return result;
        }

        bool SGFNode::hasCommand(SGFCommand command) const {
            return payload.find(command) != payload.end();
        }

        bool SGFNode::isEmpty() const {
            return payload.empty() and move == Move::nullMove;
        }

        std::vector<std::string> SGFNode::getCommand(SGFCommand command) const {
            return payload.at(command);
        }

        SGFNode::operator std::string() const {

            std::stringstream acc;

            if (move != Move()){
                acc << std::string(move) << std::endl;
            }

            for (const auto& command : payload){
                acc << toStr(command.first);
                for (const auto& entry : command.second){
                    acc << "[" << entry << "]";
                }
                acc << std::endl;
            }

            return acc.str();
        }

        bool SGFNode::operator==(const SGFNode &other) const {
            return move == other.move;
        }
    }
}