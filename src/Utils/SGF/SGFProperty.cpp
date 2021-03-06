//
// Created by arthur wesley on 8/27/21.
//

#include <algorithm>
#include <unordered_map>

#include <pybind11/pybind11.h>

#include "SGFProperty.h"
#include "../SenteExceptions.h"

#define stringify(x) #x

namespace sente::SGF {

    bool isSGFLegal(SGFProperty property, unsigned version){

        // from version list https://www.red-bean.com/sgf/proplist_ff.html
        switch (property){
            case EL:
            case EX:
            case L:
            case M:
                return version <= 2;
            case BS:
            case CH:
            case HO:
            case RG:
            case SC:
            case WS:
                return version <= 3;
            case AB:
            case AE:
            case AW:
            case B:
            case BL:
            case BM:
            case BR:
            case C:
            case DT:
            case EV:
            case FG:
            case GB:
            case GC:
            case GM:
            case GN:
            case GW:
            case HA:
            case KM:
            case N:
            case PB:
            case PC:
            case PL:
            case PW:
            case RE:
            case RO:
            case SL:
            case SO:
            case SZ:
            case TB:
            case TE:
            case TM:
            case TW:
            case US:
            case V:
            case VW:
            case W:
            case WL:
            case WR:
                return version >= 1;
            case FF:
                return version >= 2;
            case AN:
            case BT:
            case CP:
            case CR:
            case DM:
            case DO:
            case IT:
            case KO:
            case LB:
            case LN:
            case MA:
            case MN:
            case OB:
            case ON:
            case OW:
            case RU:
            case TR:
            case UC:
            case WT:
            case SE:
                return version >= 3;
            case AP:
            case AR:
            case CA:
            case DD:
                // case IP:
                // case IY:
            case OT:
            case PM:
            case SQ:
            case ST:
                // case SU:
                return version >= 4;
            case ID:
            case LT:
            case OM:
            case OP:
            case OV:
            case SI:
            case TC:
                return version == 3;
            default:
                return false;
        }

    }

    std::vector<unsigned> getPossibleSGFVersions(const std::unordered_set<SGFProperty>& properties){

        std::vector<unsigned> versions = {1, 2, 3, 4};
        std::vector<unsigned> illegalVersions;

        for (auto property : properties){

            // reset the list of illegal versions
            illegalVersions = {};

            // for each version
            for (auto version : versions){
                // check to see if this property is illegal on this version
                if (not isSGFLegal(property, version)){
                    illegalVersions.push_back(version);
                }
            }

            // remove the illegal versions from the list of versions
            for (auto version : illegalVersions){
                versions.erase(std::find(versions.begin(),  versions.end(), version));
            }
        }

        return versions;

    }

    std::unordered_map<std::string, SGFProperty> strToProperty {
            {""     , NONE},
            {stringify(B),   B},
            {stringify(KO), KO},
            {stringify(MN), MN},
            {stringify(W),   W},
            {stringify(AB), AB},
            {stringify(AE), AE},
            {stringify(AW), AW},
            {stringify(PL), PL},
            {stringify(C),   C},
            {stringify(DM), DM},
            {stringify(GB), GB},
            {stringify(GW), GW},
            {stringify(HO), HO},
            {stringify(N) ,  N},
            {stringify(UC), UC},
            {stringify(V),   V},
            {stringify(BM), BM},
            {stringify(DO), DO},
            {stringify(IT), IT},
            {stringify(TE), TE},
            {stringify(AR), AR},
            {stringify(CR), CR},
            {stringify(DD), DD},
            {stringify(LB), LB},
            {stringify(LN), LN},
            {stringify(MA), MA},
            {stringify(SL), SL},
            {stringify(SQ), SQ},
            {stringify(TR), TR},
            {stringify(AP), AP},
            {stringify(CA), CA},
            {stringify(FF), FF},
            {stringify(GM), GM},
            {stringify(ST), ST},
            {stringify(SZ), SZ},
            {stringify(AN), AN},
            {stringify(BR), BR},
            {stringify(BT), BT},
            {stringify(CP), CP},
            {stringify(DT), DT},
            {stringify(EV), EV},
            {stringify(GN), GN},
            {stringify(GC), GC},
            {stringify(ON), ON},
            {stringify(OT), OT},
            {stringify(PB), PB},
            {stringify(PC), PC},
            {stringify(PW), PW},
            {stringify(RE), RE},
            {stringify(RO), RO},
            {stringify(RU), RU},
            {stringify(SO), SO},
            {stringify(TM), TM},
            {stringify(US), US},
            {stringify(WR), WR},
            {stringify(WT), WT},
            {stringify(BL), BL},
            {stringify(OB), OB},
            {stringify(OW), OW},
            {stringify(WL), WL},
            {stringify(FG), FG},
            {stringify(PM), PM},
            {stringify(VW), VW},
            {stringify(HA), HA},
            {stringify(KM), KM},
            {stringify(TB), TB},
            {stringify(TW), TW},
            {stringify(ID), ID},
            {stringify(LT), LT},
            {stringify(OM), OM},
            {stringify(OP), OP},
            {stringify(OV), OV},
            {stringify(SE), SE},
            {stringify(SI), SI},
            {stringify(TC), TC},
            {stringify(EL), EL},
            {stringify(EX), EX},
            {stringify(L),   L},
            {stringify(M),   M},
            {stringify(BS), BS},
            {stringify(CH), CH},
            {stringify(RG), RG},
            {stringify(SC), SC},
            {stringify(WS), WS},
    };

