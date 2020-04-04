//
//  Piece.hpp
//  Chess
//
//  Created by Mack Ragland on 10/26/19.
//  Copyright © 2019 KattisPracticeProblems. All rights reserved.
//

#ifndef Piece_hpp
#define Piece_hpp

#include <stdio.h>
#include "Piece.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#define FRIENDLY 1
#define ENEMY -1
#define EMPTY 0
#define INVALID -2

using namespace std;

// Board implementation is heavy, minimum required for a piece is:
//Piece: Type, color, doubleJumpTurn, indexLocation on board, pieceMoved
// Pawn: Color, turn it double jumped on/ just double jumped, rank/file derived from array location/ index on board
// Rook:

// Implementation of board: array of strings T,C,M,D
// Player class contains array of indicies instead of pieces


class Piece {
    
private:
    int m_rank;
    int m_file;
    char m_type;
    string m_color;
    vector<string> m_moves;
    bool m_moved = false;
    int m_doubleJumpTurn = -1;
    
    
public:
    Piece();
    Piece(int rank, int file, char type, string color);
    int getRank();
    void setRank(int rank);
    int getFile();
    void setFile(int nFile);
    char getType();
    string getColor();
    bool getMoved();
    void Moved();
    void setColor(string color);
    void setType(char type);
    void setDoubleJumpTurn(int turn);
    int getDoubleJumpedTurn();
    vector<string>* getMoves(Piece* boardState, int currentTurn); // should be assigned to board class
    vector<string>* getScreenMoves(Piece* boardState); // should be assigned to board class
    string buildPositionString(int boardIndexNew); // should be assigned to board class - because it doesn't get moves the board class does
    string buildPositionString(int nFileNew, int rankNew); // should be assigned to board class
    string buildPositionString(char cFileNew, int rankNew); // should be assigned to board class
    static int cFileToIndex(char cFile);
    static char indexTo_cFile(int nFile);
    static int convertCoordinateToBoardIndex(int rank, int file);
    int countNumAttackers(vector<string> playerMoveset, int forFile, int forRank); // should be assigned to board class
    int containsFriendlyPiece(Piece* boardState, int nFile, int rank); // should be assigned to board class
    void enPassantInitiated();
    
private:
    bool isValidCoordinate(int nFile, int rank);
    vector<string>* pawnMoves(Piece* boardState, int currentTurn); // should be assigned to board class
    vector<string>* rookMoves(Piece* boardState); // should be assigned to board class
    vector<string>* knightMoves(Piece* boardState); // should be assigned to board class
    vector<string>* bishopMoves(Piece* boardState); // should be assigned to board class
    vector<string>* queenMoves(Piece* boardState); // should be assigned to board class
    vector<string>* kingMoves(Piece* boardState); // should be assigned to board class
    
    // Screen move calculators
    vector<string>* rookScreenMoves(Piece* boardState); // should be assigned to board class
    vector<string>* bishopScreenMoves(Piece* boardState); // should be assigned to board class
    vector<string>* queenScreenMoves(Piece* boardState); // should be assigned to board class

    
    
};



#endif /* Piece_hpp */
