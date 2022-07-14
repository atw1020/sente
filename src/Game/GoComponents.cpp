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
        case TROMP_TAYLOR:
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
        } else if (ruleString == "tromp-taylor") {
            return TROMP_TAYLOR;
        } else {
            return OTHER;
        }
    }

}