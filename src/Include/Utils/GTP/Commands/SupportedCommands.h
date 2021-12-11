//
// Created by arthur wesley on 12/11/21.
//

#ifndef SENTE_SUPPORTEDCOMMANDS_H
#define SENTE_SUPPORTEDCOMMANDS_H

#include <set>

std::set<std::string> commands = {
        "protocol_version",
        "name",
        "version",
        "known_command",
        "list_commands",
        "quit",
        "boardsize",
        "clear_board",
        "komi",
        "play",
        "genmove"
};

#endif //SENTE_SUPPORTEDCOMMANDS_H
