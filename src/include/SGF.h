//
// Created by arthur wesley on 7/2/21.
//

#ifndef SENTE_SGF_H
#define SENTE_SGF_H

#include <string>
#include <ciso646>

#include "Tree.h"
#include "GoGame.h"
#include "GoComponents.h"

#ifdef __CYGWIN__
#include <ciso646>
#endif

#define stringify(x) "#x"

/**
 *
 * SGF file format specifications
 *
 * https://www.red-bean.com/sgf/properties.html
 *
 */
namespace sente {
    namespace utils {

        enum SGFCommands {
            /// data taken from https://www.red-bean.com/sgf/properties.html
            /// Move properties
            B,  // black move
            KO, // ko point todo: support
            MN, // Move number todo: support
            W,  // white move
            /// Setup Properties
            AB, // add black stone todo: support
            AE, // add empty space todo: support
            AW, // add white stone todo: support
            PL, // get player name todo: support
            /// Node Annotations
            C,  // comment todo: support
            DM, // position is even for both players todo: support
            GB, // good for black to play todo: support
            GW, // good for white to play todo: support
            HO, // hot spot todo: support
            N,  // name of the node todo: support
            UC, // position is unclear todo: support
            V,  // value todo: support
            /// Move Annotation
            BM, // bad move todo: support
            DO, // doubtful move todo: support
            IT, // interesting move todo: support
            TE, // tesuji todo: support
            /// Markup properties
            AR, // arrow todo: support
            CR, // circle todo: support
            DD, // dim out todo: support
            LB, // label todo: support
            LN, // line todo: support
            MA, // mark with an x todo: support
            SL, // selected points todo: support
            SQ, // mark with a square todo: support
            TR, // mark with a triangle todo: support
            /// Root Properties
            AP, // application used to create the file todo: support
            CA, // charset todo: support
            FF, // file format
            GM, // Game mode
            ST, // how many variations should be shown todo: support
            SZ, // size of the board
            /// Game info Properties
            AN, // name of annotator todo: support
            BR, // black rank
            BT, // black's team todo: support
            CP, // copyright info todo: support
            DT, // date the game was played todo: support
            EV, // event todo: support
            GN, // game name
            GC, // extra game info todo: support
            ON, // opening name todo: support
            OT, // overtime todo: support
            PB, // black name
            PC, // place todo: support
            PW, // white player's name
            RE, // result of the game
            RO, // round number & type todo: support
            RU, // ruleset
            SO, // source todo: support
            /// timing properties
            TM, // time info todo: support
            US, // creator of the game todo: support
            WR, // white rank
            WT, // white team todo: support
            /// misc
            FG, // figure todo: support
            PM, // print move count todo: support
            VW  // only view part of the board
        };

        GoGame loadSGF(const std::string &SGFText);

        std::string dumpSGF(const GoGame& game, std::unordered_map<std::string, std::string> &attributes);

        std::unordered_map<std::string, std::string> getMetadata(const std::string& SGFText);

    }
}


#endif //SENTE_SGF_H
