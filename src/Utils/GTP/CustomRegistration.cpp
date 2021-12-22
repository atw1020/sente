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

        // obtain the argument names
        py::list argumentNames = py::list(argSpec.attr("args"));
        std::vector<ArgumentPattern> argumentPattern;

        // make sure that the first argument is the self argument
        if (std::string(py::str(argumentNames[0])) != "self"){
            throw pybind11::value_error("Custom GTP command \"" + name +"\" is not a method (i.e. it dose not belong "
                                        "to a class). custom GTP commands must be methods");
        }

        // slice off the first argument now that it has been checked
        argumentNames = argumentNames[py::slice(1, argumentNames.size(), 1)];

        // generate the argument pattern
        for (const auto& argument : argumentNames){
            // check to see if we have a type for this argument
            if (annotations.attr("__contains__")(argument)){
                // if we do, add it to the argument pattern
                argumentPattern.emplace_back(py::str(argument), argumentTypeMappings[py::str(annotations[argument])]);
            }
            else {
                throw pybind11::value_error("Custom GTP command \"" + name +"\" has no type specified for argument \"" +
                                            std::string(py::str(argument)) + "\" (custom GTP commands must be "
                                                                             "strongly typed)");
            }
        }

        // define the custom command using a lambda

        CommandMethod customMethod = [&](Engine* self, const std::vector<std::shared_ptr<Token>>& arguments) -> Response{

            py::object pySelf = py::cast(self);

            auto pyArgs = py::list();
            pyArgs.append(py::cast(self));

            Integer* integer;
            Vertex* vertex;
            Color* color;
            Float* float_;
            Move* move;
            Boolean* bool_;

            for (const auto& argument : arguments){

                auto* literal = (Literal*) argument.get();

                switch (literal->getLiteralType()){
                    case INTEGER:
                        integer = (Integer*) literal;
                        pyArgs.append(py::int_(integer->getValue()));
                        break;
                    case VERTEX:
                        vertex = (Vertex*) literal;
                        pyArgs.append(py::make_tuple(vertex->getX(), vertex->getY()));
                        break;
                    case STRING:
                        pyArgs.append(py::str(literal->getText()));
                        break;
                    case COLOR:
                        color = (Color*) literal;
                        pyArgs.append(py::cast(color->getColor()));
                        break;
                    case FLOAT:
                        float_ = (Float*) literal;
                        pyArgs.append(py::cast(float_->getValue()));
                        break;
                    case MOVE:
                        move = (Move*) literal;
                        pyArgs.append(py::cast(move->getMove()));
                        break;
                    case BOOLEAN:
                        bool_ = (Boolean*) literal;
                        pyArgs.append(py::cast(bool_->getValue()));
                        break;
                }
            }

            py::tuple args;

            py::object _response = function(*args);

            if (not py::type(_response).is(py::type(py::tuple()))){
                throw pybind11::type_error("Custom GTP command returned invalid response; "
                                           "expected tuple, got" + std::string(py::str(py::type(_response))));
            }

            auto response = py::tuple(_response);

            if (response.size() != 2){
                throw pybind11::value_error("Custom GTP command returned invalid response; "
                                            "expected two items, got " + std::to_string(response.size()));
            }

            // check that the responses are the right type
            if (not py::type(response[0]).is(py::type(py::bool_()))){
                throw pybind11::type_error("Custom GTP command returned invalid response in position 1, "
                                           "expected bool, got" + std::string(py::str(py::type(response[0]))));
            }
            if (not py::type(response[1]).is(py::type(py::str()))){
                throw pybind11::type_error("Custom GTP command returned invalid response in position 2, "
                                           "expected string, got" + std::string(py::str(py::type(response[0]))));
            }

            return {py::bool_(response[0]), py::str(response[1])};
        };

        // register the command with the engine
        engine.registerCommand(engine.engineName + "-" + name, customMethod, argumentPattern);

    }

}