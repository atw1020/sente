//
// Created by arthur wesley on 7/2/21.
//

#include <stack>
#include <regex>
#include <sstream>

#include <pybind11/pybind11.h>

#include "../Include/Utils/SGF.h"
#include "../Include/Utils/SenteExceptions.h"

namespace py = pybind11;

std::string strip(const std::string &input)
{

    // py::print("entering strip with \"" + input + "\"");
    // py::print("size: ", input.size());

    auto start_it = input.begin();
    auto end_it = input.end();
    while (std::isspace(*start_it) and start_it != end_it)
        start_it++;
    while (std::isspace(*end_it) and start_it != end_it){
        end_it--;
    }

    /*
    py::print("\"", *start_it, *end_it, "\"");
    py::print("\"" + std::string(start_it, end_it) + "\"");
    py::print("leaving strip");
     */

    return {start_it, end_it};
}

namespace sente {
    namespace utils {

        bool isSGFParam(const std::string& toCheck){

            // go through the leading characters
            auto iter = toCheck.begin();

            while (*(iter++) != '['){
                if (not isupper(*iter)){
                    return false;
                }
            }

            // go through the bracket characters
            while (*(iter++) != ']'){
                if (*iter == '\\' and *(iter + 1) == ']'){
                    // skip over the character if we get \]
                    iter += 2;
                }
            }

            return iter + 1 == toCheck.end();

        }

        void checkSGFValidity(const std::string& SGFText){

            int parenDepth = 0;
            int bracketDepth = 0;

            unsigned valid_chars = 0;

            bool oneMatch = false;

            auto lastBreak = SGFText.begin();

            for (auto iter = SGFText.begin(); iter != SGFText.end(); iter++){

                // py::print(std::string(lastBreak + 1, iter + 1));
                // py::print("bracket depth was", bracketDepth);

                if (isspace(*iter) and bracketDepth == 0){
                    lastBreak = iter;
                    valid_chars++;
                    continue;
                }
                switch (*iter){
                    case '\\':
                        iter++;
                        continue;
                    case ';':
                        if (bracketDepth == 0){
                            lastBreak = iter;
                            valid_chars++;
                        }
                        break;
                    case '(':
                        if (bracketDepth == 0){
                            parenDepth++;
                            valid_chars++;
                        }
                        break;
                    case ')':
                        if (bracketDepth == 0){
                            parenDepth--;
                            valid_chars++;
                        }
                        break;
                    case '[':
                        bracketDepth++;
                        break;
                    case ']':
                        bracketDepth--;

                        // slice out the last segment
                        std::string temp(lastBreak + 1, iter + 1);

                        if (isSGFParam(temp)){
                            throw InvalidSGFException("Invalid SGF field: " + temp);
                        }

                        oneMatch = true;

                        valid_chars += temp.size();

                        // update the lest segment
                        lastBreak = iter;
                        break;
                }

                // TODO: add highlighting of the syntax errors

                if (not (bracketDepth == 0 or bracketDepth == 1)){
                    throw InvalidSGFException("invalid brackets");
                }
                if (parenDepth < 0){
                    throw InvalidSGFException("invalid parentheses");
                }

            }

            if (not oneMatch){
                throw InvalidSGFException("File was not an SGF file");
            }

            if (valid_chars != SGFText.size()){
                throw InvalidSGFException("File did not perfectly match SGF format");
            }

        }

        SGFNode nodeFromText(const std::string& SGFText){

            // py::print("entering nodeFromText with \"" + SGFText + "\"");

            SGFNode node;
            std::string temp;

            SGFCommand lastCommand;

            auto cursor = SGFText.begin();
            auto previousSlice = cursor;

            // initialize the tree from the first item
            for (; cursor < SGFText.end(); cursor++) {
                switch (*cursor){
                    case '[':

                        // slice out the command
                        temp = strip(std::string(previousSlice, cursor));

                        // only make a new command if a new command exists
                        if (not temp.empty()){
                            if (isCommand(temp)){
                                lastCommand = fromStr(strip(temp));
                            }
                            else {
                                throw InvalidSGFException("Unknown SGF Directive: \"" + temp + "\"");
                            }
                        }
                        else {
                            // py::print("putting in \"" + temp + "\" for \"" + toStr(lastCommand));
                            node.addCommand(lastCommand, temp);
                        }

                        // update the index of the previous slice
                        previousSlice = cursor + 1;

                        break;
                    case ']':

                        // slice out the argument of the command
                        temp = strip(std::string(previousSlice, cursor));

                        // add the command
                        if (lastCommand == NONE){
                            throw InvalidSGFException("No Command listed: " + std::string(SGFText.begin(), cursor));
                        }
                        else {
                            // py::print("putting in \"" + temp + "\" for \"" + toStr(lastCommand));
                            node.addCommand(lastCommand, temp);
                        }

                        // update the index of the previous slice
                        previousSlice = cursor + 1;

                    case '\\':
                        // skip the next character
                        cursor++;
                        break;
                    default:
                        break;
                }
            }

            if (*cursor != ']'){
                // TODO: throw an exception if the last item in the sequence is not a closing bracket
            }

            return node;

        }

