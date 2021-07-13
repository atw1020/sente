//
// Created by arthur wesley on 7/2/21.
//

#ifndef SENTE_COMPONENTS_H
#define SENTE_COMPONENTS_H

#include "Board.h"
#include "Move.h"

namespace sente {

    enum Rules {
        CHINESE,
        JAPANESE
    };

    struct gameResult{

        gameResult(Rules rules, double komi, unsigned blackTerritory, unsigned whiteTerritory,
                   unsigned blackStones, unsigned whiteStones){
            this->rules = rules;
            this->komi = komi;

            if (rules == CHINESE){
                this->blackScore = blackTerritory + blackStones;
                this->whiteScore = whiteTerritory + komi + whiteStones;
            }
            if (rules == JAPANESE){
                this->blackScore = blackTerritory;
                this->whiteScore = whiteTerritory + komi;
            }
        }

        Rules rules;
        double komi;

        double blackScore;
        double whiteScore;

    };

}

#endif //SENTE_COMPONENTS_H
