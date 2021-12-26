//
// Created by arthur wesley on 12/12/21.
//

#ifndef SENTE_ENGINE_H
#define SENTE_ENGINE_H

#include "Parser.h"

#include <string>
#include <variant>

#include "../../Game/GoGame.h"

#include "Tokens/Literal.h"

namespace sente::GTP {

    class Engine;

    typedef std::pair<bool, std::string> Response;
    typedef std::pair<std::string, LiteralType> ArgumentPattern;
    typedef std::function<Response (Engine* self, const std::vector<std::shared_ptr<Token>>& arguments)> CommandMethod;

    class Engine {
    public:

        GoGame masterGame; // the game object that the GTP edits
        // GoGame scratchGame; // a game that the engine can play on and experiment with variations on

        Engine(const std::string& engineName, const std::string& engineVersion);

        // GTP interpreter
        std::string interpret(std::string text);

        // Custom GTP command Registration
        void pyRegisterCommand(const py::function& function, const py::module_& inspect);

        ///
        /// Getter and Setter Methods
        ///

        [[nodiscard]] std::string getEngineName() const;
        void setEngineName(std::string name);

        [[nodiscard]] std::string getEngineVersion() const;
        void setEngineVersion(std::string version);

        [[nodiscard]] std::unordered_map<std::string, std::vector<std::pair<CommandMethod,
                                                              std::vector<ArgumentPattern>>>> getCommands() const;

        [[nodiscard]] bool isActive() const;
        void setActive(bool set);

        ///
        /// Miscellaneous methods
        ///

        // for resting the game
        void setGTPDisplayFlags();

        ///
        /// static methods
        ///

        const static py::function& registerCommand(const py::function& function, const py::module_& inspect,
                                             const py::module_& typing);


    private:

        bool active = true;

        std::string engineName;
        std::string engineVersion;

        std::unordered_map<std::string, std::vector<std::pair<CommandMethod, std::vector<ArgumentPattern>>>> commands;

        void registerCommand(const std::string& commandName, CommandMethod method,
                             std::vector<ArgumentPattern> argumentPattern);

        Response execute(const std::string& command, const std::vector<std::shared_ptr<Token>>& arguments);

        static std::string errorMessage(const std::string& message) ;
        static std::string errorMessage(const std::string& message, unsigned i) ;
        static std::string statusMessage(const std::string& message) ;
        static std::string statusMessage(const std::string& message, unsigned i) ;

        static bool argumentsMatch(const std::vector<ArgumentPattern>& expectedTypes,
                                   const std::vector<std::shared_ptr<Token>>& arguments);
        static Response invalidArgumentsErrorMessage(const std::vector<std::vector<ArgumentPattern>>& argumentPatterns,
                                                 const std::vector<std::shared_ptr<Token>>& arguments);

        static std::unordered_map<std::string, std::vector<py::function>> globalCommands;

    };
}


#endif //SENTE_ENGINE_H
