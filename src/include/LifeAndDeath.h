//
// Created by arthur wesley on 7/5/21.
//

#ifndef SENTE_LIFEANDDEATH_H
#define SENTE_LIFEANDDEATH_H

#include <unordered_set>

#include "Move.h"
#include "Board.h"

namespace sente {
    namespace utils {

        std::unordered_set<Move> getLiberties(const Group& group, const _board& board);

        std::unordered_set<std::shared_ptr<Group>> getAdjacentGroups(const std::unordered_set<Move>& territory,
                                                                const _board& board,
                                                                const std::unordered_map<Move, std::shared_ptr<Group>>& groups);

        std::unordered_set<Move> getConnectedPoints(const Move& startMove,
                                                    const _board& board);
        void getConnectedPoints(const Move& startMove,
                                const _board& board,
                                std::unordered_set<Move>& foundConnections);

    }
}


#endif //SENTE_LIFEANDDEATH_H
