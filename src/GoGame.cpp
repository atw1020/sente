//
// Created by arthur wesley on 6/28/21.
//

#include <regex>

#include "pybind11/pybind11.h"

#include "include/SGF.h"
#include "include/GoGame.h"
#include "include/Territory.h"
#include "include/SenteExceptions.h"

namespace py = pybind11;

namespace std {

    size_t hash<pair<unsigned, unsigned>>::operator()(const pair<unsigned int, unsigned int> &toHash) const noexcept {

        size_t hash1 = hash<unsigned>()(toHash.first);
        size_t hash2 = hash<unsigned>()(toHash.second);

        return hash1 xor (hash2 << 1);

    }

}

namespace sente {

    GoGame::GoGame(unsigned side, Rules rules) {
        this->rules = rules;
        makeBoard(side);

        // some book-keeping

        resetKoPoint();
        resigned = EMPTY;
    }

    /**
     *
     * Regular Expressions visualizer
     * http://www.regexplained.co.uk
     *
     * @param filePointer
     */
    GoGame::GoGame(const std::string& SGFText) {

        // extract the move tree
        board = utils::getSGFBoardSize(SGFText);
        rules = utils::getSGFRules(SGFText);
        moveTree = utils::getSGFMoves(SGFText);

        // some book-keeping

        resetKoPoint();
        resigned = EMPTY;

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
        moveTree.advanceToRoot();

        // set the groups and captures to be empty
        groups = std::unordered_map<Move, std::shared_ptr<Group>>();
        capturedStones = std::unordered_map<unsigned, std::unordered_set<Move>>();

        // reset the ko point
        resetKoPoint();
        resigned = EMPTY;

    }

    bool GoGame::isLegal(unsigned x, unsigned y) const{
        return isLegal(Move(x, y, moveTree.getDepth() % 2 == 0 ? BLACK : WHITE));
    }

    bool GoGame::isLegal(unsigned int x, unsigned int y, Stone stone) const {
        return isLegal(Move(x, y, stone));
    }

    bool GoGame::isLegal(const Move& move) const {

        py::print(move.getX());

        bool onBoard = board->isOnBoard(move);
        if (not onBoard){
            return false;
        }
        bool isEmpty = board->isEmpty(move);
        bool notSelfCapture = isNotSelfCapture(move);
        bool notKoPoint = isNotKoPoint(move);
        bool correctColor = isCorrectColor(move);

        return isEmpty and notSelfCapture and notKoPoint and correctColor;
    }

    void GoGame::playStone(unsigned x, unsigned y){
        playStone(Move(x, y, moveTree.getDepth() % 2 == 0 ? BLACK : WHITE));
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

        // check for pass/resign
        if (move == Move(move.getStone(), PASS)){
            passCount++;
            moveTree.insert(move);
            return;
        }
        else {
            passCount = 0;
        }

        if (move == Move(move.getStone(), RESIGN)){
            // TODO: implement
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
        moveTree.insert(move);

        // with the new stone placed on the board, update the internal board state
        updateBoard(move);

    }

    void GoGame::advanceToRoot() {
        resetBoard();
    }

    void GoGame::stepUp(unsigned steps) {

        // get the moves that lead to this sequence
        auto sequence = getMoveSequence();

        // reset the board
        resetBoard();

        // play out the move sequence without the last few moves
        playMoveSequence(std::vector<Move>(sequence.begin(), sequence.end() - steps));

    }

    void GoGame::playDefaultBranch(){

        resetBoard();

        while(not moveTree.isAtLeaf()){
            moveTree.stepDown(); // step into the next move
            auto move = moveTree.get(); // get the move at this index
            moveTree.stepUp(); // step up to the previous node and play the move from that node
            playStone(move);
        }
    }

    void GoGame::playMoveSequence(const std::vector<Move>& moves) {
        // play all of the stones in the sequence
        for (const auto& move : moves){
            playStone(move);
        }
    }

    std::vector<Move> GoGame::getBranches() {
        return moveTree.getChildren();
    }

    std::vector<Move> GoGame::getMoveSequence() {
        return moveTree.getSequence();
    }

    Stone GoGame::getSpace(unsigned x, unsigned y) const {
        return board->getSpace(x, y).getStone();
    }
    Stone GoGame::getActivePlayer() const {
        return moveTree.getDepth() % 2 == 0 ? BLACK : WHITE;
    }

    const _board& GoGame::getBoard() const {
        return *board;
    }

    GoGame::operator std::string() const {
        return std::string(*board);
    }

    std::string GoGame::toSGF(std::unordered_map<std::string, std::string> attributes) const {

        // add some other parameters
        switch (rules){
            case JAPANESE:
                attributes["RU"] = "Japanese";
                break;
            case CHINESE:
                attributes["RU"] = "Chinese";
        }
        attributes["SZ"] = std::to_string(board->getSide());

        return utils::toSGF(moveTree, attributes);

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
        koPoint = Move(board->getSide() + 1, board->getSide() + 1, EMPTY);
    }

    /**
     *
     * Updates the board with the specified move
     *
     * @param move
     */
    void GoGame::updateBoard(Move move) {

        std::unordered_set<std::shared_ptr<Group>> ourAffectedGroups{};
        std::unordered_set<std::shared_ptr<Group>> theirAffectedGroups{};

        // check to see if this move extends a group
        for (const auto& adjacentSpace : move.getAdjacentMoves(board->getSide())) {
            Move temp = board->getSpace(adjacentSpace.first, adjacentSpace.second);
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
                        ko = ko and board->getSpace(adjacentSpace.first, adjacentSpace.second).getStone()
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
                    groups[stone] = nullptr;
                    board->captureStone(stone);
                    capturedStones[moveTree.getDepth()].insert(stone);
                }
            }
        }

    }

    bool GoGame::isCorrectColor(const Move &move) const {
        return move.getStone() == ((moveTree.getDepth() % 2 == 0) ? BLACK : WHITE);
    }

    bool GoGame::isNotSelfCapture(const Move &move) const{

        std::unordered_set<std::shared_ptr<Group>> ourAffectedGroups{};
        std::unordered_set<std::shared_ptr<Group>> theirAffectedGroups{};

        // check to see if this move extends a group
        for (const auto& adjacentSpace : move.getAdjacentMoves(board->getSide())) {
            Move temp = board->getSpace(adjacentSpace.first, adjacentSpace.second);
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

        return not liberties.empty() and not filledLastLiberty;
    }

    bool GoGame::isNotKoPoint(const Move &move) const{
        return move != koPoint;
    }

    bool GoGame::isOver() const {
        return passCount >= 2;
    }

    /**
     *
     * connect a vector of groups of stone using the specified move
     *
     * @param move
     * @param groups
     */
    void GoGame::connectGroups(Move move, const std::unordered_set<std::shared_ptr<Group>>& toConnect) {

        // create the new group object
        auto newGroup = std::make_shared<Group>(move, toConnect);

        // set each move in the new Group to point to the new group
        for (const auto& stone : newGroup->getMoves()){
            groups[stone] = newGroup;
        }
    }

}