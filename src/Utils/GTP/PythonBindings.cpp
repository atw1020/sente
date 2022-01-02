//
// Created by arthur wesley on 12/26/21.
//

#include "PythonBindings.h"

namespace sente::GTP {

    bool isGTPType(const py::type& type){

        bool result = type.is(py::type::of(py::int_())) or
                      type.is(py::type::of(py::cast(sente::Vertex(0, 0)))) or
                      type.is(py::type::of(py::str())) or
                      type.is(py::type::of(py::cast(Stone::BLACK))) or
                      type.is(py::type::of(py::float_())) or
                      type.is(py::type::of(py::cast(sente::Move(0, 0, Stone::BLACK)))) or
                      type.is(py::type::of(py::bool_()));

        return result;
    }

    /**
     *
     * throws an exception if the python function fails to meet the requirements of a GTP function
     *
     * @param function function to check
     * @param inspect python builtin inspect library
     * @param typing python builtin typing library
     */
    void checkGTPCommand(py::function& function, const py::module_& inspect,
                         const py::module_& typing){

        // get some attributes some the function
        // TODO: replace dunder implementation with non-implementation dependent calls
        std::string name = py::str(function.attr("__name__"));
        auto annotations = function.attr("__annotations__");

        // obtain the argument names
        py::list argumentNames = py::list(inspect.attr("getfullargspec")(function).attr("args"));

        // make sure the arguments are all have a valid type listed
        for (const auto& argument : argumentNames) {

            // throw an error if the argument doesn't have a type annotation
            if (not annotations.contains(argument)){
                throw py::value_error("Custom GTP command \"" + name + "\" has no type specified for argument \"" +
                                      std::string(py::str(argument))
                                      + "\" (custom GTP commands must be strongly typed)");
            }

            // throw an error if the argument's type is invalid
            if (not isGTPType(annotations[argument])){
                throw py::type_error("Argument \"" + std::string(py::str(argument)) + "\" for custom GTP command \""
                                     + name + " \"has invalid type \"" + std::string(py::str(annotations[argument]))
                                     + "\".");
            }
        }

        // if there is a return type, make sure that it has a valid type
        if (annotations.contains("return")) {
            // check the return type
            py::object returnType = annotations["return"];

            auto returnTypeOptions = py::list();

            // check if the return type is a union
            if (typing.attr("get_origin")(returnType).is(typing.attr("Union"))){
                // if it is, put all the types into the options
                returnTypeOptions = returnType.attr("__args__");
            }
            else {
                returnTypeOptions.append(returnType);
            }

            // go through all the type options and make sure that they are all satisfactory
            for (unsigned i = 0; i < returnTypeOptions.size(); i++){

                // if the return type is a union or named tuple
                py::object returnType = returnTypeOptions[i];

                // a valid response has two formats
                //  1) a tuple containing whether the message was an error (bool) and the response variable (must be a GTP
                //     defined type)
                //  2) The response variable on its own. The message is assumed to be successful.

                // check to see if we have a tuple response
                if (typing.attr("get_origin")(returnType).is(py::type::of(py::tuple()))){

                    // if we have a strongly typed tuple, make sure it's valid and set the option to be its second element
                    auto types = py::tuple(typing.attr("get_args")(returnType));

                    // make sure that the tuple contains exactly two elements
                    if (types.size() != 2){
                        throw py::value_error("Custom GTP command returns invalid response; "
                                              "expected two items, got " + std::to_string(types.size()));
                    }

                    // make sure that the first argument is a bool
                    if (not py::type(types[0]).is(py::type::of(py::bool_()))){
                        throw py::type_error("Custom GTP command returns invalid response in position 1, "
                                             "expected bool, got " + std::string(py::str(types[0])));
                    }

                    // if it's valid, set the option to be the second element of the tuple
                    returnType = types[1];
                }

                // throw an exception if the option is invalid
                if (not isGTPType(returnType) and not returnType.is(py::type::of(py::none()))){
                    throw py::type_error("Custom GTP command returned invalid response, expected GTP compatible "
                                               "type, got " + std::string(py::str(py::type::of(returnType))));
                }
            }
        }
    }

    bool isUnionInstance(const py::object& response, const py::object& expectedType, const py::module_& typing) {

        auto returnTypeOptions = py::list();

        // check if the return type is a union
        if (typing.attr("get_origin")(expectedType).is(typing.attr("Union"))){
            // if it is, put all the types into the options
            returnTypeOptions = expectedType.attr("__args__");
        }
        else {
            returnTypeOptions.append(expectedType);
        }

        // go through all the return type options
        for (unsigned i = 0; i < returnTypeOptions.size(); i++) {

            // get this option of the response
            py::object returnType = returnTypeOptions[i];

            // check if we have a tuple
            if (typing.attr("get_origin")(expectedType).is(py::type::of(py::tuple()))){

                // skip this comparison if the response is not a tuple as well
                if (not py::isinstance<py::tuple>(response)){
                    continue;
                }

                // generate a tuple containing the expected types
                py::tuple types = expectedType.attr("__args__");

                // skip this if the tuples are not the same size
                if (py::len(response) != py::len(types)){
                    continue;
                }

                unsigned j;

                // go through all the types
                for (j = 0; j < types.size(); j++){
                    if (not py::isinstance(py::tuple(response)[j], types[j])){
                        break;
                    }
                }

                if (j == types.size()){
                    return true;
                }

            }
            else {
                // if the response type matches, return true
                if (py::isinstance(response, returnType)){
                    return true;
                }
            }

        }

        // if we never found a match, return false
        return false;
    }

}