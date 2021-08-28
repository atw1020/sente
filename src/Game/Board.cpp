//
// Created by arthur wesley on 7/2/21.
//

#include <array>

#include "../include/Game/Board.h"

namespace sente {

    template<>
    bool Board<19>::isStar(unsigned int x, unsigned int y) const {
        return (x == 3 or x == 9 or x == 15) and (y == 3 or y == 9 or y == 15);
    }

    template<>
    bool Board<13>::isStar(unsigned x, unsigned y) const {
        return ((x == 3 or x == 9) and (y == 3 or y == 9)) or (x == 6 and y == 6);
    }

    template<>
    bool Board<9>::isStar(unsigned x, unsigned y) const {
        return ((x == 2 or x == 6) and (y == 2 or y == 6)) or (x == 4 and y == 4);
    }
}