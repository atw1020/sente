//
// Created by arthur wesley on 7/5/21.
//

#include "include/LifeAndDeath.h"

namespace sente {

    namespace utils {

        /**
         *
         * get a set containing all of the moves
         *
         * @param group
         * @param board
         * @return
         */
        std::unordered_set<Move> getLiberties(const Group& group, const _board& board) {

            std::unordered_set<Move> liberties;

            for (const auto& move : group.getMoves()){

                for (const auto& adjacentSpace : move.getAdjacentMoves(board.getSide())){
                    if (board.getSpace(adjacentSpace.first, adjacentSpace.second).getStone() == EMPTY){
                        liberties.insert(Move(adjacentSpace.first, adjacentSpace.second, EMPTY));
                    }
                }
            }

            return liberties;

        }

        /**
         *
         * counts the number of raw eyes (eyes that contain no enemy stones)
         *
         * @param group
         * @return
         */
        unsigned countRawEyes(const Group& group, const _board& board) {

            auto liberties = getLiberties(group, board);
            std::vector<std::unordered_set<Move>> eyes;

            // go through all of the group's liberties
            for (const auto& liberty : liberties){
                // find all of the adjacent empty spaces
                auto connectedStones = getConnectedPoints(liberty, board);

                if (std::find(eyes.begin(), eyes.end(), connectedStones) != eyes.end()){
                    eyes.push_back(connectedStones);
                }
            }

            return eyes.size();
        }

        std::unordered_set<Move> getConnectedPoints(const Move& startMove,
                                                    const _board& board) {
            auto connections = std::unordered_set<Move>{};
            getConnectedPoints(startMove, board, connections);
            return connections;
        }

        /**
         *
         * finds all of the stones of the same color that are connected to a given starting move
         *
         * @param startMove
         * @param foundConnections
         * @return
         */
        void getConnectedPoints(const Move& startMove,
                                const _board& board,
                                std::unordered_set<Move>& foundConnections) {

            // insert the current move into the list of found moves
            foundConnections.insert(startMove);

            for (const auto& point : startMove.getAdjacentMoves(board.getSide())){
                Move move = board.getSpace(point.first, point.second);
                // if we haven't seen the move before and the color of the stone is correct
                if (foundConnections.find(move) == foundConnections.end() and move.getStone() == startMove.getStone()){
                    // insert this stone and all of it's children into the set
                    getConnectedPoints(move, board, foundConnections);
                }
            }
        }
    }
}
