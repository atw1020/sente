//
// Created by arthur wesley on 7/2/21.
//

#include <stack>
#include <regex>
#include <sstream>

#include <pybind11/pybind11.h>

#include "SGF.h"
#include "../SenteExceptions.h"

namespace py = pybind11;

std::string strip(const std::string &input)
{

    auto start_it = input.begin();
    auto end_it = input.end();
    while (std::isspace(*start_it) and start_it != end_it)
        start_it++;
    while (std::isspace(*end_it) and start_it != end_it){
        end_it--;
    }

    return {start_it, end_it};
}

namespace sente::SGF {

    void warn(const std::string& message){
        // acquire the GIL
        py::gil_scoped_acquire acquire;
        PyErr_WarnEx(PyExc_Warning, message.c_str(), 1);
    }

    void addChildrenSGFProperties(utils::Tree<SGFNode>& SGFTree, std::unordered_set<SGFProperty>& properties){

        // add all the properties from this Node
        std::unordered_map<SGFProperty, std::vector<std::string>> currentProperties = SGFTree.get().getProperties();

        for (const auto& item : currentProperties){
            properties.insert(item.first);
        }

        // add all the children's properties
        for (auto& child : SGFTree.getChildren()){
            SGFTree.stepTo(child);
            addChildrenSGFProperties(SGFTree, properties);
            SGFTree.stepUp();
        }

    }

    std::unordered_set<SGFProperty> getTreeProperties(utils::Tree<SGFNode>& SGFTree){

        // record the current sequence of moves to get to this node
        std::vector<SGFNode> currentSequence = SGFTree.getSequence();

        // go to the root of the tree
        SGFTree.advanceToRoot();

        // add all the properties from all the nodes
        std::unordered_set<SGFProperty> properties;
        addChildrenSGFProperties(SGFTree, properties);

        // return to the original position
        SGFTree.advanceToRoot();
        for (auto& item : currentSequence){
            SGFTree.stepTo(item);
        }

        return properties;

    }

    void handleUnknownSGFProperty(const std::string& unknownProperty, bool disableWarnings,
                                                                      bool ignoreIllegalProperties) {

        std::string message = "Unknown SGF Property: \"" + unknownProperty + "\"";

        if (ignoreIllegalProperties) {
            // if we are ignoring illegal properties, warn the user (unless
            if (not disableWarnings){
                warn(message);
            }
        }
        else {
            throw utils::InvalidSGFException(message);
        }
    }

    void handleUnsupportedProperty(utils::Tree<SGFNode>& SGFTree, unsigned& FFVersion, bool disableWarnings,
                                   bool fixFileFormat) {

        unsigned oldFF = FFVersion;

        auto offendingProperty = SGFTree.get().getInvalidProperties(FFVersion)[0];

        if (fixFileFormat){
            // fix the file format if we can
            std::unordered_set<SGFProperty> properties = getTreeProperties(SGFTree);
            auto possibleVersions = getPossibleSGFVersions(properties);

            // update to the latest possible File format
            if (not possibleVersions.empty()){
                FFVersion = *std::max_element(possibleVersions.begin(),  possibleVersions.end());
            }

        }

        if (SGFTree.get().getInvalidProperties(FFVersion).empty()){

            // if the file format was fixed, issue a warning to the user if they have warnings enabled
            if (not disableWarnings){

                std::string message = "The Property \"" +
                                      toStr(offendingProperty) +
                                      "\" is not supported on this version of SGF (FF[" +
                                      std::to_string(oldFF) + "])\nThe file was automatically converted to FF[" +
                                      std::to_string(FFVersion) + "]";

                warn(message);
            }
        }
        else {
            throw utils::InvalidSGFException("The Property \"" +
                                      toStr(offendingProperty) +
                                      "\" is not supported on this version of SGF (FF[" +
                                      std::to_string(FFVersion) + "])");
        }
    }

    SGFNode nodeFromText(const std::string& SGFText, bool disableWarnings,
                                                     bool ignoreIllegalProperties){

//        std::cout << "entering nodeFromText with text \"" << SGFText << "\"" << std::endl;

        SGFNode node;
        std::string temp;

        SGFProperty lastProperty = NONE;

        auto cursor = SGFText.begin();
        auto previousSlice = cursor;

        bool inBrackets = false;

        // initialize the tree from the first item
        for (; cursor < SGFText.end(); cursor++) {
            switch (*cursor){
                case '[':

                    if (not inBrackets){
                        // slice out the property
                        temp = strip(std::string(previousSlice, cursor));

                        // only make a new property if a new property exists
                        if (not temp.empty()){
                            if (isProperty(temp)){
                                lastProperty = fromStr(strip(temp));
                            }
                            else {
                                handleUnknownSGFProperty(temp, disableWarnings, ignoreIllegalProperties);
                                lastProperty = NONE;
                            }
                        }

                        // update the index of the previous slice
                        previousSlice = cursor + 1;
                    }

                    inBrackets = true;

                    break;
                case ']':

                    // slice out the argument of the property
                    temp = strip(std::string(previousSlice, cursor));

                    // if the "last property" value is set, then add the property
                    // otherwise do nothing
                    if (lastProperty != NONE){
                        node.appendProperty(lastProperty, temp);
                    }

                    inBrackets = false;

                    // update the index of the previous slice
                    previousSlice = cursor + 1;
                    break;
                case '\\':
                    // skip the next character
                    cursor++;
                    break;
                default:
                    break;
            }
        }

        return node;

    }

