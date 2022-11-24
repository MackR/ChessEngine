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
#include "TextBoard.hpp"
#include "ChessConstants.h"
#include <Algorithm>
#include <Map>
#include <List>
#include <chrono>
#include <ctime>

using namespace std;


class Player {
private:
    CONSTANTS::Colors m_player_color;
    bool check = false;
    
    string askPlayerForValidMove(); // Only used within takeTurn()
    
    //CPU Functions
    float computerMaximizer(TextBoard::TextBoard* board, int currentDepth, int stopDepth, vector<string>* pMoves = nullptr);
    float computerMinimizer(TextBoard::TextBoard* board, int currentDepth, int stopDepth, vector<string>* pMoves = nullptr);
    float computerEvaluateBoard(TextBoard::TextBoard* board, string playerTurn);

public:
    bool humanPlayer = true;
//    std::list<std::string>& m_completeMoveset; // initialize me after finding all your pieces on the board, needs a pointer to the boardstate
    float currentTurnBoardScore;
    
public:
    Player(CONSTANTS::Colors color);
    bool takeTurn(TextBoard::TextBoard* board);
    bool achievedCheckmateOnEnemy(TextBoard::TextBoard* board);
    
    //CPU Functions
    string computerBeginThinking(TextBoard::TextBoard* board);
    

    

    
};

#endif /* Player_hpp */

/*
Looks at the set of valid moves : comp
 Tries out the moves mentally : comp
Evaluates the new state after trying move : comp



*/
