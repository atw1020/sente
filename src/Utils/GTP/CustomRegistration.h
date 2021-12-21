//
// Created by arthur wesley on 12/21/21.
//

#ifndef SENTE_CUSTOMREGISTRATION_H
#define SENTE_CUSTOMREGISTRATION_H

#include <pybind11/pybind11.h>

#include "Engine.h"

namespace sente::GTP {
    void registerCommand(Engine& engine, const py::function& function, const py::module_& inspect);
}

#endif //SENTE_CUSTOMREGISTRATION_H
