//
// Created by arthur wesley on 7/22/21.
//

#include "include/GoComponents.h"

namespace sente {

    double getKomi(Rules ruleset) {
        switch (ruleset){
        case CHINESE:
            return 7.5;
        case JAPANESE:
            return 6.5;
        }
    }

    Results::Results(Stone resigningPlayer) {
        resignedPlayer = resigningPlayer;
    }

    Results::Results(Rules rules, double komi, unsigned blackTerritory, unsigned whiteTerritory,
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

    Stone Results::winner() const{
        if (resignedPlayer != EMPTY){
            return getOpponent(resignedPlayer);
        }
        else {
            return (blackScore > whiteScore + komi) ? BLACK : WHITE;
        }
    }

    double Results::margin() const {
        return fabs(blackScore - whiteScore - komi);
    }

    Results::operator std::string() const{

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

}