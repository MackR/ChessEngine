//
//  TextBoard.hpp
//  Chess
//
//  Created by Mack Ragland on 10/12/22.
//  Copyright © 2022 KattisPracticeProblems. All rights reserved.
//

#ifndef TextBoard_hpp
#define TextBoard_hpp

#include <stdio.h>
#include <stack>
#include <list>

// In each space we should have text with "(Color)(Piece)" - and an EE if empty
// Class member variables will be: lastMove (use to calculate enpassant), blackKingMoved, whiteKingMoved, whiteARookMoved, whiteHRookMoved, blackARookMoved, blackHRookMoved, includeEnpassant,

// Could possibly encode the color and number into a single number using ENUMS, and then create a decode function



class TextBoard{
    
private:
    
    typedef enum {
        A1,B1,C1,D1,E1,F1,G1,H1,
        A2,B2,C2,D2,E2,F2,G2,H2,
        A3,B3,C3,D3,E3,F3,G3,H3,
        A4,B4,C4,D4,E4,F4,G4,H4,
        A5,B5,C5,D5,E5,F5,G5,H5,
        A6,B6,C6,D6,E6,F6,G6,H6,
        A7,B7,C7,D7,E7,F7,G7,H7,
        A8,B8,C8,D8,E8,F8,G8,H8
    } Squares;
    
    typedef enum {
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
        
    } Pieces; // Black pieces 1-6, White pieces 7-12
    
    typedef enum {
        BLACK,
        WHITE,
        EMPTY = -1
    } Colors;
    
    Pieces m_Board[8][8];
    u_int8_t m_turnNum;
    std::stack<std::string> m_moveHistory;
    std::stack<std::string[8][8]> m_boardHistory;
    std::list<std::int8_t> m_whitePieceIndices;
    std::list<std::int8_t> m_blackPieceIndices;
    std::list<std::string> m_whiteScreenMoves;
    std::list<std::string> m_blackScreenMoves;
    std::list<std::string> m_whiteMoves;
    std::list<std::string> m_blackMoves;
    
    bool m_whitesTurn;
    bool m_blackKingMoved, m_whiteKingMoved;
    bool m_whiteARookMoved,m_whiteHRookMoved, m_blackARookMoved, m_blackHRookMoved;
    
    
    inline bool isWhite(int8_t piece);
    inline bool isBlack(int8_t piece);
    inline bool isEmpty(int8_t piece);
    char getPieceType(int nFile, int rank, Pieces board[8][8] = m_board);
    int getPieceColor(int file, int row, Pieces board[8][8] = m_board);
    std::string buildMoveString(char cFileOld, int rankOld, char cFileNew, int rankNew);
    int checkSquareStatus(int playerColor, int nFile, int rank);
    static int cFileToIndex(char cFile);
    static char indexTo_cFile(int nFile);
    static int convertCoordinateToBoardIndex(int nFile, int rank);
    static void parseMove( string move, char& pieceType, char& prevFile, int& prevRank, char& newFile, int& newRank);
    void calcPawnMoves(int file, int rank, std::list<std::string> &movesResults);
    void calcRookMoves(int file, int rank, std::list<std::string> &movesResults);
    void calcKnightMoves(int file, int rank, std::list<std::string> &moveResults);
    void calcBishopMoves(int file, int rank, std::list<std::string> &moveResults);
    void calcQueenMoves(int file, int rank, std::list<std::string> &moveResults);
    void calcKingMoves(int file, int rank, std::list<std::string> &moveResults);
    void calcPieceMoves(int file, int rank, std::list<std::string> &resultsList);
    void findPlayerPieces();
    int8_t findKingIndex(Colors color);
    void rookScreenMoves(int file, int rank, std::list<std::string>& resultsList);
    void bishopScreenMoves(int file, int rank, std::list<std::string>& resultsList);
    void queenScreenMoves(int file, int rank, std::list<std::string>& movesHolder);
    void getScreenMoves(int pieceFile, int pieceRank, std::list<std::string>& resultsList);
    void calcScreenMovesets();
    static int countNumAttackers(list<string> playerMoveset, int forFile, int forRank);
    static void getAttackingMoves(list<string> playerMoves, int forFile, int forRank, list<string>* attackingMoves);
    static void makeColinearSquaresVector(std::string pieceMove, std::list<std::string>* pColinearSquaresResultVector);
    static bool squareIsBetweenSquares(string move, int testSquareFile, int testSquareRank);
    void calcPlayerMovesetV2(Colors playerColor, int currentTurn, bool validateMoveset);
    void getSquaresBetweenSquares(string move, vector<string>* returnVectorPointer); // Fix me!!!
    
public:
    void TextBoard();
    void ~TextBoard();
    void getBoardState(Pieces (&board)[8][8]);
    bool makeMove(std::string move);
    bool editBoard(int file, int rank, Pieces newPiece);
    void undoLastMove();
    const std::list<std::string>& const getLegalMoves(); // function will be called A LOT. I have defined it as const return and const func because this makes it so people can't modify the return reference value, and it specifies the function won't modify value
    bool isWinner(Colors winnerColor); // Reports if white or black is the winner when asked

    
    
    
    
}

#endif /* TextBoard_hpp */

