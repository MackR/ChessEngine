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
#include <vector>
#include <algorithm>


// In each space we should have text with "(Color)(Piece)" - and an EE if empty
// Class member variables will be: lastMove (use to calculate enpassant), blackKingMoved, whiteKingMoved, whiteARookMoved, whiteHRookMoved, blackARookMoved, blackHRookMoved, includeEnpassant,

// Could possibly encode the color and number into a single number using ENUMS, and then create a decode function



class TextBoard{
public:
    typedef CONSTANTS::Piece board[8][8]; // see getBoardState()
    
private:
    
    board m_board;
    // u_int8_t m_turnNum;
    CONSTANTS::Color m_playerTurn;
    std::stack<std::string> m_moveHistory;
    std::stack<CONSTANTS::Piece> m_captureHistory;
    // std::stack<CONSTANTS::Piece[8][8]> m_boardHistory;
    std::stack<std::vector<CONSTANTS::Piece>> m_boardHistory;
    std::list<std::int8_t> m_whitePieceIndices;
    std::list<std::int8_t> m_blackPieceIndices;
    std::stack<std::list<std::int8_t>> m_whiteIndexHistory;
    std::stack<std::list<std::int8_t>> m_blackIndexHistory;
    std::list<std::string> m_whiteScreenMoves;
    std::list<std::string> m_blackScreenMoves;
    std::list<std::string> m_whiteMoves;
    std::list<std::string> m_blackMoves;
    
    bool m_blackKingMoved, m_whiteKingMoved;
    bool m_whiteARookMoved,m_whiteHRookMoved, m_blackARookMoved, m_blackHRookMoved;
    
    
    inline bool isWhite(CONSTANTS::Piece piece);
    inline bool isBlack(CONSTANTS::Piece piece);
    inline bool isEmpty(CONSTANTS::Piece piece);
    inline CONSTANTS::Piece getPiece(int file, int rank);
    CONSTANTS::Piece getPiece(int index);
    std::list<int8_t>* getPieceLocations(CONSTANTS::Color color) const noexcept;
    void addPiece(int file, int rank, CONSTANTS::Piece piece);
    void addPiece(int index, CONSTANTS::Piece piece);
    void removePiece(int file, int rank);
    void removePiece(int index);
    void movePiece(int prevFile, int prevRank, int newFile, int newRank, CONSTANTS::Piece promotion = CONSTANTS::Piece::EMPTY);
    void movePiece(int prevIndex, int newIndex, CONSTANTS::Piece promotion = CONSTANTS::Piece::EMPTY);
    std::string buildMoveString(int nFilePrev, int rankPrev, int nFileNew, int rankNew);
    std::string buildMoveString(char cFileOld, int rankOld, char cFileNew, int rankNew);
    CONSTANTS::Status checkSquareStatus(CONSTANTS::Color playerColor, int nFile, int rank);
    static inline int cFileToIndex(char cFile);
    static inline char indexTo_cFile(int nFile);
    static int convertCoordinateToBoardIndex(int nFile, int rank);
    static void parseMove( std::string move, char& pieceType, char& prevFile, int& prevRank, char& newFile, int& newRank);
    static void parseMove( std::string move, char& pieceType, int& prevFile, int& prevRank, int& newFile, int& newRank);
    void calcPawnMoves(int file, int rank, std::list<std::string> &movesResults, bool attacksOnly = false);
    void calcRookMoves(int file, int rank, std::list<std::string> &movesResults, bool isProtecting = false);
    void calcKnightMoves(int file, int rank, std::list<std::string> &movesResults, bool isProtecting = false);
    void calcBishopMoves(int file, int rank, std::list<std::string> &movesResults, bool isProtecting = false);
    void calcQueenMoves(int file, int rank, std::list<std::string> &movesResults, bool isProtecting = false);
    void calcKingMoves(int file, int rank, std::list<std::string> &movesResults, bool isProtecting = false);
    void calcPieceMoves(int file, int rank, std::list<std::string> &resultsList, bool attacksOnly = false);
    void findPlayerPieces(bool update = false);
    int8_t findKingIndex(CONSTANTS::Color color);
    void rookScreenMoves(int file, int rank, std::list<std::string>& resultsList);
    void bishopScreenMoves(int file, int rank, std::list<std::string>& resultsList);
    void queenScreenMoves(int file, int rank, std::list<std::string>& movesHolder);
    void queenCheckScreenMoves(int file, int rank, std::list<std::string> &movesHolder);
    void getScreenMoves(int pieceFile, int pieceRank, std::list<std::string>& resultsList);
    void calcScreenMovesets();
    static int countNumAttackers(std::list<std::string> *pplayerMoveset, int forFile, int forRank);
    static void getAttackingMoves(std::list<std::string> playerMoves, int forFile, int forRank, std::list<std::string>* attackingMoves);
    static void makeColinearSquaresVector(std::string pieceMove, std::list<std::string>* pColinearSquaresResultVector);
    static bool squareIsBetweenSquares(std::string move, int testSquareFile, int testSquareRank);
    void calcPlayerMovesetV2(CONSTANTS::Color playerColor, bool validateMoveset);
    static void getSquaresBetweenSquares (std::string move, std::list<std::string>* returnVectorPointer);
    //bool updatePiecesArray(std::string move, CONSTANTS::Piece capturedPiece); // Archived function
    
public:
    
    TextBoard();
    ~TextBoard();
    char getPieceType(int nFile, int rank);
    char getPieceType(int index);
    CONSTANTS::Color getPieceColor(int file, int rank);
    CONSTANTS::Color getPieceColor(int index);
    board* getBoardState(); // returns a const pointer to a board array containing pieces(enums)
    std::list<std::string>* getLegalMoves(CONSTANTS::Color color) ;
    bool makeMove(std::string move);
    bool editBoard(int file, int rank, CONSTANTS::Piece newPiece);
    void undoLastMove();
    bool isWinner(CONSTANTS::Color winnerColor); // Reports if white or black is the winner when asked

    
    
    
    
};



#endif /* TextBoard_hpp */

