//
// Created by arthur wesley on 7/2/21.
//

#ifndef SENTE_COMPONENTS_H
#define SENTE_COMPONENTS_H

#include <sstream>
#include <ciso646>

#include "Board.h"
#include "Move.h"

namespace sente {

    enum Rules {
        CHINESE,
        JAPANESE,
        KOREAN,
        OTHER
    };

    double determineKomi(Rules ruleset);

    Rules rulesFromStr(std::string ruleString);

    struct Results{

        explicit Results(Stone resigningPlayer);

        Results(Rules rules, double komi, unsigned blackTerritory, unsigned whiteTerritory,
                       unsigned blackStones, unsigned whiteStones);

        Stone winner() const;
        double margin() const;

        Rules rules;
        double komi;

        unsigned blackScore;
        unsigned whiteScore;

        Stone resignedPlayer;

        operator std::string() const;

    };

}

#endif //SENTE_COMPONENTS_H
