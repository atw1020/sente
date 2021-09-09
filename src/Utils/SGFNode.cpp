//
// Created by arthur wesley on 8/27/21.
//

#include <sstream>
#include <pybind11/pybind11.h>

#include "../Include/Utils/SGFNode.h"
#include "../Include/Utils/SenteExceptions.h"

namespace sente {
    namespace utils {

        void replace(std::string& base, const std::string& from, const std::string& to){

            size_t start = 0;
            // if we are replacing empty strings,
            if (from.empty()){
                return;
            }

            while ((start = base.find(from, start)) != std::string::npos){
                base.replace(start, from.size(), to);
                start += to.size();
            }

        }

        std::vector<SGFProperty> precedenceOrder {
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

        void SGFNode::appendProperty(SGFProperty property, const std::string &value) {
            if (property == B or property == W){
                // the move must contain either
                if (value.empty()){
                    move = Move::pass(property == B ? BLACK : WHITE);
                }
                else {
                    // make sure the value is valid
                    if (value.size() != 2){
                        throw InvalidSGFException(std::string("invalid move \"") + (property == B ? "B" : "W") + "[" + value + "]\"");
                    }
                    // get the co-ordinates from the move
                    move = {unsigned(value[1] - 'a'), unsigned(value[0] - 'a'), property == B ? BLACK : WHITE};
                }
            }
            else {
                // replace all the closing brackets "]" with backslash closing bracket "\]"
                std::string copy = value;
                replace(copy, "]", "\\]");
                properties[property].push_back(value);
            }
        }

        void SGFNode::setProperty(SGFProperty property, const std::vector<std::string> &value) {
            if (property == B or property == W){
                // the move must contain either
                if (value[0].empty()){
                    move = Move::pass(property == B ? BLACK : WHITE);
                }
                else {
                    // make sure the value is valid
                    if (value[0].size() != 2){
                        throw InvalidSGFException(std::string("invalid move \"") + (property == B ? "B" : "W") + "[" + value[0] + "]\"");
                    }
                    // get the co-ordinates from the move
                    move = {unsigned(value[0][1] - 'a'), unsigned(value[0][0] - 'a'), property == B ? BLACK : WHITE};
                }
            }
            else {
                std::vector<std::string> copy = value;
                for (auto & item : copy){
                    replace(item, "]", "\\]");
                }
                properties[property] = copy;
            }
        }

        bool SGFNode::hasProperty(SGFProperty property) const {
            return properties.find(property) != properties.end();
        }

        bool SGFNode::isEmpty() const {
            return properties.empty() and move == Move::nullMove;
        }

        std::vector<SGFProperty> SGFNode::getInvalidProperties(unsigned version) const{

            std::vector<SGFProperty> illegalCommands;

            for (const auto& attribute : properties){
                if (not isSGFLegal(attribute.first, version)){
                    illegalCommands.push_back(attribute.first);
                }
            }

            return illegalCommands;
        }

        std::vector<std::string> SGFNode::getProperty(SGFProperty property) const {

            std::vector<std::string> values = properties.at(property);

            for (auto& item : values){
                replace(item, "\\]", "]");
            }

            return values;
        }

        std::unordered_map<SGFProperty, std::vector<std::string>> SGFNode::getProperties() const {
            return properties;
        }

        SGFNode::operator std::string() const {

            std::stringstream acc;

            if (move != Move()){
                acc << std::string(move);
            }

            for (const auto& property : precedenceOrder){
                if (properties.find(property) != properties.end()){
                    acc << toStr(property);
                    for (const auto& entry : properties.at(property)){
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