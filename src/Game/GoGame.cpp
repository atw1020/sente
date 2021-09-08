//
// Created by arthur wesley on 6/28/21.
//

#include <regex>
#include <iostream>

// #Include <pybind11/pybind11.h>

#include "../Include/Game/GoGame.h"
#include "../Include/Game/LifeAndDeath.h"
#include "../Include/Utils/SenteExceptions.h"

namespace std {

    size_t hash<pair<unsigned, unsigned>>::operator()(const pair<unsigned int, unsigned int> &toHash) const noexcept {

        size_t hash1 = hash<unsigned>()(toHash.first);
        size_t hash2 = hash<unsigned>()(toHash.second);

        return hash1 xor (hash2 << 1);

    }

}

namespace sente {

    GoGame::GoGame(unsigned side, Rules rules, double komi) {

        // default Komi values
        if (komi == INFINITY){
            this->komi = getKomi(rules);
        }
        else {
            this->komi = komi;
        }

        this->rules = rules;

        makeBoard(side);
        resetKoPoint();

        // create the rootnode
        utils::SGFNode rootNode;

        // add the defualt metadata
        rootNode.setCommand(utils::FF, {"4"});
        rootNode.setCommand(utils::SZ, {std::to_string(side)});

        switch (rules){
            case CHINESE:
                rootNode.setCommand(utils::RU, {"Chinese"});
                break;
            case JAPANESE:
                rootNode.setCommand(utils::RU, {"Japanese"});
                break;
        }

        gameTree = utils::Tree<utils::SGFNode>(rootNode);

    }

    GoGame::GoGame(utils::Tree<utils::SGFNode> &SGFTree) {
        gameTree = SGFTree;

        auto rootNode = gameTree.getRoot();

        if (rootNode.hasCommand(utils::SZ)){
            // parse if available
            makeBoard(std::stoi(rootNode.getCommand(utils::SZ)[0]));
        }
        else {
            // default board size
            makeBoard(19);
        }

        if (rootNode.hasCommand(utils::RU)){

            std::string ruleString = rootNode.getCommand(utils::RU)[0];
            rules = rulesFromStr(ruleString);
        }
        else {
            rules = CHINESE; // default
        }

        if (rootNode.hasCommand(utils::KM)){
            if (rootNode.getCommand(utils::KM)[0].empty()){
                komi = 0;
            }
            else {
                komi = std::stod(rootNode.getCommand(utils::KM)[0]);
            }
        }
        else {
            komi = getKomi(rules);
        }
    }

    /**
     *
     * resets the board to be empty
     *
     */
    void GoGame::resetBoard(){

        // create a new board
        makeBoard(board->getSide());
        // reset the tree to the root
        gameTree.advanceToRoot();

        // set the groups and captures to be empty
        groups = std::unordered_map<Move, std::shared_ptr<Group>>();
        capturedStones = std::unordered_map<unsigned, std::unordered_set<Move>>();

        // reset the ko point
        resetKoPoint();
        passCount = 0;

    }

    bool GoGame::isLegal(unsigned x, unsigned y) const{
        return isLegal(Move(x, y, gameTree.getDepth() % 2 == 0 ? BLACK : WHITE));
    }

    bool GoGame::isLegal(unsigned int x, unsigned int y, Stone stone) const {
        return isLegal(Move(x, y, stone));
    }

    bool GoGame::isLegal(const Move& move) const {
        // std::cout << "passed isOver" << std::endl;
        if (not board->isOnBoard(move)){
            return false;
        }
        // std::cout << "passed isOnBoard" << std::endl;
        bool isEmpty = board->isEmpty(move);
        // std::cout << "passed isEmpty" << std::endl;
        bool notSelfCapture = isNotSelfCapture(move);
        // std::cout << "passed isNotSelfCapture" << std::endl;
        bool notKoPoint = isNotKoPoint(move);
        // std::cout << "passed isNotKoPoint" << std::endl;
        bool correctColor = isCorrectColor(move);
        // std::cout << "passed isCorrectColor" << std::endl;

        // std::cout << "leaving isLegal" << std::endl;

        return isEmpty and notSelfCapture and notKoPoint and correctColor;
    }

    void GoGame::playStone(unsigned x, unsigned y){
        playStone(Move(x, y, gameTree.getDepth() % 2 == 0 ? BLACK : WHITE));
    }

