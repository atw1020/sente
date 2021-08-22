//
// Created by arthur wesley on 8/22/21.
//

#ifndef SENTE_RESPONSE_H
#define SENTE_RESPONSE_H

#include <string>

#include "Command.h"

namespace sente {
    namespace gtp{

        class GTPSuccess : public GTPCommand {
        public:

            explicit operator std::string();

        private:

        };

        class GTPError : public GTPCommand {
        public:
        private:

        };

    }
}

#endif //SENTE_RESPONSE_H
