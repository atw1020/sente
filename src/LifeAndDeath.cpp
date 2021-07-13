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
         * generates a list of all groups that are adjacent to a territory
         *
         * @param territory territory to look for adjacent groups in
         * @param board board to look on
         * @param groups map from moves to the group they belong to
         * @return
         */
        std::unordered_set<std::shared_ptr<Group>> getAdjacentGroups(const std::unordered_set<Move>& territory,
                                                    const _board& board,
                                                    const std::unordered_map<Move, std::shared_ptr<Group>>& groupMap){

            std::unordered_set<std::shared_ptr<Group>> groups;

            for (const Move& move : territory){
                for (const Point neighbor : move.getAdjacentMoves(board.getSide())){
                    groups.insert(groupMap.at(board.getSpace(neighbor)));
                }
            }

            return groups;

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
