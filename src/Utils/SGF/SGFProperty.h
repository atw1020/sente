//
// Created by arthur wesley on 8/27/21.
//

#ifndef SENTE_SGFPROPERTY_H
#define SENTE_SGFPROPERTY_H

#include <string>

namespace sente::SGF {
    enum SGFProperty {
        NONE,
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
        C,  // comment
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
        TM, // time info todo: support
        US, // creator of the game todo: support
        WR, // white rank
        WT, // white team todo: support
        /// timing properties
        BL, // black time left todo: support
        OB, // number of byo-yomi periods left black todo: support
        OW, // number of byo-yomi periods left white todo: support
        WL, // white time left todo: support
        /// misc
        FG, // figure todo: support
        PM, // print move count todo: support
        VW, // only view part of the board
        /// Go-specific properties
        HA, // handicap
        KM, // komi
        TB, // territory black
        TW, // territory white
        /// SGF-3 specific properties
        ID, // unique ID of the game todo: support
        LT, // enforce losing on time todo: support
        OM, // number of moves for each overtime todo: support
        OP, // length of each overtime period todo: support
        OV, // operator overhead for each move todo: support
        SE, // moves tried so far todo: support
        SI, // position marked with sigma todo: support
        TC, // territory count todo: support
        /// SGF-1 specific properties
        EL, // human evaluation of a computer's move todo: support
        EX, // expected next move todo: support
        L,  // letters on points todo: support
        M,  // marked points todo: support
        BS, // black species todo: support
        CH, // check mark todo: support
        RG, // region of the board todo: support
        SC, // secure stones todo: support
        WS, // white species todo: support
    };

    SGFProperty fromStr(const std::string& sgfProperty);
    std::string toStr(SGFProperty property);

    bool isProperty(const std::string& property);
    bool isFileWide(SGFProperty property);
    bool isSGFLegal(SGFProperty property, unsigned version);
    std::vector<unsigned> getPossibleSGFVersions(const std::unordered_set<SGFProperty>& properties);


}

namespace std {

    template<>
    struct hash<sente::SGF::SGFProperty> {
        size_t operator()(const sente::SGF::SGFProperty& sgfProperty) const noexcept;
    };
}

#endif //SENTE_SGFPROPERTY_H