    void GoGame::playStone(unsigned int x, unsigned int y, Stone stone) {
        playStone(Move(x, y, stone));
    }

    /**
     *
     * plays the specified move on the board
     *
     * @param move move to play
     */
    void GoGame::playStone(const Move &move) {

        // create a new SGF node
        utils::SGFNode node(move);

        // check for pass/resign
        if (move.isPass()){
            gameTree.insert(node);
            if (++passCount >= 2){
                gameTree.getRoot().setCommand(utils::RE, {});
            }
            return;
        }
        else {
            passCount = 0;
        }

        if (move.isResign()){
            // get the root node
            if (gameTree.getRoot().hasCommand(utils::RE)){
                // if the game has been resigned raise an exception
                throw std::domain_error("Game cannot be forfeited; the game is already over");
            }
            else {
                gameTree.getRoot().setCommand(utils::RE, {move.getStone() == BLACK ? "W+R" : "B+R"});
            }
            return;
        }

        // error handling
        if (not isLegal(move)){
            if (not board->isOnBoard(move)){
                throw utils::IllegalMoveException(utils::OFF_BOARD, move);
            }
            if (not board->isEmpty(move)){
                throw utils::IllegalMoveException(utils::OCCUPIED_POINT, move);
            }
            if (not isCorrectColor(move)){
                throw utils::IllegalMoveException(utils::WRONG_COLOR, move);
            }
            if (not isNotSelfCapture(move)){
                throw utils::IllegalMoveException(utils::SELF_CAPTURE, move);
            }
            if (not isNotKoPoint(move)){
                throw utils::IllegalMoveException(utils::KO_POINT, move);
            }
        }

        // place the stone on the board and record the move
        board->playStone(move);
        gameTree.insert(node);

        // with the new stone placed on the board, update the internal board state
        updateBoard(move);

    }

    bool GoGame::isAtRoot() const{
        return gameTree.isAtRoot();
    }

    void GoGame::advanceToRoot() {
        resetBoard();
    }

    void GoGame::stepUp(unsigned steps) {

        if (steps == 0){
            // do nothing
            return;
        }

        if (gameTree.getDepth() < steps){
            throw std::domain_error("Cannot step up past root");
        }

        // get the moves that lead to this sequence
        auto sequence = getMoveSequence();

        // reset the board
        resetBoard();

        // play out the move sequence without the last few moves
        playMoveSequence(std::vector<Move>(sequence.begin(), sequence.end() - steps));

    }

    std::vector<Move> GoGame::getDefaultBranch() {

        std::vector<Move> defaultBranch;

        auto bookmark = gameTree.getSequence();

        // advance the moveTree to the root
        gameTree.advanceToRoot();

        while (not gameTree.isAtLeaf()){
            auto child = gameTree.getChildren()[0];
            defaultBranch.push_back(child.getMove());
            gameTree.stepDown();
        }

        // now that we have found the sequence, return to our original position
        gameTree.advanceToRoot();
        for (const auto& move : bookmark){
            utils::SGFNode node(move);
            gameTree.stepTo(node);
        }

        return defaultBranch;

    }

    void GoGame::playDefaultBranch(){

        resetBoard();

        while(not gameTree.isAtLeaf()){
            gameTree.stepDown(); // step into the next move
            auto move = gameTree.get().getMove(); // get the move at this index
            gameTree.stepUp(); // step up to the previous node and play the move from that node
            playStone(move);
        }
    }

    void GoGame::playMoveSequence(const std::vector<Move>& moves) {

        auto baseMoveSequence = getMoveSequence();

        try {
            // play all of the stones in the sequence
            for (const auto& move : moves){
                playStone(move);
            }
        }
        catch (const utils::IllegalMoveException& except){
            // reset to the original position
            resetBoard();
            for (const auto& move : baseMoveSequence){
                playStone(move);
            }

            // throw the exception again
            throw except;
        }
    }

    std::vector<Move> GoGame::getBranches() {

        auto children = gameTree.getChildren();
        std::vector<Move> branches(children.size());

        for (unsigned i = 0; i < children.size(); i++){
            branches[i] = children[i].getMove();
        }

        return branches;
    }

    std::vector<Move> GoGame::getMoveSequence() {

        auto sequence = gameTree.getSequence();
        std::vector<Move> moveSequence(sequence.size());

        for (unsigned i = 0; i < sequence.size(); i++){
            moveSequence[i] = sequence[i].getMove();
        }

        return moveSequence;
    }

