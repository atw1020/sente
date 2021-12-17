//
// Created by arthur wesley on 12/11/21.
//

#include "../../../Include/Utils/GTP/Tokens/Operator.h"

namespace sente::GTP {

    Operator::Operator(std::string value) : Token(std::move(value)) {
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
