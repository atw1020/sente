//
// Created by arthur wesley on 6/28/21.
//

#include <stack>

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
            this->komi = determineKomi(rules);
        }
        else {
            this->komi = komi;
        }

        this->rules = rules;

        makeBoard(side);
        resetKoPoint();

        // create the rootnode
        sgf::SGFNode rootNode;

        // add the defualt metadata
        rootNode.setProperty(sgf::FF, {"4"});
        rootNode.setProperty(sgf::SZ, {std::to_string(side)});

        switch (rules){
            case OTHER:
            case CHINESE:
                rootNode.setProperty(sgf::RU, {"Chinese"});
                break;
            case JAPANESE:
                rootNode.setProperty(sgf::RU, {"Japanese"});
                break;
            case KOREAN:
                rootNode.setProperty(sgf::RU, {"Korean"});
                break;
        }

        gameTree = utils::Tree<sgf::SGFNode>(rootNode);

    }

    GoGame::GoGame(utils::Tree<sgf::SGFNode> &SGFTree) {
        gameTree = SGFTree;

        auto rootNode = gameTree.getRoot();

        if (rootNode.hasProperty(sgf::SZ)){
            // parse if available
            makeBoard(std::stoi(rootNode.getProperty(sgf::SZ)[0]));
        }
        else {
            // default board size
            makeBoard(19);
        }

        if (rootNode.hasProperty(sgf::RU)){

            std::string ruleString = rootNode.getProperty(sgf::RU)[0];
            rules = rulesFromStr(ruleString);
        }
        else {
            rules = CHINESE; // default
        }

        if (rootNode.hasProperty(sgf::KM)){
            if (rootNode.getProperty(sgf::KM)[0].empty()){
                komi = 0;
            }
            else {
                komi = std::stod(rootNode.getProperty(sgf::KM)[0]);
            }
        }
        else {
            komi = determineKomi(rules);
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

    bool GoGame::isLegal(unsigned x, unsigned y) {
        return isLegal(Move(x, y, gameTree.getDepth() % 2 == 0 ? BLACK : WHITE));
    }

    bool GoGame::isLegal(unsigned int x, unsigned int y, Stone stone) {
        return isLegal(Move(x, y, stone));
    }

    bool GoGame::isLegal(const Move& move) {
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
        sgf::SGFNode node(move);

        // check for pass/resign
        if (move.isPass()){
            gameTree.insert(node);
            if (++passCount >= 2){
                gameTree.getRoot().setProperty(sgf::RE, std::vector<std::string>());
            }
            return;
        }
        else {
            passCount = 0;
        }

        if (move.isResign()){
            // get the root node
            if (gameTree.getRoot().hasProperty(sgf::RE)){
                // if the game has been resigned raise an exception
                throw std::domain_error("Game cannot be forfeited; the game is already over");
            }
            else {
                gameTree.getRoot().setProperty(sgf::RE, {move.getStone() == BLACK ? "W+R" : "B+R"});
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

    /**
     *
     * checks to see if a move is legal as an "add" move
     *
     * in other words determine if the move is legal regardless of whether or not it is the player in question's turn
     *
     * @param move to determine if it is legal
     * @return whether or not the move is legal as an add move
     */
    bool GoGame::isAddLegal(const Move& move){

        if (not board->isOnBoard(move)){
            return false;
        }
        // std::cout << "passed isOnBoard" << std::endl;
        bool isEmpty = board->isEmpty(move);
        // std::cout << "passed isEmpty" << std::endl;
        bool notSelfCapture = isNotSelfCapture(move);
        // std::cout << "passed isNotSelfCapture" << std::endl;
        bool notKoPoint = isNotKoPoint(move);

        return isEmpty and notSelfCapture and notKoPoint;

    }

    /**
     *
     * adds a stone to the board
     *
     * @param move to add to the board
     */
    void GoGame::addStone(const Move& move){

        // error handling
        if (not isAddLegal(move)){
            if (not board->isOnBoard(move)){
                throw utils::IllegalMoveException(utils::OFF_BOARD, move);
            }
            if (not board->isEmpty(move)){
                throw utils::IllegalMoveException(utils::OCCUPIED_POINT, move);
            }
            if (not isNotSelfCapture(move)){
                throw utils::IllegalMoveException(utils::SELF_CAPTURE, move);
            }
            if (not isNotKoPoint(move)){
                throw utils::IllegalMoveException(utils::KO_POINT, move);
            }
        }

        // figure out what kind of property we are dealing with
        sgf::SGFProperty property;

        switch (move.getStone()){
            case BLACK:
                property = sgf::AB;
                break;
            case WHITE:
                property = sgf::AW;
                break;
            case EMPTY:
                property = sgf::AE;
                break;
        }

        // if the stone we are adding has not been to the game tree, add it
        if (gameTree.get().hasProperty(property)){

            // insert the added move into the game tree
            std::string positionInfo = std::string(move);
            positionInfo = std::string(positionInfo.begin() + 2, positionInfo.end() - 1);

            // generate the list of all stones added to the property
            auto addedStones = gameTree.get().getProperty(property);

            // if the property has not been added to the SGF tree, add it
            if (std::find(addedStones.begin(),  addedStones.end(), positionInfo) != addedStones.end()){
                gameTree.get().appendProperty(property, positionInfo);
            }
        }

        // put the stone into the board and update the board
        board->playStone(move);
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

    void GoGame::playDefaultSequence(){

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


    std::vector<Move> GoGame::getDefaultSequence() {

        std::vector<Move> defaultBranch;

        auto bookmark = gameTree.getSequence();

        while (not gameTree.isAtLeaf()){
            auto child = gameTree.getChildren()[0];
            defaultBranch.push_back(child.getMove());
            gameTree.stepDown();
        }

        // now that we have found the sequence, return to our original position
        gameTree.advanceToRoot();
        for (const auto& move : bookmark){
            sgf::SGFNode node(move);
            gameTree.stepTo(node);
        }

        return defaultBranch;

    }

    std::vector<std::vector<Move>> GoGame::getSequences(const std::vector<Move>& currentSequence) {

        std::vector<std::vector<Move>> sequences;

        if (gameTree.isAtLeaf()){
            // if we are at a leaf, set the vector to just be the current Sequence
            sequences = {currentSequence};
        }
        else {
            // otherwise, add all the children
            for (auto& child : gameTree.getChildren()){
                // copy the current sequence
                std::vector<Move> temp(currentSequence.begin(), currentSequence.end());

                // add the current move
                temp.push_back(child.getMove());

                // step into the child and get its sequences
                gameTree.stepTo(child);
                auto childSequences = getSequences(temp);
                gameTree.stepUp();

                // add all the children to the sequences
                sequences.insert(sequences.end(), childSequences.begin(), childSequences.end());
            }
        }

        return sequences;

    }

    unsigned GoGame::getMoveNumber() const {
        return gameTree.getDepth();
    }

    utils::Tree<sgf::SGFNode> GoGame::getMoveTree() const {
        return gameTree;
    }

    std::unordered_map<std::string, std::vector<std::string>> GoGame::getProperties() const {

        // get the properties from the root node
        auto node = gameTree.getRoot();

        std::unordered_map<std::string, std::vector<std::string>> properties;

        for (const auto& property : node.getProperties()){
            properties[sgf::toStr(property.first)] = property.second;
        }

        // add the properties from this node
        node = gameTree.get();

        for (const auto& property : node.getProperties()){
            properties[sgf::toStr(property.first)] = property.second;
        }

        return properties;

    }

    void GoGame::setProperty(const std::string& property, const std::string& value) {
        if (sgf::isProperty(property)){
            // get the property
            sgf::SGFProperty SGFProperty = sgf::fromStr(property);

            // check to see if the property is legal for this version of SGF
            if (not sgf::isSGFLegal(SGFProperty, std::stoi(gameTree.getRoot().getProperty(sgf::FF)[0]))){
                throw utils::InvalidSGFException("SGF Property \"" + property + "\" is not supported for SGF FF[" + gameTree.getRoot().getProperty(sgf::FF)[0] + "]");
            }

            // we can't edit the size of the board
            if (SGFProperty == sgf::SZ){
                throw std::domain_error("Cannot edit the \"SZ\" value of an SGF file (it would change the size of the board)");
            }
            if (sgf::isFileWide(SGFProperty)){
                gameTree.getRoot().setProperty(SGFProperty, {value});
            }
            else {
                gameTree.get().setProperty(SGFProperty, {value});
            }
        }
        else {
            throw utils::InvalidSGFException("unknown SGF Property \"" + property + "\"");
        }
    }

    void GoGame::setProperty(const std::string& property, const std::vector<std::string>& values) {
        if (sgf::isProperty(property)){

            sgf::SGFProperty SGFProperty = sgf::fromStr(property);

            // check to see if the property is legal for this version of SGF
            if (not sgf::isSGFLegal(SGFProperty, std::stoi(gameTree.getRoot().getProperty(sgf::FF)[0]))){
                throw utils::InvalidSGFException("SGF Property \"" + property + "\" is not supported for SGF FF[" + gameTree.getRoot().getProperty(sgf::FF)[0] + "]");
            }

            if (SGFProperty == sgf::SZ){
                throw std::domain_error("Cannot edit the \"SZ\" value of an SGF file (it would change the size of the board)");
            }

            if (sgf::isFileWide(SGFProperty)){
                gameTree.getRoot().setProperty(SGFProperty, values);
            }
            else {
                gameTree.get().setProperty(SGFProperty, values);
            }
        }
        else {
            throw utils::InvalidSGFException("unknown SGF Property \"" + property + "\"");
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
            std::string results = gameTree.getRoot().getProperty(sgf::RE)[0];
            switch (results[0]){
                case 'W':
                    return Results(BLACK);
                case 'B':
                default:
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

            // go through all the adjacent groups
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

    std::vector<Move> GoGame::getLegalMoves() {
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
        if (gameTree.get().hasProperty(sgf::C)){
            return gameTree.get().getProperty(sgf::C)[0];
        }
        else {
            return "";
        }
    };
    void GoGame::setComment(const std::string& comment) const {
        gameTree.get().setProperty(sgf::C, {comment});
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

    bool GoGame::isCorrectColor(const Move &move) {
        // go through the tree until we get our parents
        std::stack<sgf::SGFNode> previousMoves;

        while (gameTree.get().getMove() == Move::nullMove and not gameTree.isAtRoot()){
            previousMoves.push(gameTree.get());
            gameTree.stepUp();
        }

        bool result;

        if (gameTree.isAtRoot()){
            result = move.getStone() == BLACK;
        }
        else {
            result = move.getStone() != gameTree.get().getMove().getStone();
        }

        while (not previousMoves.empty()){
            gameTree.stepTo(previousMoves.top());
            previousMoves.pop();
        }

        return result;
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
        return gameTree.getRoot().hasProperty(sgf::RE);
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

    Rules GoGame::getRules() const {
        return rules;
    }

    double GoGame::getKomi() const {
        return komi;
    }

    void GoGame::setKomi(double newKomi) {
        komi = newKomi;
    }

}