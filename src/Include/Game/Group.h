//
// Created by arthur wesley on 6/27/21.
//

#ifndef SENTE_GROUP_H
#define SENTE_GROUP_H

#include <vector>
#include <ciso646>
#include <functional>
#include <unordered_set>

#include "Move.h"

namespace sente {

    class Group {
    public:

        Group();
        explicit Group(const Move& move);
        Group(const Move& move, const std::unordered_set<std::shared_ptr<Group>>& groups);
        ~Group();

        bool isValidGroup() const;
        bool contains(Move move) const;

        Stone getColor() const;

        std::unordered_set<Move> getMoves() const;

    private:

        std::unordered_set<Move> stones;

        double center_x;
        double center_y;

        Stone color;

    };

}

namespace std {

    template<>
    struct hash<sente::Group>{
        size_t operator()(const sente::Group& group) const noexcept;
    };

}


#endif //SENTE_GROUP_H
