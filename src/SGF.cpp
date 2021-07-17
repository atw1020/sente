//
// Created by arthur wesley on 7/2/21.
//

#include <stack>
#include <regex>
#include <sstream>

#include <pybind11/pybind11.h>

#include "include/SGF.h"
#include "include/SenteExceptions.h"

namespace py = pybind11;

namespace sente {
    namespace utils {

        Tree<Move> getSGFMoves(const std::string& SGFText){

            auto moves = Tree<Move>();
            std::string currentSegment;
            auto previousIndex = SGFText.begin();
            std::regex moveRegex(";\\s*[WB]\\[[a-t]*\\]");
            std::stack<unsigned> branchDepths{};

            // for each letter in the SGF
            for (auto cursor = SGFText.begin(); cursor < SGFText.end(); cursor++){

                if (*cursor == '('){

                    // find the moves in the current segment
                    currentSegment = std::string(previousIndex, cursor);
                    auto regexIter = std::sregex_iterator(currentSegment.begin(), currentSegment.end(), moveRegex);

                    // only insert moves if we find one
                    if (regexIter != std::sregex_iterator()){

                        Move temp = Move::fromSGF((regexIter++)->str());

                        // insert the move into the tree and record the step we take
                        branchDepths.push(moves.getDepth());
                        moves.insert(temp);

                        // go through all the matches
                        for (auto iter = regexIter; iter != std::sregex_iterator(); iter++){
                            // insert the move but don't advance to the new node
                            temp = Move::fromSGF(iter->str());
                            moves.insert(temp);
                        }
                    }

                    // update the previous index
                    previousIndex = cursor;
                }
                else if (*cursor == ')'){

                    // find the moves in the current segment
                    currentSegment = std::string(previousIndex, cursor);
                    auto regexIter = std::sregex_iterator(currentSegment.begin(), currentSegment.end(), moveRegex);

                    if (regexIter != std::sregex_iterator()){

                        // if there are any moves, insert them
                        Move temp = Move::fromSGF((regexIter++)->str());

                        branchDepths.push(moves.getDepth());
                        moves.insert(temp);

                        // go through all the matches
                        for (auto iter = regexIter; iter != std::sregex_iterator(); iter++){
                            // insert the move but don't advance to the new node
                            temp = Move::fromSGF(iter->str());
                            moves.insert(temp);
                        }
                    }

                    // unless the branch depths are empty
                    if (not branchDepths.empty()){
                        while (moves.getDepth() != branchDepths.top()){
                            // step up until we reach the depth of the last branch
                            moves.stepUp();
                        }
                        // pop the depth off of the stack once we get it
                        branchDepths.pop();
                    }

                    // update the previous index
                    previousIndex = cursor;
                }

            }

            if (not branchDepths.empty()){
                throw InvalidSGFException("Number of opening parentheses did not match number of closing parentheses");
            }

            return moves;

        }

        Rules getSGFRules(const std::string& SGFText){

            // py::print("the full SGF text was", SGFText);

            using namespace std::regex_constants;

            // find the rules and board size to initialize the game
            std::regex rulesRegex("RU\\[((Chinese|Japanese))\\]", icase);

            // extract the rules from the string
            auto regexIterator = std::sregex_iterator(SGFText.begin(), SGFText.end(), rulesRegex);

            if (regexIterator == std::sregex_iterator()){
                // if there were no Rules, assume Chinese rules
                return CHINESE;
            }
            else {
                std::string temp = regexIterator->str();
                std::string rulesString(temp.begin() + 3, temp.end() - 1);// create the board from the results

                // convert the rules to lowercase
                std::transform(rulesString.begin(), rulesString.end(), rulesString.begin(), ::tolower);

                if (rulesString == "japanese"){
                    return JAPANESE;
                }
                else if (rulesString == "chinese"){
                    return CHINESE;
                }
                else {
                    throw InvalidSGFException("Unknown rule specification \"" + rulesString + "\"");
                }
            }

        }
        std::unique_ptr<_board> getSGFBoardSize(const std::string& SGFText){

            std::regex boardSizeRegex("SZ\\[\\d{1,2}\\]");

            // extract the board size from the string
            auto regexIter = std::sregex_iterator(SGFText.begin(), SGFText.end(), boardSizeRegex);

            if (regexIter == std::sregex_iterator()){
                // we can't find anything so assume 19x19
                return std::unique_ptr<Board<19>>(new Board<19>());
            }
            else {
                std::string temp = regexIter->str();

                unsigned side = std::stol(std::string(temp.begin() + 3, temp.end() - 1));

                switch (side){
                    case 19:
                        return std::unique_ptr<Board<19>>(new Board<19>());
                    case 13:
                        return std::unique_ptr<Board<13>>(new Board<13>());
                    case 9:
                        return std::unique_ptr<Board<9>>(new Board<9>());
                    default:
                        throw std::domain_error("Invalid Board size " + std::to_string(side));
                }
            }

        }

        void insertIntoSGF(Tree<Move>& moves, std::stringstream& SGF){
            // for each child
            for (auto& child : moves.getChildren()){
                if (not child.isResign()){

                    // serialize the move
                    SGF << ";" << std::string(child);
                    // step to the child
                    moves.stepTo(child);
                    if (not moves.isAtLeaf()){
                        SGF << "\n(";
                        insertIntoSGF(moves, SGF);
                        SGF << ")";
                    }
                    // step up
                    moves.stepUp();
                }
            }
        }

        std::string toSGF(Tree<Move> moves, std::unordered_map<std::string, std::string>& attributes){

            // create the string stream to use
            std::stringstream SGF;

            // make sure that all of the attributes are legal
            for (const auto& attribute : attributes){
                if (attribute.first.size() != 2 or not std::all_of(attribute.first.begin(), attribute.first.end(),
                                                                   [](const char c){ return std::isupper(c);})){
                    throw std::domain_error("Invalid Attribute \"" + attribute.first + "\"");
                }
            }

            // backup the current position of the board and advance to the root node
            auto moveSequence = moves.getSequence();
            moves.advanceToRoot();

            SGF << "(";

            // add the file format as the first parameter
            SGF << ";FF[4]\n";

            for (const auto& attribute : attributes){
                SGF << attribute.first << "[" << attribute.second << "]\n";
            }

            insertIntoSGF(moves, SGF);

            SGF << ")";

            // return the board to it's original state
            moves.advanceToRoot();
            for (const auto& move : moveSequence){
                moves.stepTo(move);
            }

            return SGF.str();
        }

    }
}

