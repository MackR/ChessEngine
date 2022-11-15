//
//  Game.cpp
//  Chess
//
//  Created by Mack Ragland on 10/27/19.
//  Copyright © 2019 KattisPracticeProblems. All rights reserved.
//

#include "Game.hpp"

namespace ChessGame {
    Game::Game():m_white("White"), m_black("Black")  {};
    Game::~Game() {};

    void Game::init(){
        m_board.boardInit();
        m_white.init(&m_board);
        m_black.init(&m_board);
        
    }
}
