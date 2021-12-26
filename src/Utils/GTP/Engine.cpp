//
// Created by arthur wesley on 12/12/21.
//

#include "Engine.h"

#include <set>
#include <utility>
#include <vector>
#include <iostream>

#include "Operators.h"

namespace sente::GTP {

    std::unordered_map<std::string, std::vector<std::pair<CommandMethod, std::vector<ArgumentPattern>>>> builtins = {
            {"protocol_version", {{&protocolVersion, {{"operation", STRING}}}}},
            {"name", {{&name, {{"operation", STRING}}}}},
            {"version", {{&version, {{"operation", STRING}}}}},
            {"known_command", {{&knownCommand, {{"operation", STRING}, {"command", STRING}}}}},
            {"list_commands", {{&listCommands, {{"operation", STRING}}}}},
            {"quit", {{&quit, {{"operation", STRING}}}}},
            {"exit", {{&quit, {{"operation", STRING}}}}},
            {"boardsize", {{&boardSize, {{"operation", STRING}, {"size", INTEGER}}}}},
            {"clear_board", {{&clearBoard, {{"operation", STRING}}}}},
            {"komi", {{&komi, {{"operation", STRING}, {"komi", FLOAT}}}}},
            {"play", {{&play, {{"operation", STRING}, {"move", MOVE}}}}},
            {"undo", {{&undoOnce, {{"operation", STRING}}},
                      {&undoMultiple, {{"operation", STRING}, {"moves", INTEGER}}}}},
            {"showboard", {{&showBoard, {{"operation", STRING}}}}},
            {"loadsgf", {{&loadSGF1, {{"operation", STRING}, {"file", STRING}}},
                          {&loadSGF2, {{"operation", STRING}, {"file", STRING}, {"moves", INTEGER}}}}}
    };

    std::unordered_map<std::string, LiteralType> argumentTypeMappings = {
            {"int", INTEGER},
            {"Vertex", VERTEX},
            {"str", STRING},
            {"stone", COLOR},
            {"float", FLOAT},
            {"Move", MOVE},
            {"bool", BOOLEAN}
    };

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

    std::string gtpTypeToString(py::object object){

        if (py::type::of(object).is(py::type::of(py::int_())) or
            py::isinstance<py::str>(object) or
            py::isinstance<py::float_>(object)){

            // for any of these types, string casting is sufficient
            return py::str(object);
        }
        if (py::isinstance<py::bool_>(object)){
            return py::bool_(object) ? "true" : "false";
        }
        if (py::isinstance<sente::Vertex>(object)){
            // cast to a point
            sente::Vertex* vertex = object.cast<sente::Vertex*>();

            char first;

            // determine the letter
            if (vertex->first + 'A' < 'I'){
                first = 'A' + vertex->first;
            }
            else {
                first = 'B' + vertex->first;
            }

            // add the letter to the second co-ord
            std::string message = std::to_string(vertex->second + 1);
            message.insert(message.begin(), first);

            return message;
        }
        if (py::isinstance<Stone>(object)){
            // cast to a stone
            auto* stone = object.cast<Stone*>();

            // generate the one character code
            return *stone == Stone::BLACK ? "B" : "W";
        }
        if (py::isinstance<sente::Move>(object)){

            // cast to a move
            auto* move = object.cast<sente::Move*>();

            char first;

            // determine the letter
            if (move->getX() + 'A' < 'I'){
                first = 'A' + move->getX();
            }
            else {
                first = 'B' + move->getX();
            }

            // add the letter to the second co-ord
            std::string pointMessage = std::to_string(move->getY() + 1);
            pointMessage.insert(pointMessage.begin(), first);

            // convert the stone to a string
            std::string stoneMessage = move->getStone() == Stone::BLACK ? "B" : "W";

            return stoneMessage + " " + pointMessage;

        }
        else {
            throw std::runtime_error("called gtpTypeString on non GTP type");
        }

    }

    Engine::Engine(const std::string& engineName, const std::string& engineVersion)
        : masterGame(19, CHINESE, determineKomi(CHINESE)){
        setEngineName(engineName);
        setEngineVersion(engineVersion);

        // initialize the builtin commands
        // TODO: make sure this doesn't make commands global
        commands = builtins;

        // register the genMove command so that it can be overwritten
        registerCommand("genmove", &genMove, {{"operation", STRING}, {"color", COLOR}});

        // reset the board
        setGTPDisplayFlags();

        // cast the object to a python object to add the names
        py::object self = py::cast(this);
    }