        Tree<SGFNode> loadSGF(const std::string& SGFText){

            bool inBrackets = false;
            bool firstNode = true;

            // temporary string variable
            std::string temp;

            auto cursor = SGFText.begin();
            auto previousSlice = cursor;

            std::stack<unsigned> branchDepths{};

            Tree<SGFNode> SGFTree;
            SGFNode tempNode;

            // go through the rest of the tree

            for (; cursor < SGFText.end(); cursor++){
                // py::print("slice is currently \"" + std::string(previousSlice, cursor) + "\"");
                // py::print(SGFTree.getDepth());
                switch (*cursor){
                    case '[':
                        // enter brackets
                        inBrackets = true;
                        break;
                    case ']':
                        // leave brackets
                        if (inBrackets){
                            inBrackets = false;
                        }
                        else {
                            throw InvalidSGFException("Extra Closing Bracket");
                        }
                        break;
                    case '\\':
                        cursor++;
                        break;
                    case '(':
                        if (not inBrackets){
                            temp = strip(std::string(previousSlice, cursor));

                            if (not temp.empty()) {
                                // add the command prior to this one
                                tempNode = nodeFromText(temp);

                                if (firstNode){
                                    SGFTree = Tree<SGFNode>(tempNode);
                                    firstNode = false;
                                }
                                else {
                                    SGFTree.insert(tempNode);
                                }
                            }

                            // with the command added to the tree, the push the depth of the current node onto the stack
                            branchDepths.push(SGFTree.getDepth());

                            // update the previousSlice
                            previousSlice = cursor + 1;
                        }
                        break;
                    case ')':
                        if (not inBrackets){

                            temp = strip(std::string(previousSlice, cursor));

                            if (not temp.empty()) {
                                // add the command prior to this one
                                tempNode = nodeFromText(temp);
                                if (firstNode){
                                    SGFTree = Tree<SGFNode>(tempNode);
                                    firstNode = false;
                                }
                                else {
                                    SGFTree.insert(tempNode);
                                }
                            }

                            // update the previousSlice
                            previousSlice = cursor + 1;

                            // step up until we reach the previous branch depth
                            while (SGFTree.getDepth() > branchDepths.top()){
                                SGFTree.stepUp();
                            }

                            // update the depth
                            if (not branchDepths.empty()){
                                branchDepths.pop();
                            }
                            else {
                                throw InvalidSGFException("extra closing parentheses");
                            }

                        }
                        break;
                    case ';':
                        if (not inBrackets){
                            if (previousSlice + 1 < cursor){
                                // get the node from the text
                                tempNode = nodeFromText(strip(std::string(previousSlice, cursor)));
                                if (firstNode){
                                    SGFTree = Tree<SGFNode>(tempNode);
                                    firstNode = false;
                                }
                                else {
                                    SGFTree.insert(tempNode);
                                }
                            }

                            // update the previousSlice
                            previousSlice = cursor + 1;
                            break;
                        }
                }
            }

            if (SGFTree.getDepth() != 0){
                throw InvalidSGFException("Missing Closing parentheses");
            }

            // make sure that the game we loaded is a go game
            if (SGFTree.get().hasCommand(GM)){
                if (SGFTree.get().getCommand(GM)[0] != "1"){
                    throw InvalidSGFException("Game is not a Go Game (Sente only parses Go Games)");
                }
            }

            return SGFTree;

        }

        void insertIntoSGF(Tree<SGFNode>& moves, std::stringstream& SGF){
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
                    if (not child.getMove().isResign()){

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

        std::string dumpSGF(const GoGame& game, std::unordered_map<std::string, std::string> &attributes){

            // create the string stream to use
            std::stringstream SGF;

            attributes.erase("FF");
            attributes.erase("GM");
            attributes.erase("CA");

            auto gameAttributes = game.getAttributes();

            if (attributes.find("RU") == attributes.end()){
                attributes["RU"] = gameAttributes["RU"];
            }

            if (attributes.find("SZ") != attributes.end()){
                throw std::domain_error("\"SZ\": board size cannot be changed.");
            }
            else {
                attributes["SZ"] = gameAttributes["SZ"];
            }

            if (attributes.find("RE") == attributes.end()){
                if (game.isOver()){
                    attributes["RE"] = game.getResults();
                }
            }

            auto moves = game.getMoveTree();

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
                SGFNode node(move);
                moves.stepTo(node);
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
            metadata.erase("AB"); // add black stone
            metadata.erase("AW"); // add white stone

            return metadata;

        }

    }
}

