//
// Created by arthur wesley on 12/11/21.
//

#include "Literal.h"

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
        value = std::stoul(text);
    }

    unsigned int Integer::getValue() const {
        return value;
    }

    literalType Integer::getLiteralType() const {
        return INTEGER;
    }

    Integer::~Integer() {}

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

        y = std::stoi(temp) - 1;

    }

    literalType Vertex::getLiteralType() const {
        return VERTEX;
    }

    Vertex::~Vertex() {}

    String::String(const std::string &value) : Literal(value){}

    literalType String::getLiteralType() const {
        return STRING;
    }

    String::~String() {}

    Color::Color(std::string text) : Literal(text){
        // convert the text to lowercase
        std::transform(text.begin(), text.end(), text.begin(), ::tolower);

        if (text == "black" or text == "b"){
            color = BLACK;
        }
        else if (text == "white" or text == "w"){
            color = WHITE;
        }

    }

    literalType Color::getLiteralType() const {
        return COLOR;
    }

    Color::~Color() {}

    GoColor Color::getColor() const {
        return color;
    }

    Float::Float(const std::string &text) : Literal(text){
        value = std::stof(text);
    }

    float Float::getValue() const {
        return value;
    }

    literalType Float::getLiteralType() const {
        return FLOAT;
    }

    Float::~Float() {}

    Move::Move(Color color, Vertex vertex) : Literal(color.getText() + " " + vertex.getText()){
        move = sente::Move(vertex.getX(), vertex.getY(), color.getColor() == BLACK ? Stone::BLACK : Stone::WHITE);
    }

    sente::Move Move::getMove() {
        return move;
    }

    literalType Move::getLiteralType() const {
        return MOVE;
    }
}
