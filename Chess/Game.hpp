//
//  Game.hpp
//  Chess
//
//  Created by Mack Ragland on 10/27/19.
//  Copyright © 2019 KattisPracticeProblems. All rights reserved.
//

#ifndef Game_hpp
#define Game_hpp

#include <stdio.h>
#include <iostream>
#include "Player.hpp"
//#include "TextBoard.hpp" // Should just only have to include Player and it should include TextBoard.hpp
#include "ChessConstants.h"
using namespace std;

namespace ChessGame {
class Game {
    
public:
    TextBoard::TextBoard m_board;
    Player::Player m_white;
    Player::Player m_black;
    bool checkmateOnWhite = false;
    bool checkmateOnBlack = false;

public:
    Game();
    ~Game();
    
private:

    
    
};
    

}
#endif /* Game_hpp */
