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
        KO, // ko point Changes.txt: support
        MN, // Move number Changes.txt: support
        W,  // white move
        /// Setup Properties
        AB, // add black stone Changes.txt: support
        AE, // add empty space Changes.txt: support
        AW, // add white stone Changes.txt: support
        PL, // get player name Changes.txt: support
        /// Node Annotations
        C,  // comment
        DM, // position is even for both players Changes.txt: support
        GB, // good for black to play Changes.txt: support
        GW, // good for white to play Changes.txt: support
        HO, // hot spot Changes.txt: support
        N,  // name of the node Changes.txt: support
        UC, // position is unclear Changes.txt: support
        V,  // value Changes.txt: support
        /// Move Annotation
        BM, // bad move Changes.txt: support
        DO, // doubtful move Changes.txt: support
        IT, // interesting move Changes.txt: support
        TE, // tesuji Changes.txt: support
        /// Markup properties
        AR, // arrow Changes.txt: support
        CR, // circle Changes.txt: support
        DD, // dim out Changes.txt: support
        LB, // label Changes.txt: support
        LN, // line Changes.txt: support
        MA, // mark with an x Changes.txt: support
        SL, // selected points Changes.txt: support
        SQ, // mark with a square Changes.txt: support
        TR, // mark with a triangle Changes.txt: support
        /// Root Properties
        AP, // application used to create the file Changes.txt: support
        CA, // charset Changes.txt: support
        FF, // file format
        GM, // Game mode
        ST, // how many variations should be shown Changes.txt: support
        SZ, // size of the board
        /// Game info Properties
        AN, // name of annotator Changes.txt: support
        BR, // black rank
        BT, // black's team Changes.txt: support
        CP, // copyright info Changes.txt: support
        DT, // date the game was played Changes.txt: support
        EV, // event Changes.txt: support
        GN, // game name
        GC, // extra game info Changes.txt: support
        ON, // opening name Changes.txt: support
        OT, // overtime Changes.txt: support
        PB, // black name
        PC, // place Changes.txt: support
        PW, // white player's name
        RE, // result of the game
        RO, // round number & type Changes.txt: support
        RU, // ruleset
        SO, // source Changes.txt: support
        TM, // time info Changes.txt: support
        US, // creator of the game Changes.txt: support
        WR, // white rank
        WT, // white team Changes.txt: support
        /// timing properties
        BL, // black time left Changes.txt: support
        OB, // number of byo-yomi periods left black Changes.txt: support
        OW, // number of byo-yomi periods left white Changes.txt: support
        WL, // white time left Changes.txt: support
        /// misc
        FG, // figure Changes.txt: support
        PM, // print move count Changes.txt: support
        VW, // only view part of the board
        /// Go-specific properties
        HA, // handicap
        KM, // komi
        TB, // territory black
        TW, // territory white
        /// SGF-3 specific properties
        ID, // unique ID of the game Changes.txt: support
        LT, // enforce losing on time Changes.txt: support
        OM, // number of moves for each overtime Changes.txt: support
        OP, // length of each overtime period Changes.txt: support
        OV, // operator overhead for each move Changes.txt: support
        SE, // moves tried so far Changes.txt: support
        SI, // position marked with sigma Changes.txt: support
        TC, // territory count Changes.txt: support
        /// SGF-1 specific properties
        EL, // human evaluation of a computer's move Changes.txt: support
        EX, // expected next move Changes.txt: support
        L,  // letters on points Changes.txt: support
        M,  // marked points Changes.txt: support
        BS, // black species Changes.txt: support
        CH, // check mark Changes.txt: support
        RG, // region of the board Changes.txt: support
        SC, // secure stones Changes.txt: support
        WS, // white species Changes.txt: support
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
