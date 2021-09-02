//
// Created by arthur wesley on 8/27/21.
//

#include <sstream>
#include <pybind11/pybind11.h>

#include "../Include/Utils/SGFNode.h"
#include "../Include/Utils/SenteExceptions.h"

namespace sente {
    namespace utils {

        std::vector<SGFCommand> precedenceOrder {
                NONE,
                /// Root Properties
                AP, // application used to create the file 
                CA, // charset 
                FF, // file format
                GM, // Game mode
                ST, // how many variations should be shown 
                SZ, // size of the board
                /// Game info Properties
                AN, // name of annotator 
                BR, // black rank
                BT, // black's team 
                CP, // copyright info 
                DT, // date the game was played 
                EV, // event 
                GN, // game name
                GC, // extra game info 
                ON, // opening name 
                OT, // overtime 
                PB, // black name
                PC, // place 
                PW, // white player's name
                RE, // result of the game
                RO, // round number & type 
                RU, // ruleset
                SO, // source 
                TM, // time info 
                US, // creator of the game 
                WR, // white rank
                WT, // white team 
                /// Go-specific properties
                HA, // handicap
                KM, // komi
                TB, // territory black
                TW, // territory white
                /// data taken from https://www.red-bean.com/sgf/properties.html
                /// Move properties
                B,  // black move
                W,  // white move
                MN, // Move number 
                KO, // ko point 
                /// Setup Properties
                AB, // add black stone 
                AE, // add empty space 
                AW, // add white stone 
                PL, // get player name 
                /// Node Annotations
                C,  // comment 
                DM, // position is even for both players 
                GB, // good for black to play 
                GW, // good for white to play 
                HO, // hot spot 
                N,  // name of the node 
                UC, // position is unclear 
                V,  // value 
                /// Move Annotation
                BM, // bad move 
                DO, // doubtful move 
                IT, // interesting move 
                TE, // tesuji 
                /// Markup properties
                AR, // arrow 
                CR, // circle 
                DD, // dim out 
                LB, // label 
                LN, // line 
                MA, // mark with an x 
                SL, // selected points 
                SQ, // mark with a square 
                TR, // mark with a triangle 
                /// timing properties
                BL, // black time left 
                OB, // number of byo-yomi periods left black 
                OW, // number of byo-yomi periods left white 
                WL, // white time left 
                /// misc
                FG, // figure 
                PM, // print move count 
                VW, // only view part of the board
                /// SGF-3 specific properties
                ID, // unique ID of the game 
                LT, // enforce losing on time 
                OM, // number of moves for each overtime 
                OP, // length of each overtime period 
                OV, // operator overhead for each move 
                SE, // moves tried so far 
                SI, // position marked with sigma 
                TC, // territory count 
                /// SGF-1 specific properties
                EL, // human evaluation of a computer's move 
                EX, // expected next move 
                L,  // letters on points 
                M,  // marked points 
                BS, // black species 
                CH, // check mark 
                RG, // region of the board 
                SC, // secure stones 
                WS, // white species 
        };

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

        void SGFNode::setCommand(SGFCommand command, const std::vector<std::string> &value) {
            if (command == B or command == W){
                // the move must contain either
                if (value.empty()){
                    move = Move::pass(command == B ? BLACK : WHITE);
                }
                else {
                    // make sure the value is valid
                    if (value.size() != 2){
                        throw InvalidSGFException(std::string("invalid move \"") + (command == B ? "B" : "W") + "[" + value[0] + "]");
                    }
                    // get the co-ordinates from the move
                    move = {unsigned(value[0][1] - 'a'), unsigned(value[0][0] - 'a'), command == B ? BLACK : WHITE};
                }
            }
            else {
                attributes[command] = value;
            }
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

        std::vector<SGFCommand> SGFNode::getInvalidCommands(unsigned version) const{

            std::vector<SGFCommand> illegalCommands;

            for (const auto& attribute : attributes){
                if (not isSGFLegal(attribute.first, version)){
                    illegalCommands.push_back(attribute.first);
                }
            }

            return illegalCommands;
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

            for (const auto& command : precedenceOrder){
                if (attributes.find(command) != attributes.end()){
                    acc << toStr(command);
                    for (const auto& entry : attributes.at(command)){
                        acc << "[" << entry << "]";
                    }
                }
            }

            return acc.str();
        }

        bool SGFNode::operator==(const SGFNode &other) const {
            return move == other.move;
        }
    }
}