    std::unordered_map<SGFProperty, std::string> propertyToStr {
            {B,  stringify(B) },
            {KO, stringify(KO)},
            {MN, stringify(MN)},
            {W,  stringify(W) },
            {AB, stringify(AB)},
            {AE, stringify(AE)},
            {AW, stringify(AW)},
            {PL, stringify(PL)},
            {C,  stringify(C) },
            {DM, stringify(DM)},
            {GB, stringify(GB)},
            {GW, stringify(GW)},
            {HO, stringify(HO)},
            {N,  stringify(N) },
            {UC, stringify(UC)},
            {V,  stringify(V) },
            {BM, stringify(BM)},
            {DO, stringify(DO)},
            {IT, stringify(IT)},
            {TE, stringify(TE)},
            {AR, stringify(AR)},
            {CR, stringify(CR)},
            {DD, stringify(DD)},
            {LB, stringify(LB)},
            {LN, stringify(LN)},
            {MA, stringify(MA)},
            {SL, stringify(SL)},
            {SQ, stringify(SQ)},
            {TR, stringify(TR)},
            {AP, stringify(AP)},
            {CA, stringify(CA)},
            {FF, stringify(FF)},
            {GM, stringify(GM)},
            {ST, stringify(ST)},
            {SZ, stringify(SZ)},
            {AN, stringify(AN)},
            {BR, stringify(BR)},
            {BT, stringify(BT)},
            {CP, stringify(CP)},
            {DT, stringify(DT)},
            {EV, stringify(EV)},
            {GN, stringify(GN)},
            {GC, stringify(GC)},
            {ON, stringify(ON)},
            {OT, stringify(OT)},
            {PB, stringify(PB)},
            {PC, stringify(PC)},
            {PW, stringify(PW)},
            {RE, stringify(RE)},
            {RO, stringify(RO)},
            {RU, stringify(RU)},
            {SO, stringify(SO)},
            {TM, stringify(TM)},
            {US, stringify(US)},
            {WR, stringify(WR)},
            {WT, stringify(WT)},
            {BL, stringify(BL)},
            {OB, stringify(OB)},
            {OW, stringify(OW)},
            {WL, stringify(WL)},
            {FG, stringify(FG)},
            {PM, stringify(PM)},
            {VW, stringify(VW)},
            {HA, stringify(HA)},
            {KM, stringify(KM)},
            {TB, stringify(TB)},
            {TW, stringify(TW)},
            {ID, stringify(ID)},
            {LT, stringify(LT)},
            {OM, stringify(OM)},
            {OP, stringify(OP)},
            {OV, stringify(OV)},
            {SE, stringify(SE)},
            {SI, stringify(SI)},
            {TC, stringify(TC)},
            {EL, stringify(EL)},
            {EX, stringify(EX)},
            {L,  stringify(L) },
            {M,  stringify(M) },
            {BS, stringify(BS)},
            {CH, stringify(CH)},
            {RG, stringify(RG)},
            {SC, stringify(SC)},
            {WS, stringify(WS)}
    };

    SGFProperty fromStr(const std::string& SGFProperty){
        if (strToProperty.find(SGFProperty) != strToProperty.end()){
            return strToProperty[SGFProperty];
        }
        else {
            throw utils::InvalidSGFException("Invalid SGF command: \"" + SGFProperty + "\"");
        }
    }

    std::string toStr(SGFProperty property){
        return propertyToStr[property];
    }

    bool isProperty(const std::string& property){
        return strToProperty.find(property) != strToProperty.end();
    }

    bool isFileWide(SGFProperty command){
        switch (command){
            case AP:
            case CA:
            case FF:
            case GM:
            case ST:
            case SZ:
            case AN:
            case BR:
            case BT:
            case CP:
            case DT:
            case EV:
            case GN:
            case GC:
            case ON:
            case OT:
            case PB:
            case PC:
            case PW:
            case RE:
            case RO:
            case RU:
            case SO:
            case TM:
            case US:
            case WR:
            case WT:
            case HA:
            case KM:
            case TB:
            case TW:
                return true;
            default:
                return false;
        }
    }


}

namespace std {
    size_t std::hash<sente::SGF::SGFProperty>::operator()(const sente::SGF::SGFProperty &SGFProperty) const noexcept {
        return unsigned(SGFProperty);
    }
}
