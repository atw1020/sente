//
// Created by arthur wesley on 7/2/21.
//

#include <regex>

#include <pybind11/pybind11.h>

#include "include/SGF.h"

namespace py = pybind11;

namespace sente_utils {

    Tree<sente::Move> getSGFMoves(const std::string& SGFText){

        Tree<sente::Move> moves;

        for (const auto& character : SGFText){

            if (character == '('){

            }

        }

        return moves;

    }

    sente::Rules getSGFRules(const std::string& SGFText){

        // py::print("the full SGF text was", SGFText);

        using namespace std::regex_constants;

        // find the rules and board size to initialize the game
        std::smatch result;
        std::regex rulesRegex("RU\\[((Chinese|Japanese))\\]", icase);

        // extract the rules from the string
        std::regex_search(SGFText, result, rulesRegex);

        std::string temp = result[0].str();
        std::string rulesString(temp.begin() + 3, temp.end() - 1);// create the board from the results

        // convert the rules to lowercase
        std::transform(rulesString.begin(), rulesString.end(), rulesString.begin(), ::tolower);

        if (rulesString == "japanese"){
            return sente::JAPANESE;
        }
        else if (rulesString == "chinese"){
            return sente::CHINESE;
        }
        else {
            throw std::domain_error("Invalid Rules " + rulesString);
        }

    }
    unsigned getSGFBoardSize(const std::string& SGFText){

        std::smatch result;
        std::regex boardSizeRegex("SZ\\[\\d{1,2}\\]");

        // extract the board size from the string
        std::regex_search(SGFText, result, boardSizeRegex);

        std::string temp = result[0].str();
        return std::stol(std::string(temp.begin() + 3, temp.end() - 1));

    }

}
