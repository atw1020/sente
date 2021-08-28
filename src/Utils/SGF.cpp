//
// Created by arthur wesley on 7/2/21.
//

#include <stack>
#include <regex>
#include <sstream>

#include <pybind11/pybind11.h>

#include "../include/Utils/SGF.h"
#include "../include/Utils/SenteExceptions.h"

#define stringify(x) #x

namespace py = pybind11;

namespace sente {
    namespace utils {

        bool isSGFLegal(SGFCommand command, unsigned version){

            // from version list https://www.red-bean.com/sgf/proplist_ff.html
            switch (command){
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
            }

        }

        std::unordered_map<std::string, SGFCommand> strToCommand {
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

        std::unordered_map<SGFCommand, std::string> commandToStr {
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

        SGFCommand fromStr(const std::string& sgfCommand){

            if (strToCommand.find(sgfCommand) != strToCommand.end()){
                return strToCommand[sgfCommand];
            }
            else {
                throw InvalidSGFException("Invalid SGF command: \"" + sgfCommand + "\"");
            }
        }

        std::string toStr(SGFCommand command){
            return commandToStr[command];
        }

        std::string fromStr(SGFCommand command){
            return commandToStr[command];
        }

        bool isSGFParam(const std::string& toCheck){

            // go through the leading characters
            auto iter = toCheck.begin();

            while (*(iter++) != '['){
                if (not isupper(*iter)){
                    return false;
                }
            }

            // go through the bracket characters
            while (*(iter++) != ']'){
                if (*iter == '\\' and *(iter + 1) == ']'){
                    // skip over the character if we get \]
                    iter += 2;
                }
            }

            return iter + 1 == toCheck.end();

        }

        void checkSGFValidity(const std::string& SGFText){

            int parenDepth = 0;
            int bracketDepth = 0;

            unsigned valid_chars = 0;

            bool oneMatch = false;

            auto lastBreak = SGFText.begin();

            for (auto iter = SGFText.begin(); iter != SGFText.end(); iter++){

                // py::print(std::string(lastBreak + 1, iter + 1));
                // py::print("bracket depth was", bracketDepth);

                if (isspace(*iter) and bracketDepth == 0){
                    lastBreak = iter;
                    valid_chars++;
                    continue;
                }
                switch (*iter){
                    case '\\':
                        iter++;
                        continue;
                    case ';':
                        if (bracketDepth == 0){
                            lastBreak = iter;
                            valid_chars++;
                        }
                        break;
                    case '(':
                        if (bracketDepth == 0){
                            parenDepth++;
                            valid_chars++;
                        }
                        break;
                    case ')':
                        if (bracketDepth == 0){
                            parenDepth--;
                            valid_chars++;
                        }
                        break;
                    case '[':
                        bracketDepth++;
                        break;
                    case ']':
                        bracketDepth--;

                        // slice out the last segment
                        std::string temp(lastBreak + 1, iter + 1);

                        if (isSGFParam(temp)){
                            throw InvalidSGFException("Invalid SGF field: " + temp);
                        }

                        oneMatch = true;

                        valid_chars += temp.size();

                        // update the lest segment
                        lastBreak = iter;
                        break;
                }

                // TODO: add highlighting of the syntax errors

                if (not (bracketDepth == 0 or bracketDepth == 1)){
                    throw InvalidSGFException("invalid brackets");
                }
                if (parenDepth < 0){
                    throw InvalidSGFException("invalid parentheses");
                }

            }

            if (not oneMatch){
                throw InvalidSGFException("File was not an SGF file");
            }

            if (valid_chars != SGFText.size()){
                throw InvalidSGFException("File did not perfectly match SGF format");
            }

        }

        GoGame loadSGF(const std::string& SGFText){

            checkSGFValidity(SGFText);

            auto metadata = getMetadata(SGFText);

            if (metadata["GM"] != "1"){
                throw utils::InvalidSGFException("The desired SGF file is not a Go file.");
            }

            unsigned side = std::stoi(metadata["SZ"]);
            Rules rules;
            double komi;

            std::string ruleString = metadata["RU"];

            // convert the rules to lowercase
            std::transform(ruleString.begin(), ruleString.end(), ruleString.begin(), ::tolower);

            if (ruleString == "chinese"){
                rules = CHINESE;
            }
            else if (ruleString == "japanese"){
                rules = JAPANESE;
            }
            else {
                throw utils::InvalidSGFException("ruleset not recognized \"" + ruleString + "\" (sente only supports japanese and chinese rules at present)");
            }

            if (metadata.find("KM") != metadata.end()){
                komi = std::stod(metadata["KM"]);
            }
            else {
                komi = getKomi(rules);
            }

            GoGame game(side, rules, komi);

            std::string currentSegment;
            auto previousIndex = SGFText.begin();
            std::regex moveRegex(";\\s*[WB]\\[[a-t]*\\]");
            std::stack<unsigned> branchDepths{};

            unsigned bracketDepth = 0;

            try {
                // for each letter in the SGF
                for (auto cursor = SGFText.begin(); cursor < SGFText.end(); cursor++){

                    if (*cursor == '['){
                        bracketDepth++;
                    }
                    else if (*cursor == ']'){
                        bracketDepth--;
                    }
                    else if (*cursor == '(' and bracketDepth == 0){

                        // find the moves in the current segment
                        currentSegment = std::string(previousIndex, cursor);
                        auto regexIter = std::sregex_iterator(currentSegment.begin(), currentSegment.end(), moveRegex);

                        // only insert moves if we find one
                        if (regexIter != std::sregex_iterator()){

                            Move temp = Move::fromSGF((regexIter++)->str());

                            // insert the move into the tree and record the step we take
                            branchDepths.push(game.getMoveNumber());
                            game.playStone(temp);

                            // go through all the matches
                            for (auto iter = regexIter; iter != std::sregex_iterator(); iter++){
                                // insert the move but don't advance to the new node
                                temp = Move::fromSGF(iter->str());
                                game.playStone(temp);
                            }
                        }

                      // update the previous index
                      previousIndex = cursor;
                    }
                    else if (*cursor == ')' and bracketDepth == 0){

                        // find the moves in the current segment
                        currentSegment = std::string(previousIndex, cursor);
                        auto regexIter = std::sregex_iterator(currentSegment.begin(), currentSegment.end(), moveRegex);

                        if (regexIter != std::sregex_iterator()){

                            // if there are any moves, insert them
                            Move temp = Move::fromSGF((regexIter++)->str());

                            branchDepths.push(game.getMoveNumber());
                            game.playStone(temp);

                            // go through all the matches
                            for (auto iter = regexIter; iter != std::sregex_iterator(); iter++){
                                // insert the move but don't advance to the new node
                                temp = Move::fromSGF(iter->str());
                                game.playStone(temp);
                            }
                        }

                        // unless the branch depths are empty
                        if (not branchDepths.empty()){

                            // get the sequence of moves
                            auto sequence = game.getMoveSequence();
                            // slice the sequence of moves at the specified point
                            sequence = std::vector<Move>(sequence.begin(), sequence.begin() + branchDepths.top());

                            // advance to the root and play the sequence
                            game.advanceToRoot();
                            game.playMoveSequence(sequence);

                            // pop the depth off of the stack once we get it
                            branchDepths.pop();
                        }

                        // update the previous index
                        previousIndex = cursor;
                    }

                }
            }
            catch (const utils::IllegalMoveException& Exc){
                // rephrase the exception
                throw utils::InvalidSGFException("the SGF contained an illegal move at move " + std::to_string(game.getMoveNumber()) + std::string(Exc.what()));
            }

            if (not branchDepths.empty()){
                throw InvalidSGFException("Number of opening parentheses did not match number of closing parentheses");
            }

            return game;

        }

        void insertIntoSGF(Tree<Move>& moves, std::stringstream& SGF){
            // for each child

            if (moves.getChildren().size() == 1){

                // if there is exactly one child, we don't need to add parentheses
                auto child = *moves.getChildren().begin();

                // serialize the move
                SGF << ";" << std::string(child);

                // step to the child
                moves.stepTo(child);

                // recurse
                insertIntoSGF(moves, SGF);

                moves.stepUp();

            }
            else {
                for (auto& child : moves.getChildren()){
                    SGF << "\n(";
                    if (not child.isResign()){

                        // serialize the move
                        SGF << ";" << std::string(child);
                        // step to the child
                        moves.stepTo(child);
                        if (not moves.isAtLeaf()){
                            insertIntoSGF(moves, SGF);
                        }
                        // step up
                        moves.stepUp();
                    }
                    SGF << ")";
                }
            }

        }

        std::string dumpSGF(const GoGame& game, std::unordered_map<std::string, std::string> &attributes){

            // create the string stream to use
            std::stringstream SGF;

            attributes.erase("FF");
            attributes.erase("GM");
            attributes.erase("CA");

            auto gameAttributes = game.getAttributes();

            if (attributes.find("RU") == attributes.end()){
                attributes["RU"] = gameAttributes["RU"];
            }

            if (attributes.find("SZ") != attributes.end()){
                throw std::domain_error("\"SZ\": board size cannot be changed.");
            }
            else {
                attributes["SZ"] = gameAttributes["SZ"];
            }

            if (attributes.find("RE") == attributes.end()){
                if (game.isOver()){
                    attributes["RE"] = game.getResults();
                }
            }

            auto moves = game.getMoveTree();

            // make sure that all of the attributes are legal
            for (const auto& attribute : attributes){
                if (attribute.first.size() != 2 or not std::all_of(attribute.first.begin(), attribute.first.end(),
                                                                   [](const char c){ return std::isupper(c);})) {
                    throw std::domain_error("Invalid Attribute \"" + attribute.first + "\"");
                }
            }

            // backup the current position of the board and advance to the root node
            auto moveSequence = moves.getSequence();
            moves.advanceToRoot();

            // add the file format as the first parameter
            SGF << "(;FF[4]GM[1]CA[UTF-8]\n"; // TODO: make sure that it is safe to assume the file we create will be UTF-8

            for (const auto& attribute : attributes){
                SGF << attribute.first << "[" << attribute.second << "]\n";
            }

            insertIntoSGF(moves, SGF);

            SGF << ")";

            // return the board to it's original state
            moves.advanceToRoot();
            for (const auto& move : moveSequence){
                moves.stepTo(move);
            }

            return SGF.str();
        }

        /**
         *
         * extracts metadata from a SGF file
         *
         * @param SGFText
         * @return
         */
        std::unordered_map<std::string, std::string> getMetadata(const std::string& SGFText){

            // create the regular expression for the metadata
            std::regex metadataRegex("[A-Z]{2}\\[[^\\]]*\\]");

            // create the return object
            std::unordered_map<std::string, std::string> metadata;

            // go through all of the matches
            for (auto regexIter = std::sregex_iterator(SGFText.begin(), SGFText.end(), metadataRegex);
                 regexIter != std::sregex_iterator(); regexIter++){
                // extract the match string
                std::string match = regexIter->str();

                // slice the components apart
                std::string key = std::string(match.begin(), match.begin() + 2);
                std::string value = std::string(match.begin() + 3, match.end() - 1);

                // return the metadata
                metadata[key] = value;

            }

            // add some default values
            if (metadata.find("RU") == metadata.end()){
                metadata["RU"] = "Chinese";
            }
            if (metadata.find("SZ") == metadata.end()){
                metadata["SZ"] = "19";
            }
            if (metadata.find("GM") == metadata.end()){
                metadata["GM"] = "1";
            }

            // remove non metadata labels
            metadata.erase("SQ"); // square
            metadata.erase("TR"); // triangle
            metadata.erase("CR"); // circle
            metadata.erase("LB"); // label
            metadata.erase("AB"); // add black stone
            metadata.erase("AW"); // add white stone

            return metadata;

        }

    }
}

namespace std {
    size_t std::hash<sente::utils::SGFCommand>::operator()(const sente::utils::SGFCommand &sgfCommand) const noexcept {
        return unsigned(sgfCommand);
    }
}

