//
// Created by arthur wesley on 7/2/21.
//

#ifndef SENTE_COMPONENTS_H
#define SENTE_COMPONENTS_H

#include <sstream>

#include "Board.h"
#include "Move.h"

namespace sente {

    enum Rules {
        CHINESE,
        JAPANESE
    };

    struct Results{

        explicit Results(Stone resigningPlayer){
            resignedPlayer = resigningPlayer;
        }

        Results(Rules rules, double komi, unsigned blackTerritory, unsigned whiteTerritory,
                   unsigned blackStones, unsigned whiteStones){
            this->rules = rules;
            this->komi = komi;

            if (rules == CHINESE){
                this->blackScore = blackTerritory + blackStones;
                this->whiteScore = whiteTerritory+ whiteStones;
            }
            if (rules == JAPANESE){
                this->blackScore = blackTerritory;
                this->whiteScore = whiteTerritory;
            }

            resignedPlayer = EMPTY;
        }

        Stone winner() const{
            if (resignedPlayer != EMPTY){
                return getOpponent(resignedPlayer);
            }
            else {
                return (blackScore > whiteScore + komi) ? BLACK : WHITE;
            }
        }

        double margin() const {
            fabs(blackScore - whiteScore - komi);
        }

        Rules rules;
        double komi;

        unsigned blackScore;
        unsigned whiteScore;

        Stone resignedPlayer;

        explicit operator std::string() const{

            std::stringstream rep;
            rep << (winner() == BLACK ? "B+" : "W+");

            if (resignedPlayer == EMPTY){
                rep << margin();
            }
            else {
                rep << "R";
            }

            return rep.str();

        }

    };

}

#endif //SENTE_COMPONENTS_H
