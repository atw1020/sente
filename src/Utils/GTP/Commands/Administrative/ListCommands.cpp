//
// Created by arthur wesley on 12/11/21.
//

#include "../../../../Include/Utils/GTP/Commands/Administrative/ListCommands.h"

#include <sstream>

#include "../../../../Include/Utils/GTP/Commands/Administrative/SupportedCommands.h"

namespace sente::GTP {

    std::string ListCommands::getResponse() {

        std::stringstream response;

        for (const auto& command : commands){
            response << command << std::endl;
        }

        return response.str();

    }
}
