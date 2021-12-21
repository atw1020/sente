//
// Created by arthur wesley on 12/21/21.
//

#include "CustomRegistration.h"

namespace sente::GTP {

    std::unordered_map<std::string, LiteralType> argumentTypeMappings = {
            {"int", INTEGER},
            {"tuple", VERTEX}, // TODO: use something better than a tuple for this in python
            {"str", STRING},
            {"stone", COLOR},
            {"float", FLOAT},
            {"Move", MOVE},
            {"bool", BOOLEAN}
    };

    void registerCommand(Engine& engine, const py::function& function, const py::module_& inspect){

        // get the arguments and name from inspecting the function
        auto argSpec = inspect.attr("getfullargspec")(function);
        std::string name = py::str(function.attr("__name__"));
        auto annotations = argSpec.attr("annotations");

        // get the non-self arguments from the spec
        py::list argumentNames = py::list(argSpec.attr("args"));
        std::vector<ArgumentPattern> argumentPattern;

        // get divide the arguments into the self arguments and other arguments
        std::string selfArg = py::str(argumentNames[0]);
        argumentNames = argumentNames[py::slice(1, argumentNames.size(), 1)];

        // generate the argument pattern
        for (const auto& argument : argumentNames){
            argumentPattern.emplace_back(py::str(argument), argumentTypeMappings[py::str(annotations[argument])]);
        }

        // define the custom command using a lambda

        CommandMethod customMethod = [function](Engine* self, const std::vector<std::shared_ptr<Token>>& arguments){
            return function.attr("call");
        };

        // TODO: check to see that the response of the function is of the correct type

        // register the command with the engine
        engine.registerCommand(engine.engineName + "-" + name, customMethod, argumentPattern);

    }

}