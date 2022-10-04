//
// Created by arthur wesley on 12/18/21.
//

#ifndef SENTE_OPERATORS_H
#define SENTE_OPERATORS_H

#include "Session.h"

namespace sente::GTP {

    class DefaultSession;

    typedef std::function<Response (DefaultSession&, const std::vector<std::shared_ptr<Token>>&)> LocalCommandMethod;

    class DefaultSession: public Session {
    public:

        DefaultSession(const std::string& engineName, const std::string& engineVersion);

    private:

        void registerCommand(const std::string& commandName, CommandMethod method,
                             std::vector<ArgumentPattern> argumentPattern);

        Response protocolVersion(const std::vector<std::shared_ptr<Token>>& arguments);
        Response name(const std::vector<std::shared_ptr<Token>>& arguments);
        Response version(const std::vector<std::shared_ptr<Token>>& arguments);
        Response knownCommand(const std::vector<std::shared_ptr<Token>>& arguments);
        Response listCommands(const std::vector<std::shared_ptr<Token>>& arguments);
        Response quit(const std::vector<std::shared_ptr<Token>>& arguments);
        Response boardSize(const std::vector<std::shared_ptr<Token>>& arguments);
        Response clearBoard(const std::vector<std::shared_ptr<Token>>& arguments);
        Response komi(const std::vector<std::shared_ptr<Token>>& arguments);
        Response play(const std::vector<std::shared_ptr<Token>>& arguments);
        Response genMove(const std::vector<std::shared_ptr<Token>>& arguments);
        Response showBoard(const std::vector<std::shared_ptr<Token>>& arguments);

        Response undoOnce(const std::vector<std::shared_ptr<Token>>& arguments);
        Response undoMultiple(const std::vector<std::shared_ptr<Token>>& arguments);
        Response loadSGF1(const std::vector<std::shared_ptr<Token>>& arguments);
        Response loadSGF2(const std::vector<std::shared_ptr<Token>>& arguments);

        Response baseLoadSGF(const std::string& filePath);

        static std::unordered_set<std::string> builtins;

    };

}

#endif //SENTE_OPERATORS_H
