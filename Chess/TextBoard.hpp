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
#include <stack>
#include <list>

// In each space we should have text with "(Color)(Piece)" - and an EE if empty
// Class member variables will be: lastMove (use to calculate enpassant), blackKingMoved, whiteKingMoved, whiteARookMoved, whiteHRookMoved, blackARookMoved, blackHRookMoved, includeEnpassant,

// Could possibly encode the color and number into a single number using ENUMS, and then create a decode function

namespace TextBoard{

class TextBoard{
    
private:
    
    
    Pieces m_Board[8][8];
    u_int8_t m_turnNum;
    std::stack<std::string> m_moveHistory;
    std::stack<Pieces[8][8]> m_boardHistory;
    std::stack<std::string[8][8]> m_boardHistory;
    std::list<std::int8_t> m_whitePieceIndices;
    std::list<std::int8_t> m_blackPieceIndices;
    std::list<std::string> m_whiteScreenMoves;
    std::list<std::string> m_blackScreenMoves;
    std::list<std::string> m_whiteMoves;
    std::list<std::string> m_blackMoves;
    
    bool m_playerTurn;
    bool m_blackKingMoved, m_whiteKingMoved;
    bool m_whiteARookMoved,m_whiteHRookMoved, m_blackARookMoved, m_blackHRookMoved;
    
    
    inline bool isWhite(int8_t piece);
    inline bool isBlack(int8_t piece);
    inline bool isEmpty(int8_t piece);
    int8_t findKingIndex(Colors color);
    std::string buildMoveString(char cFileOld, int rankOld, char cFileNew, int rankNew);
    int checkSquareStatus(int playerColor, int nFile, int rank);
    static int cFileToIndex(char cFile);
    static char indexTo_cFile(int nFile);
    static int convertCoordinateToBoardIndex(int nFile, int rank);
    static void parseMove( string move, char& pieceType, char& prevFile, int& prevRank, char& newFile, int& newRank);
    static void parseMove( string move, char& pieceType, int& prevFile, int& prevRank, int& newFile, int& newRank);
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
    void calcPlayerMovesetV2(Colors playerColor, bool validateMoveset);
    void getSquaresBetweenSquares(string move, vector<string>* returnVectorPointer); // Fix me!!!
    bool updatePiecesArray(std::string move, CONSTANTS::Pieces capturedPiece); // Archived function
    
public:
    void TextBoard();
    void ~TextBoard();
    char getPieceType(int nFile, int rank, Pieces board[8][8] = m_board);
    int getPieceColor(int file, int row, Pieces board[8][8] = m_board);
    const (&Pieces)[8][8] getBoardState(Pieces board[8][8]) const; // returns a const pointer to a board array containing pieces(enums)
    const std::list<std::string>& getLegalMoves(Color color) const;
    bool makeMove(std::string move);
    bool editBoard(int file, int rank, Pieces newPiece);
    void undoLastMove();
    bool isWinner(Colors winnerColor); // Reports if white or black is the winner when asked

    
    
    
    
}

}

#endif /* TextBoard_hpp */

