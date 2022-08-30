//
// Created by arthur wesley on 6/28/21.
//

#include <stack>
#include <memory>
#include <sstream>
#include <iomanip>

// #Include <pybind11/pybind11.h>

#include "GoGame.h"
#include "LifeAndDeath.h"
#include "../Utils/SenteExceptions.h"

namespace std {

    size_t hash<pair<unsigned, unsigned>>::operator()(const pair<unsigned int, unsigned int> &toHash) const noexcept {

        size_t hash1 = hash<unsigned>()(toHash.first);
        size_t hash2 = hash<unsigned>()(toHash.second);

        return hash1 xor (hash2 << 1);

    }

}

namespace sente {

    GoGame::GoGame(unsigned side, Rules rules, double komi, std::vector<Move> handicap) {

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

        // create the root node
        SGF::SGFNode rootNode;

        // add the default metadata
        rootNode.setProperty(SGF::FF, {"4"});
        rootNode.setProperty(SGF::SZ, {std::to_string(side)});

        switch (rules){
            case OTHER:
            case CHINESE:
                rootNode.setProperty(SGF::RU, {"Chinese"});
                break;
            case JAPANESE:
                rootNode.setProperty(SGF::RU, {"Japanese"});
                break;
            case KOREAN:
                rootNode.setProperty(SGF::RU, {"Korean"});
                break;
            case TROMP_TAYLOR:
                rootNode.setProperty(SGF::RU, {"Tromp-Taylor"});
                break;
        }

        // check to see if we have a handicap.
        // handicap is prevented by including a null move in the handicap stones
        if (std::find(handicap.begin(), handicap.end(), Move::nullMove) == handicap.end()){

            // establish that we have a handicap
            rootNode.setProperty(SGF::HA, {std::to_string(handicap.size())});

            // add all the handicap stones
            addStones(handicap);
        }

        gameTree = utils::Tree<SGF::SGFNode>(rootNode);
        activeColor = getStartingColor();

    }

    GoGame::GoGame(utils::Tree<SGF::SGFNode> &SGFTree) {
        gameTree = SGFTree;

        auto rootNode = gameTree.getRoot();

        if (rootNode.hasProperty(SGF::SZ)){
            // parse if available
            makeBoard(std::stoi(rootNode.getProperty(SGF::SZ)[0]));
        }
        else {
            // default board size
            makeBoard(19);
        }

        if (rootNode.hasProperty(SGF::RU)){

            std::string ruleString = rootNode.getProperty(SGF::RU)[0];
            rules = rulesFromStr(ruleString);
        }
        else {
            rules = CHINESE; // default
        }

        if (rootNode.hasProperty(SGF::KM)){
            if (rootNode.getProperty(SGF::KM)[0].empty()){
                komi = 0;
            }
            else {
                komi = std::stod(rootNode.getProperty(SGF::KM)[0]);
            }
        }
        else {
            komi = determineKomi(rules);
        }

        addStones(rootNode.getAddedMoves());

        activeColor = getStartingColor();

    }

    /**
     *
     * resets the board to be empty
     *
     */
    void GoGame::resetBoard(){

//        std::cout << "entering resetBoard" << std::endl;

        // create a new board
        clearBoard();
        // reset the tree to the root
        gameTree.advanceToRoot();

//        std::cout << "got past advanceToRoot" << std::endl;

        // set the groups and captures to be empty
        groups = std::unordered_map<Move, std::shared_ptr<Group>>();
        capturedStones = std::unordered_map<unsigned, std::unordered_set<Move>>();

        // set the points to be zero
        blackPoints = NAN;
        whitePoints = NAN;

        // reset the ko point
        // reset the ko point
        resetKoPoint();
        passCount = 0;

//        std::cout << "adding moves" << std::endl;

        // add any stones at the root node
        addStones(gameTree.get().getAddedMoves());

//        std::cout << "added moves" << std::endl;

        activeColor = getStartingColor();

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
        bool isEmpty = board->getStone(move.getVertex()) == EMPTY;
        bool notSelfCapture = rules == TROMP_TAYLOR or isNotSelfCapture(move);
        bool notKoPoint = isNotKoPoint(move);
        bool correctColor = isCorrectColor(move);

        return isEmpty and notSelfCapture and notKoPoint and correctColor;
    }

