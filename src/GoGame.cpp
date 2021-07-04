//
// Created by arthur wesley on 6/28/21.
//

#include <regex>

#include "pybind11/pybind11.h"

#include "include/GoGame.h"
#include "include/SGF.h"

namespace py = pybind11;

namespace std {

    size_t hash<pair<unsigned, unsigned>>::operator()(const pair<unsigned int, unsigned int> &toHash) const noexcept {

        size_t hash1 = hash<unsigned>()(toHash.first);
        size_t hash2 = hash<unsigned>()(toHash.second);

        return hash1 xor (hash2 << 1);

    }

}

namespace sente {

    GoGame::GoGame(Rules rules, unsigned int side) {
        this->rules = rules;
        makeBoard(side);
    }

    /**
     *
     * Regular Expressions visualizer
     * http://www.regexplained.co.uk
     *
     * @param filePointer
     */
    GoGame::GoGame(const std::string& SGFFile) {

        // load the text from the file
        std::ifstream filePointer(SGFFile);
        std::string SGFText((std::istreambuf_iterator<char>(filePointer)),
                         std::istreambuf_iterator<char>());

        // extract the move tree
        try{
            board = sente_utils::getSGFBoardSize(SGFText);
            rules = sente_utils::getSGFRules(SGFText);
            moveTree = sente_utils::getSGFMoves(SGFText);
        }
        catch (const sente_utils::InvalidSGFException& E){
            throw sente_utils::InvalidSGFException(E, SGFFile);
        }

    }

    bool GoGame::isLegal(unsigned x, unsigned y) const{
        return isLegal(Move(x, y, moveTree.getDepth() % 2 == 0 ? BLACK : WHITE));
    }

    bool GoGame::isLegal(unsigned int x, unsigned int y, Stone stone) const {
        return isLegal(Move(x, y, stone));
    }

    bool GoGame::isLegal(const Move& move) const {

        bool onBoard = board->isOnBoard(move);
        bool isEmpty = board->isEmpty(move);
        bool notSelfCapture = isNotSelfCapture(move);
        bool notKoPoint = isNotKoPoint(move);
        bool correctColor = isCorrectColor(move);

        return onBoard and isEmpty and notSelfCapture and notKoPoint and correctColor;
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
                throw IllegalMoveException(OFF_BOARD, move);
            }
            if (not board->isEmpty(move)){
                throw IllegalMoveException(OCCUPIED_POINT, move);
            }
            if (not isNotSelfCapture(move)){
                throw IllegalMoveException(SELF_CAPTURE, move);
            }
            if (not isNotKoPoint(move)){
                throw IllegalMoveException(KO_POINT, move);
            }
        }

        // place the stone on the board and record the move
        board->playStone(move);
        moveTree.insert(move);

        // with the new stone placed on the board, update the internal board state
        updateBoard(move);

    }

    void GoGame::playDefaultBranch(){

        moveTree.advanceToRoot();

        while(not moveTree.isAtLeaf()){
            moveTree.stepDown(); // step into the next move
            auto move = moveTree.get(); // get the move at this index
            moveTree.stepUp(); // step up to the previous node and play the move from that node
            playStone(move);
        }
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
                throw std::domain_error("Invalid Board size");
        }
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
        koPoint = Move(board->getSide() + 1, board->getSide() + 1, EMPTY);

        // capture stones
        for (const auto& group : theirAffectedGroups){
#ifdef DEBUG_LOG
            py::print("group had ", getLiberties(*group), " liberties");
#endif
            if (getLiberties(*group) == 0){

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
        return (moveTree.getDepth() % 2 == 0) ? move.getStone() == BLACK : move.getStone() == WHITE;
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
            if (getLiberties(*group) == 1){
                // if an adjacent group is in atari, then playing on an empty space must capture
                // any move that captures enemy stones is legal (ignoring kos)
                return true;
            }
        }

        Group affectedGroup;

        if (ourAffectedGroups.size() > 0){
            affectedGroup = Group(move, ourAffectedGroups);
        }
        else {
            affectedGroup = Group(move);
        }

        // create a temporary group object and merge all of the affected groups into it

        return getLiberties(affectedGroup) > 0;
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

    unsigned GoGame::getLiberties(const Group& group) const {

        std::unordered_set<Move> liberties;

        for (const auto& move : group.getMoves()){

            for (const auto& adjacentSpace : move.getAdjacentMoves(board->getSide())){
                if (board->getSpace(adjacentSpace.first, adjacentSpace.second).getStone() == EMPTY){
                    liberties.insert(Move(adjacentSpace.first, adjacentSpace.second, EMPTY));
                }
            }
        }

        return liberties.size();

    }


}