//
// Created by arthur wesley on 7/22/21.
//

#include "GoComponents.h"
#include "../Utils/SenteExceptions.h"

namespace sente {

    double determineKomi(Rules ruleset) {
        switch (ruleset){
        default:
        case CHINESE:
        case OTHER:
            return 7.5;
        case JAPANESE:
        case KOREAN:
            return 6.5;
        }
    }

    Rules rulesFromStr(std::string ruleString){
        std::transform(ruleString.begin(), ruleString.end(), ruleString.begin(), ::tolower);
        if (ruleString == "japanese"){
            return JAPANESE;
        }
        else if (ruleString == "chinese"){
            return CHINESE;
        }
        else if (ruleString == "korean" or ruleString == "korea"){
            return KOREAN;
        }
        else {
            return OTHER;
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