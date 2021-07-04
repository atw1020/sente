//
// Created by arthur wesley on 7/2/21.
//

#include <regex>
#include <sstream>

#include <pybind11/pybind11.h>

#include "include/SGF.h"

namespace py = pybind11;

namespace sente_utils {

    InvalidSGFException::InvalidSGFException(const std::string &message)
    : std::domain_error(message){}

    InvalidSGFException::InvalidSGFException(const InvalidSGFException &other, const std::string &fileName)
    : std::domain_error(std::string(other.what()) + " in file: " + fileName) {}

    Tree<sente::Move> getSGFMoves(const std::string& SGFText){

        auto moves = Tree<sente::Move>();
        std::string currentSegment;
        auto previousIndex = SGFText.begin();
        std::regex moveRegex(";\\s*[WB]\\[[a-t]*\\]");

        // for each letter in the SGF
        for (auto cursor = SGFText.begin(); cursor < SGFText.end(); cursor++){

            if (*cursor == '('){

                // find the moves in the current segment
                currentSegment = std::string(previousIndex, cursor);
                sente::Move temp;

                bool noMatches = true;

                // go through all the matches
                for (auto iter = std::sregex_iterator(currentSegment.begin(), currentSegment.end(), moveRegex);
                     iter != std::sregex_iterator(); iter++){
                    // insert the move but don't advance to the new node
                    temp = sente::Move(iter->str());
                    moves.insertNoStep(temp);
                    noMatches = false;
                }

                if (not noMatches){
                    // step into the last match
                    moves.stepTo(temp);
                }

                // update the previous index
                previousIndex = cursor;
            }
            else if (*cursor == ')'){

                // find the moves in the current segment
                currentSegment = std::string(previousIndex, cursor);
                sente::Move temp;

                // go through all the matches
                for (auto iter = std::sregex_iterator(currentSegment.begin(), currentSegment.end(), moveRegex);
                     iter != std::sregex_iterator(); iter++){
                    // insert the move but don't advance to the new node
                    temp = sente::Move(iter->str());
                    moves.insertNoStep(temp);
                }

                if (not moves.isAtRoot()){
                    // step up a move if we see a closing parentheses
                    moves.stepUp();
                }

                // update the previous index
                previousIndex = cursor;
            }

        }

        moves.advanceToRoot();

        return moves;

    }

    sente::Rules getSGFRules(const std::string& SGFText){

        // py::print("the full SGF text was", SGFText);

        using namespace std::regex_constants;

        // find the rules and board size to initialize the game
        std::smatch result;
        std::regex rulesRegex("RU\\[((Chinese|Japanese))\\]", icase);

        // extract the rules from the string
        std::regex_search(SGFText, result, rulesRegex);

        std::string temp = result[0].str();
        std::string rulesString(temp.begin() + 3, temp.end() - 1);// create the board from the results

        // convert the rules to lowercase
        std::transform(rulesString.begin(), rulesString.end(), rulesString.begin(), ::tolower);

        if (rulesString == "japanese"){
            return sente::JAPANESE;
        }
        else if (rulesString == "chinese"){
            return sente::CHINESE;
        }
        else {
            throw InvalidSGFException("Could not determine Rules");
        }

    }
    std::unique_ptr<sente::_board> getSGFBoardSize(const std::string& SGFText){

        std::smatch result;
        std::regex boardSizeRegex("SZ\\[\\d{1,2}\\]");

        // extract the board size from the string
        std::regex_search(SGFText, result, boardSizeRegex);

        std::string temp = result[0].str();

        unsigned side = std::stol(std::string(temp.begin() + 3, temp.end() - 1));

        switch (side){
            case 19:
                return std::unique_ptr<sente::Board<19>>(new sente::Board<19>());
            case 13:
                return std::unique_ptr<sente::Board<13>>(new sente::Board<13>());
            case 9:
                return std::unique_ptr<sente::Board<9>>(new sente::Board<9>());
            default:
                throw std::domain_error("Invalid Board size " + std::to_string(side));
        }

    }

    void insertIntoSGF(Tree<sente::Move>& moves, std::stringstream& SGF){
        // for each child
        for (auto& child : moves.getChildren()){
            // serialize the move
            SGF << ";" << std::string(child);
            // step to the child
            moves.stepTo(child);
            if (not moves.isAtLeaf()){
                SGF << "(";
                insertIntoSGF(moves, SGF);
                SGF << ")";
            }
            // step up
            moves.stepUp();
        }
    }

    std::string toSGF(Tree<sente::Move> moves, std::unordered_map<std::string, std::string>& attributes){

        // add some default attributes
        attributes["FF"] = "4";

        // create the string stream to use
        std::stringstream SGF;

        // backup the current position of the board and advance to the root node
        auto moveSequence = moves.getSequence();
        moves.advanceToRoot();

        SGF << "(";

        if (not attributes.empty()){
            SGF << ";";
        }

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