    auto Engine::globalCommands = std::unordered_map<std::string, std::vector<py::function>>();

    std::string Engine::interpret(std::string text) {

        text = preprocess(text);
        auto tokens = parse(text);

        std::stringstream outputStream;

        unsigned start = 0;

        // iterate through the tokens
        for (unsigned index = 0; index < tokens.size(); index++){

            Response response;

            // keep incrementing until we find a seperator
            while (tokens[index]->getTokenType() != SEPERATOR and index < tokens.size()) {
                index++;
            }

            // slice the tokens and put them into a list
            auto preArguments = std::vector<std::shared_ptr<Token>>(tokens.begin() + start, tokens.begin() + index);

            // update the starting index now that we've sliced the tokens
            start = index + 1;

            // replace instances of a stone followed by a point with a move
            auto arguments = std::vector<std::shared_ptr<Token>>();

            for (unsigned i = 0; i < preArguments.size(); i++){

                bool argsCombined = false;

                if (preArguments[i]->getTokenType() == LITERAL and i < preArguments.size() - 1){
                    if (preArguments[i + 1]->getTokenType() == LITERAL){

                        // cast the arguments
                        auto* check1 = (Literal*) preArguments[i].get();
                        auto* check2 = (Literal*) preArguments[i + 1].get();

                        // if we have the arguments that make up a move
                        if (check1->getLiteralType() == COLOR and check2->getLiteralType() == VERTEX){

                            // skip the next iteration and note that we've combined the arguments
                            argsCombined = true;
                            i++;

                            Color* color = (Color*) check1;
                            Vertex* vertex = (Vertex*) check2;

                            arguments.emplace_back(std::make_shared<Move>(*color, *vertex));
                        }
                    }
                }

                if (not argsCombined){
                    arguments.push_back(preArguments[i]);
                }

            }

            // begin interpreting by checking to see if the first element is an integer literal
            std::shared_ptr<Token> candidate;

            bool precedingID = false;
            unsigned id;

            if (arguments[0]->getTokenType() == LITERAL){
                auto* literal = (Literal*) arguments[0].get();
                precedingID = literal->getLiteralType() == INTEGER;
            }

            if (precedingID){
                id = ((Integer*) arguments[0].get())->getValue();
                candidate = arguments[1];
                arguments = std::vector<std::shared_ptr<Token>>(arguments.begin() + 1, arguments.end());
            }
            else {
                candidate = arguments[0];
            }

            if (candidate->getTokenType() == LITERAL){
                // make sure we have a string literal
                auto* literal = (Literal*) candidate.get();

                if (literal->getLiteralType() == STRING){

                    // check to see if a command exists
                    if (commands.find(literal->getText()) != commands.end()){
                        // check the arguments for the command
                        response = execute(literal->getText(), arguments);
                    }
                    else {
                        response = {false, "unknown command"};
                    }

                }

            }

            if (response.first){
                // if we successfully execute the command
                if (precedingID){
                    // if there is a preceding ID, include it in the answer
                    outputStream << statusMessage(response.second, id);
                }
                else {
                    outputStream << statusMessage(response.second);
                }
            }
            else {
                if (precedingID){
                    outputStream << errorMessage(response.second, id);
                }
                else {
                    outputStream << errorMessage(response.second);
                }
            }

        }

        return outputStream.str();
    }

    void Engine::registerCommand(const std::string& commandName, CommandMethod method,
                                 std::vector<ArgumentPattern> argumentPattern){

        // raise an exception if the command is non-modifiable
        if (builtins.find(commandName) != builtins.end()){
            throw std::domain_error("Cannot overwrite standard GTP command \"" + commandName + "\"");
        }

        if (commands.find(commandName) == commands.end()){
            // create a new vector
            commands[commandName] = {{method, argumentPattern}};
        }
        else {

            bool found = false;

            for (auto& command : commands[commandName]){

                // only check if the arguments have the same size
                if (command.second.size() == argumentPattern.size()){

                    // check to see if all the arguments match
                    bool argumentsMatch = true;

                    for (unsigned i = 0; i < command.second.size(); i++){
                        if (command.second[i].second != argumentPattern[i].second){
                            argumentsMatch = false;
                            break;
                        }
                    }

                    if (argumentsMatch){
                        // set the method
                        command.first = method;

                        // exit the function
                        found = true;
                        break;
                    }
                }
            }

            if (not found){
                // append to existing options
                commands[commandName].push_back({method, argumentPattern});
            }
        }
    }