    void insertNode(utils::Tree<SGFNode>& SGFTree,
                    const std::string& nodeText,
                    bool& firstNode,
                    unsigned& FFVersion,
                    bool disableWarnings,
                    bool ignoreIllegalProperties,
                    bool fixFileFormat){

//        std::cout << "entering insertNode with text \"" << nodeText << "\"" << std::endl;

        SGFNode tempNode;

        if (not nodeText.empty()) {
            // add the property prior to this one
            tempNode = nodeFromText(nodeText, disableWarnings, ignoreIllegalProperties);

            if (firstNode){
                SGFTree = utils::Tree<SGFNode>(tempNode);
                firstNode = false;
                if (SGFTree.get().hasProperty(FF)){
                    FFVersion = std::stoi(SGFTree.get().getProperty(FF)[0]);
                }
                else {
                    // the file format must be FF[1] because it's not specified
                    FFVersion = 1;
                }
            }
            else {
                SGFTree.insert(tempNode);
            }
            // validate the result with the file format version
            if (not SGFTree.get().getInvalidProperties(FFVersion).empty()){
                handleUnsupportedProperty(SGFTree, FFVersion, disableWarnings, fixFileFormat);
            }
        }
    }

    utils::Tree<SGFNode> loadSGF(const std::string& SGFText, bool disableWarnings,
                                                      bool ignoreIllegalProperties,
                                                      bool fixFileFormat){

        if (SGFText.empty()){
            throw utils::InvalidSGFException("File is Empty or unreadable");
        }

        bool firstNode = true;
        bool inBrackets = false;
        // skip inserting nodes if a previous step inserted them
        bool nodeAddedWithParentheses = true;

        // temporary string variable
        std::string temp;

        auto cursor = SGFText.begin();
        auto nodeStart = cursor;

        unsigned FFVersion;

        std::stack<unsigned> branchDepths{};

        utils::Tree<SGFNode> SGFTree;

        // go through the rest of the tree

        for (; cursor < SGFText.end(); cursor++){
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
                        // TODO: add highlighting
                        throw utils::InvalidSGFException("Extra Closing Bracket");
                    }
                    break;
                case '\\':
                    cursor++;
                    break;
                case '(':
                    if (not inBrackets){

                        // insert a node if we need to
                        temp = strip(std::string(nodeStart, cursor));
                        insertNode(SGFTree, temp, firstNode, FFVersion, disableWarnings, ignoreIllegalProperties, fixFileFormat);

                        // we've added a node with closing parentheses
                        nodeAddedWithParentheses = true;
                        nodeStart = cursor + 1;

                        // push how deep we currently are in the tree onto a stack to record how far we have to
                        // step up to get out of the branch
                        branchDepths.push(SGFTree.getDepth());
                    }
                    break;
                case ')':
                    if (not inBrackets){

                        // insert a node if we need to
                        temp = strip(std::string(nodeStart, cursor));
                        insertNode(SGFTree, temp, firstNode, FFVersion, disableWarnings, ignoreIllegalProperties, fixFileFormat);

                        // we've added a node with closing parentheses
                        nodeAddedWithParentheses = true;
                        nodeStart = cursor + 1;

                        // update the depth
                        if (not branchDepths.empty()){
//                            std::cout << "popping a depth of " << branchDepths.top() << std::endl;
                            // step up until we reach the previous branch depth
                            while (SGFTree.getDepth() > branchDepths.top()){
                                SGFTree.stepUp();
                            }
                            branchDepths.pop();
                        }
                        else {
                            throw utils::InvalidSGFException("unmatched closing parentheses");
                        }
                    }
                    break;
                case ';':
                    if (not inBrackets){

//                        std::cout << "hit a semicolon" << std::endl;

                        // if we aren't on the first node, we should insert the previous chunk of text
                        if (not nodeAddedWithParentheses){
                            temp = strip(std::string(nodeStart, cursor));
                            insertNode(SGFTree, temp, firstNode, FFVersion, disableWarnings, ignoreIllegalProperties, fixFileFormat);
                        }

                        // seeing a semicolon means that we are about to see a node
                        nodeAddedWithParentheses = false;
                        nodeStart = cursor + 1;
                    }
                    break;
            }
        }

        if (firstNode){
            throw utils::InvalidSGFException("Unable to find any SGF nodes in file");
        }

        if (SGFTree.getDepth() != 0){
            throw utils::InvalidSGFException("Missing Closing parentheses");
        }

        // make sure that the game we loaded is a go game
        if (SGFTree.get().hasProperty(GM)){
            if (SGFTree.get().getProperty(GM)[0] != "1"){
                throw utils::InvalidSGFException("Game is not a Go Game (Sente only parses Go Games)");
            }
        }

        return SGFTree;

    }

    void insertIntoSGF(utils::Tree<SGFNode>& moves, std::stringstream& SGF){

        // insert the current node
        SGF << ";" << std::string(moves.get());

        if (moves.getDepth() == 0){
            SGF << std::endl;
        }

        for (auto& child : moves.getChildren()){
            if (moves.getChildren().size() != 1){
                SGF << "\n(";
            }
            if (not child.getMove().isResign()){

                // step to the child
                moves.stepTo(child);
                // insert the child into the SGF
                insertIntoSGF(moves, SGF);
                // step up
                moves.stepUp();
            }
            if (moves.getChildren().size() != 1){
                SGF << ")";
            }
        }

    }

    std::string dumpSGF(const GoGame& game){

        std::stringstream ss;

        // get the tree and advance to the root
        auto tree = game.getMoveTree();
        tree.advanceToRoot();

        ss << "(";

        insertIntoSGF(tree, ss);

        ss << ")";

        return ss.str();
    }
}

