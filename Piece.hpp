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
using namespace std;

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
    vector<string>* getMoves(Piece* boardState, int currentTurn);
    vector<string>* getScreenMoves(Piece* boardState, int currentTurn);
    string buildPositionString(int boardIndexNew);
    string buildPositionString(int nFileNew, int rankNew);
    string buildPositionString(char cFileNew, int rankNew);
    static int cFileToIndex(char cFile);
    static char indexTo_cFile(int nFile);
    static int convertCoordinateToBoardIndex(int rank, int file);
    int countNumAttackers(vector<string> playerMoveset, int forFile, int forRank);
    int containsFriendlyPiece(Piece* boardState, int nFile, int rank);
    void enPassantInitiated();
    
private:
    bool isValidCoordinate(int nFile, int rank);
    vector<string>* pawnMoves(Piece* boardState, int currentTurn);
    vector<string>* rookMoves(Piece* boardState);
    vector<string>* knightMoves(Piece* boardState);
    vector<string>* bishopMoves(Piece* boardState);
    vector<string>* queenMoves(Piece* boardState);
    vector<string>* kingMoves(Piece* boardState);
    
    // Screen move calculators
    vector<string>* rookScreenMoves(Piece* boardState);
    vector<string>* bishopScreenMoves(Piece* boardState);
    vector<string>* queenScreenMoves(Piece* boardState);

    
    
};



#endif /* Piece_hpp */
