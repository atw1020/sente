//
// Created by arthur wesley on 6/27/21.
// tonight

#ifndef SENTE_GOGAME_H
#define SENTE_GOGAME_H

#include <array>
#include <vector>
#include <memory>
#include <fstream>
#include <unordered_map>
#include <unordered_set>

#include <pybind11/pybind11.h>

#include "../Utils/Tree.h"
#include "Group.h"
#include "GoComponents.h"
#include "../Utils/SGFNode.h"

#ifdef __CYGWIN__
#include <ciso646>
#endif

namespace py = pybind11;

namespace std {

    template<>
    struct hash<pair<unsigned, unsigned>>{
        size_t operator()(const pair<unsigned, unsigned>& toHash) const noexcept;
    };

}

namespace sente {

    class GoGame {
    public:

        GoGame(unsigned side, Rules rules, double komi);
        explicit GoGame(utils::Tree<utils::SGFNode>& SGFTree);

        void resetBoard();

        ///
        /// Move Legality
        ///

        bool isLegal(const Move& move) const;
        bool isLegal(unsigned x, unsigned y) const;
        bool isLegal(unsigned x, unsigned y, Stone stone) const;
        bool isOver() const;

        void playStone(const Move& move);
        void playStone(unsigned x, unsigned y);
        void playStone(unsigned x, unsigned y, Stone stone);

        ///
        /// movement through the game tree
        ///

        bool isAtRoot() const;
        void advanceToRoot();
        void stepUp(unsigned steps);

        void playDefaultBranch();
        std::vector<Move> getDefaultBranch();
        void playMoveSequence(const std::vector<Move>& moves);
        std::vector<Move> getMoveSequence();
        std::vector<Move> getBranches();

        unsigned getMoveNumber() const;
        utils::Tree<utils::SGFNode> getMoveTree() const;

        std::unordered_map<std::string, std::vector<std::string>> getAttributes() const;

        Stone getSpace(unsigned x, unsigned y) const;
        Stone getActivePlayer() const;

        const _board& getBoard() const;

        Results getResults() const;
        Results score() const;
        std::vector<Move> getLegalMoves() const;

        Point getKoPoint() const;

        explicit operator std::string() const;

    private:

        Rules rules;
        Stone resignedPlayer;

        double komi;

        unsigned passCount = 0;

        // todo: look into moving the board onto the stack
        std::unique_ptr<_board> board;

        utils::Tree<utils::SGFNode> gameTree;

        std::unordered_map<Move, std::shared_ptr<Group>> groups;
        std::unordered_map<unsigned, std::unordered_set<Move>> capturedStones;

        Move koPoint;

        void makeBoard(unsigned side);
        void resetKoPoint();

        void updateBoard(const Move& move);
        void connectGroups(const Move& move, const std::unordered_set<std::shared_ptr<Group>>& toConnect);

        bool isCorrectColor(const Move& move) const;
        bool isNotSelfCapture(const Move& move) const;
        bool isNotKoPoint(const Move& move) const;

    };
}

#endif //SENTE_GOGAME_H
