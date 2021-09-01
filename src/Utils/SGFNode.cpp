//
// Created by arthur wesley on 8/27/21.
//

#include <sstream>
#include <pybind11/pybind11.h>

#include "../Include/Utils/SGFNode.h"
#include "../Include/Utils/SenteExceptions.h"

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
                // the move must contain either
                if (value.empty()){
                    move = Move::pass(command == B ? BLACK : WHITE);
                }
                else {
                    // make sure the value is valid
                    if (value.size() != 2){
                        throw InvalidSGFException(std::string("invalid move \"") + (command == B ? "B" : "W") + "[" + value + "]");
                    }
                    // get the co-ordinates from the move
                    move = {unsigned(value[1] - 'a'), unsigned(value[0] - 'a'), command == B ? BLACK : WHITE};
                }
            }
            else {
                attributes[command].push_back(value);
            }
        }

        void SGFNode::replaceCommand(SGFCommand command, const std::vector<std::string> &replacement) {
            attributes[command] = replacement;
        }

        std::vector<std::string> SGFNode::removeCommand(SGFCommand command) {
            auto result = attributes[command];
            attributes.erase(command);
            return result;
        }

        bool SGFNode::hasCommand(SGFCommand command) const {
            return attributes.find(command) != attributes.end();
        }

        bool SGFNode::isEmpty() const {
            return attributes.empty() and move == Move::nullMove;
        }

        std::vector<std::string> SGFNode::getCommand(SGFCommand command) const {
            return attributes.at(command);
        }

        std::unordered_map<SGFCommand, std::vector<std::string>> SGFNode::getAttributes() const {
            return attributes;
        }

        SGFNode::operator std::string() const {

            std::stringstream acc;

            if (move != Move()){
                acc << std::string(move);
            }

            for (const auto& command : attributes){
                acc << toStr(command.first);
                for (const auto& entry : command.second){
                    acc << "[" << entry << "]";
                }
            }

            return acc.str();
        }

        bool SGFNode::operator==(const SGFNode &other) const {
            return move == other.move;
        }
    }
}