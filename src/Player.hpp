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
    bool m_check = false;
    const int m_computerStopDepth = 4;
    
    string askPlayerForMove(); // Only used within takeTurn()
    
    //CPU Functions
    // float computerMaximizer(TextBoard *board, int currentDepth, int stopDepth); // Archived
    // float computerMinimizer(TextBoard *board, int currentDepth, int stopDepth); // Archived
    float computerMaxMin(TextBoard *board, int currentDepth, int stopDepth, const CONSTANTS::Color color);
    float computerEvaluateBoard(TextBoard* board, const CONSTANTS::Color playerTurn);

public:
    bool m_isHuman = true;
    float m_currentTurnBoardScore;
//    std::list<std::string>& m_completeMoveset; // initialize me after finding all your pieces on the board, needs a pointer to the boardstate
    Player(const CONSTANTS::Color color, const bool isHuman);
    bool takeTurn(TextBoard* board);
    bool achievedCheckmateOnEnemy(TextBoard* board);
    
    //CPU Functions
    string computeMove(TextBoard* board);
    

    

    
};

#endif /* Player_hpp */

/*
Looks at the set of valid moves : comp
 Tries out the moves mentally : comp
Evaluates the new state after trying move : comp



*/
