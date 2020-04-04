//
//  Board.hpp
//  Chess
//
//  Created by Mack Ragland on 11/5/19.
//  Copyright © 2019 KattisPracticeProblems. All rights reserved.
//

#ifndef Board_hpp
#define Board_hpp

#include <stdio.h>

#include <iostream>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <vector>
#include "Screen.hpp"
#include "Piece.hpp"
#include <chrono>
#include <ctime>
#include <cmath>

namespace chess {
    class Board{
        
    private:
        Piece m_Boardstate[64];
        vector<Piece*> m_blackPieces;
        vector<Piece*> m_whitePieces;
        vector<string> m_whiteMoves;
        vector<string> m_blackMoves;
        vector<string> m_whiteScreenMoves;
        vector<string> m_blackScreenMoves;
        
    public:
        Board();
        ~Board();
        void boardInit();
        Piece* getBoardstate();
        void findPlayerPieces(); // find all pieces and put them into vector m_whitePieces and m_blackPieces vectors
        vector<Piece*>* getPieces(string color); // return a pointer to the vector containing the pieces the player wants
        Piece* findKing(string color); // search through string array for string sequence
        void calcValidatedPlayerMoveset(string playerColor, int currentTurn, bool checkMoveValidity);
        void calcScreenMovesets(); // get the set of moves that pin a piece in place by xraying through to the king
        void calcPlayerMovesetV2(string playerColor, int currentTurn, bool validateMoveset);
        vector<string>* getWhiteMoves();
        vector<string>* getBlackMoves();
        int countNumAttackers(vector<string> playerMoveset, int forFile, int forRank);
        void getAttackingMoves(vector<string> playerMoves, int forFile, int forRank, vector<string>* pCalcResultVector);
        void makeColinearSquaresVector(string pieceMove, vector<string>* pColinearSquaresResultVector);
        void calcScreenMoveset(string playerColor);
        static void parseMove( string move, char* pieceTypeAddress, char* prevFileAddress, int* prevRankAddress, char* newFileAddress, int* newRankAddress);
        static bool squareIsBetweenSquares(string move, int testSquareFile, int testSquareRank);
        void getSquaresBetweenSquares(string move, vector<string>* returnVectorPointer);
        int containsFriendlyPiece(string playerColor, int nFile, int rank);
    };
}

#endif /* Board_hpp */
