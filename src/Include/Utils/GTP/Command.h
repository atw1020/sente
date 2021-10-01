//
// Created by arthur wesley on 8/22/21.
//

#ifndef SENTE_COMMAND_H
#define SENTE_COMMAND_H

#include <string>

namespace sente {
    namespace gtp {

        class GTPCommand {
        public:

            virtual explicit operator std::string() const = 0;

        private:

            const unsigned id;
            static const std::string commandName;

        };
    }
}

#endif //SENTE_COMMAND_H
