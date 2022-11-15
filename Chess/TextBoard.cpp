//
//  TextBoard.cpp
//  Chess
//
//  Created by Mack Ragland on 10/12/22.
//  Copyright © 2022 MackRagland. All rights reserved.
//

#include "TextBoard.hpp"
#include <stdio.h>


namespace TextBoard{
TextBoard(){
    // Init the board itself
    m_Board[A1] = WROOK;
    m_Board[B1] = WKNIGHT;
    m_Board[C1] = WBISHOP;
    m_Board[D1] = WKING;
    m_Board[E1] = WQUEEN;
    m_Board[F1] = WBISHOP;
    m_Board[G1] = WKNIGHT;
    m_Board[H1] = WROOK;
    for(int i = 8; i <16; ++i){
        m_Board[i] = WPAWN;
    }
    for(int i = 16; i <48; ++i){
        m_Board[i] = EMPTY;
    }
    for(int i = 48; i <56; ++i){
        m_Board[i] = BPAWN;
    }
    m_Board[A8] = BROOK;
    m_Board[B8] = BKNIGHT;
    m_Board[C8] = BBISHOP;
    m_Board[D8] = BKING;
    m_Board[E8] = BQUEEN;
    m_Board[F8] = BBISHOP;
    m_Board[G8] = BKNIGHT;
    m_Board[H8] = BROOK;
    
    //Create the first board state in the m_boardHistory stack
    m_boardHistory.append(m_Board);
    
    //Init the supporting variables
    m_turnNum = 1;
    std::stack<std::string> m_moveHistory;
    m_whitesTurn = true;
    m_blackKingMoved = false;
    m_whiteKingMoved = false;
    m_whiteARookMoved = false;
    m_whiteHRookMoved = false;
    m_blackARookMoved = false;
    m_blackHRookMoved = false;
    m_includeEnpassant = false;
    
    findPlayerPieces();
    
}
~TextBoard(){
    
}

inline bool isWhite(int8_t piece){return (piece >= WPAWN && piece <= WKING);}
inline bool isBlack(int8_t piece){return (piece >= BPAWN && piece <= BKING);}
inline bool isEmpty(int8_t piece){return (piece == 0);}

char getPieceType(int nFile, int rank, Pieces board[8][8] = m_board){
    if (file <0 || file > 7 || row < 0 || row > 7) {std::cout << "Error, invalid row or file input" << std::endl; return 'E';}
    int piece = board[rank][nFile];
    switch(piece){
        case BPAWN || WPAWN: return 'P';
            break;
        case BROOK || WROOK: return 'R';
            break;
        case BKNIGHT || WKNIGHT: return 'N';
            break;
        case BBISHOP || WBISHOP: return 'B';
            break;
        case BQUEEN || WQUEEN: return 'Q';
            break;
        case BKING || WKING: return 'K';
            break;
        case EMPTY: std::cout << "Error, passed empty square to getPieceType()" << std::endl;
            return 'E';
            break;
    }
}

int getPieceColor(int file, int row, Pieces board[8][8] = m_board){
    if (file <0 || file > 7 || row < 0 || row > 7) {std::cout << "Error, invalid row or file input" << std::endl; return -2;}
    
    uint8_t piece = board[rank][file];
    if (isEmpty(piece)) return -1
    else if (isWhite(piece)) return 1;
    else if(isBlack(piece)) return 0;
    else std::cout << "Error, piece/ piece color invalid" << std::endl; return -2
    
}

std::string buildMoveString(char cFileOld, int rankOld, char cFileNew, int rankNew){
    string sFilePrev (1,cFileOld);
    int nFileOld = cFileToIndex(cFileOld);
    int nFileNew = cFileToIndex(cFileNew);
    string sFileNew (1,cFileNew);
    string sRankOld = to_string(rankOld+1);
    string sRankNew = to_string(rankNew+1);
    string type = getPieceType(nFileOld, rankOld);
    string moveString = type + sFilePrev + sRankOld + sFileNew + sRankNew;
    //cout << "Building position from (char, int): " << moveString << endl;
    return moveString;
}

int checkSquareStatus(int playerColor, int nFile, int rank){
    if (nFile < 0 || nFile > 7 || rank < 0 || rank > 7) { cout << "Warning, illegal rank and file numbers" << endl; return -2;}
    if (playerColor != 1 && playerColor != 0) {
        std::cout << "ERROR: returning -3, invalid playerColor input: checkSquareStatus() Color: " << playerColor << std::endl; return -3;}
    
    int squareContentsColor = getPieceColor(nFile, rank);
    if (squareContentsColor == -1){
        //  cout << "Square " << indexTo_cFile(nFile) << rank+1 << " is empty" << endl;
        return 0;
    }
    else if (squareContentsColor == playerColor){
        //  cout << "Friendly piece on square " << indexTo_cFile(nFile) << rank << endl;
        return 1;
    }
    else {
        //  cout << "Enemy piece on square " << indexTo_cFile(nFile) << rank << endl;
        return -1;
    }
}

static int cFileToIndex(char cFile){
    return cFile-'A';
}
static char indexTo_cFile(int nFile){
    return static_cast<char>(nFile + 'A');
}

static int convertCoordinateToBoardIndex(int nFile, int rank){
    
    if (nFile >= 0 && nFile <= 7 && rank >= 0 && rank <= 7) {
        return rank*8 + nFile;
    }
    //cout << "Warning, illegal rank and file numbers" << endl;
    
    return -1;
    
}

static void parseMove( string move, char& pieceType, char& prevFile, int& prevRank, char& newFile, int& newRank){
    pieceType = move[0];
    prevFile = move[1];
    prevRank = move[2] - '0'-1;
    newFile = move[3];
    newRank = move[4] -'0'-1;    // Convert string to int
}

void calcPawnMoves(int file, int rank, std::list<string> &movesResults){
    typedef enum { NA = -1, BLACK = 0, WHITE = 1} Color;
    int inFrontPawnRank = -1;
    int piece = m_board[rank][file];
    int pieceColor = getPieceColor(file,rank);
    int doubleJumpPawnRank = -1;
    int doubleJumpPawnFile = -1;
    int enemyDoubleJumpPawnRank = -1;
    int enemyDoubleJumpPawnFile = -1;
    int pawnLeftDiagonalRank = -1;
    int pawnLeftDiagonalFile = -1;
    int pawnRightDiagonalRank = -1;
    int pawnRightDiagonalFile = -1;
    int singleJumpSquareStatus = -1;
    int prePromotionRank = -1;
    bool enPassantValidLeft = false;
    bool enPassantValidRight = false;
    Piece leftPiece;
    Piece rightPiece;
    
    bool findEnpassant = false;
    if ((moveHistory.top[0] == "P") && (abs(moveHistory.top[2]-moveHistory.top[4]) > 1)){
        
        findEnpassant = true;
        enemyDoubleJumpPawnRank = m_moveHistory.top[4]-'0'; // Convert from char to int
        enemyDoubleJumpPawnFile = m_moveHistory.top[3]-'A'; // Convert from char to int
        
    }
    
    //Build the possible moves depending on the location of the pawn
    if (pieceColor == WHITE) {
        prePromotionRank = 6;
        if (rank != 7) {
            inFrontPawnRank = rank+1;
        }
        if(file != 0 && (rank != 0 || rank != 7)) {
            pawnLeftDiagonalRank = rank+1;
            pawnLeftDiagonalFile = file-1;
        }
        if(file != 7 && (rank != 0 || rank != 7)){
            pawnRightDiagonalRank = rank+1;
            pawnRightDiagonalFile = file+1;
        }
        if (rank == 1 ) { // Pawn hasn't moved, enable double jump
            doubleJumpPawnRank = rank+2;
            singleJumpSquareStatus = checkSquareStatus(WHITE, file, rank+1); // Returns ILLEGAL, ENEMY, EMPTY, FRIENDLY enums
            
        }
        if(findEnpassant){
            if (rank == 4) { // En passant
                
                if (file != 0) {
                    leftPiece = m_board[rank][file-1];
                    bool isBlack = (leftPiece >= BPAWN && leftPiece <=BKING);
                    bool enPassentLeft = false;
                    if (rank == enemyDoubleJumpRank && file-1 == enemyDoubleJumpFile) enPassentLeft = true; // if the pawns are on same rank and doubleJumper is adjacent the pawn, then enable enPassent
                    if (isBlack && enPassentLeft){
                        movesResults.push_back(buildMoveString(file, rank, enemyDoubleJumpPawnFile, rank-1));
                    }
                }
                if (file != 7){
                    rightPiece = m_board[rank][file+1];
                    bool isBlack = (rightPiece >=BPAWN && rightPiece <=BKING)
                    bool enPassentRight = false;
                    if (rank == enemyDoubleJumpPawnRank && file+1 == enemyDoubleJumpPawnFile) enPassentRight = true;
                    if (isBlack && enPassentRight){
                        movesResults.push_back(buildMoveString(file, rank, enemyDoubleJumpPawnFile, rank+1));
                    }
                }
            }
        }
    }
    else if (pieceColor == BLACK){
        prePromotionRank = 1;
        if (rank != 0) {
            inFrontPawnRank = rank-1;
        }
        if (file != 0 && (rank != 0 || rank != 7)) {
            pawnLeftDiagonalRank = rank-1;
            pawnLeftDiagonalFile = file-1;
        }
        if(file != 7 && (rank != 0 || rank != 8)){
            pawnRightDiagonalRank = rank-1;
            pawnRightDiagonalFile = file+1;
        }
        if (rank == 6 ) { // black side double jump
            doubleJumpPawnRank = rank-2;
            singleJumpSquareStatus = checkSquareStatus( file, rank-1);
        }
        if (findEnpassant){
            if (rank == 3) { // En passant
                
                if (file != 0) {
                    leftPiece = m_board[rank][file-1];
                    bool isWhite = (leftPiece >= WPAWN && leftPiece <=WKING);
                    bool enPassentLeft = false;
                    if (rank == enemyDoubleJumpPawnRank && file-1 == enemyDoubleJumpPawnFile) enPassentLeft = true;
                    if (isWhite && enPassentLeft){
                        movesResults.push_back(buildMoveString(file, rank, enemyDoubleJumpPawnFile, rank-1));
                    }
                }
                if (file != 7){
                    rightPiece = m_board[rank][file+1];
                    bool isWhite = (rightPiece >=WPAWN && rightPiece <=WKING)
                    bool enPassentRight = false;
                    if (rank == enemyDoubleJumpPawnRank && file+1 == enemyDoubleJumpPawnFile) enPassentRight = true;
                    if (isWhite && enPassentRight){
                        movesResults.push_back(buildMoveString(file, rank, enemyDoubleJumpPawnFile, rank+1));
                    }
                }
            }
        }
    }
    else{
        std::cout << "No piece at location pawnMoves( int " << file << ", int " << rank << ")" << std::endl;
    }
    
    
    
    //Add the possible moves to the index depending if there is no piece, a friendly piece, or enemy piece on the coordinate
    if (inFrontPawnRank != -1) {
        
        if (m_board[inFrontPawnRank][file] == EMPTY) {
            if (rank != prePromotionRank) {
                movesResults->push_back(buildPosition(inFrontPawnIndex));
            }
            else { // Pawn promotion by going straight
                movesResults->push_back(buildPosition(inFrontPawnIndex)+"R");
                movesResults->push_back(buildPosition(inFrontPawnIndex)+"N");
                movesResults->push_back(buildPosition(inFrontPawnIndex)+"B");
                movesResults->push_back(buildPosition(inFrontPawnIndex)+"Q");
            }
            
        }
        
    }
    if (doubleJumpPawnRank != -1) {
        if (m_board[doubleJumpPawnRank][file] == EMPTY && singleJumpSquareStatus == EMPTY) {
            movesResults.push_back(buildMoveString(file, rank, file, doubleJumpPawnRank));
        }
    }
    if (pawnLeftDiagonalIndex != -1) {
        bool isWhite = (leftPiece >= WPAWN && leftPiece <=WKING);
        if ((boardState[pawnLeftDiagonalIndex].getColor() != m_color && boardState[pawnLeftDiagonalIndex].getColor() != EMPTY) || enPassantValidLeft) {
            if (rank != prePromotionRank) {
                movesResults.push_back(buildPosition(pawnLeftDiagonalIndex));
            }
            else { // Pawn promotion left diagonal capture
                movesResults.push_back(buildPosition(pawnLeftDiagonalIndex)+"R");
                movesResults.push_back(buildPosition(pawnLeftDiagonalIndex)+"N");
                movesResults.push_back(buildPosition(pawnLeftDiagonalIndex)+"B");
                movesResults.push_back(buildPosition(pawnLeftDiagonalIndex)+"Q");
            }
            
        }
    }
    if (pawnRightDiagonalIndex != -1) {
        int pawnRightDiagonal = m_board[pawnRightDiagonalRank][pawnRightDiagonalFile];
        int pawnRightDiagonalColor = -2;
        
        if (pawnRightDiagonal >= WPAWN && pawnRightDiagonal <= WKING) pawnRightDiagonalColor = 1;
        else if (pawnRightDiagonal >= BPAWN && pawnRightDiagonal <= BKING) pawnRightDiagonalColor = 0;
        else pawnRightDiagonalColor = -1;
        
        if (pawnRightDiagonalColor != m_color && pawnRightDiagonalColor != EMPTY) {
            if (rank != prePromotionRank) {
                movesResults.push_back(buildMoveString(file, rank, pawnRightDiagonalFile, pawnRightDiagonalRank));
            }
            else { // Pawn promotion right diagonal capture
                movesResults.push_back(buildMoveString(file, rank, pawnRightDiagonalFile, pawnRightDiagonalRank)+"R");
                movesResults.push_back(buildMoveString(file, rank, pawnRightDiagonalFile, pawnRightDiagonalRank)+"N");
                movesResults.push_back(buildMoveString(file, rank, pawnRightDiagonalFile, pawnRightDiagonalRank)+"B");
                movesResults.push_back(buildMoveString(file, rank, pawnRightDiagonalFile, pawnRightDiagonalRank)+"Q");
            }
        }
    }
    
    // return one space infront of pawn, two if on its first move
    
    return;
    
}
void calcRookMoves(int file, int rank, std::list<string> &movesResults){
    
    int pieceColor = getPieceColor(file,rank);
    bool stopFlag[4] = false;
    
    for ( int i = 1; i < 8; i++) {
        
        
        if (stopFlag[0] == false) {
            
            int squareStatus = checkSquareStatus(pieceColor, file +i, rank);
            if (squareStatus == INVALID) {
                stopFlag[0] = true;
            }
            else if (squareStatus == ENEMY){
                movesResults.push_back(buildMoveString(file, rank, file +i, rank));
                stopFlag[0] = true;
            }
            else if (squareStatus == EMPTY){
                movesResults.push_back(buildMoveString(file, rank, file +i, rank));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[0] = true;
            }
        }
        if (stopFlag[1] == false) {
            
            int squareStatus = checkSquareStatus(pieceColor, file -i, rank);
            if (squareStatus == INVALID) {
                stopFlag[1] = true;
            }
            else if (squareStatus == ENEMY){
                movesResults.push_back(buildMoveString(file, rank, file -i, rank));
                stopFlag[1] = true;
            }
            else if (squareStatus == EMPTY){
                movesResults.push_back(buildMoveString(file, rank, file -i, rank));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[1] = true;
            }
        }
        if (stopFlag[2] == false) {
            
            int squareStatus = checkSquareStatus(pieceColor, file, rank + i);
            if (squareStatus == INVALID) {
                stopFlag[2] = true;
            }
            else if (squareStatus == ENEMY){
                movesResults.push_back(buildMoveString(file, rank, file, rank +i));
                stopFlag[2] = true;
            }
            else if (squareStatus == EMPTY){
                movesResults.push_back(buildMoveString(file, rank, file, rank +i));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[2] = true;
            }
        }
        if (stopFlag[3] == false) {
            
            int squareStatus = checkSquareStatus(pieceColor, file, rank-i);
            if (squareStatus == INVALID) {
                stopFlag[3] = true;
            }
            else if (squareStatus == ENEMY){
                movesResults.push_back(buildMoveString(file, rank, file, rank-i));
                stopFlag[3] = true;
            }
            else if (squareStatus == EMPTY){
                movesResults.push_back(buildMoveString(file, rank, file, rank-i));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[3] = true;
            }
        }
        
    }
    
    return;
}
void calcKnightMoves(int file, int rank, std::list<string> &moveResults){
    
    typedef enum {
        ILLEGAL = -2,
        ENEMY = -1,
        EMPTY = 0,
        FRIENDLY = 1
    } Status;
    int pieceColor = getPieceColor(file,rank);
    
    std::List<std::Pair<int, int>> legalSquares;
    legalSquares.push_back(std::Pair<int,int>(nFile+2, nRank+1));
    legalSquares.push_back(std::Pair<int,int>(nFile+2, nRank-1));
    legalSquares.push_back(std::Pair<int,int>(nFile-2, nRank+1));
    legalSquares.push_back(std::Pair<int,int>(nFile-2, nRank-1));
    legalSquares.push_back(std::Pair<int,int>(nFile+1, nRank+2));
    legalSquares.push_back(std::Pair<int,int>(nFile+1, nRank-2));
    legalSquares.push_back(std::Pair<int,int>(nFile-1, nRank+2));
    legalSquares.push_back(std::Pair<int,int>(nFile-1, nRank-2));
    
    for (auto squarePair: legalSquares){
        int squareStatus = checkSquareStatus(pieceColor, squarePair.first, squarePair.second);
        
        if (squareStatus != FRIENDLY && squareStatus != ILLEGAL) {
            movesResults->push_back(buildMoveString(file, rank, squarePair.first, squarePair.second));
        }
        
    }
    
    // moves to 8 spots except off board
    
    return;
}
void calcBishopMoves(int file, int rank, std::list<string> &moveResults){
    
    typedef enum {
        INVALID = -2,
        ENEMY = -1,
        EMPTY = 0,
        FRIENDLY = 1
    } Status;
    
    int pieceColor = getPieceColor(file,rank);
    bool stopFlag[4] = {false, false, false, false};
    
    for ( int i = 1; i < 8; i++) {
        if (stopFlag[0] == false) {
            
            int squareStatus = checkSquareStatus(pieceColor, file + i, rank + i);
            if (squareStatus == INVALID) {
                stopFlag[0] = true;
            }
            else if (squareStatus == ENEMY){
                moveResults.push_back(buildMoveString(file, rank, file +i, rank +i));
                stopFlag[0] = true;
            }
            else if (squareStatus == EMPTY){
                moveResults.push_back(buildMoveString(file, rank, file +i, rank +i));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[0] = true;
            }
        }
        if (stopFlag[1] == false) {
            
            int squareStatus = checkSquareStatus(pieceColor, file + i, rank -i);
            if (squareStatus == INVALID) {
                stopFlag[1] = true;
            }
            else if (squareStatus == ENEMY){
                moveResults.push_back(buildMoveString(file, rank, file +i, rank -i));
                stopFlag[1] = true;
            }
            else if (squareStatus == EMPTY){
                moveResults.push_back(buildMoveString(file, rank, file +i, rank -i));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[1] = true;
            }
        }
        if (stopFlag[2] == false) {
            
            int squareStatus = checkSquareStatus(pieceColor, file -i, rank + i);
            if (squareStatus == INVALID) {
                stopFlag[2] = true;
            }
            else if (squareStatus == ENEMY){
                moveResults.push_back(buildMoveString(file, rank, file -i, rank+i));
                stopFlag[2] = true;
            }
            else if (squareStatus == EMPTY){
                moveResults.push_back(buildMoveString(file, rank, file -i, rank +i));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[2] = true;
            }
        }
        if (stopFlag[3] == false) {
            
            int squareStatus = checkSquareStatus(pieceColor,file -i, rank -i);
            if (squareStatus == INVALID) {
                stopFlag[3] = true;
            }
            else if (squareStatus == ENEMY){
                moveResults.push_back(buildMoveString(file, rank, file -i, rank -i));
                stopFlag[3] = true;
            }
            else if (squareStatus == EMPTY){
                moveResults.push_back(buildMoveString(file, rank, file -i, rank -i));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[3] = true;
            }
        }
    }
    
    return;
}
void calcQueenMoves(int file, int rank, std::list<string> &moveResults){
    
    int pieceColor = getPieceColor(file,rank);
    
    bool stopFlag[8] = false;
    
    for ( int i = 1; i < 8; i++) {
        if (stopFlag[0] == false) {
            
            int squareStatus = checkSquareStatus(pieceColor, file + i, rank + i);
            if (squareStatus == INVALID) {
                stopFlag[0] = true;
            }
            else if (squareStatus == ENEMY){
                moves -> push_back(buildMoveString(file, rank, file +i, rank +i));
                stopFlag[0] = true;
            }
            else if (squareStatus == EMPTY){
                moves -> push_back(buildMoveString(file, rank, file +i, rank +i));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[0] = true;
            }
        }
        if (stopFlag[1] == false) {
            
            int squareStatus = checkSquareStatus(pieceColor,file + i, rank -i);
            if (squareStatus == INVALID) {
                stopFlag[1] = true;
            }
            else if (squareStatus == ENEMY){
                moves -> push_back(buildMoveString(file, rank, file +i, rank -i));
                stopFlag[1] = true;
            }
            else if (squareStatus == EMPTY){
                moves -> push_back(buildMoveString(file, rank, file +i, rank -i));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[1] = true;
            }
        }
        if (stopFlag[2] == false) {
            
            int squareStatus = checkSquareStatus(pieceColor,file -i, rank + i);
            if (squareStatus == INVALID) {
                stopFlag[2] = true;
            }
            else if (squareStatus == ENEMY){
                moves -> push_back(buildMoveString(file, rank, file -i, rank +i));
                stopFlag[2] = true;
            }
            else if (squareStatus == EMPTY){
                moves -> push_back(buildMoveString(file, rank, file -i, rank +i));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[2] = true;
            }
        }
        if (stopFlag[3] == false) {
            
            int squareStatus = checkSquareStatus(pieceColor,file -i, rank -i);
            if (squareStatus == INVALID) {
                stopFlag[3] = true;
            }
            else if (squareStatus == ENEMY){
                moves -> push_back(buildMoveString(file, rank, file -i, rank -i));
                stopFlag[3] = true;
            }
            else if (squareStatus == EMPTY){
                moves -> push_back(buildMoveString(file, rank, file -i, rank -i));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[3] = true;
            }
        }
        if (stopFlag[4] == false) {
            
            int squareStatus = checkSquareStatus(pieceColor,file + i, rank);
            if (squareStatus == INVALID) {
                stopFlag[4] = true;
            }
            else if (squareStatus == ENEMY){
                moves -> push_back(buildMoveString(file, rank, file +i, rank));
                stopFlag[4] = true;
            }
            else if (squareStatus == EMPTY){
                moves -> push_back(buildMoveString(file, rank, file +i, rank));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[4] = true;
            }
        }
        if (stopFlag[5] == false) {
            
            int squareStatus = checkSquareStatus(pieceColor,file -i, rank);
            if (squareStatus == INVALID) {
                stopFlag[5] = true;
            }
            else if (squareStatus == ENEMY){
                moves -> push_back(buildMoveString(file, rank, file -i, rank));
                stopFlag[5] = true;
            }
            else if (squareStatus == EMPTY){
                moves -> push_back(buildMoveString(file, rank, file -i, rank));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[5] = true;
            }
        }
        if (stopFlag[6] == false) {
            
            int squareStatus = checkSquareStatus(pieceColor,file, rank + i);
            if (squareStatus == INVALID) {
                stopFlag[6] = true;
            }
            else if (squareStatus == ENEMY){
                moves -> push_back(buildMoveString(file, rank, file, rank +i));
                stopFlag[6] = true;
            }
            else if (squareStatus == EMPTY){
                moves -> push_back(buildMoveString(file, rank, file, rank +i));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[6] = true;
            }
        }
        if (stopFlag[7] == false) {
            
            int squareStatus = checkSquareStatus(pieceColor,file, rank -i);
            if (squareStatus == INVALID) {
                stopFlag[7] = true;
            }
            else if (squareStatus == ENEMY){
                moves -> push_back(buildMoveString(file, rank, file, rank -i));
                stopFlag[7] = true;
            }
            else if (squareStatus == EMPTY){
                moves -> push_back(buildMoveString(file, rank, file, rank -i));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[7] = true;
            }
        }
    }
    
    return;
}
void calcKingMoves(int file, int rank, std::list<std::string> &moveResults){
    int pieceColor = getPieceColor(file, rank);
    for (int row = -1; row <=1; ++row){
        for (int col = -1; col <= 1; ++col){
            int squareStatus = -2;
            if (row == 0 && col == 0) continue; // Skip the position the king is already sitting in
            else {checkSquareStatus(pieceColor, file+col, rank+row);}
            if (squareStatus == 0 || squareStatus == -1) {
                moveResults.push_back(buildPositionString(file+1, rank+1));
            }
        }
    }
        return;
    
}

void calcPieceMoves(int file, int rank, std::list<std::string> &resultsList){
    char pieceType = getPieceType(file, rank);
    int pieceColor = getPieceColor(file, row);
    switch(pieceType){
        case 'P': calcPawnMoves(file, rank, resultsList);
            break;
        case 'R': calcRookMoves(file, rank, resultsList);
            break;
        case 'N': calcKnightMoves(file, rank, resultsList);
            break;
        case 'B': calcBishopMoves(file, rank, resultsList);
            break;
        case 'Q': calcQueenMoves(file, rank, resultsList);
            break;
        case 'K': calcKingMoves(file, rank, resultsList);
            break;
        case 'E': std::cout << "Error, passed empty square to getPieceType()" << std::endl;
            break;
    }
    return;
}

void findPlayerPieces(){  // Function should only be used once on init, then other functions should keep piece lists updated
//    int8_t whiteCounter = 0;
//    int8_t blackCounter = 0;
    
    
    for (int i = 0; i < 16; ++i){ // does this work for increasing vector size? or do I need other method
        m_whitePieceIndices[i] = i;
        m_blackPieceIndices[i] = i+48;
    }
    
    //This section finds the pieces' locations on the board, but since I know where they all are in the beginning, I don't need it
//    for (int square = 0; square < 64; ++square){
//        uint8_t squareContents = m_board[square];
//        if (isEmpty(squareContents)) break;
//        else if (isWhite(squareContents)){
//            m_whitePieceIndices[whiteCounter] = square
//            ++whiteCounter;
//        }
//        else if (isBlack(squareContents)){
//            m_blackPieceIndices[blackCounter] = square
//            ++blackCounter;
//        }
//        else std::cout << "Error! squareContents are not black, white, or empty" << std::endl;
//    }
    return;
}

int8_t findKingIndex(Colors color){ // this function is only needed for kings // such as myself :D
    int8_t (*pieceListIndexes)[16] = nullptr;
    if (color == WHITE) {
        pieceListIndexes = m_whitePieceIndices; // Set my array pointer to point to the first row of m_whitePieceIndices
    }
    else if(color == BLACK){
        pieceListIndexes = m_blackPieceIndices; // Set my array pointer to point to the first row of m_blackPieceIndices
    }
    else {
        cout << "Invalid player color, returning -1" << endl;
        return -1;
    }
    for (int i = 0; i<16; i++) {
        int8_t testPieceIndex = pieceListIndexes[i]
        if (getPieceType(testPieceIndex%8, testPieceIndex/8) == 'K'){
            return testPieceIndex;
        }
    }
    
    cout << "King not found (captured/non-present), returning -1" << endl;
    return -1;
    
}

void rookScreenMoves(int file, int rank, std::list<std::string>& resultsList){
    resultsList.clear()
    int pieceColor = getPieceColor(file, row);

    
    bool stopFlag[4] = {false, false, false, false}; // tells the pieces when to stop adding additional distance to their possible move direction (4 for rook)
    int screenNum[4] = {0,0,0,0}; // Counts the number of pieces we've looked through to try and find the king behind
    
    for ( int i = 1; i < 8; i++) {
        
        if (!stopFlag[0]) { // if we can keep adding move distance to the right of a piece
            
            int squareStatus = checkSquareStatus(pieceColor, file +i, rank); // check the space i squares to the right of the piece
            
            if (squareStatus == EMPTY){ // if the space is empty then do nothing
                //moves -> push_back(buildPositionString(file +i, rank));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[0] = true;
            }
            else if (squareStatus == INVALID) {
                stopFlag[0] = true; // Stop adding moves to the right
            }
            else if (squareStatus == ENEMY){
                
                ++screenNum[0]; // Add one to the count of attackers in the way of the piece
                if (screenNum[0] == 1) break; // Does this break from the local if statement, or the larger stopFlag[0] if statement?
                else if (screenNum[0] == 2 && getPieceType(file+i, rank) == 'K') { // if there are two attackers being hit, is the second one their king?
                    resultsList.push_back(buildPositionString(file +i, rank)); // It is a king!  so add the "screen" attack to the moves list
                    stopFlag[0] = true; // stop looking further for more pieces in the way
                }
               // screenedPiece = boardState[convertCoordinateToBoardIndex(file +i, rank)].getType();
                
            }
        }
        if (!stopFlag[1]) {
            
            int squareStatus = checkSquareStatus(pieceColor, file -i, rank); // check the space i squares to the right of the piece
            
            if (squareStatus == EMPTY){} // if the space is empty then do nothing
            else if (squareStatus == FRIENDLY){stopFlag[1] = true;} // Stop checking for screen moves
            else if (squareStatus == INVALID) {stopFlag[1] = true;} // You're looking off the board. Stop adding moves to the right
            else if (squareStatus == ENEMY){
                
                ++screenNum[1]; // Add one to the count of attackers in the way of the piece
                if (screenNum[1] == 1) break; // Does this break from the local if statement, or the larger stopFlag[0] if statement?
                else if (screenNum[1] == 2 && getPieceType(file-i, rank) == 'K') { // if there are two attackers being hit, is the second one their king?
                    resultsList.push_back(buildPositionString(file -i, rank)); // It is a king!  so add the "screen" attack to the moves list
                    stopFlag[1] = true; // stop looking further for more pieces in the way
                }
               // screenedPiece = boardState[convertCoordinateToBoardIndex(file +i, rank)].getType();
                
            }
        }
        if (!stopFlag[2]) {
            
            int squareStatus = checkSquareStatus(pieceColor, file, rank+i); // check the space i squares to the right of the piece
            
            if (squareStatus == EMPTY){ // if the space is empty then do nothing
                //moves -> push_back(buildPositionString(file +i, rank));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[2] = true;
            }
            else if (squareStatus == INVALID) {
                stopFlag[2] = true; // Stop adding moves to the right
            }
            else if (squareStatus == ENEMY){
                
                ++screenNum[2]; // Add one to the count of attackers in the way of the piece
                if (screenNum[2] == 1) break; // Does this break from the local if statement, or the larger stopFlag[0] if statement?
                else if (screenNum[2] == 2 && getPieceType(file, rank+i) == 'K') { // if there are two attackers being hit, is the second one their king?
                    resultsList.push_back(buildPositionString(file, rank+i)); // It is a king!  so add the "screen" attack to the moves list
                    stopFlag[2] = true; // stop looking further for more pieces in the way
                }
               // screenedPiece = boardState[convertCoordinateToBoardIndex(file +i, rank)].getType();
                
            }
        }
        if (!stopFlag[3]) {
            
            int squareStatus = checkSquareStatus(pieceColor, file, rank-i); // check the space i squares to the right of the piece
            
            if (squareStatus == EMPTY){ // if the space is empty then do nothing
                //moves -> push_back(buildPositionString(file +i, rank));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[3] = true;
            }
            else if (squareStatus == INVALID) {
                stopFlag[3] = true; // Stop adding moves to the right
            }
            else if (squareStatus == ENEMY){
                
                ++screenNum[3]; // Add one to the count of attackers in the way of the piece
                if (screenNum[3] == 1) break; // Does this break from the local if statement, or the larger stopFlag[0] if statement?
                else if (screenNum[3] == 2 && getPieceType(file, rank-i) == 'K') { // if there are two attackers being hit, is the second one their king?
                    resultsList.push_back(buildPositionString(file, rank-i)); // It is a king!  so add the "screen" attack to the moves list
                    stopFlag[3] = true; // stop looking further for more pieces in the way
                }
               // screenedPiece = boardState[convertCoordinateToBoardIndex(file +i, rank)].getType();
                
            }
        }
    }
    
    return;
}
  
void bishopScreenMoves(int file, int rank, std::list<std::string>& resultsList){
    resultsList.clear();
    int pieceColor = getPieceColor(file, row);

    
    bool stopFlag[4] = {false, false, false, false}; // tells the pieces when to stop adding additional distance to their possible move direction (4 for bishop)
    int screenNum[4] = {0,0,0,0}; // Counts the number of pieces we've looked through to try and find the king behind
    
    for ( int i = 1; i < 8; i++) {
        
        if (!stopFlag[0]) { // if we can keep adding move distance to the right of a piece
            
            int squareStatus = checkSquareStatus(pieceColor, file +i, rank+i); // check the space i squares to the right of the piece
            
            if (squareStatus == EMPTY){ // if the space is empty then do nothing
                //moves -> push_back(buildPositionString(file +i, rank));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[0] = true;
            }
            else if (squareStatus == INVALID) {
                stopFlag[0] = true; // Stop adding moves to the right
            }
            else if (squareStatus == ENEMY){
                
                ++screenNum[0]; // Add one to the count of attackers in the way of the piece
                if (screenNum[0] == 1) break; // Does this break from the local if statement, or the larger stopFlag[0] if statement?
                else if (screenNum[0] == 2 && getPieceType(file+i, rank+i) == 'K') { // if there are two attackers being hit, is the second one their king?
                    resultsList.push_back(buildPositionString(file +i, rank+i)); // It is a king!  so add the "screen" attack to the moves list
                    stopFlag[0] = true; // stop looking further for more pieces in the way
                }
                // screenedPiece = boardState[convertCoordinateToBoardIndex(file +i, rank)].getType();
                
            }
        }
        if (!stopFlag[1]) {
            
            int squareStatus = checkSquareStatus(pieceColor, file -i, rank+i); // check the space i squares to the right of the piece
            
            if (squareStatus == EMPTY){ // if the space is empty then do nothing
                //moves -> push_back(buildPositionString(file +i, rank));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[1] = true;
            }
            else if (squareStatus == INVALID) {
                stopFlag[1] = true; // Stop adding moves to the right
            }
            else if (squareStatus == ENEMY){
                
                ++screenNum[1]; // Add one to the count of attackers in the way of the piece
                if (screenNum[1] == 1) break; // Does this break from the local if statement, or the larger stopFlag[0] if statement?
                else if (screenNum[1] == 2 && getPieceType(file-i, rank+i) == 'K') { // if there are two attackers being hit, is the second one their king?
                    resultsList.push_back(buildPositionString(file -i, rank+i)); // It is a king!  so add the "screen" attack to the moves list
                    stopFlag[1] = true; // stop looking further for more pieces in the way
                }
                // screenedPiece = boardState[convertCoordinateToBoardIndex(file +i, rank)].getType();
                
            }
        }
        if (!stopFlag[2]) {
            
            int squareStatus = checkSquareStatus(pieceColor, file+i, rank-i); // check the space i squares to the right of the piece
            
            if (squareStatus == EMPTY){ // if the space is empty then do nothing
                //moves -> push_back(buildPositionString(file +i, rank));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[2] = true;
            }
            else if (squareStatus == INVALID) {
                stopFlag[2] = true; // Stop adding moves to the right
            }
            else if (squareStatus == ENEMY){
                
                ++screenNum[2]; // Add one to the count of attackers in the way of the piece
                if (screenNum[2] == 1) break; // Does this break from the local if statement, or the larger stopFlag[0] if statement?
                else if (screenNum[2] == 2 && getPieceType(file+i, rank-i) == 'K') { // if there are two attackers being hit, is the second one their king?
                    resultsList.push_back(buildPositionString(file+i, rank-i)); // It is a king!  so add the "screen" attack to the moves list
                    stopFlag[2] = true; // stop looking further for more pieces in the way
                }
                // screenedPiece = boardState[convertCoordinateToBoardIndex(file +i, rank)].getType();
                
            }
        }
        if (!stopFlag[3]) {
            
            int squareStatus = checkSquareStatus(pieceColor, file-i, rank-i); // check the space i squares to the right of the piece
            
            if (squareStatus == EMPTY){ // if the space is empty then do nothing
                //moves -> push_back(buildPositionString(file +i, rank));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[3] = true;
            }
            else if (squareStatus == INVALID) {
                stopFlag[3] = true; // Stop adding moves to the right
            }
            else if (squareStatus == ENEMY){
                
                ++screenNum[3]; // Add one to the count of attackers in the way of the piece
                if (screenNum[3] == 1) break; // Does this break from the local if statement, or the larger stopFlag[0] if statement?
                else if (screenNum[3] == 2 && getPieceType(file-i, rank-i) == 'K') { // if there are two attackers being hit, is the second one their king?
                    resultsList.push_back(buildPositionString(file-i, rank-i)); // It is a king!  so add the "screen" attack to the moves list
                    stopFlag[3] = true; // stop looking further for more pieces in the way
                }
                // screenedPiece = boardState[convertCoordinateToBoardIndex(file +i, rank)].getType();
                
            }
        }
    }
    
    return;
}

void queenScreenMoves(int file, int rank, std::list<std::string>& movesHolder){
    movesHolder.clear(); // Make sure to give a vector you don't care about
    
    int pieceColor = getPieceColor(file,rank);
    
    bool stopFlag[8] = {false, false, false, false, false, false, false, false};
    int screenNum[8] = {0,0,0,0,0,0,0,0};
    
    for ( int i = 1; i < 8; i++) {
        if (!stopFlag[0]) { // if we can keep adding move distance to the right of a piece
            
            int squareStatus = checkSquareStatus(pieceColor, file +i, rank+i); // check the space i squares to the right of the piece
            
            if (squareStatus == EMPTY){ // if the space is empty then do nothing
                //moves -> push_back(buildPositionString(file +i, rank));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[0] = true;
            }
            else if (squareStatus == INVALID) {
                stopFlag[0] = true; // Stop adding moves to the right
            }
            else if (squareStatus == ENEMY){
                
                ++screenNum[0]; // Add one to the count of attackers in the way of the piece
                if (screenNum[0] == 1) break; // Does this break from the local if statement, or the larger stopFlag[0] if statement?
                else if (screenNum[0] == 2 && getPieceType(file+i, rank+i) == 'K') { // if there are two attackers being hit, is the second one their king?
                    movesHolder.push_back(buildPositionString(file +i, rank+i)); // It is a king!  so add the "screen" attack to the moves list
                    stopFlag[0] = true; // stop looking further for more pieces in the way
                }
               // screenedPiece = boardState[convertCoordinateToBoardIndex(file +i, rank)].getType();
                
            }
        }
        if (!stopFlag[1]) {
            
            int squareStatus = checkSquareStatus(pieceColor, file -i, rank+i); // check the space i squares to the right of the piece
            
            if (squareStatus == EMPTY){ // if the space is empty then do nothing
                //moves -> push_back(buildPositionString(file +i, rank));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[1] = true;
            }
            else if (squareStatus == INVALID) {
                stopFlag[1] = true; // Stop adding moves to the right
            }
            else if (squareStatus == ENEMY){
                
                ++screenNum[1]; // Add one to the count of attackers in the way of the piece
                if (screenNum[1] == 1) break; // Does this break from the local if statement, or the larger stopFlag[0] if statement?
                else if (screenNum[1] == 2 && getPieceType(file-i, rank+i) == 'K') { // if there are two attackers being hit, is the second one their king?
                    movesHolder.push_back(buildPositionString(file -i, rank+i)); // It is a king!  so add the "screen" attack to the moves list
                    stopFlag[1] = true; // stop looking further for more pieces in the way
                }
               // screenedPiece = boardState[convertCoordinateToBoardIndex(file +i, rank)].getType();
                
            }
        }
        if (!stopFlag[2]) {
            
            int squareStatus = checkSquareStatus(pieceColor, file+i, rank-i); // check the space i squares to the right of the piece
            
            if (squareStatus == EMPTY){ // if the space is empty then do nothing
                //moves -> push_back(buildPositionString(file +i, rank));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[2] = true;
            }
            else if (squareStatus == INVALID) {
                stopFlag[2] = true; // Stop adding moves to the right
            }
            else if (squareStatus == ENEMY){
                
                ++screenNum[2]; // Add one to the count of attackers in the way of the piece
                if (screenNum[2] == 1) break; // Does this break from the local if statement, or the larger stopFlag[0] if statement?
                else if (screenNum[2] == 2 && getPieceType(file+i, rank-i) == 'K') { // if there are two attackers being hit, is the second one their king?
                    movesHolder.push_back(buildPositionString(file+i, rank-i)); // It is a king!  so add the "screen" attack to the moves list
                    stopFlag[2] = true; // stop looking further for more pieces in the way
                }
               // screenedPiece = boardState[convertCoordinateToBoardIndex(file +i, rank)].getType();
                
            }
        }
        if (!stopFlag[3]) {
            
            int squareStatus = checkSquareStatus(pieceColor, file-i, rank-i); // check the space i squares to the right of the piece
            
            if (squareStatus == EMPTY){ // if the space is empty then do nothing
                //moves -> push_back(buildPositionString(file +i, rank));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[3] = true;
            }
            else if (squareStatus == INVALID) {
                stopFlag[3] = true; // Stop adding moves to the right
            }
            else if (squareStatus == ENEMY){
                
                ++screenNum[3]; // Add one to the count of attackers in the way of the piece
                if (screenNum[3] == 1) break; // Does this break from the local if statement, or the larger stopFlag[0] if statement?
                else if (screenNum[3] == 2 && getPieceType(file-i, rank-i) == 'K') { // if there are two attackers being hit, is the second one their king?
                    movesHolder.push_back(buildPositionString(file-i, rank-i)); // It is a king!  so add the "screen" attack to the moves list
                    stopFlag[3] = true; // stop looking further for more pieces in the way
                }
               // screenedPiece = boardState[convertCoordinateToBoardIndex(file +i, rank)].getType();
                
            }
        }
        if (!stopFlag[4]) { // if we can keep adding move distance to the right of a piece
            
            int squareStatus = checkSquareStatus(pieceColor, file +i, rank); // check the space i squares to the right of the piece
            
            if (squareStatus == EMPTY){ // if the space is empty then do nothing
                //moves -> push_back(buildPositionString(file +i, rank));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[4] = true;
            }
            else if (squareStatus == INVALID) {
                stopFlag[4] = true; // Stop adding moves to the right
            }
            else if (squareStatus == ENEMY){
                
                ++screenNum[4]; // Add one to the count of attackers in the way of the piece
                if (screenNum[4] == 1) break; // Does this break from the local if statement, or the larger stopFlag[0] if statement?
                else if (screenNum[4] == 2 && getPieceType(file+i, rank) == 'K') { // if there are two attackers being hit, is the second one their king?
                    movesHolder.push_back(buildPositionString(file +i, rank)); // It is a king!  so add the "screen" attack to the moves list
                    stopFlag[4] = true; // stop looking further for more pieces in the way
                }
               // screenedPiece = boardState[convertCoordinateToBoardIndex(file +i, rank)].getType();
                
            }
        }
        if (!stopFlag[5]) {
            
            int squareStatus = checkSquareStatus(pieceColor, file -i, rank); // check the space i squares to the right of the piece
            
            if (squareStatus == EMPTY){ // if the space is empty then do nothing
                //moves -> push_back(buildPositionString(file +i, rank));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[5] = true;
            }
            else if (squareStatus == INVALID) {
                stopFlag[5] = true; // Stop adding moves to the right
            }
            else if (squareStatus == ENEMY){
                
                ++screenNum[5]; // Add one to the count of attackers in the way of the piece
                if (screenNum[5] == 1) break; // Does this break from the local if statement, or the larger stopFlag[0] if statement?
                else if (screenNum[5] == 2 && getPieceType(file-i, rank) == 'K') { // if there are two attackers being hit, is the second one their king?
                    movesHolder.push_back(buildPositionString(file -i, rank)); // It is a king!  so add the "screen" attack to the moves list
                    stopFlag[5] = true; // stop looking further for more pieces in the way
                }
               // screenedPiece = boardState[convertCoordinateToBoardIndex(file +i, rank)].getType();
                
            }
        }
        if (!stopFlag[6]) {
            
            int squareStatus = checkSquareStatus(pieceColor, file, rank+i); // check the space i squares to the right of the piece
            
            if (squareStatus == EMPTY){ // if the space is empty then do nothing
                //moves -> push_back(buildPositionString(file +i, rank));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[6] = true;
            }
            else if (squareStatus == INVALID) {
                stopFlag[6] = true; // Stop adding moves to the right
            }
            else if (squareStatus == ENEMY){
                
                ++screenNum[6]; // Add one to the count of attackers in the way of the piece
                if (screenNum[6] == 1) break; // Does this break from the local if statement, or the larger stopFlag[0] if statement?
                else if (screenNum[6] == 2 && getPieceType(file, rank+i) == 'K') { // if there are two attackers being hit, is the second one their king?
                    movesHolder.push_back(buildPositionString(file, rank+i)); // It is a king!  so add the "screen" attack to the moves list
                    stopFlag[6] = true; // stop looking further for more pieces in the way
                }
               // screenedPiece = boardState[convertCoordinateToBoardIndex(file +i, rank)].getType();
                
            }
        }
        if (!stopFlag[7]) {
            
            int squareStatus = checkSquareStatus(pieceColor, file, rank-i); // check the space i squares to the right of the piece
            
            if (squareStatus == EMPTY){ // if the space is empty then do nothing
                //moves -> push_back(buildPositionString(file +i, rank));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[7] = true;
            }
            else if (squareStatus == INVALID) {
                stopFlag[7] = true; // Stop adding moves to the right
            }
            else if (squareStatus == ENEMY){
                
                ++screenNum[7]; // Add one to the count of attackers in the way of the piece
                if (screenNum[7] == 1) break; // Does this break from the local if statement, or the larger stopFlag[0] if statement?
                else if (screenNum[7] == 2 && getPieceType(file, rank-i) == 'K') { // if there are two attackers being hit, is the second one their king?
                    movesHolder.push_back(buildPositionString(file, rank-i)); // It is a king!  so add the "screen" attack to the moves list
                    stopFlag[7] = true; // stop looking further for more pieces in the way
                }
               // screenedPiece = boardState[convertCoordinateToBoardIndex(file +i, rank)].getType();
                
            }
        }
    }
    
    return;
}

void getScreenMoves(int pieceFile, int pieceRank, std::list<std::string>& resultsList){
    
    char pieceType = getPieceType(pieceFile, pieceRank);
    
    switch (pieceType) { // check the type of piece being calculated for and runs its move calc function with results contained in movesHolder
        case 'R': rookScreenMoves(pieceFile, pieceRank, resultsList); return;
        case 'B': bishopScreenMoves(pieceFile, pieceRank, resultsList); return;
        case 'Q': queenScreenMoves(pieceFile, pieceRank, resultsList); return;
        case 'P': return;
        case 'N': return;
        case 'K': return;
        case 'E': return;
        default: cout << "Passed bad piece type to function getScreenMoves()"; return;
            
    }
    
}


void calcScreenMovesets(){ // The pointers in this are very obscure, but I might have done them right, fingers crossed
    
    //cout << "Calculating moveset for player: " << playerColor << endl;
    
    // Piece kingPiece = *findPiece(playerColor, 'K');  // commented this out because think i dont need it
    
    m_whiteScreenMoves.clear();
    m_blackScreenMoves.clear();
    typedef enum {BLACK,WHITE} Colors;
    std::list<int8_t> *pieceIndicesList;
    std::list<std::string> *pieceScreenMoveHolder;

    for (int color = BLACK; color <WHITE+1; color++){
        if (color == BLACK){
            pieceIndicesList = m_blackPieceIndices;
            pieceScreenMoveHolder = m_blackScreenMoves;
        }
        else if (color == WHITE){
            pieceIndicesList = m_whitePieceIndices;
            pieceScreenMoveHolder = m_whiteScreenMoves;
        }
        else {std::cout<< "Error with for loop or if statement in calcScreenMovesets()" << std::endl; return;}
        
        for(auto it = pieceIndicesList.begin(); it != pieceIndicesList.end(); it++){ // go through all the white pieces
            int8_t tempPiece = m_board[*it];
            int8_t index = *it;
            int8_t tempPieceRank = index/8;
            int8_t tempPieceFile = index%8;
            
            // get the moves for white pieces on the board that are "screen" attacks  (pinning attacks)
            std::list<std::string> partialScreenMoveset;
            getScreenMoves(tempPieceFile, tempPieceIndex, partialScreenMoveset);
            pieceScreenMoveHolder.insert(pieceScreenMoveHolder.end(), partialScreenMoveset->begin(), partialScreenMoveset->end()); // add them all up
        }
    }
    
//    for(auto it = m_blackPieceIndices.begin(); it != m_blackPieceIndices.end(); it++){ // do the same for black
//        int8_t tempPiece = m_board[*it];
//        int8_t index = *it;
//        int8_t tempPieceRank = index/8;
//        int8_t tempPieceFile = index%8;
//
//        // get the piece moves from the pieces that are on the board that are your color
//        std::list<std::string> partialScreenMoveset;
//        getScreenMoves(tempPieceFile, tempPieceIndex, partialScreenMoveset);
//        m_blackScreenMoves.insert(m_blackScreenMoves.end(), partialScreenMoveset->begin(),partialScreenMoveset->end());
//    }
    
}

static int countNumAttackers(list<string> playerMoveset, int forFile, int forRank){
    int attackerCount = 0;
    for (auto it = playerMoveset.begin(); it != playerMoveset.end(); it++) {
        string move = *it;
        int file = Piece::cFileToIndex(move[3]);
        int rank = move[4] - '0'-1; // converts from one char within a string to an int, and decrements from rank number to array index num
        
        if (file == forFile && rank == forRank) {
            attackerCount++;
        }
    }
    return attackerCount;
}

static void getAttackingMoves(list<string> playerMoves, int forFile, int forRank, list<string>* attackingMoves){
    for (auto it = playerMoves.begin(); it != playerMoves.end(); it++) {
        string simple = it->c_str();
        int file = Piece::cFileToIndex(simple[3]);
        int rank = simple[4] - '0'-1;
        
        if (file == forFile && rank == forRank) {
            attackingMoves->push_back(simple);
        }
    }
}

static void makeColinearSquaresVector(std::string pieceMove, std::list<std::string>* pColinearSquaresResultVector){
    pColinearSquaresResultVector->clear();
    char pieceType;
    char cPrevFile;
    int prevRank;
    char cNewFile;
    int newRank;
    
    parseMove(pieceMove, pieceType, cPrevFile, prevRank, cNewFile, newRank);
    
    if (pieceType == 'N') {
        cout << "Cannot make colinear squares vector from knight moves" << endl;
        return;
    }
    
    
    int8_t fileDiff = cNewFile - cPrevFile;
    int8_t rankDiff = newRank - prevRank; // gets difference between squares
    int8_t fileDirection = 0;
    int8_t rankDirection = 0;
    int8_t loopIterations = 0;
    
    if (fileDiff != 0) {
        fileDirection = fileDiff/abs(fileDiff);
        loopIterations = abs(fileDiff);
    }
    if (rankDiff != 0) {
        rankDirection = rankDiff/abs(rankDiff); // if rankDiff is negative/positive, it increments negative/positive respectively
        loopIterations = abs(rankDiff);
    }
    
    
    
    for (int i = 0; i < loopIterations; i++) {
        
        char colinearSquareFile = static_cast<char>(cPrevFile + i*fileDirection);
        int colinearSquareRank = prevRank + i*rankDirection;
        
        string sColinearSquareFile (1,colinearSquareFile);
        string sColinearSquareRank = to_string(colinearSquareRank);
        
        pColinearSquaresResultVector->push_back(sColinearSquareFile+sColinearSquareRank);
        
        
    }
    
    cout << "Colinear squares created" << endl;
    
} // Not as useful when you just use squareIsBetweenSquares in order to check for getting between two pieces. Still, using this might be faster with storage.

static bool squareIsBetweenSquares(string move, int testSquareFile, int testSquareRank){
    
    char pieceType, prevFile, newFile;
    int prevRank, newRank;
    
    parseMove(move, pieceType, prevFile, prevRank, newFile, newRank);
    
    if (pieceType == 'N') {
        return false;
    }
    
    
    int square2File = newFile - 'A';
    int square2Rank = newRank;
    int square1File = prevFile - 'A';
    int square1Rank = prevRank;
    
    int x1 = 0, x2=0, y1=0, y2=0;
    
    int y, x;
    
    x = testSquareFile;
    y = testSquareRank;
    
    if (square1Rank >= square2Rank) {
        y2 = square1Rank;
        x2 = square1File;
        y1 = square2Rank;
        x1 = square2File;
    }
    else if (square2Rank > square1Rank){
        y2 = square2Rank;
        x2 = square2File;
        y1 = square1Rank;
        x1 = square1File;
    }
    
    // if it's a vertical line, if the file is the same, and the square is inbetween or on the endpoints, return true
    if (x2 == x1) {
        if (x == x1 && y1 <= y && y <= y2) {
            return true;
        }
        else {
            return false;
        }
    }
    // if it's a horizontal line, if the test rank is the same, and the square is inbetween or on the endpoints, return true
    if (y2 == y1) {
        if (y == y1) {
            if (x1 <= x2) {
                if (x1 <= x && x <= x2) {
                    return true;
                }
                else {
                    return false;
                }
            }
            else if (x2<=x1){
                if (x2<=x && x <= x1) {
                    return true;
                }
                else {
                    return false;
                }
            }
        }
        else {
            return false;
        }
    }
    
    float slope = (y2-y1)/(x2-x1); // calculate slope of line  (the only valid slope is 1 or -1)
    
    int b = y1-slope*x1; // calculate the y intercept
    
    if (y == slope*x+b && y1 <= y && y <= y2) { // if the point is on the line and the y value is on or between endpoints, return true
        return true;
    }
    
    return false; // all other cases the test square isn't on the line between the pieces
    
    
}

static void getSquaresBetweenSquares(string move, list<string>* returnVectorPointer){
    
    char pieceType, prevFile, newFile;
    int prevRank, newRank;
    
    parseMove(move, pieceType, prevFile, prevRank, newFile, newRank);
    returnVectorPointer->clear();
    
    
    
    int square2File = newFile - 'A';
    int square2Rank = newRank;
    int square1File = prevFile - 'A';
    int square1Rank = prevRank;
    
    int x1 = 0, x2=0, y1=0, y2=0;
    
    if (square1Rank >= square2Rank) {
        y2 = square1Rank;
        y1 = square2Rank;
    }
    else if (square2Rank > square1Rank){
        y2 = square2Rank;
        y1 = square1Rank;
    }
    
    if (square1File >= square2File) {
        x2 = square1File;
        x1 = square2File;
    }
    else if (square2File > square1File){
        x2 = square2File;
        x1 = square1File;
    }
    
    // if it's a vertical line, if the file is the same, and the square is inbetween or on the endpoints, return true
    if (x2 == x1) {
        for (int i = 0; i < y2-y1; i++) {
            char y, x;
            y = static_cast<char>(y1+i);
            x = static_cast<char>(x1);
            string sX(1, x);
            string sY(1,y);
            
            returnVectorPointer->push_back(sX+sY); // Dereference the data storage location of variable and add a square to the list
        }
        return;
    }
    // if it's a horizontal line, if the test rank is the same, and the square is inbetween or on the endpoints, return true
    if (y2 == y1) {
        for (int i = 0; i < x2-x1; i++) {
            char y, x;
            y = static_cast<char>(y1);
            x = static_cast<char>(x1+i);
            string sX(1, x);
            string sY(1,y);
            
            returnVectorPointer->push_back(sX+sY);
        }
        return;
    }
    
    float slope = (y2-y1)/(x2-x1); // calculate slope of line  (the only valid slope is 1 or -1)
    //test the slope
    if (abs(slope) != 1) {
        std::cout << "two non linear squares given, invalid inputs" << endl;
    }
    
    for (int i = 0; i < x2-x1; i++) {
        char y, x;
        y = static_cast<char>(y1+i);
        x = static_cast<char>(x1+i);
        string sX(1, x);
        string sY(1,y);
        
        returnVectorPointer->push_back(sX+sY);
    }
    return;
    
}

void calcPlayerMovesetV2(Colors playerColor, int currentTurn, bool validateMoveset){
    
    //This function is called recursively, so we need to clear it when we first start calculation, but not on recursive calls.
    if(validateMoveset){
    m_whiteMoves.clear();
    m_blackMoves.clear();
    }
    
    std::list<Piece>* pPlayerPieceIndices = m_whitePieceIndices;
    std::list<std::string>* completePlayerMoveset;
    std::list<std::string>* enemyMoveset;
    std::list<std::string>* enemyScreenMoveset;
    string enemyColor;
    bool kingHasMoved;
    bool aRookHasMoved;
    bool hRookHasMoved;
    bool addCastling = false;
    
    if (playerColor == WHITE) {
        completePlayerMoveset = &m_whiteMoves;
        enemyMoveset = &m_blackMoves;
        enemyScreenMoveset = &m_blackScreenMoves;
        enemyColor = BLACK;
        kingHasMoved = m_whiteKingMoved
        aRookHasMoved = m_whiteARookMoved;
        hRookHasMoved = m_whiteHRookMoved;
        
    }
    else if (playerColor == BLACK){
        completePlayerMoveset = &m_blackMoves;
        enemyMoveset = &m_whiteMoves;
        enemyScreenMoveset = &m_whiteScreenMoves;
        enemyColor = WHITE;
        kingHasMoved = m_blackKingMoved
        aRookHasMoved = m_blackARookMoved;
        hRookHasMoved = m_blackHRookMoved;
    }
    else return;
    
    
    calcScreenMovesets(); // calculate all the screening attacks (attacks that hit the king through another piece). Can optimize by making it color specific
    
    // find king, calculate checks on king, calculate screen attackers on king
    
    int8_t kingIndex = findKingIndex(playerColor);  // might have issue with this if comp is simulating taking enemy king
    int8_t kingFile = kingIndex%8;
    int8_t kingRank = kingIndex/8;
    
    if (kingPieceIndex == -1) {
        SDL_Delay(1);
        return;
    }
    
    vector<string> attackingKingMoves;
    vector<string> apparentScreenAttackingKingMoves;
    vector<string> screenAttackingKingMoves;
    vector<string> kingUnfilteredMoveset;
    vector<string> kingFilteredMoveset;
    vector<string> castlingMoveset;
    int checksOnKing = 0;
    
    if (!validateMoveset) { // if we don't need to apply normal move rules with regards to the king then just get all the unabridged moves
        for (auto it = pPlayerPieceIndices->begin(); it != pPlayerPieceIndices->end(); it++) {
            Piece tempPieceIndex = *it;
            int8_t rank = tempPieceIndex/8;
            int8_t file = tempPieceIndex%8;
            char tempPieceType = getPieceType(file, rank);
            if (tempPieceType != 'K') {
                std::list<std::string> partialMoveset;
                calcPieceMoves(file, rank, partialMoveset);
                completePlayerMoveset->insert(completePlayerMoveset->end(), partialMoveset->begin(),partialMoveset->end());
            }
        }
        return;
    }
    
    calcPlayerMovesetV2(enemyColor, currentTurn, false); // calculate all the possible places the opponents pieces can move at the moment
    
    getAttackingMoves(*enemyMoveset, kingFile, kingRank, &attackingKingMoves); // get all the moves that attack the king
    getAttackingMoves(*enemyScreenMoveset, kingFile, kingRank, &screenAttackingKingMoves); // get all the screen moves on the king
    
    checksOnKing = attackingKingMoves.size();  // find the number of attacks on king
    
    
    if (checksOnKing == 2){ // If there are two pieces attacking the king, the king must move to survive -  shortest: linear time
        // only get king moves and don't do moveset creation below
        
        std::list<std::string> kingUnfilteredMoveset;
        calcKingMoves(kingFile, kingRank, &kingUnfilteredMoveset);
        for (auto it = kingUnfilteredMoveset.begin(); it != kingUnfilteredMoveset.end();) {
            string move = *it;
            
            char pieceType, prevFile, newFile;
            int prevRank, newRank;
            
            parseMove(move, pieceType, prevFile, prevRank, newFile, newRank);
            
            COME_BACK_TO_ME_AND_FIX!!!;
            if (countNumAttackers(*enemyMoveset, static_cast<int>(newFile-'A'), newRank) != 0) {// CAN IMPROVE SPEED BY GETTING THE MOVES AND DOING COMPARISON
                kingUnfilteredMoveset.erase(it); // The enemy is attacking our observed square.
            }
            else {
                ++it;
            }
            
        }
        kingFilteredMoveset = kingUnfilteredMoveset; // create an updated variable
        completePlayerMoveset->insert(completePlayerMoveset->end(), kingFilteredMoveset.begin(),kingFilteredMoveset.end());
        
        return; // Return because the only viable moves are ones where the king moves
        
    } // if there are two attacks on the king, the king must move
    for(auto it = pPlayerPieceIndices->begin(); it != pPlayerPieceIndices->end(); it++){
        int8_t tempPieceIndex = **it;
        int tempFile = tempPieceIndex%8;
        int tempRank = tempPieceIndex/8;
        Piece tempPieceType = getPieceType(tempFile, tempRank);
        
        if (tempPieceType != 'K'){ // if a piece is not the king
            if (!screenAttackingKingMoves.empty()) { // if there is still an unremoved pinning move
                std::list<std::string> partialMoveset;
                calcPieceMoves(tempFile, tempRank, &partialMoveset);
                
                for (auto it = screenAttackingKingMoves.begin(); it != screenAttackingKingMoves.end();) { // for each of the pinning moves
                    string move = *it;
                    
                    if (squareIsBetweenSquares(move, tempPiece.getFile(), tempPiece.getRank())) { // if the piece is the one being pinned (in front of king)
                        for (auto it1 = partialMoveset->begin(); it1 != partialMoveset->end();) { // for each of the moves in the single piece's moveset
                            auto possibleMove = *it1;
                            int testFile = possibleMove[3]-'A';
                            int testRank = possibleMove[4]-'1';
                            if (!squareIsBetweenSquares(move, testFile, testRank)) { // if the move is not between the king and the attacker
                                partialMoveset->erase(it1); // erase the possible move from the vector as invalid
                            }
                            else {
                                ++it1;
                            }
                        }
                        screenAttackingKingMoves.erase(it); // erase the pinning move from the pinning move moveset because we've found the piece being pinned and have modified its moveset to account for being pinned
                    }
                    else {
                        ++it;
                    }
                }
                
                completePlayerMoveset->insert(completePlayerMoveset->end(), partialMoveset->begin(),partialMoveset->end()); // add the filtered moveset to the complete moveset
            }
            else { // add one of the regular unpinned movesets from pieces
                
                std::list<std::string> partialMoveset;
                calcPieceMoves(tempFile, tempRank, &partialMoveset);
                completePlayerMoveset->insert(completePlayerMoveset->end(), partialMoveset->begin(),partialMoveset->end());
                
                //cout << endl;
            }
        }
        else if (tempPieceType == 'K') { // add king's viable moves depending on what would put it in check
            std::list<std::string> kingUnfilteredMoveset;
            calcPieceMoves(tempFile, tempRank, &kingUnfilteredMoveset);;
            for (auto it = kingUnfilteredMoveset.begin(); it != kingUnfilteredMoveset.end();) {
                string move = *it;
                
                char pieceType, prevFile, newFile;
                int prevRank, newRank;
                
                parseMove(move, pieceType, prevFile, prevRank, newFile, newRank);
                
                if (countNumAttackers(*enemyMoveset, static_cast<int>(newFile-'A'), newRank) != 0) {// CAN IMPROVE SPEED BY GETTING THE MOVES AND DOING COMPARISON
                    kingUnfilteredMoveset.erase(it);
                }
                else {
                    ++it;
                }
                
            }
            
            kingFilteredMoveset = kingUnfilteredMoveset; // create an updated variable
            completePlayerMoveset->insert(completePlayerMoveset->end(), kingFilteredMoveset.begin(),kingFilteredMoveset.end()); // add filtered moveset to complete moveset
            
            // INSERT HERE
            //Add castling to the move choices if it is viable
            if (tempPieceType == 'K' && kingHasMoved == false) { // if the king hasn't moved
                bool check = false;
                castlingMoveset.clear();
                int attackerNum = countNumAttackers(*enemyMoveset, tempFile, tempRank);
                if (attackerNum != 0) {check = true;}
                
                //All conditions for castling
                
                //Mutual:
                bool notInCheck = !check;
                kingHasMoved;
                
                // Kingside:
                
                hRookHasMoved;
                int pieceTypeOnSameRankPlus1FILE = checkSquareStatus(playerColor, tempFile+1, tempRank);
                int pieceTypeOnSameRankPlus2FILE = checkSquareStatus(playerColor, tempFile+2, tempRank);
                int numAttackersOnSameRankPlus1FILE = countNumAttackers(*enemyMoveset, tempFile+1, tempRank);
                int numAttackersOnSameRankPlus2FILE = countNumAttackers(*enemyMoveset, tempFile+2, tempRank);
                
                // Queenside:
                aRookHasMoved;
                int pieceTypeOnSameRankMinus1FILE = checkSquareStatus(playerColor, tempFile-1, tempRank);
                int pieceTypeOnSameRankMinus2FILE = checkSquareStatus(playerColor, tempFile-2, tempRank);
                int pieceTypeOnSameRankMinus3FILE = checkSquareStatus(playerColor, tempFile-3, tempRank);
                int numAttackersOnSameRankMinus1FILE = countNumAttackers(*enemyMoveset, tempFile-1, tempRank);
                int numAttackersOnSameRankMinus2FILE = countNumAttackers(*enemyMoveset, tempFile-2, tempRank);
                
                
                
                if (notInCheck && pieceTypeOnSameRankPlus1FILE == EMPTY && pieceTypeOnSameRankPlus2FILE == EMPTY && !kingHasMoved && !hRookHasMoved && numAttackersOnSameRankPlus1FILE == 0 && numAttackersOnSameRankPlus2FILE == 0){
                    
                    castlingMoveset.push_back("O-O");
                    addCastling = true;
                    //std::cout << "Player may castle king-side" << endl;
                }
                if (notInCheck && pieceTypeOnSameRankMinus1FILE == EMPTY && pieceTypeOnSameRankMinus2FILE == EMPTY && pieceTypeOnSameRankMinus3FILE == EMPTY && !kingHasMoved && !aRookHasMoved && numAttackersOnSameRankMinus1FILE == 0 && numAttackersOnSameRankMinus2FILE == 0) {
                    castlingMoveset.push_back("O-O-O");
                    addCastling = true;
                    //std::cout << "Player may castle queen-side" << endl;
                }
            } // add castling if viable
            
        }
    }
    
    if (checksOnKing == 1) { // if there is only one attacker on the king then
        
        for (auto pMove = completePlayerMoveset->begin(); pMove != completePlayerMoveset->end();) {
            string move = *pMove;
            char pieceType = move[0];
            int moveEndFile = move[3] - 'A';
            int moveEndRank = move[4] - '1';
            char pieceAttackingKingType = attackingKingMoves.first[0]; // get the attacking enemy piece type
            if (pieceType == 'K'){ // IF WE'RE LOOKING AT KING MOVES, THEN IT CAN MOVE OUT OF CHECK, or capture the attacker. We've already filtered the king moves for legality, so all the king moves are okay.
                ++pMove; // this move is fine, move on to the next move in the list
            }
            else if (pieceAttackingKingType == 'N') { // if the attacker is a knight
                int knightAttackingKingFile = attackingKingMoves.first[1]-'A'; // attackingKingMoves is a string vector of moves
                int knightAttackingKingRank = attackingKingMoves.first[2]-'1';
                if (moveEndRank == knightAttackingKingRank && moveEndFile == knightAttackingKingFile) { // if we can capture the knight
                    ++pMove; // then keep the move as valid move, because we are removing the knight threat
                }
                else{ // if we can't capture the knight
                    completePlayerMoveset->erase(pMove); // the move won't get us out of check. Erase it.
                }
            }
            else if (!squareIsBetweenSquares(attackingKingMoves.first, moveEndFile, moveEndRank)) { // if the piece is not a knight and your possible move cannot get between the attacker or capture it
                completePlayerMoveset->erase(pMove); // throw out the move
            }
            else {
                ++pMove; // The move goes between king and the attacking piece, blocking the check. So leave it in the moveset
            }
            
            
        }
        
        
        
    }
    // Writes out list of moves:
    //cout << "Moveset List for " << playerColor << endl;
    //for (auto it = completePlayerMoveset->begin(); it != completePlayerMoveset->end(); it++) {
    //    cout << *it << ", ";
    //}
    //cout << endl;
    
    if (addCastling) {
        completePlayerMoveset->insert(completePlayerMoveset->end(), castlingMoveset.begin(),castlingMoveset.end()); // add filtered moveset to complete moveset
    }
    
    
    
    
    //Check the moves and remove invalid ones here
    // to know if move is invalid
} // Fast move calculation


// Public functions excluding constructor and destructor
void getBoardState(Pieces (&board)[8][8]){
    board = m_board;
    return;
}
bool makeMove(std::string move){
    
}
bool editBoard(int file, int rank, Pieces newPiece){};
void undoLastMove(){};
Colors getLegalMoves(std::list<std::string> &legalMoves){}; // function will be called A LOT, should look into how to get read only access to the list of player moves. Because I don't want to allow the function to accidentally modify the list, and I don't want to keep copying the entire string of moves to playerMoves list
bool isWinner(Colors winnerColor){}; // Reports if white or black is the winner when asked
bool makeMove(std::string move){

    int prevFile = 0;
    int prevRank = 0;
    int newFile = 0;
    int newRank = 0;
    char pieceType;
    int prevIndex = 0;
    int newIndex = 0;
    char promotionChoice = 'P';
    bool promotePawn = false;
    int prevKingIndex = 0;
    int newKingIndex = 0;
    int prevRookIndex = 0;
    int newRookIndex = 0;
    
    if ((move.size() != 5)||(move.size() != 6) {
        cout << "Function passed bad move" << endl;
        return;
    }
    
    if (!(move == "O-O" || move == "O-O-O")) {
        
        TextBoard::parseMove( move, pieceType, prevFile, prevRank, newFile, newRank);
        
        if (move.size() == 6) {
            promotionChoice = move[5];
            promotePawn = true;
        }
        
        // ################### modifies the board with the move the player chose #########################
        
        Piece destination = m_board[newRank][newFile];
        int destinationColor = getPieceColor(newFile, newRow);
        char destinationType = getPieceType(newFile, newRank);
        prevIndex = Piece::convertCoordinateToBoardIndex(prevFile, prevRank);
        newIndex = Piece::convertCoordinateToBoardIndex(newFile, newRank);
        
        if (destinationColor == EMPTY && destinationType == 'P' && abs(newFile-prevFile) == 1) {
            int enPassantedPieceIndex = Piece::convertCoordinateToBoardIndex(newFile, prevRank);
            boardState[enPassantedPieceIndex].setColor("Empty");
            boardState[enPassantedPieceIndex].setType('E');
        }
        
        boardState[newIndex].setColor(boardState[prevIndex].getColor());
        if (!promotePawn) {
            boardState[newIndex].setType(boardState[prevIndex].getType());
        }
        else {
            boardState[newIndex].setType(promotionChoice);
        }
        boardState[prevIndex].setColor("Empty");
        boardState[prevIndex].setType('E');
        // SETTING ENPASSANT FLAG HERE
        if (boardState[newIndex].getType() == 'P' && (abs(newRank - prevRank) == 2)) {
            boardState[newIndex].setDoubleJumpTurn(turnCounter);
        }
        
        boardState[prevIndex].Moved();
        
    }
    else if (move == "O-O"){
        
        // finds the king on the board
        auto kingPiece = *board->findKing(m_player_color);
        
        // makes the move the player chose on this hypothetical board
        prevKingIndex = Piece::convertCoordinateToBoardIndex(kingPiece.getFile(), kingPiece.getRank());
        newKingIndex = Piece::convertCoordinateToBoardIndex(kingPiece.getFile()+2, kingPiece.getRank());
        prevRookIndex = Piece::convertCoordinateToBoardIndex(kingPiece.getFile()+3, kingPiece.getRank());
        newRookIndex = newKingIndex-1;
        
        boardState[newKingIndex].setColor(boardState[prevKingIndex].getColor());
        boardState[newKingIndex].setType(boardState[prevKingIndex].getType());
        boardState[prevKingIndex].setColor("Empty");
        boardState[prevKingIndex].setType('E');
        boardState[prevKingIndex].Moved();
        
        boardState[newRookIndex].setColor(boardState[prevRookIndex].getColor());
        boardState[newRookIndex].setType(boardState[prevRookIndex].getType());
        boardState[prevRookIndex].setColor("Empty");
        boardState[prevRookIndex].setType('E');
        boardState[prevRookIndex].Moved();
        
    }
    else if (move == "O-O-O"){
        // Check conditions for the player to castle Queenside
        
        // finds the king on the board
        auto kingPiece = *board->findKing(m_player_color);
        
        // makes the move the player chose on this hypothetical board
        prevKingIndex = Piece::convertCoordinateToBoardIndex(kingPiece.getFile(), kingPiece.getRank());
        newKingIndex = Piece::convertCoordinateToBoardIndex(kingPiece.getFile()-2, kingPiece.getRank());
        prevRookIndex = Piece::convertCoordinateToBoardIndex(kingPiece.getFile()-4, kingPiece.getRank());
        newRookIndex = newKingIndex+1;
        
        boardState[newKingIndex].setColor(boardState[prevKingIndex].getColor());
        boardState[newKingIndex].setType(boardState[prevKingIndex].getType());
        boardState[prevKingIndex].setColor("Empty");
        boardState[prevKingIndex].setType('E');
        boardState[prevKingIndex].Moved();
        
        boardState[newRookIndex].setColor(boardState[prevRookIndex].getColor());
        boardState[newRookIndex].setType(boardState[prevRookIndex].getType());
        boardState[prevRookIndex].setColor("Empty");
        boardState[prevRookIndex].setType('E');
        boardState[prevRookIndex].Moved();
        
    }
    
    board->findPlayerPieces(); // after making the move, update the lists containing the locations of all player pieces
    
}





}
