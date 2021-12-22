//
// Created by arthur wesley on 12/11/21.
//

#include "Literal.h"

#include <string>
#include <utility>

namespace sente::GTP {

    std::string toString(LiteralType type){
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
            case MOVE:
                return "move";
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

    LiteralType Integer::getLiteralType() const {
        return INTEGER;
    }

    Integer::~Integer() = default;

    unsigned int Vertex::getX() const {
        return x;
    }

    unsigned Vertex::getY() const {
        return y;
    }

    Vertex::Vertex(const std::string& vertex) : Literal(vertex) {

        if (vertex[0] < 'I'){
            x = vertex[0] - 'A';
        }
        else {
            x = vertex[0] - 'B';
        }

        auto temp = std::string(vertex.begin() + 1, vertex.end());

        y = std::stoi(temp) - 1;

    }

    LiteralType Vertex::getLiteralType() const {
        return VERTEX;
    }

    Vertex::~Vertex() = default;

    String::String(const std::string &value) : Literal(value){}

    LiteralType String::getLiteralType() const {
        return STRING;
    }

    String::~String() = default;

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

    LiteralType Color::getLiteralType() const {
        return COLOR;
    }

    Color::~Color() = default;

    Stone Color::getColor() const {
        return color == BLACK ? Stone::BLACK : Stone::WHITE;
    }

    bool Color::isColor(std::string text) {
        std::transform(text.begin(), text.end(), text.begin(), ::tolower);
        return text == "b" or text == "black" or text == "w" or text == "white";
    }

    Float::Float(const std::string &text) : Literal(text){
        value = std::stof(text);
    }

    float Float::getValue() const {
        return value;
    }

    LiteralType Float::getLiteralType() const {
        return FLOAT;
    }

    Float::~Float() = default;

    Move::Move(Color color, Vertex vertex, unsigned boardSize) : Literal(color.getText() + " " + vertex.getText()){
        move = sente::Move(boardSize - 1 - vertex.getY(), vertex.getX(), color.getColor());
    }

    sente::Move Move::getMove() {
        return move;
    }

    LiteralType Move::getLiteralType() const {
        return MOVE;
    }

    Boolean::Boolean(std::string text) : Literal(text) {
        std::transform(text.begin(), text.end(), text.begin(), ::tolower);
        value = text.find("true") != std::string::npos;
    }

    bool Boolean::getValue() const {
        return value;
    }

    LiteralType Boolean::getLiteralType() const {
        return BOOLEAN;
    }
}
