//
// Created by arthur wesley on 8/27/21.
//

#ifndef SENTE_SGFNODE_H
#define SENTE_SGFNODE_H

#include "SGFProperty.h"
#include "../../Game/Move.h"

namespace sente::SGF {

    class SGFNode {
    public:

        explicit SGFNode() = default;
        explicit SGFNode(const Move& move);
        explicit SGFNode(const std::vector<std::string>& addedMoves);

        Move getMove() const;
        std::unordered_set<Move> getAddedMoves() const;

        void setProperty(SGFProperty property, const std::vector<std::string>& value);
        void appendProperty(SGFProperty property, const std::string& value);
        void removeItem(SGFProperty property, const std::string& del);

        bool hasProperty(SGFProperty property) const;
        bool isEmpty() const;
        std::vector<SGFProperty> getInvalidProperties(unsigned version) const;

        std::unordered_map<SGFProperty, std::vector<std::string>> getProperties() const;

        std::vector<std::string> getProperty(SGFProperty property) const;

        explicit operator std::string() const;
        bool operator==(const SGFNode& other) const;

    private:

        Move move;
        std::unordered_set<Move> addedMoves;
        std::unordered_map<SGFProperty, std::vector<std::string>> properties;

    };

}


#endif //SENTE_SGFNODE_H
