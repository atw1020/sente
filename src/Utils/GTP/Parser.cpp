//
// Created by arthur wesley on 12/11/21.
//

#include <regex>
#include <sstream>
#include <iostream>

#include "Parser.h"

#include "Tokens/Literal.h"
#include "Tokens/Seperator.h"

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
                case '\t':
                    if (not inComment){
                        output << "    ";
                    }
                    break;
                case '\n':
                    inComment = false;
                    inNewLine = true;
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

    std::vector<std::shared_ptr<Token>> parse(const std::string& text){

        // begin by splitting the text on spaces
        std::vector<std::shared_ptr<Token>> tokens;

        size_t start_index = 0;
        size_t end_index;

        while ((end_index = std::min({text.find(' ', start_index),
                                      text.find('\n', start_index),
                                      text.find('"', start_index)})) !=
               std::string::npos) {

            // check if we just hit a string
            if (text[start_index - 1] == '"'){

                // if we are in a string, the end index is determined only by the position of the closing quote
                end_index = text.find('"', end_index + 1);

                // push the string
                tokens.push_back(std::make_shared<String>(std::string(text.begin() + start_index,
                                                                      text.begin() + end_index)));
            }
            else {

                // if we are not in a string, obtain the next token
                auto token = std::string(text.begin() + start_index, text.begin() + end_index);

                // if the token is not an empty string, push it
                if (not token.empty()){
                    tokens.push_back(parseToken(token));
                }
            }

            // if we hit a newline, add a newline token
            if (text.find('\n', start_index) < text.find(' ', start_index) and
                text.find('\n', start_index) < text.find('"', start_index)){
                tokens.push_back(std::make_shared<Seperator>("\n"));
            }

            // update the start index
            start_index = end_index + 1;

        }

        // parse the final token
        auto token = std::string(text.begin() + start_index, text.end());
        if (not token.empty()){
            tokens.push_back(parseToken(std::string(text.begin() + start_index, text.end())));
        }

        // add a newline if the last element we parse is not a seperator
        if (not tokens.empty()){
            if ((*tokens.rbegin())->getTokenType() != SEPERATOR){
                tokens.push_back(std::make_shared<Seperator>("\n"));
            }
        }

        return tokens;

    }

    std::shared_ptr<Token> parseToken(const std::string& token){

        // regex for a vertex
        std::regex vertex("[A-H,J-Z]\\d{1,2}");
        // regex for floating point number
        std::regex floatRegex("[-+]?([0-9]*\\.[0-9]+|[0-9]+)");

        std::string lowercase = token;
        std::transform(lowercase.begin(), lowercase.end(), lowercase.begin(), ::tolower);

        if (std::regex_match(token, vertex)){
            return std::make_shared<Vertex>(token);
        }

        if (std::all_of(token.begin(), token.end(), ::isdigit)){
            return std::make_shared<Integer>(token);
        }

        if (Color::isColor(token)){
            return std::make_shared<Color>(token);
        }

        if (std::regex_match(token, floatRegex)){
            return std::make_shared<Float>(token);
        }

        if (lowercase == "true"){
            return std::make_shared<Boolean>(lowercase);
        }

        if (lowercase == "false"){
            return std::make_shared<Boolean>(lowercase);
        }

        return std::make_shared<String>(token);

    }

}