    unsigned GoGame::getMoveNumber() const {
        return gameTree.getDepth();
    }

    utils::Tree<utils::SGFNode> GoGame::getMoveTree() const {
        return gameTree;
    }

    std::unordered_map<std::string, std::vector<std::string>> GoGame::getMetadata() const {

        // get the attributes from the root node
        auto node = gameTree.getRoot();

        std::unordered_map<std::string, std::vector<std::string>> attributes;

        for (const auto& attribute : node.getAttributes()){
            attributes[utils::toStr(attribute.first)] = attribute.second;
        }

        // add the attributes from this node
        node = gameTree.get();

        for (const auto& attribute : node.getAttributes()){
            attributes[utils::toStr(attribute.first)] = attribute.second;
        }

        return attributes;

    }

    void GoGame::setMetadata(const std::string& command, const std::string& value) {
        if (utils::isCommand(command)){
            // get the command
            utils::SGFProperty SGFProperty = utils::fromStr(command);

            // check to see if the command is legal for this version of SGF
            if (not utils::isSGFLegal(SGFProperty, std::stoi(gameTree.getRoot().getCommand(utils::FF)[0]))){
                throw utils::InvalidSGFException("SGF Command \"" + command + "\" is not supported for SGF FF[" + gameTree.getRoot().getCommand(utils::FF)[0] + "]");
            }

            // we can't edit the size of the board
            if (SGFProperty == utils::SZ){
                throw std::domain_error("Cannot edit the \"SZ\" value of an SGF file (it would change the size of the board)");
            }
            if (utils::isFileWide(SGFProperty)){
                gameTree.getRoot().setCommand(SGFProperty, {value});
            }
            else {
                gameTree.get().setCommand(SGFProperty, {value});
            }
        }
        else {
            throw utils::InvalidSGFException("unknown SGF command \"" + command + "\"");
        }
    }

    void GoGame::setMetadata(const std::string& command, const std::vector<std::string>& values) {
        if (utils::isCommand(command)){

            utils::SGFProperty SGFProperty = utils::fromStr(command);

            // check to see if the command is legal for this version of SGF
            if (not utils::isSGFLegal(SGFProperty, std::stoi(gameTree.getRoot().getCommand(utils::FF)[0]))){
                throw utils::InvalidSGFException("SGF Command \"" + command + "\" is not supported for SGF FF[" + gameTree.getRoot().getCommand(utils::FF)[0] + "]");
            }

            if (SGFProperty == utils::SZ){
                throw std::domain_error("Cannot edit the \"SZ\" value of an SGF file (it would change the size of the board)");
            }

            if (utils::isFileWide(SGFProperty)){
                gameTree.getRoot().setCommand(SGFProperty, values);
            }
            else {
                gameTree.get().setCommand(SGFProperty, values);
            }
        }
        else {
            throw utils::InvalidSGFException("unknown SGF command \"" + command + "\"");
        }
    }

    Stone GoGame::getSpace(unsigned x, unsigned y) const {
        return board->getSpace(x, y).getStone();
    }
    Stone GoGame::getActivePlayer() const {
        return gameTree.getDepth() % 2 == 0 ? BLACK : WHITE;
    }

    const _board& GoGame::getBoard() const {
        return *board;
    }

    Results GoGame::getResults() const {

        if (not isOver()){
            throw std::domain_error("could not get results of game, the game is still ongoing");
        }

        if (passCount >= 2){
            return score();
        }
        else {
            std::string results = gameTree.getRoot().getCommand(utils::RE)[0];
            switch (results[0]){
                case 'W':
                    return Results(BLACK);
                case 'B':
                    return Results(WHITE);
            }
        }

    }


