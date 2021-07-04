//
// Created by arthur wesley on 6/27/21.
//

#ifndef SENTE_GOGAME_H
#define SENTE_GOGAME_H

#include <map>
#include <array>
#include <vector>
#include <memory>
#include <fstream>
#include <unordered_map>
#include <unordered_set>

#include "Tree.h"
#include "GoComponents.h"
#include "Group.h"

namespace std {

    template<>
    struct hash<pair<unsigned, unsigned>>{
        size_t operator()(const pair<unsigned, unsigned>& toHash) const noexcept;
    };

}

namespace sente {

    class GoGame {
    public:

        GoGame(unsigned side, Rules rules);
        explicit GoGame(const std::string& sgf_file);
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

        void advanceToRoot();
        void stepUp(unsigned steps);
        void playDefaultBranch();
        void playMoveSequence(const std::vector<Move>& moves);
        std::vector<Move> getMoveSequence();
        const std::unordered_set<Move>& getBranches();

        Stone getSpace(unsigned x, unsigned y) const;
        Stone getActivePlayer() const;

        const _board& getBoard() const;

        std::map<Stone, double> score(double komi) const;
        std::unordered_set<Move> getLegalMoves() const;

        explicit operator std::string() const;

        std::string toSGF(std::unordered_map<std::string, std::string> attributes) const;

    private:

        Rules rules;
        Stone resigned;

        unsigned passCount = 0;

        // todo: look into moving the board onto the stack
        std::unique_ptr<_board> board;

        sente_utils::Tree<Move> moveTree;

        std::unordered_map<Move, std::shared_ptr<Group>> groups;
        std::unordered_map<unsigned, std::unordered_set<Move>> capturedStones;

        Move koPoint;

        void makeBoard(unsigned side);
        void resetKoPoint();

        void updateBoard(Move move);
        void connectGroups(Move move, const std::unordered_set<std::shared_ptr<Group>>& toConnect);

        bool isCorrectColor(const Move& move) const;
        bool isNotSelfCapture(const Move& move) const;
        bool isNotKoPoint(const Move& move) const;

        unsigned getLiberties(const Group& group) const;

    };
}

#endif //SENTE_GOGAME_H
