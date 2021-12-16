//
// Created by arthur wesley on 12/11/21.
//

#include "../../../Include/Utils/GTP/Tokens/Literal.h"

#include <string>
#include <utility>

namespace sente::GTP {

    std::string toString(literalType type){
        switch (type){
            case INTEGER:
                return "integer";
            case VERTEX:
                return "vertex";
            case STRING:
                return "string";
            case COLOR:
                return "color";
            case FLOAT:
                return "float";
            case BOOLEAN:
                return "bool";
        }
    }

    Literal::Literal(const std::string &text) : Token(text) {}

    tokenType Literal::getTokenType() const {
        return LITERAL;
    }

    Integer::Integer(const std::string &text) : Literal(text) {
        value = std::stoi(text);
    }

    unsigned int Integer::getValue() const {
        return value;
    }

    literalType Integer::getLiteralType() const {
        return INTEGER;
    }

    unsigned int Vertex::getX() const {
        return x;
    }

    unsigned Vertex::getY() const {
        return y;
    }

    Vertex::Vertex(const std::string& vertex) : Literal(vertex) {

        if (vertex[0] > 'I'){
            x = vertex[0] - 'A';
        }
        else {
            x = vertex[0] - 'B';
        }

        auto temp = std::string(vertex.begin() + 1, vertex.end());

        y = std::stoi(temp);

    }

    literalType Vertex::getLiteralType() const {
        return VERTEX;
    }

    literalType String::getLiteralType() const {
        return STRING;
    }


    Color::Color(const std::string &text) : Literal(text){

    }

    literalType Color::getLiteralType() const {
        return COLOR;
    }
}
