//
// Created by arthur wesley on 12/11/21.
//

#include "pybind11/pybind11.h"

#include "../../../Include/Utils/GTP/Tokens/Operator.h"

namespace py = pybind11;

namespace sente::GTP {

    Operator::Operator(const std::string& value) : Token(value) {
        name = OPERATORS[value];
    }

    OperatorName Operator::getName() const {
        return name;
    }

    Operator::~Operator() {}

    tokenType Operator::getTokenType() const {
        return OPERATOR;
    }

}
