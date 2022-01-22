//
// Created by arthur wesley on 8/7/21.
//

#ifndef SENTE_NUMPY_H
#define SENTE_NUMPY_H

#include <vector>

#include <pybind11/numpy.h>

#include "../Game/GoGame.h"

namespace sente::utils {

    py::array_t<uint8_t> getFeatures(const GoGame& game, const std::vector<std::string>& features);

}

#endif //SENTE_NUMPY_H
