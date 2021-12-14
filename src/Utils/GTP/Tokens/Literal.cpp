//
// Created by arthur wesley on 12/11/21.
//

#include <string>
#include <utility>

#include "../../../Include/Utils/GTP/Tokens/Literal.h"

namespace sente::GTP {

    Literal::Literal(std::string text) : Token(std::move(text)){}

    Integer::Integer(const std::string &text) : Literal(text) {
        value = std::stoi(text);
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

    String::String(const std::string& text) : Literal(text) {
        this->text = text;
    }
}
