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

        void insertIntoSGF(Tree<Move>& moves, std::stringstream& SGF){
            // for each child

            if (moves.getChildren().size() == 1){

                // if there is exactly one child, we don't need to add parentheses
                auto child = *moves.getChildren().begin();

                // serialize the move
                SGF << ";" << std::string(child);

                // step to the child
                moves.stepTo(child);

                // recurse
                insertIntoSGF(moves, SGF);

                moves.stepUp();

            }
            else {
                for (auto& child : moves.getChildren()){
                    SGF << "\n(";
                    if (not child.isResign()){

                        // serialize the move
                        SGF << ";" << std::string(child);
                        // step to the child
                        moves.stepTo(child);
                        if (not moves.isAtLeaf()){
                            insertIntoSGF(moves, SGF);
                        }
                        // step up
                        moves.stepUp();
                    }
                    SGF << ")";
                }
            }

        }

        std::string toSGF(Tree<Move> moves, std::unordered_map<std::string, std::string>& attributes){

            // create the string stream to use
            std::stringstream SGF;

            attributes.erase("FF");
            attributes.erase("GM");
            attributes.erase("CA");

            // make sure that all of the attributes are legal
            for (const auto& attribute : attributes){
                if (attribute.first.size() != 2 or not std::all_of(attribute.first.begin(), attribute.first.end(),
                                                                   [](const char c){ return std::isupper(c);})) {
                    throw std::domain_error("Invalid Attribute \"" + attribute.first + "\"");
                }
            }

            // backup the current position of the board and advance to the root node
            auto moveSequence = moves.getSequence();
            moves.advanceToRoot();

            // add the file format as the first parameter
            SGF << "(;FF[4]GM[1]CA[UTF-8]\n"; // TODO: make sure that it is safe to assume the file we create will be UTF-8

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

        /**
         *
         * extracts metadata from a SGF file
         *
         * @param SGFText
         * @return
         */
        std::unordered_map<std::string, std::string> getMetadata(const std::string& SGFText){

            // create the regular expression for the metadata
            std::regex metadataRegex("[A-Z]{2}\\[[^\\]]*\\]");

            // create the return object
            std::unordered_map<std::string, std::string> metadata;

            // go through all of the matches
            for (auto regexIter = std::sregex_iterator(SGFText.begin(), SGFText.end(), metadataRegex);
                 regexIter != std::sregex_iterator(); regexIter++){
                // extract the match string
                std::string match = regexIter->str();

                // slice the components apart
                std::string key = std::string(match.begin(), match.begin() + 2);
                std::string value = std::string(match.begin() + 3, match.end() - 1);

                // return the metadata
                metadata[key] = value;

            }

            // add some default values
            if (metadata.find("RU") == metadata.end()){
                metadata["RU"] = "Chinese";
            }
            if (metadata.find("SZ") == metadata.end()){
                metadata["SZ"] = "19";
            }
            if (metadata.find("GM") == metadata.end()){
                metadata["GM"] = "1";
            }

            // remove non metadata labels
            metadata.erase("SQ"); // square
            metadata.erase("TR"); // triangle
            metadata.erase("CR"); // circle
            metadata.erase("LB"); // label

            return metadata;

        }

    }
}