    // TODO: re-implement registration with decorators instead of this
    void Engine::pyRegisterCommand(const py::function& function, const py::module_& inspect) {

        // obtain a reference to the function
        // function.inc_ref();

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

        // add the command to the argument pattern
        argumentPattern.emplace_back("command", STRING);

        // generate the argument pattern
        for (const auto& argument : argumentNames){
            // check to see if we have a type for this argument
            if (py::bool_(annotations.attr("__contains__")(argument))){

                std::string type = py::str(annotations[argument].attr("__name__"));

                if (argumentTypeMappings.find(type) == argumentTypeMappings.end()){
                    throw py::type_error("Argument \"" + std::string(py::str(argument)) + "\" for custom GTP command \""
                                        + name + " \"has invalid type \"" + type + "\".");
                }

                // if we do, add it to the argument pattern
                argumentPattern.emplace_back(py::str(argument), argumentTypeMappings[type]);
            }
            else {
                throw pybind11::value_error("Custom GTP command \"" + name + "\" has no type specified for argument \"" +
                                            std::string(py::str(argument)) + "\" (custom GTP commands must be "
                                                                             "strongly typed)");
            }
        }

        // check to see if the argument pattern contains a color followed by a vertex
        for (unsigned i = 0; i < argumentPattern.size() - 1; i++){
            if (argumentPattern[i].second == COLOR and argumentPattern[i + 1].second == VERTEX){
                throw pybind11::value_error("Custom GTP command \"" + name + "\" contains a Color followed by a Vertex,"
                                            " use a sente.Move instead.");
            }
        }

        // define the custom command using a lambda

        CommandMethod wrapper = [function](Engine* self, const std::vector<std::shared_ptr<Token>>& arguments)
                -> Response{

            // the self argument is automatically passed by python
            (void) self;

            auto pyArgs = py::list();

            // remove the first argument
            auto strippedArguments = std::vector<std::shared_ptr<Token>>(arguments.begin() + 1, arguments.end());

            Integer* integer;
            Vertex* vertex;
            Color* color;
            Float* float_;
            Move* move;
            Boolean* bool_;

            for (const auto& argument : strippedArguments){

                auto* literal = (Literal*) argument.get();

                switch (literal->getLiteralType()){
                    case INTEGER:
                        integer = (Integer*) literal;
                        pyArgs.append(py::int_(integer->getValue()));
                        break;
                    case VERTEX:
                        vertex = (Vertex*) literal;
                        pyArgs.append(py::cast(sente::Vertex(vertex->getX(), vertex->getY())));
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

            // pack the arguments and call the function
            auto args = py::tuple(pyArgs);
            py::object _response = function(*args);

            // a valid response has two formats
            //  1) a tuple containing whether the message was an error (bool) and the response variable (must be a GTP
            //     defined type)
            //  2) The response variable on its own. The message is assumed to be successful.

            bool status;
            py::object response;

            if (py::isinstance<py::tuple>(_response)){

                // if we have a tuple we have to validate it
                py::tuple responseTuple = py::tuple(_response);

                // make sure that the tuple is of size two
                if (responseTuple.size() != 2){
                    throw py::value_error("Custom GTP command returned invalid response; "
                                                "expected two items, got " + std::to_string(responseTuple.size()));
                }

                // make sure the first item is a bool
                if (not py::isinstance<py::bool_>(responseTuple[0])){
                    throw py::type_error("Custom GTP command returned invalid response in position 1, "
                                         "expected bool, got" + std::string(py::str(py::type::of(responseTuple[0]))));
                }

                status = py::bool_(responseTuple[0]);
                response = responseTuple[1];

            }
            else {
                // otherwise, the message is successful and the response is just what was returned
                status = true;
                response = _response;
            }

            // check if we have a valid type
            if (not isGTPType(response)) {
                // we have an invalid type
                if (py::isinstance<py::tuple>(_response)){
                    throw pybind11::type_error("Custom GTP command returned invalid response in position 2, "
                                               "expected GTP compatible type, got " +
                                               std::string(py::str(py::type::of(response))));
                }
                else {
                    throw pybind11::type_error("Custom GTP command returned invalid response, expected GTP compatible "
                                               "type, got " + std::string(py::str(py::type::of(response))));
                }
            }

            return {status, gtpTypeToString(response)};
        };

        if (name != "genmove"){
            name = engineName + "-" + name;
        }

        // register the command with the engine
        registerCommand(name, wrapper, argumentPattern);
    }

    void Engine::registerCommands(const std::string &qualName) {
        // register all the commands associated with this qualName
        for (const auto& )
    }

    std::string Engine::getEngineName() const {
        return engineName;
    }

    void Engine::setEngineName(std::string name){
        if (name.find(' ') != std::string::npos){
            throw py::value_error("engine name \"" + name + "\"contains invalid character ' ' in position " +
                                  std::to_string(name.find(' ')) + ".");
        }
        if (name.find('-') != std::string::npos){
            throw py::value_error("engine name \"" + name + "\"contains invalid character '-' in position " +
                                  std::to_string(name.find('-')) + ".");
        }
        engineName = name;
    }

    std::string Engine::getEngineVersion() const {
        return engineVersion;
    }

    void Engine::setEngineVersion(std::string version){
        engineVersion = std::move(version);
    }

    std::unordered_map<std::string, std::vector<std::pair<CommandMethod,
            std::vector<ArgumentPattern>>>> Engine::getCommands() const {
        return commands;
    }

    bool Engine::isActive() const {
        return active;
    }

    void Engine::setActive(bool active) {
        this->active = active;
    }

    void Engine::setGTPDisplayFlags() {
        // flip the co-ordinate label for the board
        masterGame.setASCIIMode(true);
        masterGame.setLowerLeftCornerCoOrdinates(true);
    }

    const py::function& Engine::registerCommand(const py::function& function, const py::module_& inspect,
                                                const py::module_& typing) {

        // get some attributes some the function
        // TODO: replace dunder implementation with non-implementation dependent calls
        std::string name = py::str(function.attr("__name__"));
        std::string qualName = py::str(function.attr("__qualname__"));
        auto annotations = function.attr("__annotations__");

        // obtain the argument names
        py::list argumentNames = py::list(inspect.attr("getfullargspec")(function).attr("args"));

        // make sure that we are working with a method and not a function
        if (qualName.find('.') == std::string::npos or std::string(py::str(argumentNames[0])) != "self"){
            throw py::value_error("Custom GTP command \"" + name + "\" is not a method (i.e. it dose not belong "
                                                                   "to a class). custom GTP commands must be methods");
        }

        // make sure the arguments are all have a valid type listed
        for (const auto& argument : argumentNames) {

            // skip this strict type checking for the self argument
            if (std::string(py::str(argument)) == "self"){
                continue;
            }

            // throw an error if the argument doesn't have a type annotation
            if (not annotations.contains(argument)){
                throw py::value_error("Custom GTP command \"" + name + "\" has no type specified for argument \"" +
                                      std::string(py::str(argument))
                                      + "\" (custom GTP commands must be strongly typed)");
            }

            py::print("checking to see if the type was valid");
            // throw an error if the argument's type is invalid
            if (not isGTPType(annotations[argument])){
                throw py::type_error("Argument \"" + std::string(py::str(argument)) + "\" for custom GTP command \""
                                     + name + " \"has invalid type \"" + std::string(py::str(annotations[argument]))
                                     + "\".");
            }
        }

        if (not annotations.contains("return")) {
            throw py::value_error("Custom GTP command \"" + name + "\" has no specified return type "
                                  "(custom GTP commands must be strongly typed)");
        }

        // check the return type
        py::object returnType = annotations["return"];

        auto returnTypeOptions = py::list();

        // check if the return type is a union
        if (typing.attr("get_origin")(returnType).is(typing.attr("Union"))){
            // if it is, put all the types into the options
            returnTypeOptions = returnType.attr("__args__");
        }
        else {
            returnTypeOptions = {returnType};
        }

        // go through all the type options and make sure that they are all satisfactory
        for (unsigned i = 0; i < returnTypeOptions.size(); i++){

            auto option = returnTypeOptions[i];

            // if we have a strongly typed tuple, make sure it's valid and set the option to be it's second element
            if (typing.attr("get_origin")(option).is(py::type::of(py::tuple()))){

                auto types = py::tuple(typing.attr("get_args")(option));

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

                // if it's valid, set the option to be the second elemnt of the tuple
                option = py::tuple(typing.attr("get_args")(option))[1];
            }

            // throw an exception if the option is invalid
            if (not isGTPType(option)){
                throw pybind11::type_error("Custom GTP command returned invalid response, expected GTP compatible "
                                           "type, got " + std::string(py::str(py::type::of(returnType))));
            }
        }

        qualName = std::string(qualName.begin(), qualName.begin() + qualName.rfind("."));

        // we've conformed that the function's arguments and return type are valid, so we can register it
        if (globalCommands.find(qualName) == globalCommands.end()){
            globalCommands[qualName] = {function};
        }
        else {
            globalCommands[qualName].push_back(function);
        }

        return function;
    }

    std::string Engine::errorMessage(const std::string& message) {
        return "? " + message + "\n\n";
    }

    std::string Engine::errorMessage(const std::string &message, unsigned id) {
        return "?" + std::to_string(id) + " " + message + "\n\n";
    }

    std::string Engine::statusMessage(const std::string &message) {
        return "= " + message + "\n\n";
    }

    std::string Engine::statusMessage(const std::string &message, unsigned id) {
        return "=" + std::to_string(id) + " " + message + "\n\n";
    }

    bool Engine::argumentsMatch(const std::vector<ArgumentPattern> &expectedArguments,
                                const std::vector<std::shared_ptr<Token>> &arguments) {

        if (arguments.size() != expectedArguments.size()){
            return false;
        }

        for (unsigned i = 0; i < arguments.size(); i++){
            // if we have a literal, cast the argument to a literal and see if we have
            // a literal
            auto argument = (Literal*) arguments[i].get();
            if (argument->getLiteralType() != expectedArguments[i].second){
                return false;
            }
        }

        return true;

    }

    Response Engine::invalidArgumentsErrorMessage(const std::vector<std::vector<ArgumentPattern>>& argumentPatterns,
                                                  const std::vector<std::shared_ptr<Token>> &arguments) {

        std::stringstream message;

        std::vector<std::vector<ArgumentPattern>> candidates;

        // first, find all the candidate patterns (patters with the correct number of arguments)
        for (const auto& pattern : argumentPatterns){
            if (pattern.size() == arguments.size()){
                candidates.push_back(pattern);
            }
        }

        if (candidates.empty()){
            // if there are no valid candidates, give an error based on the number of arguments
            message << "invalid number of arguments for command \"" << arguments[0]->getText() << "\"; expected ";

            std::set<unsigned> expectedArguments;

            for (const auto& pattern : argumentPatterns){
                expectedArguments.insert(pattern.size() - 1); // minus 1 because the first argument is the operator
            }

            message << *expectedArguments.begin();

            if (expectedArguments.size() > 2){
                for (auto argumentCount = ++expectedArguments.begin();
                     argumentCount != --expectedArguments.end(); argumentCount++){
                    message << ", " << *argumentCount;
                }
            }

            if (expectedArguments.size() > 1){
                message << " or " << *(--expectedArguments.end());
            }

            message << ", got " << arguments.size() - 1;

        }
        else {

            message << "no viable argument pattern for command \"" << arguments[0]->getText() << "\";";

            for (const auto& candidate : candidates){
                // find the error
                for (unsigned i = 0; i < arguments.size(); i++){
                    auto argument = (Literal*) arguments[i].get();
                    if (argument->getLiteralType() != candidate[i].second){
                        message << " candidate pattern not valid: expected " << toString(candidate[i].second)
                                << " in position " << i << ", got " << toString(argument->getLiteralType());
                    }
                }
            }

        }

        return {false, message.str()};

    }

    Response Engine::execute(const std::string &command, const std::vector<std::shared_ptr<Token>> &arguments) {

        // generate a list of possible argument patterns
        std::vector<std::vector<ArgumentPattern>> patterns;

        for (auto& definition : commands[command]){
            patterns.push_back(definition.second);
        }

        // find a matching pattern
        auto iter = std::find_if(patterns.begin(), patterns.end(),
                                 [arguments](const std::vector<ArgumentPattern>& pattern){
            return argumentsMatch(pattern, arguments);
        });

        if (iter != patterns.end()){
            // look up the matching function in the table and evaluate it
            return commands[command][iter - patterns.begin()].first(this, arguments);
        }
        else {
            return invalidArgumentsErrorMessage(patterns, arguments);
        }

    }

}