    /**
     *
     * Score the game with the specified Komi
     *
     * @param komi
     * @return
     */
    Results GoGame::score() const {

        if (passCount < 2){
            throw std::domain_error("game did not end from passing; could not score");
        }

        unsigned blackTerritory = 0;
        unsigned whiteTerritory = 0;

        unsigned blackStones = 0;
        unsigned whiteStones = 0;

        // TODO: add functionality to remove dead stones

        // generate a list of all the regions in the board
        auto regions = utils::getEmptySpaces(*board);

        for (const auto& region : regions){

            // go through all of the adjacent groups
            auto adjacentGroups = utils::getAdjacentGroups(region, *board, groups);

            auto iter = adjacentGroups.begin();

            if (iter != adjacentGroups.end()){
                Stone firstColor = (*iter)->getColor();

                bool isTerritory = true;

                for (iter++; iter != adjacentGroups.end(); iter++){
                    if ((*iter)->getColor() != firstColor){
                        isTerritory = false;
                        break;
                    }
                }

                if (isTerritory){
                    if (firstColor == BLACK){
                        blackTerritory += region.size();
                    }
                    if (firstColor == WHITE){
                        whiteTerritory += region.size();
                    }
                }
            }

        }

        if (rules == CHINESE){
            // if we have chinese rules, we score a point for every stone we've played on the board
            for (unsigned i = 0; i < board->getSide(); i++){
                for (unsigned j = 0; j < board->getSide(); j++){
                    if (board->getStone(i, j) == BLACK){
                        blackStones++;
                    }
                    if (board->getStone(i, j) == WHITE){
                        whiteStones++;
                    }
                }
            }
        }
        else {
            // for japanese rules, subtract a point for each captured stone
            for (const auto& group : capturedStones){
                if (group.second.begin()->getStone() == BLACK){
                    blackTerritory -= group.second.size();
                }
                if (group.second.begin()->getStone() == WHITE){
                    whiteTerritory -= group.second.size();
                }
            }
        }

        return {rules, komi, blackTerritory, whiteTerritory, blackStones, whiteStones};
    }

    std::vector<Move> GoGame::getLegalMoves() const {
        py::gil_scoped_release release;

        // go through the entire board
        Stone player = getActivePlayer();
        std::vector<Move> moves;

        // std::cout << "entering getLegalMoves" << std::endl;

        for (unsigned i = 0; i < board->getSide(); i++){
            for (unsigned j = 0; j < board->getSide(); j++){
                // std::cout << std::string(Move(i, j, player)) << std::endl;
                if (isLegal(i, j)){
                    moves.emplace_back(i, j, player);
                }
            }
        }

        // add resignation and passing
        moves.emplace_back(Move::pass(getActivePlayer()));
        moves.emplace_back(Move::resign(getActivePlayer()));

        // std::cout << "leaving getLegalMoves" << std::endl;

        return moves;

    }

    Point GoGame::getKoPoint() const {
        return {koPoint.getX(), koPoint.getY()};
    }

    std::string GoGame::getComment() const {
        if (gameTree.get().hasCommand(utils::C)){
            return gameTree.get().getCommand(utils::C)[0];
        }
        else {
            return "";
        }
    };
    void GoGame::setComment(const std::string& comment) const {
        gameTree.get().setCommand(utils::C, {comment});
    };

    GoGame::operator std::string() const {
        if (getComment().empty()){
            return std::string(*board);
        }
        else {
            return std::string(*board) + "\n" + getComment();
        }
    }

    void GoGame::makeBoard(unsigned int side) {

        switch (side){
            case 19:
                board = std::unique_ptr<Board<19>>(new Board<19>());
                break;
            case 13:
                board = std::unique_ptr<Board<13>>(new Board<13>());
                break;
            case 9:
                board = std::unique_ptr<Board<9>>(new Board<9>());
                break;
            default:
                throw std::domain_error("Invalid Board size " +
                                            std::to_string(side) + " only 9x9, 13x13 and 19x19 are currently supported");
        }
    }

    void GoGame::resetKoPoint(){
        koPoint = Move::pass(getActivePlayer());
    }

