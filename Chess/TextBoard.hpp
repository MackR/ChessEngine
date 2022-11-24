//
//  TextBoard.hpp
//  Chess
//
//  Created by Mack Ragland on 10/12/22.
//  Copyright © 2022 KattisPracticeProblems. All rights reserved.
//

#ifndef TextBoard_hpp
#define TextBoard_hpp

#include "ChessConstants.h"
#include <stdio.h>
#include <iostream>
#include <stack>
#include <list>

// In each space we should have text with "(Color)(Piece)" - and an EE if empty
// Class member variables will be: lastMove (use to calculate enpassant), blackKingMoved, whiteKingMoved, whiteARookMoved, whiteHRookMoved, blackARookMoved, blackHRookMoved, includeEnpassant,

// Could possibly encode the color and number into a single number using ENUMS, and then create a decode function

namespace TextBoard{

class TextBoard{
    
private:
    
    bool m_whitesTurn;
    CONSTANTS::Piece m_board[8][8];
    u_int8_t m_turnNum;
    std::stack<std::string> m_moveHistory;
    std::stack<CONSTANTS::Piece[8][8]> m_boardHistory;
    std::list<std::int8_t> m_whitePieceIndices;
    std::list<std::int8_t> m_blackPieceIndices;
    std::list<std::string> m_whiteScreenMoves;
    std::list<std::string> m_blackScreenMoves;
    std::list<std::string> m_whiteMoves;
    std::list<std::string> m_blackMoves;
    
    CONSTANTS::Color m_playerTurn;
    bool m_blackKingMoved, m_whiteKingMoved;
    bool m_whiteARookMoved,m_whiteHRookMoved, m_blackARookMoved, m_blackHRookMoved;
    
    
    inline bool isWhite(CONSTANTS::Piece piece);
    inline bool isBlack(CONSTANTS::Piece piece);
    inline bool isEmpty(CONSTANTS::Piece piece);
    std::string buildMoveString(int nFilePrev, int rankPrev, int nFileNew, int rankNew);
    std::string buildMoveString(char cFileOld, int rankOld, char cFileNew, int rankNew);
    CONSTANTS::Status checkSquareStatus(CONSTANTS::Color playerColor, int nFile, int rank);
    static int cFileToIndex(char cFile);
    static char indexTo_cFile(int nFile);
    static int convertCoordinateToBoardIndex(int nFile, int rank);
    static void parseMove( std::string move, char& pieceType, char& prevFile, int& prevRank, char& newFile, int& newRank);
    static void parseMove( std::string move, char& pieceType, int& prevFile, int& prevRank, int& newFile, int& newRank);
    void calcPawnMoves(int file, int rank, std::list<std::string> &movesResults);
    void calcRookMoves(int file, int rank, std::list<std::string> &movesResults);
    void calcKnightMoves(int file, int rank, std::list<std::string> &moveResults);
    void calcBishopMoves(int file, int rank, std::list<std::string> &moveResults);
    void calcQueenMoves(int file, int rank, std::list<std::string> &moveResults);
    void calcKingMoves(int file, int rank, std::list<std::string> &moveResults);
    void calcPieceMoves(int file, int rank, std::list<std::string> &resultsList);
    void findPlayerPieces();
    int8_t findKingIndex(CONSTANTS::Color color);
    void rookScreenMoves(int file, int rank, std::list<std::string>& resultsList);
    void bishopScreenMoves(int file, int rank, std::list<std::string>& resultsList);
    void queenScreenMoves(int file, int rank, std::list<std::string>& movesHolder);
    void getScreenMoves(int pieceFile, int pieceRank, std::list<std::string>& resultsList);
    void calcScreenMovesets();
    static int countNumAttackers(std::list<std::string> playerMoveset, int forFile, int forRank);
    static void getAttackingMoves(std::list<std::string> playerMoves, int forFile, int forRank, std::list<std::string>* attackingMoves);
    static void makeColinearSquaresVector(std::string pieceMove, std::list<std::string>* pColinearSquaresResultVector);
    static bool squareIsBetweenSquares(std::string move, int testSquareFile, int testSquareRank);
    void calcPlayerMovesetV2(CONSTANTS::Color playerColor, bool validateMoveset);
    void getSquaresBetweenSquares(std::string move, std::vector<std::string>* returnVectorPointer); // Fix me!!!
    bool updatePiecesArray(std::string move, CONSTANTS::Piece capturedPiece); // Archived function
    
public:
    TextBoard();
    ~TextBoard();
    char getPieceType(int nFile, int rank);
    CONSTANTS::Color getPieceColor(int file, int rank);
    typedef CONSTANTS::Piece (&board)[8][8]; // see below
    board getBoardState(CONSTANTS::Piece board[8][8]) const; // returns a const pointer to a board array containing pieces(enums)
    const std::list<std::string>& getLegalMoves(CONSTANTS::Color color) const;
    bool makeMove(std::string move);
    bool editBoard(int file, int rank, CONSTANTS::Piece newPiece);
    void undoLastMove();
    bool isWinner(CONSTANTS::Color winnerColor); // Reports if white or black is the winner when asked

    
    
    
    
};

}

#endif /* TextBoard_hpp */

