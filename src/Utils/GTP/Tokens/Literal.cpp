//
// Created by arthur wesley on 12/11/21.
//

#include "../../../Include/Utils/GTP/Tokens/Literal.h"

namespace sente::GTP {

    unsigned int Vertex::getX() const {
        return x;
    }

    unsigned Vertex::getY() const {
        return y;
    }

    Vertex::Vertex(const std::string& vertex) {

        if (vertex[0] > 'I'){
            x = vertex[0] - 'A';
        }
        else {
            x = vertex[0] - 'B';
        }

        auto temp = std::string(vertex.begin() + 1, vertex.end());

        y = std::stoi(temp);

    }

}
