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
#include "../Utils/SGF/SGFNode.h"

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

        GoGame(unsigned side, Rules rules, double komi,
               std::vector<Move> handicap);
        explicit GoGame(utils::Tree<SGF::SGFNode>& SGFTree);

        void resetBoard();

        ///
        /// Moves
        ///

        bool isLegal(const Move& move);
        bool isLegal(unsigned x, unsigned y);
        bool isLegal(unsigned x, unsigned y, Stone stone);
        bool isGTPLegal(const Move& move);
        bool isOver() const;

        void playStone(const Move& move);
        void playStone(unsigned x, unsigned y);
        void playStone(unsigned x, unsigned y, Stone stone);

        bool isAddLegal(const Move& move);

        // void addStones(const std::vector<Move>& move);
        void addStone(const Move& move);

        ///
        /// movement through the game tree
        ///

        [[nodiscard]] bool isAtRoot() const;
        void stepUp(unsigned steps);

        void playDefaultSequence();
        void playMoveSequence(const std::vector<Move>& moves);

        std::vector<Move> getBranches();
        std::vector<Move> getMoveSequence();
        std::vector<Move> getDefaultSequence();

        std::vector<std::vector<Move>> getSequences(const std::vector<Move>& currentSequence);

        [[nodiscard]] unsigned getMoveNumber() const;
        [[nodiscard]] utils::Tree<SGF::SGFNode> getMoveTree() const;

        ///
        /// Getting and setting properties
        ///

        std::unordered_map<std::string, std::vector<std::string>> getProperties() const;

        void setProperty(const std::string& command, const std::string& value);
        void setProperty(const std::string& command, const std::vector<std::string>& values);

        [[nodiscard]] std::string getComment() const;
        void setComment(const std::string& comment) const;

        ///
        /// Getter and Setter methods
        ///

        void setASCIIMode(bool useASCII);
        void setLowerLeftCornerCoOrdinates(bool useLowerLeftOrigin);

        [[nodiscard]] Stone getSpace(Vertex point) const;
        [[nodiscard]] Stone getSpace(unsigned x, unsigned y) const;
        [[nodiscard]] Stone getActivePlayer() const;

        [[nodiscard]] std::unique_ptr<_board> copyBoard() const;
        [[nodiscard]] unsigned getSide() const;

        void score();
        std::string getResult() const;
        sente::Stone getWinner() const;
        py::dict getScores() const;
        std::vector<Move> getLegalMoves();

        Vertex getKoPoint() const;

        Rules getRules() const;
        double getKomi() const;

        void setKomi(double newKomi);

        explicit operator std::string() const;

        std::unordered_set<Move> getHandicapStones() const;

    private:

        // TODO: get more optimal memory placement to minimize padding

        Rules rules; // 4 bytes
        unsigned passCount = 0; // 4 bytes
        // TODO: Switch from doubles to floats
        double komi; // 8 bytes

        double blackPoints = NAN; // 8 bytes
        double whitePoints = NAN; // 8 bytes

        // Changes.txt: look into moving the board onto the stack
        std::shared_ptr<_board> board; // 16 bytes

        utils::Tree<SGF::SGFNode> gameTree; // 32 bytes

        std::unordered_map<Move, std::shared_ptr<Group>> groups; // 40 bytes
        std::unordered_map<unsigned, std::unordered_set<Move>> capturedStones; // 40 bytes

        // total size: 64 + 80 = 144 bytes

        Move koPoint;

        void makeBoard(unsigned side);
        void clearBoard();
        void resetKoPoint();

        [[nodiscard]] Stone getStartingColor() const;

        void updateBoard(const Move& move);
        void connectGroups(const Move& move, const std::unordered_set<std::shared_ptr<Group>>& toConnect);

        bool isCorrectColor(const Move& move);
        bool isNotSelfCapture(const Move& move) const;
        bool isNotKoPoint(const Move& move) const;
    };
}

#endif //SENTE_GOGAME_H