    /**
     *
     * Updates the board with the specified move
     *
     * @param move
     */
    void GoGame::updateBoard(const Move& move) {

        std::unordered_set<std::shared_ptr<Group>> ourAffectedGroups{};
        std::unordered_set<std::shared_ptr<Group>> theirAffectedGroups{};

        // check to see if this move extends a group
        for (const auto& adjacentSpace : move.getAdjacentMoves(board->getSide())) {
            Move temp = board->getSpace(adjacentSpace);
            if (groups.find(temp) != groups.end()) {
                if (groups.at(temp)->getColor() == move.getStone()) {
                    ourAffectedGroups.insert(groups.at(temp));
                }
                else if (groups.at(temp)->getColor() == getOpponent(move.getStone())){
                    theirAffectedGroups.insert(groups.at(temp));
                }
            }
        }

        // connect stones
        if (ourAffectedGroups.empty()){
            // if we not connected to any group, create a new group!
            groups[move] = std::make_shared<Group>(move);
        }
        else {
            // if we are connected to a group, connect our stones
            connectGroups(move, ourAffectedGroups);
        }

#ifdef DEBUG_LOG
        py::print("checking for captured stones in ", theirAffectedGroups.size(), " groups");
#endif

        // reset the ko point
        resetKoPoint();

        // capture stones
        for (const auto& group : theirAffectedGroups){
#ifdef DEBUG_LOG
            py::print("group had ", utils::getLiberties(*group, *board), " liberties");
#endif
            if (utils::getLiberties(*group, *board).empty()){

#ifdef DEBUG_LOG
                py::print("found a group with zero liberties");
#endif

                // check for a Ko
                // if we captured a stone without affecting any of our groups
                if (ourAffectedGroups.empty() and group->getMoves().size() == 1){

                    bool ko = true;

                    Move KoMove = *group->getMoves().begin();

                    // make sure that the capturing stone is captured before the stone is removed
                    for (const auto& adjacentSpace : move.getAdjacentMoves(board->getSide())){
                        ko = ko and board->getSpace(adjacentSpace).getStone()
                                    == getOpponent(move.getStone());

                    }

                    // if this is a real ko, update the ko move
                    if (ko){
                        koPoint = KoMove;
                    }

                }

                // capture the stones
                for (const auto& stone : group->getMoves()){
#ifdef DEBUG_LOG
                    py::print("capturing stone ", std::string(stone));
#endif
                    // erase the item
                    groups.erase(stone);
                    board->captureStone(stone);
                    capturedStones[gameTree.getDepth()].insert(stone);
                }
            }
        }

    }

    bool GoGame::isCorrectColor(const Move &move) const {
        return move.getStone() == ((gameTree.getDepth() % 2 == 0) ? BLACK : WHITE);
    }

    bool GoGame::isNotSelfCapture(const Move &move) const{

        // std::cout << "entering isNotSelfCapture" << std::endl;

        std::unordered_set<std::shared_ptr<Group>> ourAffectedGroups{};
        std::unordered_set<std::shared_ptr<Group>> theirAffectedGroups{};

        // check to see if this move extends a group
        for (const auto& adjacentSpace : move.getAdjacentMoves(board->getSide())) {
            Move temp = board->getSpace(adjacentSpace);
            if (groups.find(temp) != groups.end()) {
                if (groups.at(temp)->getColor() == move.getStone()) {
                    ourAffectedGroups.insert(groups.at(temp));
                }
                else if (groups.at(temp)->getColor() == getOpponent(move.getStone())){
                    theirAffectedGroups.insert(groups.at(temp));
                }
            }
        }

        // first of all check to see if we capture any stones
        for (const auto& group : theirAffectedGroups){
            if (utils::getLiberties(*group, *board).size() == 1){
                // if an adjacent group is in atari, then playing on an empty space must capture
                // any move that captures enemy stones is legal (ignoring kos)
                return true;
            }
        }

        // create a temporary group object and merge all of the affected groups into it
        Group affectedGroup;

        if (not ourAffectedGroups.empty()){
            affectedGroup = Group(move, ourAffectedGroups);
        }
        else {
            affectedGroup = Group(move);
        }

        // temporarily put the stone on the board and count the liberties
        auto liberties = utils::getLiberties(affectedGroup, *board);

        // check to see if this move fills the last liberty
        bool filledLastLiberty = false;
        if (liberties.size() == 1){
            auto liberty = *liberties.begin();
            filledLastLiberty = liberty.getX() == move.getX() and liberty.getY() == move.getY();
        }

        // std::cout << "leaving isNotSelfCapture" << std::endl;

        return not liberties.empty() and not filledLastLiberty;
    }

    bool GoGame::isNotKoPoint(const Move &move) const{
        return move != koPoint;
    }

    bool GoGame::isOver() const {
        return gameTree.getRoot().hasCommand(utils::RE);
    }

    /**
     *
     * connect a vector of groups of stone using the specified move
     *
     * @param move
     * @param groups
     */
    void GoGame::connectGroups(const Move& move, const std::unordered_set<std::shared_ptr<Group>>& toConnect) {

        // create the new group object
        auto newGroup = std::make_shared<Group>(move, toConnect);

        // set each move in the new Group to point to the new group
        for (const auto& stone : newGroup->getMoves()){
            groups[stone] = newGroup;
        }
    }

}