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
#include "Board.hpp"
using namespace std;

namespace ChessGame {
class Game {
    
public:
    chess::Board m_board;
    Player::Player m_white;
    Player::Player m_black;
    bool checkmateOnWhite = false;
    bool checkmateOnBlack = false;

public:
    Game();
    ~Game();
    void init();
    
private:

    
    
};
    

}
#endif /* Game_hpp */
