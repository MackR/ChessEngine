//
//  ChessConstants.h
//  ChessText
//
//  Created by Mack Ragland on 11/18/22.
//  Copyright © 2022 KattisPracticeProblems. All rights reserved.
//

#ifndef ChessConstants_h
#define ChessConstants_h

namespace CONSTANTS {

typedef enum{
    A1,B1,C1,D1,E1,F1,G1,H1,
    A2,B2,C2,D2,E2,F2,G2,H2,
    A3,B3,C3,D3,E3,F3,G3,H3,
    A4,B4,C4,D4,E4,F4,G4,H4,
    A5,B5,C5,D5,E5,F5,G5,H5,
    A6,B6,C6,D6,E6,F6,G6,H6,
    A7,B7,C7,D7,E7,F7,G7,H7,
    A8,B8,C8,D8,E8,F8,G8,H8, OO
}Square;



enum class Piece {
    EMPTY = 0,
    BPAWN,
    BROOK,
    BKNIGHT,
    BBISHOP,
    BQUEEN,
    BKING,
    WPAWN,
    WROOK,
    WKNIGHT,
    WBISHOP,
    WQUEEN,
    WKING
    
}; // Black pieces 1-6, White pieces 7-12

enum class Color {
    EMPTY = -1,
    BLACK,
    WHITE,
    INVALID = -2
};

enum class Status {
        INVALID = -2,
        ENEMY = -1,
        EMPTY = 0,
        FRIENDLY = 1
};

}



#endif /* ChessConstants_h */