    /**
     *
     * GTP considers move to be valid even if they belong to the wrong color
     *
     * @param move move to check if legal
     * @return whether the move is legal according to GTP rules
     */
    bool GoGame::isGTPLegal(const sente::Move &move) {
        if (not board->isOnBoard(move)){
            return false;
        }

        bool isEmpty = board->getStone(move.getVertex()) == EMPTY;
        bool notSelfCapture = rules == TROMP_TAYLOR or isNotSelfCapture(move);
        bool notKoPoint = isNotKoPoint(move);

        return isEmpty and notSelfCapture and notKoPoint;
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

        py::gil_scoped_release release;

        // create a new SGF node
        SGF::SGFNode node(move);

//        std::cout << "playing stone " << std::string(move) << std::endl;

        // check for pass/resign
        if (move.isPass()){
            gameTree.insert(node);
            if (++passCount >= 2){
                // score the game
                score();
            }
            activeColor = getOpponent(activeColor);
            return;
        }
        else {
            passCount = 0;
        }

        if (move.isResign()){
            // get the root node
            if (gameTree.getRoot().hasProperty(SGF::RE)){
                // if the game has been resigned raise an exception
                throw std::domain_error("Game cannot be forfeited; the game is already over");
            }
            else {
                gameTree.getRoot().setProperty(SGF::RE, {move.getStone() == BLACK ? "W+R" : "B+R"});
            }
            return;
        }

        // error handling
        if (not isLegal(move)){
            if (not board->isOnBoard(move)){
                throw utils::IllegalMoveException(utils::OFF_BOARD, move);
            }
            if (board->getStone(move.getVertex()) != EMPTY){
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

        //        std::cout << "made it past isLegal" << std::endl;

        // place the stone on the board and record the move
        board->playStone(move);
        gameTree.insert(node);

        // with the new stone placed on the board, update the internal board state
        updateBoard(move);

        // update the active color
        if (gameTree.get().hasProperty(SGF::PL)){
            // if we just set the player in this node, set the player
            switch (gameTree.get().getProperty(SGF::PL)[0][0]){
                case 'B':
                    activeColor = BLACK;
                    break;
                case 'W':
                    activeColor = WHITE;
                    break;
            }
        }
        else {
            // otherwise, set the player to be the opposite as what we just had
            activeColor = getOpponent(activeColor);
        }

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
//        bool isEmpty = board->getStone(move.getVertex()) == EMPTY;
        // std::cout << "passed isEmpty" << std::endl;
        bool notSelfCapture = rules == TROMP_TAYLOR or isNotSelfCapture(move);
        // std::cout << "passed isNotSelfCapture" << std::endl;
//        bool notKoPoint = isNotKoPoint(move);

//        std::cout << "isEmpty:" << std::boolalpha << isEmpty << std::endl;
//        std::cout << "notSelfCapture:" << std::boolalpha << notSelfCapture << std::endl;
//        std::cout << "notKoPoint:" << std::boolalpha << notKoPoint << std::endl;

        return notSelfCapture;

    }

    /**
     *
     * adds a vector of stones to the board
     *
     * @param move to add to the board
     */
    void GoGame::addStones(const std::vector<Move>& moves){

//        py::gil_scoped_release release;
//        std::cout << "entering addStone" << std::endl;

        // handle errors before moving forward
        for (const auto & move : moves){
            // error handling
            if (not isAddLegal(move)){
                if (not board->isOnBoard(move)){
                    throw utils::IllegalMoveException(utils::OFF_BOARD, move);
                }
            }
        }

        // get a reference to the node we will be working with
        SGF::SGFNode& node = gameTree.get();
        bool insert = false;

        if (node.getMove() != Move::nullMove){
            // create a new node
            node = SGF::SGFNode(Move::nullMove);
            insert = true;
        }

        // add all the moves
        for (const auto& move : moves){

            // figure out what kind of property we are dealing with
            SGF::SGFProperty property;

            switch (move.getStone()){
                case BLACK:
                    property = SGF::AB;
                    break;
                case WHITE:
                    property = SGF::AW;
                    break;
                case EMPTY:
                    property = SGF::AE;
                    break;
            }

            auto existingMoves = node.getAddedMoves();

            bool skipAddingMove = false;

            // check to see if there is a move of any color that has been added and remove it if it exists
            Move blackMove = Move(move.getX(), move.getY(), BLACK);
            Move whiteMove = Move(move.getX(), move.getY(), WHITE);
            Move emptyMove = Move(move.getX(), move.getY(), EMPTY);

            // obtain a string to insert
            std::string positionInfo = move.toSGF();
            positionInfo = std::string(positionInfo.begin() + 2, positionInfo.end() - 1);

            if (std::find(existingMoves.begin(), existingMoves.end(), blackMove) != existingMoves.end()){
                node.removeItem(SGF::AB, positionInfo);
                // we are skipping if the type of the move is empty
                skipAddingMove = property == SGF::AE;
            }
            if (std::find(existingMoves.begin(), existingMoves.end(), whiteMove) != existingMoves.end()){
                node.removeItem(SGF::AW, positionInfo);
                // we are skipping if the type of the move is empty
                skipAddingMove = property == SGF::AE;
            }
            if (std::find(existingMoves.begin(), existingMoves.end(), emptyMove) != existingMoves.end()){
                node.removeItem(SGF::AE, positionInfo);
            }

            // also skip if we are adding an empty move
            skipAddingMove = skipAddingMove or (board->getSpace(move.getX(), move.getY()).getStone() == EMPTY and
                                                property == SGF::AE);

            // check to see if we are on a normal node or an add stone node

            if (not skipAddingMove){
                //            std::cout << "adding move " << std::string(move) << std::endl;
                node.appendProperty(property, positionInfo);
            }

            // put the stone into the board and update the board
            board->playStone(move);
            updateBoard(move);
        }

        if (insert){
            gameTree.insert(node);
        }

        // update the player if necessary
        if (gameTree.get().hasProperty(SGF::PL)){
            // if we just set the player in this node, set the player
            switch (gameTree.get().getProperty(SGF::PL)[0][0]){
                case 'B':
                    activeColor = BLACK;
                    break;
                case 'W':
                    activeColor = WHITE;
                    break;
            }
        }
    }

    void GoGame::setPlayer(Stone player) {
        if (player == EMPTY){
            throw std::domain_error("Cannot set the current player to be an empty player");
        }

        // set the active player
        activeColor = player;

        // set the player in the game tree
        gameTree.get().setProperty(SGF::PL, {activeColor == BLACK ? "B" : "W"});
    }

    bool GoGame::isAtRoot() const{
        return gameTree.isAtRoot();
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
        playMoveSequence(std::vector<Playable>(sequence.begin(), sequence.end() - steps));

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

    void GoGame::playMoveSequence(const std::vector<Playable>& moves) {

        auto moveSequence = getMoveSequence();

        for (const Playable& move : moveSequence){
            if (std::holds_alternative<Move>(move)){
                playStone(std::get<Move>(move));
            }
            else {
                addStones(std::get<std::vector<Move>>(move));
            }
        }
    }

    void GoGame::setASCIIMode(bool useASCII) {
        board->setUseASCII(useASCII);
    }
    void GoGame::setLowerLeftCornerCoOrdinates(bool useLowerLeftOrigin) {
        board->setLowerLeftOrigin(useLowerLeftOrigin);
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
        std::vector<Move> moveSequence;

        for (unsigned i = 0; i < sequence.size(); i++){
            if (sequence[i].getMove() != Move::nullMove){
                moveSequence.push_back(sequence[i].getMove());
            }
        }

        return moveSequence;
    }


    std::vector<Playable> GoGame::getDefaultSequence() {

        std::vector<Playable> defaultBranch;

        auto bookmark = gameTree.getSequence();

        while (not gameTree.isAtLeaf()){
            auto child = gameTree.getChildren()[0];
            if (child.getMove() != Move::nullMove){
                defaultBranch.push_back(child.getMove());
            }
            else {
                defaultBranch.push_back(child.getAddedMoves());
            }
            gameTree.stepDown();
        }

        // now that we have found the sequence, return to our original position
        gameTree.advanceToRoot();
        for (const auto& move : bookmark){
            SGF::SGFNode node(move);
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

    utils::Tree<SGF::SGFNode> GoGame::getMoveTree() const {
        return gameTree;
    }

    std::unordered_map<std::string, std::vector<std::string>> GoGame::getProperties() const {

        // get the properties from the root node
        auto node = gameTree.getRoot();

        std::unordered_map<std::string, std::vector<std::string>> properties;

        for (const auto& property : node.getProperties()){
            properties[SGF::toStr(property.first)] = property.second;
        }

        // add the properties from this node
        node = gameTree.get();

        for (const auto& property : node.getProperties()){
            properties[SGF::toStr(property.first)] = property.second;
        }

        return properties;

    }

    void GoGame::setProperty(const std::string& property, const std::string& value) {
        if (SGF::isProperty(property)){
            // get the property
            SGF::SGFProperty SGFProperty = SGF::fromStr(property);

            // check to see if the property is legal for this version of SGF
            if (not SGF::isSGFLegal(SGFProperty, std::stoi(gameTree.getRoot().getProperty(SGF::FF)[0]))){
                throw utils::InvalidSGFException("SGF Property \"" + property + "\" is not supported for SGF FF[" + gameTree.getRoot().getProperty(SGF::FF)[0] + "]");
            }

            // we can't edit the size of the board
            if (SGFProperty == SGF::SZ){
                throw std::domain_error("Cannot edit the \"SZ\" value of an SGF file (it would change the size of the board)");
            }
            if (SGF::isFileWide(SGFProperty)){
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
        if (SGF::isProperty(property)){

            SGF::SGFProperty SGFProperty = SGF::fromStr(property);

            // check to see if the property is legal for this version of SGF
            if (not SGF::isSGFLegal(SGFProperty, std::stoi(gameTree.getRoot().getProperty(SGF::FF)[0]))){
                throw utils::InvalidSGFException("SGF Property \"" + property + "\" is not supported for SGF FF[" + gameTree.getRoot().getProperty(SGF::FF)[0] + "]");
            }

            if (SGFProperty == SGF::SZ){
                throw std::domain_error("Cannot edit the \"SZ\" value of an SGF file (it would change the size of the board)");
            }

            if (SGF::isFileWide(SGFProperty)){
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

    Stone GoGame::getSpace(Vertex point) const {
        return getSpace(point.getX(), point.getY());
    }
    Stone GoGame::getSpace(unsigned x, unsigned y) const {
        return board->getSpace(x, y).getStone();
    }
    Stone GoGame::getActivePlayer() const {
        return gameTree.getDepth() % 2 == 0 ? getStartingColor() : oppositeColor(getStartingColor());
    }

    std::unique_ptr<_board> GoGame::copyBoard() const {

        std::unique_ptr<_board> newBoard;

        switch (board->getSide()){
            case 19:
                newBoard = std::make_unique<Board<19>>(*((Board<19>*) board.get()));
                break;
            case 13:
                newBoard =  std::make_unique<Board<13>>(*((Board<13>*) board.get()));
                break;
            case 9:
                newBoard = std::make_unique<Board<9>>(*((Board<9>*) board.get()));
                break;
            default:
                throw py::value_error("cannot construct board of size " + std::to_string(board->getSide()));
        }

        return newBoard;
    }

    unsigned GoGame::getSide() const {
        return board->getSide();
    }

    /**
     *
     * Obtains the winner of the game
     *
     * @return
     */
    sente::Stone GoGame::getWinner() const {
        if (not gameTree.getRoot().hasProperty(SGF::RE)){
            // return an empty stone
            return EMPTY;
        }
        else {
            // obtain the winner from the property
            return gameTree.getRoot().getProperty(SGF::RE).at(0).at(0) == 'B' ? BLACK : WHITE;
        }
    }

    /**
     *
     * Score the game automatically using chinese rules
     *
     * TODO: score using korean and Chinese rules
     *
     */
    void GoGame::score() {

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

        // compute the black and white raw scores
        blackPoints = blackTerritory + blackStones;
        whitePoints = whiteTerritory + whiteStones + komi;

        std::stringstream results;

        results << (blackPoints > whitePoints ? "B" : "W") << "+" << std::fixed << std::setprecision(1) << std::fabs(blackPoints - whitePoints);

        // convert the result to a string
        gameTree.getRoot().setProperty(SGF::RE, {results.str()});
    }

    std::string GoGame::getResult() const {
        if (isOver()){
            return getProperties().at("RE")[0];
        }
        else {
            throw std::domain_error("game is not yet over, results cannot be obtained");
        }
    }

    py::dict GoGame::getScores() const {

        if (not isOver()){
            throw std::domain_error("game is not yet over, scores cannot be obtained");
        }

        py::dict result;

        result[py::cast(BLACK)] = blackPoints;
        result[py::cast(WHITE)] = whitePoints;
        result["result"] = getResult();

        return result;
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

        return moves;

    }

    Vertex GoGame::getKoPoint() const {
        return {koPoint.getX(), koPoint.getY()};
    }

    std::string GoGame::getComment() const {
        if (gameTree.get().hasProperty(SGF::C)){
            return gameTree.get().getProperty(SGF::C)[0];
        }
        else {
            return "";
        }
    }
    void GoGame::setComment(const std::string& comment) const {
        gameTree.get().setProperty(SGF::C, {comment});
    }

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
                board = std::make_shared<Board<19>>(false, false);
                break;
            case 13:
                board = std::make_shared<Board<13>>(false, false);
                break;
            case 9:
                board = std::make_shared<Board<9>>(false, false);
                break;
            default:
                throw std::domain_error("Invalid Board size " +
                                            std::to_string(side) + " only 9x9, 13x13 and 19x19 are currently supported");
        }
    }
    void GoGame::clearBoard() {

        switch (board->getSide()){
            case 19:
                board = std::make_shared<Board<19>>(board->getUseASCII(), board->getLowerLeftOrigin());
                break;
            case 13:
                board = std::make_shared<Board<13>>(board->getUseASCII(), board->getLowerLeftOrigin());
                break;
            case 9:
                board = std::make_shared<Board<9>>(board->getUseASCII(), board->getLowerLeftOrigin());
                break;
            default:
                throw std::domain_error("Invalid Board size " +
                                        std::to_string(board->getSide()) + " only 9x9, 13x13 and 19x19 are currently supported");
        }
    }

    void GoGame::resetKoPoint(){
        koPoint = Move::pass(getActivePlayer());
    }

    /**
     *
     * gets the color of the player that starts the game
     *
     * @return
     */
    Stone GoGame::getStartingColor() const {
        return gameTree.getRoot().hasProperty(SGF::HA) ? WHITE : BLACK;
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
            // if we are not connected to any group, create a new group!
            groups[move] = std::make_shared<Group>(move);
        }
        else {
            // if we are connected to a group, connect our stones
            connectGroups(move, ourAffectedGroups);
        }

        // reset the ko point
        resetKoPoint();

        // capture stones
        for (const auto& group : theirAffectedGroups){
            if (utils::getLiberties(*group, *board).empty()){

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
                    // erase the item
                    groups.erase(stone);
                    board->captureStone(stone);
                    capturedStones[gameTree.getDepth()].insert(stone);
                }
            }
        }

        // Handle legal self-captures under Tromp-Taylor rules
        if (rules == TROMP_TAYLOR and not isNotSelfCapture(move)) {
            // erase the item
            groups.erase(move);
            board->captureStone(move);
            capturedStones[gameTree.getDepth()].insert(move);
        }
    }

    bool GoGame::isCorrectColor(const Move &move) {
        return activeColor == move.getStone();
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

        // create a temporary group object and merge all the affected groups into it
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
        return gameTree.getRoot().hasProperty(SGF::RE);
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