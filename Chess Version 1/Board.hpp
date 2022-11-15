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

namespace chess {
    class Board{
        
    private:
        Piece m_Boardstate[64];
        vector<Piece*> m_blackPieces;
        vector<Piece*> m_whitePieces;
        vector<string> m_whiteMoves;
        vector<string> m_blackMoves;
        
    public:
        Board();
        ~Board();
        void boardInit();
        Piece* getBoardstate();
        vector<Piece*>* findPlayerPieces(string playerColor);
        Piece* findPiece(string color, char type);
        void calcPlayerMoveset(string playerColor, int currentTurn, bool checkMoveValidity);
        vector<string>* getWhiteMoves();
        vector<string>* getBlackMoves();
        int countNumAttackers(vector<string> playerMoveset, int forFile, int forRank);
        void getAttackingMoves(vector<string> playerMoves, int forFile, int forRank, vector<string>* pCalcResultVector);
        void makeColinearSquaresVector(string pieceMove, vector<string>* pColinearSquaresResultVector);
        void parseMove( string move, char* pieceTypeAddress, char* prevFileAddress, int* prevRankAddress, char* newFileAddress, int* newRankAddress);
    };
}

#endif /* Board_hpp */
