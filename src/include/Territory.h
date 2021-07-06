//
// Created by arthur wesley on 7/5/21.
//

#ifndef SENTE_TERRITORY_H
#define SENTE_TERRITORY_H

#include <unordered_set>

#include "Move.h"
#include "Board.h"

namespace sente {
    namespace utils {

        std::unordered_set<Move> getLiberties(const Group& group, const _board& board);

        unsigned countRawEyes(const Group& group, const _board& board);

        std::unordered_set<Move> getConnectedPoints(const Move& startMove,
                                                    const _board& board);
        void getConnectedPoints(const Move& startMove,
                                const _board& board,
                                std::unordered_set<Move>& foundConnections);

    }
}


#endif //SENTE_TERRITORY_H
