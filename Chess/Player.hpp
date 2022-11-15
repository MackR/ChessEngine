//
//  Player.hpp
//  Chess
//
//  Created by Mack Ragland on 10/27/19.
//  Copyright © 2019 KattisPracticeProblems. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp

#include <stdio.h>
#include <sstream>
#include <iostream>
#include <string>
#include "Piece.hpp"
#include "Board.hpp"
#include <Algorithm>
#include <Map>
#include <chrono>
#include <ctime>

using namespace std;

namespace Player {

class Player {
private:
    string m_player_color;
    bool check = false;
    int turnCounter = 1;

public:
    vector<Piece*> m_pieces; // pointer to a vector containing pointers to pieces, should look at the board state and init once only and have it's ranks and files updated each time a move is made
    bool humanPlayer = true;
    vector<string> m_completeMoveset; // initialize me after finding all your pieces on the board, needs a pointer to the boardstate
    float currentTurnBoardScore;
    
public:
    Player(string color);
    string getPlayerColor();
    void init(chess::Board* board);
    string askPlayerForValidMove(vector<string>* validMoveset);
    bool makeMove(chess::Board* board);
    void modifyBoardWithMove(chess::Board* board, string move);
    bool achievedCheckmateOnEnemy(chess::Board* board);
    void incrementTurn();
    
    //CPU Functions
    string computerBeginThinking(chess::Board* board);
    float computerEvaluateBoard(chess::Board* board, string playerTurn);
    float computerMaximizer(chess::Board* board, int currentDepth, int stopDepth, vector<string>* pMoves = nullptr);
    float computerMinimizer(chess::Board* board, int currentDepth, int stopDepth, vector<string>* pMoves = nullptr);
    float MiniMaxCalc(chess::Board* board, int maxDepth, vector<string>* pMoves = nullptr); // not used
    

    
};
    
}
#endif /* Player_hpp */

/*
Looks at the set of valid moves : comp
 Tries out the moves mentally : comp
Evaluates the new state after trying move : comp



*/
