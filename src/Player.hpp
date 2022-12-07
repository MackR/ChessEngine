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
#include <algorithm>
#include <map>
#include <list>
#include <vector>
#include <chrono>
#include <ctime>
#include <memory>

using namespace std;


class Player {
private:
    CONSTANTS::Color m_player_color;
    std::list<std::string> *m_pCompleteMoveset; // Pointer to a 
    bool check = false;
    
    string askPlayerForValidMove(); // Only used within takeTurn()
    
    //CPU Functions
    float computerMaximizer(TextBoard *board, int currentDepth, int stopDepth);
    float computerMinimizer(TextBoard *board, int currentDepth, int stopDepth);
    float computerMaxMin(TextBoard *board, int currentDepth, int stopDepth, CONSTANTS::Color color);
    float computerEvaluateBoard(TextBoard* board, CONSTANTS::Color playerTurn);

public:
    bool humanPlayer = true;
//    std::list<std::string>& m_completeMoveset; // initialize me after finding all your pieces on the board, needs a pointer to the boardstate
    float currentTurnBoardScore;
    Player(CONSTANTS::Color color);
    bool takeTurn(TextBoard* board);
    bool achievedCheckmateOnEnemy(TextBoard* board);
    
    //CPU Functions
    string computerBeginThinking(TextBoard* board);
    

    

    
};

#endif /* Player_hpp */

/*
Looks at the set of valid moves : comp
 Tries out the moves mentally : comp
Evaluates the new state after trying move : comp



*/
