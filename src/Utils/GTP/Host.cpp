//
// Created by arthur wesley on 12/12/21.
//

#include "../../Include/Utils/GTP/Host.h"

#include "../../Include/Utils/GTP/Parser.h"
#include "../../Include/JavaUtils.h"
#include "../../Include/Utils/GTP/Tokens/Seperator.h"

namespace sente::GTP {

    std::string Host::evaluate(std::string text) {

        auto tokens = parse(text);

        // iterate through the tokens
        for (unsigned i = 0; i < tokens.size(); i++){
            while (not instanceof<Seperator>(tokens[i].get())){
                // keep incrementing until we find a seperator
                i++;
            }
        }


    }

}