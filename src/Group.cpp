//
// Created by arthur wesley on 6/27/21.
//

#include "include/Group.h"

#include <iostream>

namespace std {

    size_t hash<sente::Group>::operator()(const sente::Group &group) const noexcept {
        // TODO: implement
        return 0;
    }

}

namespace sente {

    Group::Group(){

    }

    Group::Group(const Move& move) {
#ifdef DEBUG_LOG
        std::cout << "creating a group..." << std::endl;
#endif

        stones = std::unordered_set<Move>{};
        stones.insert(move);

        color = move.getStone();
    }

    Group::Group(const Move& move, const std::unordered_set<std::shared_ptr<Group>>& groups) {

#ifdef DEBUG_LOG
        std::cout << "merging " << groups.size() << " groups together..." << std::endl;
#endif

        stones = std::unordered_set<Move>{};
        color = (*groups.begin())->color;

        assert(color == move.getStone());
        stones.insert(move);

        for (const auto& group : groups){

            // make sure that everything is OK
            assert(group->color == color);

            // combine the stones
            stones.insert(group->stones.begin(), group->stones.end());
        }

    }

    Group::~Group(){
#ifdef DEBUG_LOG
        std::cout << "destroying group with " << stones.size() << " stones..." << std::endl;
#endif
    }

    /**
     *
     * Check to see if the group contains a specific move
     *
     * @param move
     * @return
     */
    bool Group::contains(Move move) const {
        return stones.find(move) != stones.end();
    }

    Stone Group::getColor() const {
        return color;
    }

    std::unordered_set<Move> Group::getMoves() const {
        return stones;
    }

}