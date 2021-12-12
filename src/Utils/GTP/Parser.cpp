//
// Created by arthur wesley on 12/11/21.
//

#include <regex>
#include <sstream>

#include "../../Include/Utils/GTP/Parser.h"

#include "../../Include/Utils/GTP/Tokens/Literal.h"
#include "../../Include/Utils/GTP/Tokens/Keyword.h"
#include "../../Include/Utils/GTP/Tokens/Operator.h"

namespace sente::GTP {

    std::string preprocess(const std::string& text){

        std::stringstream output;

        bool inComment = false;
        bool inNewLine = true;

        for (char ch : text){

            if (inNewLine && ch == '\n'){
                continue;
            }
            else {
                inNewLine = false;
            }

            switch(ch){
                case '#':
                    inComment = true;
                    break;
                case '\n':
                    inComment = false;
                case '\t':
                    if (not inComment){
                        output << "    ";
                    }
                    break;
                default:
                    if (not inComment){
                        output << ch;
                    }
                case '\r':
                    break;
            }
        }

        return output.str();

    }

    std::vector<std::unique_ptr<Token>> parse(const std::string& text){

        // begin by splitting the text on spaces
        std::vector<std::unique_ptr<Token>> tokens;

        unsigned position = 0;

        while (text.find(' ', position) != std::string::npos){

            unsigned end_index = text.find(' ', position);
            auto token = std::string(text.begin() + position, text.begin() + end_index - 1);
            tokens.push_back(parseToken(token));

            position = end_index;

        }

        return tokens;

    }

    std::unique_ptr<Token> parseToken(const std::string& token){

        // regex for a vertex
        std::regex vertex("[A-H,J-Z]\\d{1,2}");

        if (operators.find(token) != operators.end()){
            return std::make_unique<Operator>(token);
        }

        if (keywords.find(token) != keywords.end()){
            return std::make_unique<Keyword>(token);
        }

        if (std::regex_match(token, vertex)){
            return std::make_unique<Vertex>(token);
        }

    }

}
