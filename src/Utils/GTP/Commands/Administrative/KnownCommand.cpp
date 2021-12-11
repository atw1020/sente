//
// Created by arthur wesley on 12/11/21.
//

#include "../../../../Include/Utils/GTP/Commands/Administrative/KnownCommand.h"

#include "../../../../Include/Utils/GTP/Commands/Administrative/SupportedCommands.h"

namespace sente::GTP {

    KnownCommand::KnownCommand(const String& command) {
        arguments = {command};
    }

    std::string KnownCommand::getResponse() {
        if (commands.find(arguments[0].getValue()) == commands.end()){
            return "false";
        }
        else {
            return "true";
        }
    }

}